/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "kn_platform.h"
#include "kn_linux_sleep.h"

int kn_platform_init(void){
	return kn_platform_linux_sleep_init();
}

int kn_platform_exit(void){
	return kn_platform_linux_sleep_exit();
}