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

typedef struct kn_event_reg_t kn_event_reg_t;
typedef struct kn_event_dispatcher_t kn_event_dispatcher_t;





struct kn_event_reg_t{
	int id;
	void *user_data;
	// This is not the same prototype as kn_event_worker_t
	// I'm still not sure if this is a good idea or not
	int (*handler)(kn_event_t *event, void *user_data);
};


struct kn_event_dispatcher_t{
	kn_event_worker_t worker;
	kn_event_reg_t* map;
	int map_size;
	int map_elements_count;
};


//dans l'implementaation de la machine
//on d'eclare
// struct registered[]={
// 	EVENT_REGISTER(event_id, callback, user_data)
// 	EVENT_REGISTER(event_id, callback, user_data)
// 	EVENT_REGISTER(event_id, callback, user_data)
// }

#define KN_EVENT_REGISTER(_event_id, _callback, _user_data){\
	.id=_event_id,\
	.user_data=_user_data,\
	.handler=_callback}


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
int kn_eventdisp_init(kn_event_dispatcher_t *disp, kn_event_reg_t* map, int map_count);

int kn_eventdisp_register_subscriber_array(kn_event_dispatcher_t *disp, kn_event_reg_t* array, int elt_count);
int kn_eventdisp_register_subscriber(kn_event_dispatcher_t *disp, kn_event_reg_t* event);


int kn_eventdisp_get_registered_count(kn_event_dispatcher_t *disp);


int kn_eventdisp_broadcast(kn_event_dispatcher_t *disp, kn_event_t *event);


//Protected interface. 
//These functions need to be declared here to allow static initialization of struct
//But you are not supposed to call them directly

int kn_eventdisp_on_event(kn_event_worker_t *worker, kn_event_t *event);


#endif //KN_EVENTS_H