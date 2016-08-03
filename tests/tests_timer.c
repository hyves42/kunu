/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "tests_timer.h"
#include "kn_timer.h"

#include "kn_sched.h"
#include "kn_tick.h"
#include <check.h>
#include <stdio.h>
#include <stdbool.h>




START_TEST(timer_tests_init)
{
	int timer_cb(kn_timer_t *timer, void *user_data){}
	kn_timer_t t={};

	// These are needed for timer_init
	kn_sched_t sched = SCHED_INIT_VALUE;
	kn_tick_controller_t tic = {};
	kn_sched_set_default(&sched);
	kn_tick_controller_set_default(&tic);
	//


	int ret=kn_timer_init(&t, timer_cb, NULL, 1);


	ck_assert(ret == 0);


	//verify that interfaces are correctly initialized
	ck_assert(KN_TIMER_GET_SCHEDULABLE(&t).schedule != NULL);
	ck_assert(KN_TIMER_GET_SCHEDULABLE(&t).remaining_ticks != NULL);
	ck_assert(KN_TIMER_GET_SCHEDULABLE(&t).priority == 1);
	ck_assert(KN_TIMER_GET_SCHEDULABLE(&t).state == SLEEPING);
	ck_assert(t.is_armed == false);
	ck_assert(KN_TIMER_GET_TICK_CLIENT(&t).tick != NULL);
	//TODO verify that timer is in tick_controller list and in sched list
}
END_TEST


START_TEST(timer_tests_simple_sched)
{
	kn_timer_t t={};
	bool cb_called=false;
	kn_schedulable_t *s=&(KN_TIMER_GET_SCHEDULABLE(&t));

	int timer_cb(kn_timer_t *timer, void *user_data){
		ck_assert(timer == &t);
		ck_assert(user_data == (void*)0xdeadbeef);
		cb_called=true;
	}


	// These are needed for timer_init
	kn_sched_t sched = SCHED_INIT_VALUE;
	kn_tick_controller_t tic = {};
	kn_sched_set_default(&sched);
	kn_tick_controller_set_default(&tic);
	//


	int ret=kn_timer_init(&t, timer_cb, (void*)0xdeadbeef, 1);


	ck_assert(ret == 0);

	ck_assert(s->remaining_ticks(s, s->user_data) < 0);

	ret= kn_timer_start(&t, TICK_TO_MS(1));
	ck_assert(ret == 0);
	ck_assert(s->state == SLEEPING);
	ck_assert(t.is_armed == true);
	ck_assert(s->remaining_ticks(s, s->user_data) == 1);

	KN_TIMER_GET_TICK_CLIENT(&t).tick(1, KN_TIMER_GET_TICK_CLIENT(&t).user_data);

	ck_assert(s->state == NEEDS_SCHEDULE);
	ck_assert(s->remaining_ticks(s, s->user_data) == 0);

	kn_schedulable_return_t r = s->schedule(s, s->user_data);
	ck_assert(r == DID_SOMETHING);
	ck_assert(cb_called==true);

	// normally it should not be scheduled until another tick happens
	cb_called=false;
	ck_assert(s->state == SLEEPING);
	ck_assert(t.is_armed == true);
	ck_assert(s->remaining_ticks(s, s->user_data) == 1);

	r = s->schedule(s, s->user_data);
	ck_assert(r == DID_NOTHING);
	ck_assert(cb_called==false);
	//TODO verify that timer is in tick_controller list and in sched list
}
END_TEST


int tests_timer_run(void){
	Suite *s1 = suite_create("Sched");
	TCase *tc1_1 = tcase_create("Sched");
	SRunner *sr = srunner_create(s1);
	int nf;

	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, timer_tests_init);
	tcase_add_test(tc1_1, timer_tests_simple_sched);

	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	return nf == 0 ? 0 : 1;
}
