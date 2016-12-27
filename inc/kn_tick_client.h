/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_TICK_CLIENT_H
#define KN_TICK_CLIENT_H

//How many ms between two ticks ?
#define TICK_PERIOD_MS 10

#define MS_TO_TICK(a)  ((a)/TICK_PERIOD_MS)
#define TICK_TO_MS(a)  ((a)*TICK_PERIOD_MS)

typedef struct kn_tick_client_t kn_tick_client_t;

struct kn_tick_client_t{
	void (*tick)(int num_tick, void *user_data);
	void *user_data;
};

#endif //KN_TICK_CLIENT_H