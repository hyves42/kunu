/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "kn_tick.h"
#include <stdio.h>

static kn_tick_controller_t *default_tick_controller = NULL;

kn_tick_controller_t *kn_tick_controller_get_default(void){
	return default_tick_controller;
}

void kn_tick_controller_set_default(kn_tick_controller_t *c){
	default_tick_controller = c;
}

int kn_tick_register_client(kn_tick_controller_t *c, kn_tick_client_t *client){
	if (!c || !client){
		return -1;
	}
	return 0;
}

int kn_tick_unregister_client(kn_tick_controller_t *c, kn_tick_client_t *client){
	if (!c || !client){
		return -1;
	}
	return 0;
}