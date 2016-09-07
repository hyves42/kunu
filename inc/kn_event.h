/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_EVENT_H
#define KN_EVENT_H

typedef struct kn_event_t kn_event_t;
typedef struct kn_event_worker_t kn_event_worker_t;

struct kn_event_t{
	int id;
	void *user_data;
	kn_event_worker_t *sender; // Can be optionally provided if your application requires answers to broadcasted events
};

// The interface for something that is either an event consumer os an event sender or both
// Both are represented by the same structure so that when you receive an event and want to send a reply
// You only have one type of data to manipulate
struct kn_event_worker_t{
	// choose what you want. Not sure if this is useful
	unsigned int rtti_id; 
	// Can be null if the worker only sends events
	// Worker is the recipient of the event
	// event is the event. it is advised to provide the event->sender reference

	int (*on_event)(kn_event_worker_t *worker, kn_event_t *event);
	void *user_data;
};

// Prototype is (kn_event_worker_t *worker, kn_event_t *event)
#define kn_event_send_to_worker(worker, event)  \
	(((worker) && (worker)->on_event)?(worker)->on_event((worker), (event)):-1);  
	




#endif //EVENT_H