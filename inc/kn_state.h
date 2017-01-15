/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_STATE_H
#define KN_STATE_H

#include "kn_event.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct kn_state_t kn_state_t;
typedef struct kn_state_interface_t kn_state_interface_t;
typedef struct kn_state_machine_t kn_state_machine_t;




//interface, protocol
struct kn_state_interface_t{
	void (*init)(kn_state_t *self, kn_state_machine_t *machine);   // Called at the creation of the state, ie at the creation of the state machine
	void (*enter)(kn_state_t *self, kn_state_machine_t *machine);
	void (*quit)(kn_state_t *self, kn_state_machine_t *machine);
	int (*on_event)(kn_state_t *self, kn_state_machine_t *machine, kn_event_t *event);
};

struct kn_state_t{
	kn_state_t *parent;
	kn_state_interface_t interface;
	//kn_state_machine_t *machine; //cyclic dependancy if I include this one
	//void *user_if;
};

struct kn_state_machine_t{
	kn_state_t top_level_state;
	kn_state_t *current_state;
	kn_state_t *states;
	unsigned int states_count;
};



#define KN_INTERFACE_DEFINE(p) {\
	.init = p ## _init,\
	.enter = p ##_enter,\
	.quit= p ## _quit,\
	.on_event= p ## _on_event}



#define KN_STATE_INIT_SUB(par, prefix) {\
	.parent=(par),\
	.interface=KN_INTERFACE_DEFINE(prefix)}


// A macro to initialize a state without a parent
// In practice, the machine top level will be their parent
#define KN_STATE_INIT(prefix) KN_STATE_INIT_SUB(NULL, prefix)

// A macro to initialize a state with a parent
#define KN_STATE_MACHINE_INIT(top_level_prefix, states_array) {\
	.top_level_state=KN_STATE_INIT(top_level_prefix),\
	.current_state=NULL,\
	.states=(states_array),\
	.states_count=sizeof(states_array)/sizeof((states_array)[0])}


int kn_machine_init(kn_state_machine_t *machine, unsigned int default_state);

void kn_machine_send_event(kn_state_machine_t *machine, kn_event_t *event);

int kn_machine_set_state(kn_state_machine_t *machine, unsigned int state);


#define KN_EVENT_HANDLED 1
#define KN_EVENT_UNHANDLED 0

#ifdef __cplusplus
}
#endif

#endif //KN_STATE_H