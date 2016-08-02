/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef THREAD_HELPER_H
#define THREAD_HELPER_H

#include <semaphore.h>

//Similar to sem_timedwait() but takes a relative timeout in ms
int sem_timeoutwait(sem_t *sem, int timeout_ms);

// This helper creates a thread and runs the function thread_under_test_in it.
// A semaphore is provisioned for your ease to synchronise the steps 
// in command_thead() and thread_under_test()

// Example :
// 

// void command_thread(sem_t *sem){
// 	..do something to trigger an action in thread_under_test
//  sem_timeoutwait(sem, 100);
// 	.. test result
// }
// void thread_under_test(sem_t *sem){
// 	.. wait for an order and execute something
// 	sem_post(sem); // notify main thread it's done
// }
// run_threaded_test(command_thread, thread_under_test);
//
int run_threaded_test(void (*command_thread)(sem_t *sem), void (*thread_under_test)(sem_t *sem));


#endif //THREAD_HELPER_H