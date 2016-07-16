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
#include <check.h>
#include <stdio.h>
#include <stdbool.h>


static void m1_top_state_init(kn_state_t *self, kn_state_machine_t *m){
}
static void m1_top_state_enter(kn_state_t *self, kn_state_machine_t *m){
}
static void m1_top_state_quit(kn_state_t *self, kn_state_machine_t *m){
}
static int m1_top_state_on_event(kn_state_t *self, kn_state_machine_t *m, event_t *event){
	return KN_EVENT_HANDLED;
}
static void m1_state0_init(kn_state_t *self, kn_state_machine_t *m){
}
static void m1_state0_enter(kn_state_t *self, kn_state_machine_t *m){
}
static void m1_state0_quit(kn_state_t *self, kn_state_machine_t *m){
}
static int m1_state0_on_event(kn_state_t *self, kn_state_machine_t *m, event_t *event){
	return KN_EVENT_HANDLED;
}
static void m1_state1_init(kn_state_t *self, kn_state_machine_t *m){
}
static void m1_state1_enter(kn_state_t *self, kn_state_machine_t *m){
}
static void m1_state1_quit(kn_state_t *self, kn_state_machine_t *m){
}
static int m1_state1_on_event(kn_state_t *self, kn_state_machine_t *m, event_t *event){
	return KN_EVENT_HANDLED;
}
static void m1_state2_init(kn_state_t *self, kn_state_machine_t *m){
}
static void m1_state2_enter(kn_state_t *self, kn_state_machine_t *m){
}
static void m1_state2_quit(kn_state_t *self, kn_state_machine_t *m){
}
static int m1_state2_on_event(kn_state_t *self, kn_state_machine_t *m, event_t *event){
	return KN_EVENT_HANDLED;
}





static void m2_top_state_init(kn_state_t *self, kn_state_machine_t *m){
}
static void m2_top_state_enter(kn_state_t *self, kn_state_machine_t *m){
}
static void m2_top_state_quit(kn_state_t *self, kn_state_machine_t *m){
}
static int m2_top_state_on_event(kn_state_t *self, kn_state_machine_t *m, event_t *event){
	return KN_EVENT_HANDLED;
}
static void m2_state0_init(kn_state_t *self, kn_state_machine_t *m){
}
static void m2_state0_enter(kn_state_t *self, kn_state_machine_t *m){
}
static void m2_state0_quit(kn_state_t *self, kn_state_machine_t *m){
}
static int m2_state0_on_event(kn_state_t *self, kn_state_machine_t *m, event_t *event){
	return KN_EVENT_HANDLED;
}
static void m2_state1_init(kn_state_t *self, kn_state_machine_t *m){
}
static void m2_state1_enter(kn_state_t *self, kn_state_machine_t *m){
}
static void m2_state1_quit(kn_state_t *self, kn_state_machine_t *m){
}
static int m2_state1_on_event(kn_state_t *self, kn_state_machine_t *m, event_t *event){
	return KN_EVENT_HANDLED;
}
static void m2_state2_init(kn_state_t *self, kn_state_machine_t *m){
}
static void m2_state2_enter(kn_state_t *self, kn_state_machine_t *m){
}
static void m2_state2_quit(kn_state_t *self, kn_state_machine_t *m){
}
static int m2_state2_on_event(kn_state_t *self, kn_state_machine_t *m, event_t *event){
	return KN_EVENT_HANDLED;
}


enum MACHINE1_STATES{
	MACHINE1_STATE_0,
	MACHINE1_STATE_1,
	MACHINE1_STATE_2,
	MACHINE1_STATES_COUNT
};

enum MACHINE2_STATES{
	MACHINE2_STATE_0,
	MACHINE2_STATE_1,
	MACHINE2_STATE_2,
	MACHINE2_STATES_COUNT
};

kn_state_t machine1_states[MACHINE1_STATES_COUNT]={
	[MACHINE1_STATE_0]=STATE_INIT(m1_state0),
	[MACHINE1_STATE_1]=STATE_INIT(m1_state1),
	[MACHINE1_STATE_2]=STATE_INIT(m1_state2),
};

kn_state_t machine2_states[MACHINE2_STATES_COUNT]={
	[MACHINE2_STATE_0]=STATE_INIT(m2_state0),
	[MACHINE2_STATE_1]=STATE_INIT(m2_state1),
	[MACHINE2_STATE_2]=STATE_INIT(m2_state2),
};

kn_state_machine_t machine1=KN_STATE_MACHINE_INIT(m1_top_state, machine1_states);
kn_state_machine_t machine2=KN_STATE_MACHINE_INIT(m2_top_state, machine2_states);


START_TEST(kn_sched_tests_init)
{




}
END_TEST

int tests_sched_run(void){
	Suite *s1 = suite_create("Events");
	TCase *tc1_1 = tcase_create("Events");
	SRunner *sr = srunner_create(s1);
	int nf;

	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, kn_sched_tests_init);


	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	return nf == 0 ? 0 : 1;
}
