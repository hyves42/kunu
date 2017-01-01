/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_EVENTS_DISPATCHER_H
#define KN_EVENTS_DISPATCHER_H

#include "kn_event.h"


// Dispatches events to interested listeners using the publish-subscribe model
// As generic as possible.

// In the basic form, you register a (callback function / event id) couple, 
// where the callback function will be called whenever an events that matches the id is published

// subscriber lists can be defined 
// * statically at compile-time : for example to map a list of functions to incoming events inside a component
// * dynamically to plug components into other components at run time

// The dispatcher can work with simple callbacks or event workers

// Dispatches events to one or more listeners
// One listener can subscribe to one or several events
// One or more listeners can subscribe to an event 

typedef struct kn_event_reg_t kn_event_reg_t;
typedef struct kn_event_dispatcher_t kn_event_dispatcher_t;




// Represents a subscription inside the dispatcher
struct kn_event_reg_t{
	int id;
	void *user_data;
	// Note that this is not the same prototype as the on_event() callback in kn_event_worker_t
	// because we do no want to be tightly coupled to the event_worker interface
	// In many occasions we may want to work directly with simple callbacks
	void (*handler)(kn_event_t *event, void *user_data);
};

// In a real implementation, events subscribtions lists are declared like :
// kn_event_reg_t subscriptions_map[]={
// 	KN_EVENT_REGISTER(event_id, callback, user_data)
// 	KN_EVENT_REGISTER(event_id, callback, user_data)
// 	KN_EVENT_REGISTER(event_id, callback, user_data)
// }

#define KN_EVENT_REGISTER(_event_id, _callback, _user_data){\
	.id=(_event_id),\
	.user_data=(_user_data),\
	.handler=(_callback)}

// The dispatcher object
// has an ordered list of subscribers
// has an event worker interface
struct kn_event_dispatcher_t{
	kn_event_worker_t worker;
	kn_event_reg_t* map;
	int map_size;
	int map_elements_count;
};


#define KN_EVENTDISP_INIT_WITH_FIXED_MAP(_disp, _map){\
	.map=(_map),\
	.map_size=sizeof(_map)/sizeof((_map)[0]),\
	.map_elements_count=sizeof(_map)/sizeof((_map)[0]),\
	.worker={\
		.on_event=kn_eventdisp_on_event,\
		.user_data=&(_disp)}}

#define KN_EVENTDISP_INIT_WITH_EMPTY_MAP(_disp, _map){\
	.map=(_map),\
	.map_size=sizeof(_map)/sizeof((_map)[0]),\
	.map_elements_count=0,\
	.worker={\
		.on_event=kn_eventdisp_on_event,\
		.user_data=&(_disp)}}



//Public interface

// initialize an event dispatcher
// This is similar to an initialization with KN_EVENTDISP_INIT_WITH_FIXED_MAP or KN_EVENTDISP_INIT_WITH_EMPTY_MAP
int kn_eventdisp_init(kn_event_dispatcher_t *disp, kn_event_reg_t* map, int map_count);


// Subscribe a callback to an event
int kn_eventdisp_subscribe(kn_event_dispatcher_t *disp, int event_id, 
	void (*handler)(kn_event_t *event, void *user_data), void *user_data);

// Subscribe an event worker to an event
int kn_eventdisp_subscribe_worker(kn_event_dispatcher_t *disp,  int event_id, kn_event_worker_t* worker);

// subscribe a callback to an event, using kn_event_reg_t structure
int kn_eventdisp_subscribe_reg(kn_event_dispatcher_t *disp, kn_event_reg_t* event);
// subscribe a list of callbacks to events, using kn_event_reg_t structure
int kn_eventdisp_subscribe_reg_array(kn_event_dispatcher_t *disp, kn_event_reg_t* array, int elt_count);


int kn_eventdisp_get_registered_count(kn_event_dispatcher_t *disp);


int kn_eventdisp_publish(kn_event_dispatcher_t *disp, kn_event_t *event);


//Protected interface. 
//These functions need to be declared here to allow static initialization of struct
//But you are not supposed to call them directly
int kn_eventdisp_on_event(kn_event_worker_t *worker, kn_event_t *event);


#endif //KN_EVENTS_H