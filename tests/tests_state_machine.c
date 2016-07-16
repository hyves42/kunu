/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "tests_state_machine.h"
#include "kn_state.h"
#include <check.h>
#include <stdio.h>
#include <stdbool.h>


// I use these pointers to solve the cyclic dependancy
// between the functions implementation and the states/machine definition
static kn_state_machine_t *machine_p=NULL;

// Bools used to keep track of where we passed through
bool top_state_initialized=false;
bool state0_initialized=false;
bool state1_initialized=false;
bool state2_initialized=false;
bool top_state_event=false;
bool state0_event=false;
bool state1_event=false;
bool state2_event=false;
bool top_state_entered=false, top_state_exited=false;
bool state0_entered=false, state0_exited=false;
bool state1_entered=false, state1_exited=false;
bool state2_entered=false, state2_exited=false;

enum STATES{
	STATE_0,
	STATE_1,
	STATE_2,
	STATES_COUNT
};


//Typical implementation of machine for tests
static void top_state_init(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->top_level_state);
	top_state_initialized=true;
}

static void top_state_enter(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->top_level_state);
	top_state_entered=true;
}
static void top_state_quit(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->top_level_state);
	top_state_exited=true;
}
static int top_state_on_event(kn_state_t *self, kn_state_machine_t *m, kn_event_t *event){
	ck_assert(m==machine_p);
	ck_assert(self==&m->top_level_state);		
	top_state_event=true;
	return KN_EVENT_HANDLED;
}

static void state0_init(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_0]);
	state0_initialized=true;
}
static void state0_enter(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_0]);
	state0_entered=true;
}
static void state0_quit(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_0]);
	state0_exited=true;
}
static int state0_on_event(kn_state_t *self, kn_state_machine_t *m, kn_event_t *event){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_0]);		
	state0_event=true;
	return KN_EVENT_HANDLED;
}

static void state1_init(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_1]);
	state1_initialized=true;
}
static void state1_enter(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_1]);
	state1_entered=true;
}
static void state1_quit(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_1]);
	state1_exited=true;
}
static int state1_on_event(kn_state_t *self, kn_state_machine_t *m, kn_event_t *event){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_1]);		
	state1_event=true;
	return KN_EVENT_HANDLED;
}
static void state2_init(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_2]);
	state2_initialized=true;
}
static void state2_enter(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_2]);
	state2_entered=true;
}
static void state2_quit(kn_state_t *self, kn_state_machine_t *m){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_2]);
	state2_exited=true;
}
static int state2_on_event(kn_state_t *self, kn_state_machine_t *m, kn_event_t *event){
	ck_assert(m==machine_p);
	ck_assert(self==&m->states[STATE_2]);		
	state2_event=true;
	return KN_EVENT_HANDLED;
}

static void reset_flags(void){
	top_state_initialized=false;
	state0_initialized=false;
	state1_initialized=false;
	state2_initialized=false;
	top_state_event=false;
	state0_event=false;
	state1_event=false;
	state2_event=false;
	top_state_entered=false;
	top_state_exited=false;
	state0_entered=false;
	state0_exited=false;
	state1_entered=false;
	state1_exited=false;
	state2_entered=false;
	state2_exited=false;
}


START_TEST(kn_machine_init_static)
{
	kn_state_t states[1]={};
	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, states);

	ck_assert(machine.top_level_state.parent == NULL);
	ck_assert(machine.top_level_state.interface.init == top_state_init);
	ck_assert(machine.top_level_state.interface.enter == top_state_enter);
	ck_assert(machine.top_level_state.interface.quit == top_state_quit);
	ck_assert(machine.top_level_state.interface.on_event == top_state_on_event);
	ck_assert(machine.states == states);
	ck_assert(machine.states_count == 1);
}
END_TEST

START_TEST(kn_machine_init_static2)
{
	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, NULL);

	ck_assert(machine.states == NULL);
	//printf("machine.states_count = %u\n", machine.states_count);
	//ck_assert(machine.states_count == 0);
}
END_TEST


