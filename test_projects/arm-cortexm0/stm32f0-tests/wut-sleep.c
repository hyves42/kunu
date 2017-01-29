/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2017 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

// Test sleep with wake up timer interrupt

// Tested NOT functional on stm30f030f4, because WUT is not present in chip.
// needs to be tested on an other stm32 variant


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




// For setup and wakeup setup explanations, please refer to AN3371 "Using the hardware RTC in STM32"
static int setup_rtc(void){
	/* turn on power block to enable unlocking */
	rcc_periph_clock_enable(RCC_PWR);
	rcc_periph_clock_enable(RCC_RTC);
	// rcc_periph_clock_enable(RCC_CRS);
	// rcc_periph_clock_enable(RCC_SRAM);
	pwr_disable_backup_domain_write_protect();


	RCC_BDCR |= RCC_BDCR_BDRST;
	RCC_BDCR &= ~RCC_BDCR_BDRST;


	// We want to use the low speed internal oscillator
	rcc_osc_on(RCC_LSI);
	rcc_wait_for_osc_ready(RCC_LSI);

	// I need to set both RTCEN and internal clock before entering init mode
	// Else I get stuck waiting for RTC_ISR_INITF
	RCC_BDCR |= RCC_BDCR_RTCEN;
	RCC_BDCR |= RCC_BDCR_RTCSEL_LSI;


	rtc_unlock();


	// enter init mode
	RTC_ISR |= RTC_ISR_INIT;
	while ((RTC_ISR & RTC_ISR_INITF) != RTC_ISR_INITF);

	// set synch prescaler, using defaults for 1Hz out
	uint32_t sync = 255;
	uint32_t async = 127;
	rtc_set_prescaler(sync, async);


	//useful ?
	RTC_CR |= (RTC_CR_OSEL_WAKEUP << RTC_CR_OSEL_SHIFT);

	// exit init mode 
	RTC_ISR &= ~(RTC_ISR_INIT);
	rtc_lock();

	rtc_wait_for_synchro();
	return 0;
}



/* For chip variants that support it, configure the periodic wake up timer
  currently only supported on STM32F070xB and STM32F030xC
  Return 0 on success , -1 on error, -2 if WUT not supported
  Unfortunately, I only tested this function on chips that don't support WUT.
  quite similar to  rtc_set_wakeup_time with more error checks
*/
static int kn_rtc_wakeuptimer_enable(uint16_t period){
	int ret =0;
	int safe_cnt=1000;
	
	rtc_unlock();


	// Disable WUT (required to allow write access to WUT registers)
	RTC_CR &= ~RTC_CR_WUTE;

	// Wait for RTC WUT registers to be writeable
	safe_cnt=1000;
	while ((RTC_ISR & RTC_ISR_WUTWF) == 0){
		if (--safe_cnt ==0){
			ret=-1;
			goto bail;
		}
	}

	RTC_WUTR = period;
	// Not all variants of STM32F0 implement the wake up timer
	// Let's check that we actually wrote the period in the register.
	if (RTC_WUTR != period){
		// Well this chip doesn't implement the wake up timer. Abort
		ret=-2;
		goto bail;
	}

	// WUCKSEL[2:0] = 0xxb for short wakeup periods
	RTC_CR &= ~(RTC_CR_WUCLKSEL_MASK << RTC_CR_WUCLKSEL_SHIFT);
	RTC_CR |= (RTC_CR_WUCLKSEL_RTC_DIV2 << RTC_CR_WUCLKSEL_SHIFT);

	// Restart WakeUp unit and activate interrupt
	RTC_CR |= RTC_CR_WUTE | RTC_CR_WUTIE;

	
	// WUT is wired to EXTI20, enable it
	exti_set_trigger(EXTI20, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI20);
	nvic_enable_irq(NVIC_RTC_IRQ);
	
bail:
	// Always lock rtc back before leaving
	rtc_lock();

	return ret;
}




void rtc_isr(void){
	rtc_clear_wakeup_flag();
	//gpio_toggle(PORT_LED, PIN_LED);
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

void exti0_1_isr(void) {
	EXTI_PR&=2;
	gpio_toggle(PORT_LED, PIN_LED);
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

	setup_rtc();

	int ret=kn_rtc_wakeuptimer_enable(0xFFF);
	if (ret<0){
		gpio_toggle(PORT_LED, PIN_LED);
		return;
	}

	

	cm_enable_interrupts();

	while (1) {
		//The STM32F0 can be woken-up from STANDBY mode by a few special WKUP pins (rising edge), IWDG reset, external reset on NRST pin, RTC alarm.
		pwr_set_standby_mode();
		pwr_clear_wakeup_flag();

		pwr_set_stop_mode();
		__WFI();


		clock_setup();
		rcc_periph_clock_enable(RCC_GPIOA);

		gpio_toggle(PORT_LED, PIN_LED);	/* LED on/off */
	}
	return 0;
}
