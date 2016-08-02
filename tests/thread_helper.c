/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

/* Helper to run multi threaded tests 
 */

#include "thread_helper.h"

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
	long tmp = time_limit.tv_nsec+timeout_ms*1000000;
	time_limit.tv_sec += tmp/1000000000;
	time_limit.tv_nsec = tmp%1000000000;

	return sem_timedwait(sem, &time_limit);
}


int run_threaded_test(void (*command_thread)(sem_t *sem), void (*thread_under_test)(sem_t *sem)){
	pthread_t thread;
	void *ret_val;
	sem_t sem;
	void *thread_func(void *dummy){
		thread_under_test(&sem);
		return NULL;
	}

	if (!command_thread || !thread_under_test){
		return -1;
	}

	sem_init(&sem, 0, 0);

	pthread_create(&thread, NULL, thread_func, NULL);

	command_thread(&sem);

	pthread_join(thread, &ret_val);

	sem_destroy(&sem);

	return 0;
}