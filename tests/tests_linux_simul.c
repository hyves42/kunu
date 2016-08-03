/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

/* test the implementation of the linux simulation platform 
 */

#include "tests_linux_simul.h"
#include "kn_platform.h"
#include "kn_linux_sleep.h"
#include "thread_helper.h"
#include <check.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>




START_TEST(kn_linux_test_sleep_forever)
{
	bool wokeup=false;

	void command_thread(sem_t *sem){
		kn_platform_linux_simulate_interrupt();
		int ret=sem_timeoutwait(sem, 100);

		ck_assert(ret==0);
		ck_assert(wokeup==true);
	}
	
	void thread_under_test(sem_t *sem){
		int ret=kn_platform_sleep_forever();
		ck_assert(ret==0);
		wokeup=true;
		sem_post(sem);
	}


	run_threaded_test(command_thread, thread_under_test);
}
END_TEST



START_TEST(kn_linux_test_sleep_1tick)
{
	bool wokeup=false;

	void command_thread(sem_t *sem){
		kn_platform_linux_simulate_tick();
		int ret=sem_timeoutwait(sem, 100);
		ck_assert(ret==0);
		ck_assert(wokeup==true);
	}

	void thread_under_test(sem_t *sem){
		int ret=kn_platform_sleep_for(1);
		ck_assert(ret==0);
		wokeup=true;
		sem_post(sem);
	}

	run_threaded_test(command_thread, thread_under_test);
}
END_TEST


START_TEST(kn_linux_test_sleep_100tick)
{
	bool wokeup=false;
	int i;

	void command_thread(sem_t *sem){
		for(i=0; i<100; i++){
			kn_platform_linux_simulate_tick();
		}
		int ret=sem_timeoutwait(sem, 100);

		ck_assert(ret==0);
		ck_assert(wokeup==true);
	}

	void thread_under_test(sem_t *sem){
		int ret=kn_platform_sleep_for(100);
		ck_assert(ret==0);
		wokeup=true;
		sem_post(sem);
	}

	run_threaded_test(command_thread, thread_under_test);
}
END_TEST


START_TEST(kn_linux_test_sleep_1tick_interrupt)
{
	bool wokeup=false;

	void command_thread(sem_t *sem){
		kn_platform_linux_simulate_interrupt();
		int ret=sem_timeoutwait(sem, 100);

		ck_assert(ret==0);
		ck_assert(wokeup==true);
	}

	void thread_under_test(sem_t *sem){
		int ret=kn_platform_sleep_for(1);
		ck_assert(ret==0);
		wokeup=true;
		sem_post(sem);
	}

	run_threaded_test(command_thread, thread_under_test);

}
END_TEST


START_TEST(kn_linux_test_sleep_doesnt_exit)
{
	bool wokeup=false;

	void command_thread(sem_t *sem){
		// This wait should timeout because we didn't interrupt or send tick
		int ret=sem_timeoutwait(sem, 100);
		ck_assert(ret<0);

		kn_platform_linux_simulate_interrupt();
		//This wait should not timeout
		ret=sem_timeoutwait(sem, 100);
		ck_assert(ret==0);
		ck_assert(wokeup==true);
	}

	void thread_under_test(sem_t *sem){
		int ret=kn_platform_sleep_for(1);
		ck_assert(ret==0);
		wokeup=true;
		sem_post(sem);
	}

	run_threaded_test(command_thread, thread_under_test);

}
END_TEST


START_TEST(kn_linux_test_sleep_1tick_doesnt_exit)
{
	bool wokeup=false;

	void command_thread(sem_t *sem){
		kn_platform_linux_simulate_tick();
		//This one should timeout
		int ret=sem_timeoutwait(sem, 10);
		ck_assert(ret<0);

		kn_platform_linux_simulate_tick();
		ret=sem_timeoutwait(sem, 100);

		ck_assert(ret==0);
		ck_assert(wokeup==true);
	}

	void thread_under_test(sem_t *sem){
		int ret=kn_platform_sleep_for(2);
		ck_assert(ret==0);
		wokeup=true;
		sem_post(sem);
	}

	run_threaded_test(command_thread, thread_under_test);

}
END_TEST


int tests_linux_simul_run(void){
	Suite *s1 = suite_create("Linux");
	TCase *tc1_1 = tcase_create("Linux");
	SRunner *sr = srunner_create(s1);
	int nf;


	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, kn_linux_test_sleep_forever);
	tcase_add_test(tc1_1, kn_linux_test_sleep_1tick);
	tcase_add_test(tc1_1, kn_linux_test_sleep_100tick);
	tcase_add_test(tc1_1, kn_linux_test_sleep_1tick_interrupt);
	tcase_add_test(tc1_1, kn_linux_test_sleep_doesnt_exit);
	tcase_add_test(tc1_1, kn_linux_test_sleep_1tick_doesnt_exit);

	kn_platform_init();

	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	kn_platform_exit();

	return nf == 0 ? 0 : 1;
}