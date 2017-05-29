/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2017 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

// Test the uart in non blocking mode, using the ISR for receiving data and echoing it back
// GPIO A0 is toggled on each end of message
// DMA not used in this example
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
#include <libopencm3/cm3/scb.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static inline __attribute__((always_inline)) void __WFI(void){
	__asm volatile ("wfi");
}


static volatile char c = 'Q';
static volatile bool received = false;



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
	nvic_enable_irq(NVIC_USART1_IRQ);

	rcc_periph_clock_enable(RCC_USART1);

	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_stopbits(USART1, USART_CR2_STOP_1_0BIT);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	// Interrupt on RX not empty
	USART_CR1(USART1) |= USART_CR1_RXNEIE;

	// Interrupt on RTOF= receiver timeout
	USART_CR2(USART1) |= USART_CR2_RTOEN;

	usart_enable(USART1);
}


static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	// GPIO A0 and A! are output LED
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);


	// GPIO A9 is uart TX
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO9);

	// GPIO A10 is uart RX
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10);
	gpio_set_af(GPIOA, GPIO_AF1, GPIO10);
}


int main(void)
{

	clock_setup();

	gpio_setup();
	usart_setup();

	cm_enable_interrupts();

	while (1) {
		__WFI();
		// Something was received
		if (received){
			//gpio_toggle(GPIOA, GPIO1);
			USART_CR1(USART1) |= USART_CR1_TXEIE;
			received=false;
		}

	}

	return 0;
}




void usart1_isr(void)
{

	// Read data register not empty : RX data triggered the interrupt
	if ((USART_CR1(USART1) & USART_CR1_RXNEIE)
		&& (USART_ISR(USART1) & USART_ISR_RXNE)) {

		c = usart_recv(USART1);

		received=true;
	}

	// Ready to send data
	if ((USART_CR1(USART1) & USART_CR1_TXEIE)
		&& (USART_ISR(USART1) & USART_ISR_TXE)) {

		usart_send(USART1, c);

		USART_CR1(USART1) &= ~USART_CR1_TXEIE;
	}

	// Receive timeout = end of rx message
	if ((USART_CR2(USART1) & USART_CR2_RTOEN)
		&& (USART_ISR(USART1) & USART_ISR_RTOF)){
			USART_ICR(USART1) |= USART_ICR_RTOCF;

			gpio_toggle(GPIOA, GPIO0);

	}
}

