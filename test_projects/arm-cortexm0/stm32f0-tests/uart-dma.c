/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2017 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

// Test the uart in blocking mode
// Tested functional on stm30f030f4


#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/f0/nvic.h>


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>



// Setup external 8MHz oscillator
static void clock_setup(void)
{
	rcc_osc_on(RCC_HSE);
	rcc_wait_for_osc_ready(RCC_HSE);
	rcc_set_sysclk_source(RCC_HSE);

	rcc_set_hpre(RCC_CFGR_HPRE_NODIV);
	rcc_set_ppre(RCC_CFGR_PPRE_NODIV);

	flash_set_ws(FLASH_ACR_LATENCY_024_048MHZ);

	// 8MHz * 8 / 2 = 32MHz
	rcc_set_pll_multiplication_factor(RCC_CFGR_PLLMUL_MUL8);

	RCC_CFGR &= ~RCC_CFGR_PLLSRC;

	rcc_osc_on(RCC_PLL);
	rcc_wait_for_osc_ready(RCC_PLL);
	rcc_set_sysclk_source(RCC_PLL);

	rcc_apb1_frequency = 32000000;
	rcc_ahb_frequency = 32000000;
}



// Setup and enable usart1 for 115200 bps communication
static void usart_setup(void)
{
	rcc_periph_clock_enable(RCC_USART1);

	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_stopbits(USART1, USART_CR2_STOP_1_0BIT);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	usart_enable(USART1);
}


static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	// GPIO A0 is an output LED
	// gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);

	// GPIO A9 is uart TX
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO9);
}

int main(void)
{
	int c = 0;

	clock_setup();

	gpio_setup();
	usart_setup();

	while (1) {
		usart_send_blocking(USART1, c + '0');
		if (++c > 9) c=0;
	}

	return 0;
}
