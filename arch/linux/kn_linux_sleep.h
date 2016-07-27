/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_LINUX_SLEEP_H
#define KN_LINUX_SLEEP_H

int kn_platform_linux_sleep_init(void);

int kn_platform_linux_simulate_interrupt(void);

int kn_platform_linux_simulate_tick(void);


#endif