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
typedef struct kn_event_reg_entry_t kn_event_reg_entry_t;
typedef struct kn_event_dispatcher_t kn_event_dispatcher_t;





struct kn_event_reg_t{
	int id;
	void *user_data;
	// This is not the same prototype as kn_event_worker_t
	// And this is on purpose. 
	// My idea here is to have one lightweight callback per type of event inside of the worker
	int (*handler)(kn_event_t *event, void *user_data);
};

struct kn_event_reg_entry_t{
	int id;
	kn_event_reg_t *reg;
};


struct kn_event_dispatcher_t{
	kn_event_worker_t worker;
	kn_event_reg_entry_t* map;
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


int kn_eventdisp_init(kn_event_dispatcher_t *disp, kn_event_reg_entry_t* map, int map_count);

int kn_eventdisp_register_subscriber_array(kn_event_dispatcher_t *disp, kn_event_reg_t* array, int elt_count);
int kn_eventdisp_register_subscriber(kn_event_dispatcher_t *disp, kn_event_reg_t* event);


int kn_eventdisp_get_registered_count(kn_event_dispatcher_t *disp);


int kn_eventdisp_broadcast(kn_event_dispatcher_t *disp, kn_event_t *event);


#endif //KN_EVENTS_H