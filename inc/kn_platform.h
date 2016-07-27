/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef PLATFORM_H
#define PLATFORM_H


// Platform-specific initialisation
int kn_platform_init(void);
// Platform-specific deinitialisation
int kn_platform_exit(void);

// Sleep for a given amount of tick or until an exception occurs
// Whichever comes first
int kn_platform_sleep_for(int ticks);

// Sleep until an exception occurs
int kn_platform_sleep_forever(void);


#endif //PLATFORM_H