/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "tests_system.h"
#include "kn_sched.h"
#include "thread_helper.h"

#include <check.h>
#include <stdio.h>
#include <stdbool.h>




START_TEST(kn_system_test_init)
{
	bool wokeup=false;

	// These are needed for timer_init
	kn_sched_t sched = SCHED_INIT_VALUE;
	kn_tick_controller_t tic = {};
	kn_sched_set_default(&sched);
	kn_tick_controller_set_default(&tic);
	//



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


int tests_system_run(void){
	Suite *s1 = suite_create("System");
	TCase *tc1_1 = tcase_create("System");
	SRunner *sr = srunner_create(s1);
	int nf;



	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, kn_system_test_init);

	kn_platform_init();

	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	kn_platform_exit();

	return nf == 0 ? 0 : 1;
}
