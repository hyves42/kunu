/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_TICK_H
#define KN_TICK_H

#include "kn_tick_client.h"

typedef struct kn_tick_controller_t kn_tick_controller_t;

int kn_tick_register_client(kn_tick_controller_t *c, kn_tick_client_t *client);

int kn_tick_unregister_client(kn_tick_controller_t *c, kn_tick_client_t *client);


#endif //KN_TICK_H