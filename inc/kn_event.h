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

struct kn_event_t{
	int id;
	void *user_data;
};


#endif //EVENT_H