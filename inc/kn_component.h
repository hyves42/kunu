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
	kn_event_dispatcher_t dispatcher;
	kn_fifo_t events_fifo;
};

// example use :
// kn_event_t buf[FIFO_SIZE_32];
// kn_component_t c=KN_COMPONENT_INIT(c, buf);
#define KN_COMPONENT_INIT(_comp, _fifo_buf, _dispatcher_map) {\
	.worker_if={\
		.on_event=kn_component_on_event,\
		.user_data=&(_comp),\
	},\
	.schedulable_if={\
		.schedule=kn_component_schedule,\
		.user_data=&(_comp),\
	},\
	.events_fifo=KN_FIFO_INIT(_fifo_buf),\
	.dispatcher=KN_EVENTDISP_INIT_WITH_FIXED_MAP(_comp.dispatcher, _dispatcher_map)}

//Public interface



//Protected interface. 
//These functions need to be declared here to allow static initialization of struct
//But you are not supposed to call them directly
int kn_component_on_event(kn_event_worker_t *worker, kn_event_t *event);
kn_schedulable_return_t kn_component_schedule(kn_schedulable_t *s, void* user_data);



#endif