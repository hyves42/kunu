/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_POWER_SCHED_H
#define KN_POWER_SCHED_H

#include "kn_sched.h"

int kn_power_sched_init(kn_sched_t* s);

int kn_power_sched_run(void);


#endif //KN_POWER_SCHED_H