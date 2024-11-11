/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <stdlib.h>
/*${standard_header_anchor}*/

#include "uvc_camera.h"

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_clock.h"
#include "fsl_smartdma.h"
#include "fsl_inputmux.h"
#include "fsl_inputmux_connections.h"
#include "fsl_ov7670.h"

#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
#include "fsl_sysmpu.h"
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

#if ((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
#include "usb_phy.h"
#endif

#include "image.h"
#include "image_fundamentals.h"
#include "noise.h"
#include "segmentation.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_InitHardware(void);
void USB_DeviceClockInit(void);
void USB_DeviceIsrEnable(void);
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

static void USB_DeviceVideoPrepareVideoData(void);
static usb_status_t USB_DeviceVideoRequest(class_handle_t handle, uint32_t event, void *param);
static usb_status_t USB_DeviceVideoCallback(class_handle_t handle, uint32_t event, void *param);
static void USB_DeviceVideoApplicationSetDefault(void);
static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern usb_device_class_struct_t g_UsbDeviceVideoUVCCameraConfig;

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_probe_and_commit_controls_struct_t s_ProbeStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_probe_and_commit_controls_struct_t s_CommitStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_still_probe_and_commit_controls_struct_t s_StillProbeStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static usb_device_video_still_probe_and_commit_controls_struct_t s_StillCommitStruct;
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint32_t s_ClassRequestBuffer[(sizeof(usb_device_video_probe_and_commit_controls_struct_t) >> 2U) + 1U];
/* this buffer is used to do transfer */
USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) static uint8_t s_ImageBuffer[HS_STREAM_IN_PACKET_SIZE];
usb_video_uvc_camera_struct_t g_UsbDeviceVideoUVCCamera;

usb_device_class_config_struct_t g_UsbDeviceVideoConfig[1] = {{
    USB_DeviceVideoCallback,
    (class_handle_t)NULL,
    &g_UsbDeviceVideoUVCCameraConfig,
}};

usb_device_class_config_list_struct_t g_UsbDeviceVideoConfigList = {
    g_UsbDeviceVideoConfig,
    USB_DeviceCallback,
    1U,
};

extern volatile uint32_t image_available_for_usb;
extern image_t *usb;

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U))
void USB1_HS_IRQHandler(void)
{
    USB_DeviceEhciIsrFunction(g_UsbDeviceVideoUVCCamera.deviceHandle);
}
#endif
#if (defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U))
void USB0_FS_IRQHandler(void)
{
    USB_DeviceKhciIsrFunction(g_UsbDeviceVideoUVCCamera.deviceHandle);
}
#endif

