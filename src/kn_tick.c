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
	int i;
	if (!c || !client || c->clients_count == TICKS_CLIENTS_SIZE){
		return -1;
	}

	// It is forbidden not to implement this callback
	if (!client->tick){
		return -1;
	}

	// Add client at the end of the list
	c->tick_clients[c->clients_count] = client;
	c->clients_count++;

	return 0;
}

int kn_tick_unregister_client(kn_tick_controller_t *c, kn_tick_client_t *client){
	int i;
	if (!c || !client || !c->clients_count){
		return -1;
	}

	for (i=0; i<c->clients_count; i++){
		if (c->tick_clients[i]==client){
			int j;
			for (j=i; j<c->clients_count-1; j++){
				c->tick_clients[j]=c->tick_clients[j+1];
			}
			c->tick_clients[j]=NULL;
			c->clients_count--;
			return 0;
		}
	}
	//Not found
	return -1;
}


int kn_tick_broadcast_ticks(kn_tick_controller_t *c, int tick_count){
	int i;
	if (!c){
		return -1;
	}

	for (i=0; i<c->clients_count; i++){
		kn_tick_client_t *client=c->tick_clients[i];
		// Maybe add some sanity in debug mode here, check for null and null cb
		client->tick(tick_count, client->user_data);
	}

	return 0;
}