// This test checks the static initialisation of the machine and state structures
START_TEST(kn_machine_init_with_states)
{
	kn_state_t states[STATES_COUNT]={
		[STATE_0]=KN_STATE_INIT(state0),
		[STATE_1]=KN_STATE_INIT(state1),
		[STATE_2]=KN_STATE_INIT(state2),
	};

	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, states);
	machine_p=&machine;

	ck_assert(machine.top_level_state.parent == NULL);
	ck_assert(machine.top_level_state.interface.init == top_state_init);
	ck_assert(machine.top_level_state.interface.enter == top_state_enter);
	ck_assert(machine.top_level_state.interface.quit == top_state_quit);
	ck_assert(machine.top_level_state.interface.on_event == top_state_on_event);

	ck_assert(machine.states == states);
	ck_assert(machine.states_count == STATES_COUNT);
}
END_TEST

// This test checks the init call of every substates
START_TEST(kn_machine_init_with_states2)
{
	kn_state_t states[STATES_COUNT]={
		[STATE_0]=KN_STATE_INIT(state0),
		[STATE_1]=KN_STATE_INIT(state1),
		[STATE_2]=KN_STATE_INIT(state2),
	};

	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, states);

	machine_p=&machine;
	reset_flags();
	ck_assert(machine.states == states);
	ck_assert(machine.states_count == STATES_COUNT);

	int ret = kn_machine_init(&machine, STATE_1);

	ck_assert(ret == 0);
	ck_assert(top_state_init);
	ck_assert(state0_init);
	ck_assert(state1_init);
	ck_assert(state2_init);

	ck_assert(machine.current_state==&states[STATE_1]);
}
END_TEST


START_TEST(kn_machine_init_with_states3)
{
	int ret = kn_machine_init(NULL, 0);
	ck_assert(ret == -1);
}
END_TEST

START_TEST(kn_machine_init_with_states4)
{
	kn_state_t states[STATES_COUNT]={
		[STATE_0]=KN_STATE_INIT(state0),
		[STATE_1]=KN_STATE_INIT(state1),
		[STATE_2]=KN_STATE_INIT(state2),
	};

	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, states);
	machine_p=&machine;
	reset_flags();
	ck_assert(machine.states == states);
	ck_assert(machine.states_count == STATES_COUNT);

	int ret = kn_machine_init(&machine, STATES_COUNT);
	ck_assert(ret == -1);
	ret = kn_machine_init(&machine, STATES_COUNT+2);
	ck_assert(ret == -1);	
}
END_TEST

START_TEST(kn_machine_init_with_states5)
{
	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, NULL);
	machine_p=&machine;
	reset_flags();
	ck_assert(machine.states == NULL);

	int ret = kn_machine_init(&machine, 0);
	ck_assert(ret == -1);
}
END_TEST



START_TEST(machine_kn_state_transition_simple)
{
	kn_state_t states[STATES_COUNT]={
		[STATE_0]=KN_STATE_INIT(state0),
		[STATE_1]=KN_STATE_INIT(state1),
		[STATE_2]=KN_STATE_INIT(state2),
	};

	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, states);

	machine_p=&machine;
	reset_flags();
	ck_assert(machine.states == states);
	ck_assert(machine.states_count == STATES_COUNT);

	int ret = kn_machine_init(&machine, STATE_0);
	ck_assert(state0_entered);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_0]);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_1);
	ck_assert(ret==0);
	ck_assert(state0_entered==false);
	ck_assert(state0_exited);
	ck_assert(state1_entered);
	ck_assert(state1_exited==false);
	ck_assert(state2_entered==false);
	ck_assert(state2_exited==false);
	ck_assert(machine.current_state==&machine.states[STATE_1]);	

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_2);
	ck_assert(ret==0);
	ck_assert(state0_entered==false);
	ck_assert(state0_exited==false);
	ck_assert(state1_entered==false);
	ck_assert(state1_exited);
	ck_assert(state2_entered);
	ck_assert(state2_exited==false);
	ck_assert(machine.current_state==&machine.states[STATE_2]);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_0);
	ck_assert(ret==0);
	ck_assert(state0_entered);
	ck_assert(state0_exited==false);
	ck_assert(state1_entered==false);
	ck_assert(state1_exited==false);
	ck_assert(state2_entered==false);
	ck_assert(state2_exited);
	ck_assert(machine.current_state==&machine.states[STATE_0]);

}
END_TEST