void USB_DeviceClockInit(void)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };
#endif
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    SPC0->ACTIVE_VDELAY = 0x0500;
    /* Change the power DCDC to 1.8v (By deafult, DCDC is 1.8V), CORELDO to 1.1v (By deafult, CORELDO is 1.0V) */
    SPC0->ACTIVE_CFG &= ~SPC_ACTIVE_CFG_CORELDO_VDD_DS_MASK;
    SPC0->ACTIVE_CFG |= SPC_ACTIVE_CFG_DCDC_VDD_LVL(0x3) | SPC_ACTIVE_CFG_CORELDO_VDD_LVL(0x3) |
                        SPC_ACTIVE_CFG_SYSLDO_VDD_DS_MASK | SPC_ACTIVE_CFG_DCDC_VDD_DS(0x2u);
    /* Wait until it is done */
    while (SPC0->SC & SPC_SC_BUSY_MASK)
        ;
    if (0u == (SCG0->LDOCSR & SCG_LDOCSR_LDOEN_MASK))
    {
        SCG0->TRIM_LOCK = 0x5a5a0001U;
        SCG0->LDOCSR |= SCG_LDOCSR_LDOEN_MASK;
        /* wait LDO ready */
        while (0U == (SCG0->LDOCSR & SCG_LDOCSR_VOUT_OK_MASK))
            ;
    }
    SYSCON->AHBCLKCTRLSET[2] |= SYSCON_AHBCLKCTRL2_USB_HS_MASK | SYSCON_AHBCLKCTRL2_USB_HS_PHY_MASK;
    SCG0->SOSCCFG &= ~(SCG_SOSCCFG_RANGE_MASK | SCG_SOSCCFG_EREFS_MASK);
    /* xtal = 20 ~ 30MHz */
    SCG0->SOSCCFG = (1U << SCG_SOSCCFG_RANGE_SHIFT) | (1U << SCG_SOSCCFG_EREFS_SHIFT);
    SCG0->SOSCCSR |= SCG_SOSCCSR_SOSCEN_MASK;
    while (1)
    {
        if (SCG0->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)
        {
            break;
        }
    }
    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_CLKIN_ENA_MASK | SYSCON_CLOCK_CTRL_CLKIN_ENA_FM_USBH_LPT_MASK;
    CLOCK_EnableClock(kCLOCK_UsbHs);
    CLOCK_EnableClock(kCLOCK_UsbHsPhy);
    CLOCK_EnableUsbhsPhyPllClock(kCLOCK_Usbphy480M, 24000000U);
    CLOCK_EnableUsbhsClock();
    USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL0_CLK_HZ, &phyConfig);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    CLOCK_AttachClk(kCLK_48M_to_USB0);
    CLOCK_EnableClock(kCLOCK_Usb0Ram);
    CLOCK_EnableClock(kCLOCK_Usb0Fs);
    CLOCK_EnableUsbfsClock();
#endif
}

void USB_DeviceIsrEnable(void)
{
    uint8_t irqNumber;
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    uint8_t usbDeviceEhciIrq[] = USBHS_IRQS;
    irqNumber                  = usbDeviceEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    uint8_t usbDeviceKhciIrq[] = USBFS_IRQS;
    irqNumber                  = usbDeviceKhciIrq[CONTROLLER_ID - kUSB_ControllerKhci0];
#endif
    /* Install isr, set priority, and enable IRQ. */
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_DEVICE_INTERRUPT_PRIORITY);
    EnableIRQ((IRQn_Type)irqNumber);
}
#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle)
{
#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
    USB_DeviceEhciTaskFunction(deviceHandle);
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
    USB_DeviceKhciTaskFunction(deviceHandle);
#endif
}
#endif


AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t usb_buffer[1024], 64);



