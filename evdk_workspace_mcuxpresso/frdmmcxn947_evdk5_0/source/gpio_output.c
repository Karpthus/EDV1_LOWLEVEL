/*! ***************************************************************************
 *
 * \brief     General purpose input/output - output
 * \file      gpio_output.c
 * \author    Hugo Arends
 * \date      February 2024
 *
 * \copyright 2024 HAN University of Applied Sciences. All Rights Reserved.
 *            \n\n
 *            Permission is hereby granted, free of charge, to any person
 *            obtaining a copy of this software and associated documentation
 *            files (the "Software"), to deal in the Software without
 *            restriction, including without limitation the rights to use,
 *            copy, modify, merge, publish, distribute, sublicense, and/or sell
 *            copies of the Software, and to permit persons to whom the
 *            Software is furnished to do so, subject to the following
 *            conditions:
 *            \n\n
 *            The above copyright notice and this permission notice shall be
 *            included in all copies or substantial portions of the Software.
 *            \n\n
 *            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *            OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *            NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *            HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *            WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *            FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *            OTHER DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/
#include <stdio.h>
#include <MCXN947_cm33_core0.h>

void gpio_output_init(void)
{
    // Enable module clocks
	SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_PORT0(1);
	SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_PORT1(1);
	SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_GPIO0(1);
	SYSCON->AHBCLKCTRLSET[0] = SYSCON_AHBCLKCTRL0_GPIO1(1);

    // Configure P0_10, P0_27 and P1_2
    // LK : [1] = Locks this PCR
    // INV: [0] = Does not invert
    // IBE: [0] = Disables
    // MUX: [0000] = Alternative 0 (GPIO)
    // DSE: [0] = low drive strength is configured on the corresponding pin,
    //            if the pin is configured as a digital output
    // ODE: [0] = Disables
    // SRE: [0] = Fast
    // PE:  [0] = Disables
    // PS:  [0] = n.a.
	PORT0->PCR[10] = 0x00008000; // LED_RED
	PORT0->PCR[27] = 0x00008000; // LED_GREEN
	PORT1->PCR[2] = 0x00008000; // LED_BLUE

    // PDO10: [1] = Logic level 1 – LED off
    // PDO27: [1] = Logic level 1 – LED off
    // PD12: [1] = Logic level 1 – LED off
    GPIO0->PDOR |= (1<<10);
    GPIO0->PDOR |= (1<<27);
    GPIO1->PDOR |= (1<<2);

    // PDD10: [1] = Output
    // PDD27: [1] = Output
    // PDD2: [1] = Output
    GPIO0->PDDR |= (1<<10);
    GPIO0->PDDR |= (1<<27);
    GPIO1->PDDR |= (1<<2);
}