START_TEST(machine_kn_state_transition_hierarchical)
{
	kn_state_t states[STATES_COUNT]={
		[STATE_0]=KN_STATE_INIT(state0),
		[STATE_1]=KN_STATE_INIT(state1),
		[STATE_2]=KN_STATE_INIT_SUB(&states[STATE_1], state2),
	};

	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, states);

	machine_p=&machine;
	reset_flags();
	ck_assert(machine.states == states);
	ck_assert(machine.states_count == STATES_COUNT);

	int ret = kn_machine_init(&machine, STATE_0);
	ck_assert(state0_entered);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_0]);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_1);
	ck_assert(ret==0);
	ck_assert(state0_entered==false);
	ck_assert(state0_exited);
	ck_assert(state1_entered);
	ck_assert(state1_exited==false);
	ck_assert(state2_entered==false);
	ck_assert(state2_exited==false);
	ck_assert(machine.current_state==&machine.states[STATE_1]);	

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_2);
	ck_assert(ret==0);
	ck_assert(state0_entered==false);
	ck_assert(state0_exited==false);
	ck_assert(state1_entered==false);
	ck_assert(state1_exited==false);
	ck_assert(state2_entered);
	ck_assert(state2_exited==false);
	ck_assert(machine.current_state==&machine.states[STATE_2]);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_0);
	ck_assert(ret==0);
	ck_assert(state0_entered);
	ck_assert(state0_exited==false);
	ck_assert(state1_entered==false);
	ck_assert(state1_exited);
	ck_assert(state2_entered==false);
	ck_assert(state2_exited);
	ck_assert(machine.current_state==&machine.states[STATE_0]);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_2);
	ck_assert(ret==0);
	ck_assert(state0_entered==false);
	ck_assert(state0_exited);
	ck_assert(state1_entered);
	ck_assert(state1_exited==false);
	ck_assert(state2_entered);
	ck_assert(state2_exited==false);
	ck_assert(machine.current_state==&machine.states[STATE_2]);
}
END_TEST



START_TEST(machine_state_event_send_simple)
{
	kn_state_t states[STATES_COUNT]={
		[STATE_0]=KN_STATE_INIT(state0),
		[STATE_1]=KN_STATE_INIT(state1),
		[STATE_2]=KN_STATE_INIT(state2),
	};

	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, states);

	machine_p=&machine;
	reset_flags();
	ck_assert(machine.states == states);
	ck_assert(machine.states_count == STATES_COUNT);

	int ret = kn_machine_init(&machine, STATE_0);
	ck_assert(state0_entered);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_0]);

	kn_machine_send_event(&machine, 0);
	ck_assert(state0_event);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_1);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_1]);	

	kn_machine_send_event(&machine, 0);
	ck_assert(state1_event);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_2);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_2]);

	kn_machine_send_event(&machine, 0);
	ck_assert(state2_event);
	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_0);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_0]);

	kn_machine_send_event(&machine, 0);
	ck_assert(state0_event);

}
END_TEST


START_TEST(machine_state_event_send_hierarchical)
{
	kn_state_t states[STATES_COUNT]={
		[STATE_0]=KN_STATE_INIT(state0),
		[STATE_1]=KN_STATE_INIT(state1),
		[STATE_2]=KN_STATE_INIT_SUB(&states[STATE_1], state2),
	};

	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, states);

	machine_p=&machine;
	reset_flags();
	ck_assert(machine.states == states);
	ck_assert(machine.states_count == STATES_COUNT);

	int ret = kn_machine_init(&machine, STATE_0);
	ck_assert(state0_entered);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_0]);

	kn_machine_send_event(&machine, 0);
	ck_assert(state0_event);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_1);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_1]);	

	kn_machine_send_event(&machine, 0);
	ck_assert(state1_event);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_2);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_2]);

	kn_machine_send_event(&machine, 0);
	ck_assert(state2_event);
	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_0);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_0]);

	kn_machine_send_event(&machine, 0);
	ck_assert(state0_event);

}
END_TEST