/* Prepare next transfer payload */
static void USB_DeviceVideoPrepareVideoData(void)
{
    usb_device_video_uncompressed_payload_header_struct_t *payloadHeader;
    uint32_t maxPacketSize;
    uint32_t i;
    uint32_t sendLength;

    g_UsbDeviceVideoUVCCamera.currentTime += 100000U; // was 10000U

	if(image_available_for_usb == 0)
	{
		return;
	}

#if (USB_IMAGE_TYPE_UYVY == 1)
	if(usb->type != IMGTYPE_UYVY)
	{
		usb_echo("USB error: expected a uyvy_pixel_t image\r\n");
		while(1)
		{}
	}
#endif

#if (USB_IMAGE_TYPE_BGR888 == 1)
	if(usb->type != IMGTYPE_BGR888)
	{
		usb_echo("USB error: expected a bgr888_pixel_t image\r\n");
		while(1)
		{}
	}
#endif

    if (g_UsbDeviceVideoUVCCamera.imagePosition < USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE)
    {
    	memcpy(usb_buffer + sizeof(usb_device_video_uncompressed_payload_header_struct_t),
    		usb->data + g_UsbDeviceVideoUVCCamera.imagePosition,
    		sizeof(usb_buffer) - sizeof(usb_device_video_uncompressed_payload_header_struct_t));

    	g_UsbDeviceVideoUVCCamera.imageBuffer = usb_buffer;

    }

    payloadHeader = (usb_device_video_uncompressed_payload_header_struct_t *)&g_UsbDeviceVideoUVCCamera.imageBuffer[0];
    for (i = 0; i < sizeof(usb_device_video_uncompressed_payload_header_struct_t); i++)
    {
        g_UsbDeviceVideoUVCCamera.imageBuffer[i] = 0x00U;
    }
    payloadHeader->bHeaderLength = sizeof(usb_device_video_uncompressed_payload_header_struct_t);
    payloadHeader->headerInfoUnion.bmheaderInfo = 0U;
    payloadHeader->headerInfoUnion.headerInfoBits.frameIdentifier = g_UsbDeviceVideoUVCCamera.currentFrameId;
    g_UsbDeviceVideoUVCCamera.imageBufferLength = sizeof(usb_device_video_uncompressed_payload_header_struct_t);

    if (g_UsbDeviceVideoUVCCamera.stillImageTransmission)
    {
        payloadHeader->headerInfoUnion.headerInfoBits.stillImage = 1U;
        maxPacketSize = g_UsbDeviceVideoUVCCamera.stillCommitStruct->dwMaxPayloadTransferSize;
    }
    else
    {
        maxPacketSize = g_UsbDeviceVideoUVCCamera.commitStruct->dwMaxPayloadTransferSize;
    }
    maxPacketSize = maxPacketSize - sizeof(usb_device_video_uncompressed_payload_header_struct_t);
    if (g_UsbDeviceVideoUVCCamera.waitForNewInterval)
    {
        if (g_UsbDeviceVideoUVCCamera.currentTime < g_UsbDeviceVideoUVCCamera.commitStruct->dwFrameInterval)
        {
            return;
        }
        else
        {
        	image_available_for_usb = 0;

            g_UsbDeviceVideoUVCCamera.imagePosition = 0U;
            g_UsbDeviceVideoUVCCamera.currentTime = 0U;
            g_UsbDeviceVideoUVCCamera.waitForNewInterval = 0U;
            payloadHeader->headerInfoUnion.headerInfoBits.endOfFrame = 1U;
            g_UsbDeviceVideoUVCCamera.stillImageTransmission = 0U;
            g_UsbDeviceVideoUVCCamera.currentFrameId ^= 1U;
            if (USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_TRANSMIT_STILL_IMAGE ==
                g_UsbDeviceVideoUVCCamera.stillImageTriggerControl)
            {
                g_UsbDeviceVideoUVCCamera.stillImageTriggerControl =
                    USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
                g_UsbDeviceVideoUVCCamera.stillImageTransmission = 1U;
            }
            return;
        }
    }

    if (g_UsbDeviceVideoUVCCamera.imagePosition < USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE)
    {
        sendLength = USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE - g_UsbDeviceVideoUVCCamera.imagePosition;
        if (sendLength > maxPacketSize)
        {
            sendLength = maxPacketSize;
        }
        g_UsbDeviceVideoUVCCamera.imagePosition += sendLength;

        if (g_UsbDeviceVideoUVCCamera.imagePosition >= USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE)
        {
            if (g_UsbDeviceVideoUVCCamera.currentTime < g_UsbDeviceVideoUVCCamera.commitStruct->dwFrameInterval)
            {
                g_UsbDeviceVideoUVCCamera.waitForNewInterval = 1U;
            }
            else
            {
            	image_available_for_usb = 0;

                g_UsbDeviceVideoUVCCamera.imagePosition = 0U;
                g_UsbDeviceVideoUVCCamera.currentTime = 0U;
                payloadHeader->headerInfoUnion.headerInfoBits.endOfFrame = 1U;
                g_UsbDeviceVideoUVCCamera.stillImageTransmission = 0U;
                g_UsbDeviceVideoUVCCamera.currentFrameId ^= 1U;
                if (USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_TRANSMIT_STILL_IMAGE ==
                    g_UsbDeviceVideoUVCCamera.stillImageTriggerControl)
                {
                    g_UsbDeviceVideoUVCCamera.stillImageTriggerControl =
                        USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
                    g_UsbDeviceVideoUVCCamera.stillImageTransmission = 1U;
                }
            }
        }
        g_UsbDeviceVideoUVCCamera.imageBufferLength += sendLength;
    }
}

