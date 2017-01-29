/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2017 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

// Minimal experiment program to test the systick interrupt wake up function of the STM32f0 chip
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


#define PORT_LED GPIOA
#define PIN_LED GPIO0


volatile uint32_t system_millis;

static inline __attribute__((always_inline)) void __WFI(void){
	__asm volatile ("wfi");
}

static inline __attribute__((always_inline)) void __WFE(void){
	__asm volatile ("wfe");
}

static inline __attribute__((always_inline)) void __NOP(void){
	__asm volatile ("nop");
}



// Called when systick fires
void sys_tick_handler(void)
{
	system_millis++;
}

// sleep for delay milliseconds 
static void msleep(uint32_t delay)
{
	system_millis=0;
	uint32_t wake = system_millis + delay;
	pwr_set_standby_mode();
	rcc_periph_clock_disable(RCC_PWR);
	rcc_periph_clock_disable(RCC_RTC);
	while (wake > system_millis){
		__WFI();
	}
	rcc_periph_clock_enable(RCC_PWR);
	rcc_periph_clock_enable(RCC_RTC);
}

// Setup systick timer for a tick every 1ms
static void systick_setup(void)
{
	systick_set_reload(32000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();
	systick_interrupt_enable();
}

// Setup gpios for led blinking
static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_mode_setup(PORT_LED, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, PIN_LED);

	// PA10 is also the uart RX. 
	// Configure it to output so that the corresponding LED doesn't blink on my dev board
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10);

}


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

int main(void)
{
	int i;

	clock_setup();

	gpio_setup();

	systick_setup();

	cm_enable_interrupts();

	while (1) {
		msleep(100);
		gpio_toggle(PORT_LED, PIN_LED);
	}
	return 0;
}
