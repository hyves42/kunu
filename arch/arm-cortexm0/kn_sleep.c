/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2017 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */


#include "kn_tick.h"

int kn_platform_sleep_for(int ticks){
	return 0;
}

int kn_platform_sleep_forever(void){
	return 0;
}

