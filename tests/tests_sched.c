/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "tests_sched.h"
#include "kn_sched.h"
#include <check.h>
#include <stdio.h>
#include <stdbool.h>


static kn_schedulable_return_t dummy_schedule(kn_schedulable_t *s, void* user_data){}
static int dummy_remaining_ticks(kn_schedulable_t *s, void* user_data){}


START_TEST(kn_sched_tests_init)
{
	kn_sched_t sched=KN_SCHED_INIT_VALUE;

	ck_assert(sched.schedulable_count == 0);
	ck_assert(sched.schedulables[0] == NULL);
	ck_assert(sched.schedulables[2] == NULL);	
}
END_TEST


START_TEST(kn_sched_tests_add_schedulables)
{
	kn_sched_t sched=KN_SCHED_INIT_VALUE;
	int i;

	kn_schedulable_t schedable1={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=1,
	};

	kn_schedulable_t schedable2={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=2,
	};

	kn_schedulable_t schedable3={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=3,
	};

	kn_schedulable_t schedable4={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=4,
	};

	kn_schedulable_t schedable5={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=5,
	};

	int ret=kn_sched_add_schedulable(&sched, &schedable1);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable2);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable3);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable4);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable5);
	ck_assert(ret == 0);

	ck_assert(sched.schedulable_count == 5);

	// Check schedulables initialisation
	for (i=0; i<sched.schedulable_count; i++){
		ck_assert(sched.schedulables[i]->user_data==(void*)0xdeadbeef);
	}

	// Check array order
	for (i=1; i<sched.schedulable_count; i++){
		ck_assert(sched.schedulables[i]->priority>=sched.schedulables[i-1]->priority);
	}

}
END_TEST


START_TEST(kn_sched_tests_add_schedulables2)
{
	kn_sched_t sched=KN_SCHED_INIT_VALUE;
	int i;

	kn_schedulable_t schedable1={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=1,
	};

	kn_schedulable_t schedable2={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=2,
	};

	kn_schedulable_t schedable3={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=3,
	};

	kn_schedulable_t schedable4={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=4,
	};

	kn_schedulable_t schedable5={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=5,
	};

	int ret=kn_sched_add_schedulable(&sched, &schedable5);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable3);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable1);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable2);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable4);
	ck_assert(ret == 0);

	ck_assert(sched.schedulable_count == 5);

	// Check schedulables initialisation
	for (i=0; i<sched.schedulable_count; i++){
		ck_assert(sched.schedulables[i]->user_data==(void*)0xdeadbeef);
		//printf("priority[%d] %d\n", i, sched.schedulables[i].priority);
	}

	// Check array order
	for (i=1; i<sched.schedulable_count; i++){
		ck_assert(sched.schedulables[i]->priority>=sched.schedulables[i-1]->priority);
	}

}
END_TEST


START_TEST(kn_sched_tests_add_schedulables_limit)
{
	kn_sched_t sched=KN_SCHED_INIT_VALUE;
	int i, ret;

	kn_schedulable_t schedable={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=1,
	};

	for (i=0; i<SCHEDULABLES_SIZE; i++){
		ret=kn_sched_add_schedulable(&sched, &schedable);
		ck_assert(ret == 0);
	}

	ret=kn_sched_add_schedulable(&sched, &schedable);
	ck_assert(ret < 0);

}
END_TEST

START_TEST(kn_sched_tests_add_bad_schedulables)
{
	kn_sched_t sched=KN_SCHED_INIT_VALUE;
	int i, ret;

	kn_schedulable_t schedable={
		.schedule=NULL,
		.remaining_ticks=NULL,
		.user_data=NULL,
		.priority=1,
	};

	ret=kn_sched_add_schedulable(&sched, &schedable);
	ck_assert(ret < 0);

}
END_TEST


START_TEST(kn_sched_tests_remove_schedulables)
{
	kn_sched_t sched=KN_SCHED_INIT_VALUE;
	int i;

	kn_schedulable_t schedable1={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=1,
	};

	kn_schedulable_t schedable2={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=2,
	};

	kn_schedulable_t schedable3={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=3,
	};

	kn_schedulable_t schedable4={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=4,
	};

	kn_schedulable_t schedable5={
		.schedule=dummy_schedule,
		.remaining_ticks=dummy_remaining_ticks,
		.user_data=(void*)0xdeadbeef,
		.priority=5,
	};

	int ret=kn_sched_add_schedulable(&sched, &schedable1);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable2);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable3);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable4);
	ck_assert(ret == 0);
	ret=kn_sched_add_schedulable(&sched, &schedable5);
	ck_assert(ret == 0);

	ck_assert(sched.schedulable_count == 5);

	ret=kn_sched_remove_schedulable(&sched, &schedable3);
	ck_assert(ret == 0);
	ck_assert(sched.schedulable_count == 4);
	ck_assert(sched.schedulables[0]==&schedable1);
	ck_assert(sched.schedulables[1]==&schedable2);
	ck_assert(sched.schedulables[2]==&schedable4);
	ck_assert(sched.schedulables[3]==&schedable5);
	ck_assert(sched.schedulables[4]==NULL);

}
END_TEST

int tests_sched_run(void){
	Suite *s1 = suite_create("Sched");
	TCase *tc1_1 = tcase_create("Sched");
	SRunner *sr = srunner_create(s1);
	int nf;

	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, kn_sched_tests_init);
	tcase_add_test(tc1_1, kn_sched_tests_add_schedulables);
	tcase_add_test(tc1_1, kn_sched_tests_add_schedulables2);
	tcase_add_test(tc1_1, kn_sched_tests_add_schedulables_limit);
	tcase_add_test(tc1_1, kn_sched_tests_add_bad_schedulables);
	tcase_add_test(tc1_1, kn_sched_tests_remove_schedulables);

	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	return nf == 0 ? 0 : 1;
}
