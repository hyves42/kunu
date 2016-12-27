/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_TIMER_H
#define KN_TIMER_H

#include "kn_schedulable.h"
#include "kn_tick_client.h"
#include <stdbool.h>

typedef struct kn_timer_t kn_timer_t;

struct kn_timer_t{
	kn_schedulable_t schedulable;
	kn_tick_client_t tick;
	void *user_data;
	void (*timer_cb)(kn_timer_t *timer, void *user_data);
	int tick_interval;
	int tick_remaining;
	bool is_armed;
	bool must_repeat;
};

#define KN_TIMER_GET_SCHEDULABLE(t) ((t)->schedulable)
#define KN_TIMER_GET_TICK_CLIENT(t) ((t)->tick)

//Public interface
int kn_timer_init(kn_timer_t *t, void (*timer_cb)(kn_timer_t *timer, void *user_data), void *user_data, int priority);

int kn_timer_start(kn_timer_t *t, int ms_interval);

int kn_timer_stop(kn_timer_t *t);

int kn_timer_start_one_shot(kn_timer_t *t, int ms_interval);

#endif //KN_TIMER_H