static usb_status_t USB_DeviceVideoRequest(class_handle_t handle, uint32_t event, void *param)
{
    usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;
    usb_device_video_probe_and_commit_controls_struct_t *probe =
        (usb_device_video_probe_and_commit_controls_struct_t *)(request->buffer);
    usb_device_video_probe_and_commit_controls_struct_t *commit =
        (usb_device_video_probe_and_commit_controls_struct_t *)(request->buffer);
    usb_device_video_still_probe_and_commit_controls_struct_t *still_probe =
        (usb_device_video_still_probe_and_commit_controls_struct_t *)(request->buffer);
    usb_device_video_still_probe_and_commit_controls_struct_t *still_commit =
        (usb_device_video_still_probe_and_commit_controls_struct_t *)(request->buffer);
    uint32_t temp32;
    usb_status_t error = kStatus_USB_Success;

    switch (event)
    {
        /* probe request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_PROBE_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(probe->dwFrameInterval);
            if ((temp32 >= USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL) &&
                (temp32 <= USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32, g_UsbDeviceVideoUVCCamera.probeStruct->dwFrameInterval);
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(probe->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoUVCCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32,
                                               g_UsbDeviceVideoUVCCamera.probeStruct->dwMaxPayloadTransferSize);
            }
            g_UsbDeviceVideoUVCCamera.probeStruct->bFormatIndex = probe->bFormatIndex;
            g_UsbDeviceVideoUVCCamera.probeStruct->bFrameIndex  = probe->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_PROBE_CONTROL:
            request->buffer = (uint8_t *)g_UsbDeviceVideoUVCCamera.probeStruct;
            request->length = g_UsbDeviceVideoUVCCamera.probeLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoUVCCamera.probeLength;
            request->length = sizeof(g_UsbDeviceVideoUVCCamera.probeLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoUVCCamera.probeInfo;
            request->length = sizeof(g_UsbDeviceVideoUVCCamera.probeInfo);
            break;
        /* commit request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_COMMIT_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(commit->dwFrameInterval);
            if ((temp32 >= USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MIN_INTERVAL) &&
                (temp32 <= USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MAX_INTERVAL))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32, g_UsbDeviceVideoUVCCamera.commitStruct->dwFrameInterval);
            }

            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(commit->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoUVCCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(temp32,
                                               g_UsbDeviceVideoUVCCamera.commitStruct->dwMaxPayloadTransferSize);
            }
            g_UsbDeviceVideoUVCCamera.commitStruct->bFormatIndex = commit->bFormatIndex;
            g_UsbDeviceVideoUVCCamera.commitStruct->bFrameIndex  = commit->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_COMMIT_CONTROL:
            request->buffer = (uint8_t *)g_UsbDeviceVideoUVCCamera.commitStruct;
            request->length = g_UsbDeviceVideoUVCCamera.commitLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoUVCCamera.commitLength;
            request->length = sizeof(g_UsbDeviceVideoUVCCamera.commitLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoUVCCamera.commitInfo;
            request->length = sizeof(g_UsbDeviceVideoUVCCamera.commitInfo);
            break;
        /* still probe request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_PROBE_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(still_probe->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoUVCCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(
                    temp32, g_UsbDeviceVideoUVCCamera.stillProbeStruct->dwMaxPayloadTransferSize);
            }

            g_UsbDeviceVideoUVCCamera.stillProbeStruct->bFormatIndex = still_probe->bFormatIndex;
            g_UsbDeviceVideoUVCCamera.stillProbeStruct->bFrameIndex  = still_probe->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_STILL_PROBE_CONTROL:
            request->buffer = (uint8_t *)g_UsbDeviceVideoUVCCamera.stillProbeStruct;
            request->length = g_UsbDeviceVideoUVCCamera.stillProbeLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_STILL_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoUVCCamera.stillProbeLength;
            request->length = sizeof(g_UsbDeviceVideoUVCCamera.stillProbeLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_STILL_PROBE_CONTROL:
            request->buffer = &g_UsbDeviceVideoUVCCamera.stillProbeInfo;
            request->length = sizeof(g_UsbDeviceVideoUVCCamera.stillProbeInfo);
            break;
        /* still commit request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_COMMIT_CONTROL:
            if ((request->buffer == NULL) || (request->length == 0U))
            {
                return kStatus_USB_InvalidRequest;
            }
            temp32 = USB_LONG_FROM_LITTLE_ENDIAN_DATA(still_commit->dwMaxPayloadTransferSize);
            if ((temp32) && (temp32 < g_UsbDeviceVideoUVCCamera.currentMaxPacketSize))
            {
                USB_LONG_TO_LITTLE_ENDIAN_DATA(
                    temp32, g_UsbDeviceVideoUVCCamera.stillCommitStruct->dwMaxPayloadTransferSize);
            }

            g_UsbDeviceVideoUVCCamera.stillCommitStruct->bFormatIndex = still_commit->bFormatIndex;
            g_UsbDeviceVideoUVCCamera.stillCommitStruct->bFrameIndex  = still_commit->bFrameIndex;
            break;
        case USB_DEVICE_VIDEO_GET_CUR_VS_STILL_COMMIT_CONTROL:
            request->buffer = (uint8_t *)g_UsbDeviceVideoUVCCamera.stillCommitStruct;
            request->length = g_UsbDeviceVideoUVCCamera.stillCommitLength;
            break;
        case USB_DEVICE_VIDEO_GET_LEN_VS_STILL_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoUVCCamera.stillCommitLength;
            request->length = sizeof(g_UsbDeviceVideoUVCCamera.stillCommitLength);
            break;
        case USB_DEVICE_VIDEO_GET_INFO_VS_STILL_COMMIT_CONTROL:
            request->buffer = &g_UsbDeviceVideoUVCCamera.stillCommitInfo;
            request->length = sizeof(g_UsbDeviceVideoUVCCamera.stillCommitInfo);
            break;
        /* still image trigger request */
        case USB_DEVICE_VIDEO_SET_CUR_VS_STILL_IMAGE_TRIGGER_CONTROL:
            g_UsbDeviceVideoUVCCamera.stillImageTriggerControl = *(request->buffer);
            break;
        default:
            error = kStatus_USB_InvalidRequest;
            break;
    }
    return error;
}

