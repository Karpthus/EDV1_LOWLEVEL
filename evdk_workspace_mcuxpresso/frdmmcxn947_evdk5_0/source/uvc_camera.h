/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __USB_VIDEO_UVC_CAMERA_H__
#define __USB_VIDEO_UVC_CAMERA_H__

#include "usb_device_config.h"
#include "usb.h"
#include "usb_device.h"

#include "usb_device_class.h"
#include "usb_device_video.h"

#include "usb_device_ch9.h"
#include "usb_device_descriptor.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* @TEST_ANCHOR */

#if defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerEhci0
#endif
#endif
#if defined(USB_DEVICE_CONFIG_KHCI) && (USB_DEVICE_CONFIG_KHCI > 0U)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerKhci0
#endif
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511FS) && (USB_DEVICE_CONFIG_LPCIP3511FS > 0U)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Fs0
#endif
#endif
#if defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U)
#ifndef CONTROLLER_ID
#define CONTROLLER_ID kUSB_ControllerLpcIp3511Hs0
#endif
#endif

#if defined(__GIC_PRIO_BITS)
#define USB_DEVICE_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_DEVICE_INTERRUPT_PRIORITY (6U)
#else
#define USB_DEVICE_INTERRUPT_PRIORITY (3U)
#endif

void USB_DeviceApplicationInit(void);

typedef struct _usb_video_uvc_camera_struct
{
    usb_device_handle deviceHandle;
    class_handle_t videoHandle;
    uint32_t currentMaxPacketSize;
    uint8_t *imageBuffer;
    uint32_t imageBufferLength;
    usb_device_video_probe_and_commit_controls_struct_t *probeStruct;
    usb_device_video_probe_and_commit_controls_struct_t *commitStruct;
    usb_device_video_still_probe_and_commit_controls_struct_t *stillProbeStruct;
    usb_device_video_still_probe_and_commit_controls_struct_t *stillCommitStruct;
    uint32_t imageIndex;
    uint32_t currentTime;
    uint32_t *classRequestBuffer;
    uint32_t imagePosition;
    uint8_t *image_header;
    uint8_t currentFrameId;
    uint8_t waitForNewInterval;
    uint8_t currentStreamInterfaceAlternateSetting;
    uint8_t probeLength;
    uint8_t commitLength;
    uint8_t probeInfo;
    uint8_t commitInfo;
    uint8_t stillProbeLength;
    uint8_t stillCommitLength;
    uint8_t stillProbeInfo;
    uint8_t stillCommitInfo;
    uint8_t stillImageTransmission;
    uint8_t stillImageTriggerControl;
    uint8_t currentConfiguration;
    uint8_t currentInterfaceAlternateSetting[USB_VIDEO_UVC_CAMERA_INTERFACE_COUNT];
    uint8_t speed;
    uint8_t attach;
} usb_video_uvc_camera_struct_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#endif /* __USB_VIDEO_UVC_CAMERA_H__ */
