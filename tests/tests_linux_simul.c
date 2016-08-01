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
#include <semaphore.h>
#include <time.h>


int sem_timeoutwait(sem_t *sem, int timeout_ms){
	struct timespec time_limit={};
	if (!sem){
		return -1;
	}

	clock_gettime(CLOCK_REALTIME, &time_limit);
	printf ("time_limit sec %ld nsec %ld\n", time_limit.tv_sec, time_limit.tv_nsec);
	long tmp = time_limit.tv_nsec+timeout_ms*1000000;
	time_limit.tv_sec += tmp/1000000000;
	time_limit.tv_nsec = tmp%1000000000;
	printf ("time_limit sec %ld nsec %ld\n", time_limit.tv_sec, time_limit.tv_nsec);


	return sem_timedwait(sem, &time_limit);
}


START_TEST(kn_linux_test_sleep_forever)
{
	pthread_t thread;
	bool wokeup=false;
	void *ret_val;
	sem_t sem;


	void *thread_func(void *dummy){
		int ret=kn_platform_sleep_forever();
		ck_assert(ret==0);
		wokeup=true;
		sem_post(&sem);
		return NULL;
	}

	sem_init(&sem, 0, 0);

	pthread_create(&thread, NULL, thread_func, NULL);

	kn_platform_linux_simulate_interrupt();
	int ret=sem_timeoutwait(&sem, 100);
	//int ret=sem_wait(&sem);

	ck_assert(ret==0);

	pthread_join(thread, &ret_val);
	ck_assert(wokeup==true);

	sem_destroy(&sem);
}
END_TEST



START_TEST(kn_linux_test_sleep_1tick)
{
	pthread_t thread;
	bool wokeup=false;
	void *ret_val;
	sem_t sem;


	void *thread_func(void *dummy){
		int ret=kn_platform_sleep_for(1);
		ck_assert(ret==0);
		wokeup=true;
		sem_post(&sem);
		return NULL;
	}

	sem_init(&sem, 0, 0);

	pthread_create(&thread, NULL, thread_func, NULL);

	kn_platform_linux_simulate_tick();
	int ret=sem_timeoutwait(&sem, 100);

	ck_assert(ret==0);

	pthread_join(thread, &ret_val);
	ck_assert(wokeup==true);

	sem_destroy(&sem);
}
END_TEST


START_TEST(kn_linux_test_sleep_100tick)
{
	pthread_t thread;
	bool wokeup=false;
	void *ret_val;
	sem_t sem;
	int i;

	void *thread_func(void *dummy){
		int ret=kn_platform_sleep_for(100);
		ck_assert(ret==0);
		wokeup=true;
		sem_post(&sem);
		return NULL;
	}

	sem_init(&sem, 0, 0);

	pthread_create(&thread, NULL, thread_func, NULL);

	for(i=0; i<100; i++){
		kn_platform_linux_simulate_tick();
	}
	int ret=sem_timeoutwait(&sem, 100);

	ck_assert(ret==0);

	pthread_join(thread, &ret_val);
	ck_assert(wokeup==true);

	sem_destroy(&sem);
}
END_TEST


START_TEST(kn_linux_test_sleep_1tick_interrupt)
{
	pthread_t thread;
	bool wokeup=false;
	void *ret_val;
	sem_t sem;


	void *thread_func(void *dummy){
		int ret=kn_platform_sleep_for(1);
		ck_assert(ret==0);
		wokeup=true;
		sem_post(&sem);
		return NULL;
	}

	sem_init(&sem, 0, 0);

	pthread_create(&thread, NULL, thread_func, NULL);

	kn_platform_linux_simulate_interrupt();
	int ret=sem_timeoutwait(&sem, 100);

	ck_assert(ret==0);

	pthread_join(thread, &ret_val);
	ck_assert(wokeup==true);

	sem_destroy(&sem);
}
END_TEST


START_TEST(kn_linux_test_sleep_doesnt_exit)
{
	pthread_t thread;
	bool wokeup=false;
	void *ret_val;
	sem_t sem;


	void *thread_func(void *dummy){
		int ret=kn_platform_sleep_for(1);
		ck_assert(ret==0);
		wokeup=true;
		sem_post(&sem);
		return NULL;
	}

	sem_init(&sem, 0, 0);

	pthread_create(&thread, NULL, thread_func, NULL);

	// This wait should timeout because we didn't interrupt or send tick
	int ret=sem_timeoutwait(&sem, 100);
	ck_assert(ret<0);

	kn_platform_linux_simulate_interrupt();
	//This wait should not timeout
	ret=sem_timeoutwait(&sem, 100);
	ck_assert(ret==0);


	pthread_join(thread, &ret_val);
	ck_assert(wokeup==true);

	sem_destroy(&sem);
}
END_TEST


START_TEST(kn_linux_test_sleep_1tick_doesnt_exit)
{
	pthread_t thread;
	bool wokeup=false;
	void *ret_val;
	sem_t sem;


	void *thread_func(void *dummy){
		int ret=kn_platform_sleep_for(2);
		ck_assert(ret==0);
		wokeup=true;
		sem_post(&sem);
		return NULL;
	}

	sem_init(&sem, 0, 0);

	pthread_create(&thread, NULL, thread_func, NULL);

	kn_platform_linux_simulate_tick();
	//This one should timeout
	int ret=sem_timeoutwait(&sem, 10);
	ck_assert(ret<0);

	kn_platform_linux_simulate_tick();
	ret=sem_timeoutwait(&sem, 100);

	ck_assert(ret==0);

	pthread_join(thread, &ret_val);
	ck_assert(wokeup==true);

	sem_destroy(&sem);
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