/* USB device Video class callback */
static usb_status_t USB_DeviceVideoCallback(class_handle_t handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_InvalidRequest;

    switch (event)
    {
        case kUSB_DeviceVideoEventStreamSendResponse:
            /* Stream data dent */
            if (g_UsbDeviceVideoUVCCamera.attach)
            {
                /* Prepare the next stream data */
                USB_DeviceVideoPrepareVideoData();
                error = USB_DeviceVideoSend(
                    g_UsbDeviceVideoUVCCamera.videoHandle, USB_VIDEO_UVC_CAMERA_STREAM_ENDPOINT_IN,
                    g_UsbDeviceVideoUVCCamera.imageBuffer, g_UsbDeviceVideoUVCCamera.imageBufferLength);
            }
            break;
        case kUSB_DeviceVideoEventClassRequestBuffer:
            if (param && (g_UsbDeviceVideoUVCCamera.attach))
            {
                /* Get the class-specific OUT buffer */
                usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;

                if (request->length <= sizeof(usb_device_video_probe_and_commit_controls_struct_t))
                {
                    request->buffer = (uint8_t *)g_UsbDeviceVideoUVCCamera.classRequestBuffer;
                    error           = kStatus_USB_Success;
                }
            }
            break;
        default:
            if (param && (event > 0xFFU))
            {
                /* If the event is the class-specific request(Event > 0xFFU), handle the class-specific request */
                error = USB_DeviceVideoRequest(handle, event, param);
            }
            break;
    }

    return error;
}

