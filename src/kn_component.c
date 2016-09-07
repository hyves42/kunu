/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "kn_component.h"


int kn_component_on_event(kn_event_worker_t *worker, kn_event_t *event){
	return 0;
}
kn_schedulable_return_t kn_component_schedule(kn_schedulable_t *s, void* user_data){
	return DID_NOTHING;
}