START_TEST(machine_state_event_send_hierarchical2)
{	
	//specific implementation for this test case
	void state2__init(kn_state_t *self, kn_state_machine_t *m){
		state2_init(self, m);
	}
	void state2__enter(kn_state_t *self, kn_state_machine_t *m){
		state2_enter(self, m);
	}
	void state2__quit(kn_state_t *self, kn_state_machine_t *m){
		state2_quit(self, m);
	}
	int state2__on_event(kn_state_t *self, kn_state_machine_t *m, kn_event_t *event){
		state2_on_event(self, m, event);
		return KN_EVENT_UNHANDLED; // simulate unhandled event, see if it is transfered to state 1
	}

	kn_state_t states[STATES_COUNT]={
		[STATE_0]=KN_STATE_INIT(state0),
		[STATE_1]=KN_STATE_INIT(state1),
		[STATE_2]=KN_STATE_INIT_SUB(&states[STATE_1], state2_),
	};

	kn_state_machine_t machine=KN_STATE_MACHINE_INIT(top_state, states);

	machine_p=&machine;
	reset_flags();
	ck_assert(machine.states == states);
	ck_assert(machine.states_count == STATES_COUNT);

	int ret = kn_machine_init(&machine, STATE_0);
	ck_assert(state0_entered);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_0]);

	kn_machine_send_event(&machine, 0);
	ck_assert(state0_event);
	ck_assert(state1_event==false);
	ck_assert(state2_event==false);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_1);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_1]);	

	kn_machine_send_event(&machine, 0);
	ck_assert(state0_event==false);
	ck_assert(state1_event);
	ck_assert(state2_event==false);

	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_2);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_2]);

	kn_machine_send_event(&machine, 0);
	ck_assert(state0_event==false);
	ck_assert(state1_event);
	ck_assert(state2_event);
	reset_flags();

	ret = kn_machine_set_state(&machine, STATE_0);
	ck_assert(ret==0);
	ck_assert(machine.current_state==&machine.states[STATE_0]);

	kn_machine_send_event(&machine, 0);
	ck_assert(state0_event);
	ck_assert(state1_event==false);
	ck_assert(state2_event==false);

}
END_TEST



int tests_state_machine_run(void){
	Suite *s1 = suite_create("State_machine");
	TCase *tc1_1 = tcase_create("State_machine");
	SRunner *sr = srunner_create(s1);
	int nf;

	suite_add_tcase(s1, tc1_1);
	tcase_add_test(tc1_1, kn_machine_init_static);
	tcase_add_test(tc1_1, kn_machine_init_static2);	
	tcase_add_test(tc1_1, kn_machine_init_with_states);
	tcase_add_test(tc1_1, kn_machine_init_with_states2);
	tcase_add_test(tc1_1, kn_machine_init_with_states3);
	tcase_add_test(tc1_1, kn_machine_init_with_states4);
	tcase_add_test(tc1_1, kn_machine_init_with_states5);

	tcase_add_test(tc1_1, machine_kn_state_transition_simple);
	tcase_add_test(tc1_1, machine_kn_state_transition_hierarchical);


	tcase_add_test(tc1_1, machine_state_event_send_simple);
	tcase_add_test(tc1_1, machine_state_event_send_hierarchical);
	tcase_add_test(tc1_1, machine_state_event_send_hierarchical2);

	srunner_run_all(sr, CK_ENV);
	nf = srunner_ntests_failed(sr);
	srunner_free(sr);

	return nf == 0 ? 0 : 1;
}