/* Set to default state */
static void USB_DeviceVideoApplicationSetDefault(void)
{
    g_UsbDeviceVideoUVCCamera.speed                = USB_SPEED_FULL;
    g_UsbDeviceVideoUVCCamera.attach               = 0U;
    g_UsbDeviceVideoUVCCamera.currentMaxPacketSize = HS_STREAM_IN_PACKET_SIZE;
    g_UsbDeviceVideoUVCCamera.imageBuffer          = s_ImageBuffer;
    g_UsbDeviceVideoUVCCamera.probeStruct          = &s_ProbeStruct;
    g_UsbDeviceVideoUVCCamera.commitStruct         = &s_CommitStruct;
    g_UsbDeviceVideoUVCCamera.stillProbeStruct     = &s_StillProbeStruct;
    g_UsbDeviceVideoUVCCamera.stillCommitStruct    = &s_StillCommitStruct;
    g_UsbDeviceVideoUVCCamera.classRequestBuffer   = &s_ClassRequestBuffer[0];
    for (uint8_t i = 0; i < USB_VIDEO_UVC_CAMERA_INTERFACE_COUNT; i++)
    {
        g_UsbDeviceVideoUVCCamera.currentInterfaceAlternateSetting[i] = 0;
    }

    g_UsbDeviceVideoUVCCamera.probeStruct->bFormatIndex = USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoUVCCamera.probeStruct->bFrameIndex  = USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL,
                                   g_UsbDeviceVideoUVCCamera.probeStruct->dwFrameInterval);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoUVCCamera.currentMaxPacketSize,
                                   g_UsbDeviceVideoUVCCamera.probeStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoUVCCamera.probeStruct->dwMaxVideoFrameSize);

    g_UsbDeviceVideoUVCCamera.commitStruct->bFormatIndex = USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoUVCCamera.commitStruct->bFrameIndex  = USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_DEFAULT_INTERVAL,
                                   g_UsbDeviceVideoUVCCamera.commitStruct->dwFrameInterval);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoUVCCamera.currentMaxPacketSize,
                                   g_UsbDeviceVideoUVCCamera.commitStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoUVCCamera.commitStruct->dwMaxVideoFrameSize);

    g_UsbDeviceVideoUVCCamera.probeInfo    = 0x03U;
    g_UsbDeviceVideoUVCCamera.probeLength  = 26U;
    g_UsbDeviceVideoUVCCamera.commitInfo   = 0x03U;
    g_UsbDeviceVideoUVCCamera.commitLength = 26U;

    g_UsbDeviceVideoUVCCamera.stillProbeStruct->bFormatIndex      = USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoUVCCamera.stillProbeStruct->bFrameIndex       = USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    g_UsbDeviceVideoUVCCamera.stillProbeStruct->bCompressionIndex = 0x01U;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoUVCCamera.currentMaxPacketSize,
                                   g_UsbDeviceVideoUVCCamera.stillProbeStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoUVCCamera.stillProbeStruct->dwMaxVideoFrameSize);

    g_UsbDeviceVideoUVCCamera.stillCommitStruct->bFormatIndex      = USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FORMAT_INDEX;
    g_UsbDeviceVideoUVCCamera.stillCommitStruct->bFrameIndex       = USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_INDEX;
    g_UsbDeviceVideoUVCCamera.stillCommitStruct->bCompressionIndex = 0x01U;
    USB_LONG_TO_LITTLE_ENDIAN_DATA(g_UsbDeviceVideoUVCCamera.currentMaxPacketSize,
                                   g_UsbDeviceVideoUVCCamera.stillCommitStruct->dwMaxPayloadTransferSize);
    USB_LONG_TO_LITTLE_ENDIAN_DATA(USB_VIDEO_UVC_CAMERA_UNCOMPRESSED_FRAME_MAX_FRAME_SIZE,
                                   g_UsbDeviceVideoUVCCamera.stillCommitStruct->dwMaxVideoFrameSize);

    g_UsbDeviceVideoUVCCamera.stillProbeInfo    = 0x03U;
    g_UsbDeviceVideoUVCCamera.stillProbeLength  = sizeof(s_StillProbeStruct);
    g_UsbDeviceVideoUVCCamera.stillCommitInfo   = 0x03U;
    g_UsbDeviceVideoUVCCamera.stillCommitLength = sizeof(s_StillCommitStruct);

    g_UsbDeviceVideoUVCCamera.currentTime                            = 0U;
    g_UsbDeviceVideoUVCCamera.currentFrameId                         = 0U;
    g_UsbDeviceVideoUVCCamera.currentStreamInterfaceAlternateSetting = 0U;
    g_UsbDeviceVideoUVCCamera.imageBufferLength                      = 0U;
    g_UsbDeviceVideoUVCCamera.imageIndex                             = 0U;
    g_UsbDeviceVideoUVCCamera.waitForNewInterval                     = 0U;
    g_UsbDeviceVideoUVCCamera.stillImageTransmission                 = 0U;
    g_UsbDeviceVideoUVCCamera.stillImageTriggerControl = USB_DEVICE_VIDEO_STILL_IMAGE_TRIGGER_NORMAL_OPERATION;
}

