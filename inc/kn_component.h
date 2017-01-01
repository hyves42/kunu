/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_COMPONENT_H
#define KN_COMPONENT_H

#include "kn_event.h"
#include "kn_events_fifo.h"
#include "kn_schedulable.h"
#include "kn_events_dispatch.h"

typedef struct kn_component_t kn_component_t;



struct kn_component_t{
	kn_event_worker_t worker_if;
	kn_schedulable_t schedulable_if;
	//input dispatcher, dispatch events from internal events fifo to local callback implementation
	// Typically when you implement a component you will declare your callbacks implementation here
	kn_event_dispatcher_t input_disp; 
	//output dispatcher, dispatch events to other components
	kn_event_dispatcher_t output_disp;
	kn_fifo_t events_fifo;
	void *user_data;
};

// example use :
// kn_event_t buf[FIFO_SIZE_32];
// kn_component_t c=KN_COMPONENT_INIT(c, buf);
#define KN_COMPONENT_INIT(_self, _fifo_buf, _input_disp_map, _output_disp_map, _priority, _user_data) {\
	.worker_if={\
		.on_event=kn_component_on_event,\
		.user_data=&(_self),\
	},\
	.schedulable_if=KN_SCHEDULABLE_INIT(kn_component_schedule, NULL, _priority, &(_self)),\
	.events_fifo=KN_FIFO_INIT(_fifo_buf),\
	.input_disp=KN_EVENTDISP_INIT_WITH_FIXED_MAP(((_self.input_disp)), (_input_disp_map)),\
	.output_disp=KN_EVENTDISP_INIT_WITH_EMPTY_MAP((_self.output_disp), (_output_disp_map)),\
	.user_data=(_user_data)}

//Public interface

int kn_component_publish_evt(kn_component_t *comp, kn_event_t *evt);

int kn_component_add_listener(kn_component_t *comp, int event_id, kn_event_worker_t *worker);
int kn_component_add_listener_comp(kn_component_t *comp, int event_id, kn_component_t *listener_component);

//Protected interface. 
//These functions need to be declared here to allow static initialization of struct
//But you are not supposed to call them directly

// This one is called by other components through our event_worker interface
int kn_component_on_event(kn_event_worker_t *worker, kn_event_t *event);
kn_schedulable_return_t kn_component_schedule(kn_schedulable_t *s, void* user_data);



#endif