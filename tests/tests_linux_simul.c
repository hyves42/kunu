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
#include <check.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>


START_TEST(kn_linux_test1)
{
	pthread_t thread;
	bool wokeup=false;
	void *ret_val;
	void *thread_func(void *dummy){
		int ret=kn_platform_sleep_forever();
		ck_assert(ret==0);
		wokeup=true;
		return NULL;
	}

    pthread_create(&thread, NULL, thread_func, NULL);

    kn_platform_linux_simulate_interrupt();


    pthread_join(thread, &ret_val);
    ck_assert(wokeup==true);
}
END_TEST

int tests_linux_simul_run(void){
	Suite *s1 = suite_create("Linux");
	TCase *tc1_1 = tcase_create("Linux");
	SRunner *sr = srunner_create(s1);
	int nf;


	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, kn_linux_test1);


	kn_platform_init();

	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	kn_platform_exit();

	return nf == 0 ? 0 : 1;
}