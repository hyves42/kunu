/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_SCHEDULABLE_H
#define KN_SCHEDULABLE_H

// type definition for a schedulable component interface

typedef enum {
	NEEDS_SCHEDULE, // needs to perform something right now
	WAITING,        // Waiting on a condition, needs to be scheduled to check it. If not realized, will return immediately
	SLEEPING,       // Certain nothing needs to be performed right now
}kn_schedulable_kn_state_t;

// The design difference between NEEDS_SCHEDULE and WAITING is to allow a smart power management
// If at least one of the schedulables is in NEEDS_SCHEDULE state, we cannot go into sleep mode at all
// If a schedulable is in WAITING mode, and after scheduling it returns DID_NOTHING, we can go into sleep mode
// And the next schedule will be after wake up
// Of course, the condition that will make the task use its scheduling slot needs to be a wake-up source for this to work completely




typedef enum {
	DID_NOTHING,
	DID_SOMETHING,
}kn_schedulable_return_t;

typedef struct kn_schedulable_t kn_schedulable_t;

struct kn_schedulable_t{
	// perform the action
	kn_schedulable_return_t (*schedule)(kn_schedulable_t *s, void* user_data);
	// Information to the scheduler about when the next action needs to be performed for this component
	// -1 = I don't know
	// >0 = number of ticks
	// Can be NULL if implementation not relevant
	int (*remaining_ticks)(kn_schedulable_t *s, void* user_data);
	int priority; 
	kn_schedulable_kn_state_t state;
	void *user_data;
};


#endif //KN_SCHEDULABLE_H