/* The device callback */
static usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t error = kStatus_USB_InvalidRequest;
    uint8_t *temp8     = (uint8_t *)param;
    uint16_t *temp16   = (uint16_t *)param;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
        {
            /* The device BUS reset signal detected */
            USB_DeviceVideoApplicationSetDefault();
            g_UsbDeviceVideoUVCCamera.attach               = 0U;
            g_UsbDeviceVideoUVCCamera.currentConfiguration = 0U;
#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (kStatus_USB_Success == USB_DeviceGetStatus(g_UsbDeviceVideoUVCCamera.deviceHandle,
                                                           kUSB_DeviceStatusSpeed,
                                                           &g_UsbDeviceVideoUVCCamera.speed))
            {
                USB_DeviceSetSpeed(g_UsbDeviceVideoUVCCamera.deviceHandle, g_UsbDeviceVideoUVCCamera.speed);
            }

            if (USB_SPEED_HIGH == g_UsbDeviceVideoUVCCamera.speed)
            {
                g_UsbDeviceVideoUVCCamera.currentMaxPacketSize = HS_STREAM_IN_PACKET_SIZE;
            }
#endif
            error = kStatus_USB_Success;
        }
        break;
        case kUSB_DeviceEventSetConfiguration:
            if (0 == (*temp8))
            {
                g_UsbDeviceVideoUVCCamera.attach               = 0U;
                g_UsbDeviceVideoUVCCamera.currentConfiguration = 0U;
                error                                              = kStatus_USB_Success;
            }
            else if (USB_VIDEO_UVC_CAMERA_CONFIGURE_INDEX == (*temp8))
            {
                /* Set the configuration request */
                g_UsbDeviceVideoUVCCamera.attach               = 1U;
                g_UsbDeviceVideoUVCCamera.currentConfiguration = *temp8;
                error                                              = kStatus_USB_Success;
            }
            else
            {
                /* no action */
            }
            break;
        case kUSB_DeviceEventSetInterface:
            if ((g_UsbDeviceVideoUVCCamera.attach) && param)
            {
                /* Set alternateSetting of the interface request */
                uint8_t interface        = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                uint8_t alternateSetting = (uint8_t)(*temp16 & 0x00FFU);

                if (USB_VIDEO_UVC_CAMERA_CONTROL_INTERFACE_INDEX == interface)
                {
                    if (alternateSetting < USB_VIDEO_UVC_CAMERA_CONTROL_INTERFACE_ALTERNATE_COUNT)
                    {
                        g_UsbDeviceVideoUVCCamera.currentInterfaceAlternateSetting[interface] = alternateSetting;
                        error = kStatus_USB_Success;
                    }
                }
                else if (USB_VIDEO_UVC_CAMERA_STREAM_INTERFACE_INDEX == interface)
                {
                    if (alternateSetting < USB_VIDEO_UVC_CAMERA_STREAM_INTERFACE_ALTERNATE_COUNT)
                    {
                        if (USB_VIDEO_UVC_CAMERA_STREAM_INTERFACE_ALTERNATE_0 ==
                            g_UsbDeviceVideoUVCCamera.currentInterfaceAlternateSetting[interface])
                        {
                            USB_DeviceVideoPrepareVideoData();
                            error = USB_DeviceSendRequest(g_UsbDeviceVideoUVCCamera.deviceHandle,
                                                          USB_VIDEO_UVC_CAMERA_STREAM_ENDPOINT_IN,
                                                          g_UsbDeviceVideoUVCCamera.imageBuffer,
                                                          g_UsbDeviceVideoUVCCamera.imageBufferLength);
                        }
                        else
                        {
                            error = kStatus_USB_Success;
                        }
                        g_UsbDeviceVideoUVCCamera.currentInterfaceAlternateSetting[interface] = alternateSetting;
                    }
                }
                else
                {
                    /* no action */
                }
            }
            break;
        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                /* Get the current configuration request */
                *temp8 = g_UsbDeviceVideoUVCCamera.currentConfiguration;
                error  = kStatus_USB_Success;
            }
            break;
        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                /* Set the alternateSetting of the interface request */
                uint8_t interface = (uint8_t)((*temp16 & 0xFF00U) >> 0x08U);
                if (interface < USB_VIDEO_UVC_CAMERA_INTERFACE_COUNT)
                {
                    *temp16 =
                        (*temp16 & 0xFF00U) | g_UsbDeviceVideoUVCCamera.currentInterfaceAlternateSetting[interface];
                    error = kStatus_USB_Success;
                }
            }
            break;
        case kUSB_DeviceEventGetDeviceDescriptor:
            if (param)
            {
                /* Get the device descriptor request */
                error = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                /* Get the configuration descriptor request */
                error = USB_DeviceGetConfigurationDescriptor(handle,
                                                             (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;
        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                /* Get the string descriptor request */
                error = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;
        default:
            /* no action */
            break;
    }

    return error;
}

void USB_DeviceApplicationInit(void)
{
    USB_DeviceClockInit();
#if (defined(FSL_FEATURE_SOC_SYSMPU_COUNT) && (FSL_FEATURE_SOC_SYSMPU_COUNT > 0U))
    SYSMPU_Enable(SYSMPU, 0);
#endif /* FSL_FEATURE_SOC_SYSMPU_COUNT */

    USB_DeviceVideoApplicationSetDefault();

    if (kStatus_USB_Success !=
        USB_DeviceClassInit(CONTROLLER_ID, &g_UsbDeviceVideoConfigList, &g_UsbDeviceVideoUVCCamera.deviceHandle))
    {
        usb_echo("USB device video uvc camera failed\r\n");
        return;
    }
    else
    {
        g_UsbDeviceVideoUVCCamera.videoHandle = g_UsbDeviceVideoConfigList.config->classHandle;
    }

    USB_DeviceIsrEnable();

    /*Add one delay here to make the DP pull down long enough to allow host to detect the previous disconnection.*/
    SDK_DelayAtLeastUs(5000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    USB_DeviceRun(g_UsbDeviceVideoUVCCamera.deviceHandle);
}
