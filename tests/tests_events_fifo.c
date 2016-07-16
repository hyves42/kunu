/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "tests_events_fifo.h"
#include "kn_events_fifo.h"
#include <check.h>
#include <stdio.h>
#include <stdbool.h>

START_TEST(kn_fifo_tests_init)
{
	kn_event_t buf[FIFO_SIZE_4];
	kn_fifo_t fifo={};

	int ret=kn_fifo_init(&fifo, buf, FIFO_SIZE_4);

	ck_assert(ret == 0);
}
END_TEST


START_TEST(kn_fifo_tests_post_limit)
{
	kn_event_t buf[FIFO_SIZE_4];
	kn_fifo_t fifo={};
	kn_event_t event={
		.id=42
	};
	kn_event_t pop_event={};
	int ret=kn_fifo_init(&fifo, buf, FIFO_SIZE_4);
	ck_assert(ret == 0);

	ret=kn_fifo_post(&fifo, &event);
	ck_assert(ret == 0);
	ret=kn_fifo_post(&fifo, &event);
	ck_assert(ret == 0);
	ret=kn_fifo_post(&fifo, &event);
	ck_assert(ret == 0);
	//Fifo is full, this one should fail
	ret=kn_fifo_post(&fifo, &event);
	ck_assert(ret < 0);

	ck_assert(kn_fifo_count(&fifo)==3);
	ck_assert(kn_fifo_free(&fifo)==0);

	pop_event.id=0;
	ret=kn_fifo_get(&fifo, &pop_event);
	ck_assert(ret == 0);
	ck_assert(pop_event.id == 42);
	pop_event.id=0;
	ret=kn_fifo_get(&fifo, &pop_event);
	ck_assert(ret == 0);
	ck_assert(pop_event.id == 42);
	pop_event.id=0;
	ret=kn_fifo_get(&fifo, &pop_event);
	ck_assert(ret == 0);
	ck_assert(pop_event.id == 42);

	ret=kn_fifo_get(&fifo, &pop_event);
	ck_assert(ret < 0);
}
END_TEST


START_TEST(kn_fifo_tests_post_wrap)
{
	kn_event_t buf[FIFO_SIZE_8];
	kn_fifo_t fifo={};
	kn_event_t event={
		.id=42
	};
	kn_event_t pop_event={};
	int i;
	int ret=kn_fifo_init(&fifo, buf, FIFO_SIZE_8);
	ck_assert(ret == 0);

	for (i=0; i<60; i++){
		ret=kn_fifo_post(&fifo, &event);
		ck_assert(ret == 0);

		ret=kn_fifo_post(&fifo, &event);
		ck_assert(ret == 0);

		ret=kn_fifo_post(&fifo, &event);
		ck_assert(ret == 0);

		ck_assert(kn_fifo_count(&fifo)==3);
		ck_assert(kn_fifo_free(&fifo)==4);

		pop_event.id=0;
		ret=kn_fifo_get(&fifo, &pop_event);
		ck_assert(ret == 0);
		ck_assert(pop_event.id == 42);

		pop_event.id=0;
		ret=kn_fifo_get(&fifo, &pop_event);
		ck_assert(ret == 0);
		ck_assert(pop_event.id == 42);

		pop_event.id=0;
		ret=kn_fifo_get(&fifo, &pop_event);
		ck_assert(ret == 0);
		ck_assert(pop_event.id == 42);
	}
}
END_TEST



int tests_events_fifo_run(void){
	Suite *s1 = suite_create("Events Fifo");
	TCase *tc1_1 = tcase_create("Events Fifo");
	SRunner *sr = srunner_create(s1);
	int nf;

	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, kn_fifo_tests_init);
	tcase_add_test(tc1_1, kn_fifo_tests_post_limit);
	tcase_add_test(tc1_1, kn_fifo_tests_post_wrap);


	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	return nf == 0 ? 0 : 1;
}