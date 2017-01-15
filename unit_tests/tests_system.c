/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "tests_system.h"
#include "kn_timer.h"
#include "kn_sched.h"
#include "kn_tick.h"
#include "thread_helper.h"

#include <check.h>
#include <stdio.h>
#include <stdbool.h>




START_TEST(kn_system_test_timer)
{
	kn_timer_t t={};
	sem_t timer_sem;
	bool timer_called = false;

	// This is the timer callback. 
	// As it will be called in a thread under test, 
	// I'm signaling the main thread that it was called
	void timer_cb(kn_timer_t *timer, void *user_data){
		//printf("timer timer_cb\n");
		sem_post(&timer_sem);
		timer_called = true;
	}

	
	// These are needed for timer_init
	kn_sched_t sched = KN_SCHED_INIT_VALUE;
	kn_tick_controller_t tic = {};
	kn_sched_set_default(&sched);
	kn_tick_controller_set_default(&tic);
	kn_power_sched_init(&sched, &tic);
	//
	sem_init(&timer_sem, 0, 0);


	int ret=kn_timer_init(&t, timer_cb, NULL, 1);
	ck_assert(ret==0);

	ret=kn_timer_start(&t, TICK_TO_MS(2));
	ck_assert(ret==0);

	void command_thread(sem_t *sem){
		// This first wait should timeout because I didn't send a tick yet
		int ret=sem_timeoutwait(&timer_sem, 10);
		ck_assert(ret<0);
		kn_platform_linux_simulate_tick();
		//Same
		ret=sem_timeoutwait(&timer_sem, 10);
		ck_assert(ret<0);

		kn_platform_linux_simulate_tick();
		//2 ticks sent, this wait should not timeout
		ret=sem_timeoutwait(&timer_sem, 100);
		ck_assert(ret==0);

		//One more time for fun
		kn_platform_linux_simulate_tick();
		ret=sem_timeoutwait(&timer_sem, 10);
		ck_assert(ret<0);		
		kn_platform_linux_simulate_tick();
		ret=sem_timeoutwait(&timer_sem, 100);
		ck_assert(ret==0);		

		// Force other thread to get out of sleep
		kn_platform_linux_simulate_tick();
		kn_platform_linux_simulate_tick();
	}

	void thread_under_test(sem_t *sem){
		// 1st call : this one should do nothing and sleep only (for 2 ticks)
		int t = kn_power_sched_run();
		ck_assert(t==2);
		ck_assert(timer_called==false);

		// 2nd call : this one should call the timer callback
		kn_power_sched_run();
		ck_assert(timer_called==true);

		timer_called=false;
		kn_power_sched_run();
		ck_assert(timer_called==true);
	}

	run_threaded_test(command_thread, thread_under_test);
}
END_TEST


int tests_system_run(void){
	Suite *s1 = suite_create("System");
	TCase *tc1_1 = tcase_create("System");
	SRunner *sr = srunner_create(s1);
	int nf;



	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, kn_system_test_timer);

	kn_platform_init();

	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	kn_platform_exit();

	return nf == 0 ? 0 : 1;
}
