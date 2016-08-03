/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_SCHED_H
#define KN_SCHED_H

#include <stdbool.h>
#include "kn_schedulable.h"


typedef struct kn_sched_t kn_sched_t;

#define SCHEDULABLES_SIZE 16
struct kn_sched_t{
	kn_schedulable_t *schedulables[SCHEDULABLES_SIZE]; //schedulables ordered by priority
	int schedulable_count;
};


#define SCHED_INIT_VALUE {\
	.schedulables={},\
	.schedulable_count=0}


kn_sched_t *kn_sched_get_default(void);
void kn_sched_set_default(kn_sched_t *s);

int kn_sched_add_schedulable(kn_sched_t *s, kn_schedulable_t *schedulable);
int kn_sched_remove_schedulable(kn_sched_t *s, kn_schedulable_t *schedulable);
int kn_sched_step(kn_sched_t *s);
unsigned int kn_sched_get_next_date(kn_sched_t *s);



#endif //KN_SCHED_H