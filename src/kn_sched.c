/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 

#include "kn_sched.h"
#include <stdio.h>


static kn_sched_t *default_sched=NULL;

kn_sched_t *kn_sched_get_default(void){
	return default_sched;
}
void kn_sched_set_default(kn_sched_t *s){
	default_sched=s;
}


int kn_sched_add_schedulable(kn_sched_t *s, kn_schedulable_t *schedulable){
	int i;
	if (!s || !schedulable || s->schedulable_count == SCHEDULABLES_SIZE){
		return -1;
	}

	// It is forbidden not to implement this callback
	if (!schedulable->schedule){
		return -1;
	}

	// Not the smartest implementation but it does the job for now
	// we have like 16 schedulable objects at most
	for (i=0; i<s->schedulable_count; i++){
		if (!s->schedulables[i]){
			return -1;
		}
		if (s->schedulables[i]->priority > schedulable->priority){
			break;
		}
	}


	//make room for new schedulable if needed
	if (i<s->schedulable_count){
		int j;
		for (j=s->schedulable_count; j>i; --j){	
			s->schedulables[j]=s->schedulables[j-1];
		}
	}
	s->schedulables[i]=schedulable;
	s->schedulable_count++;
	return 0;
}

int kn_sched_remove_schedulable(kn_sched_t *s, kn_schedulable_t *schedulable){
	int i;
	if (!s || !schedulable || s->schedulable_count == 0){
		return -1;
	}

	for (i=0; i<s->schedulable_count; i++){
		if (s->schedulables[i]==schedulable){
			int j;
			for (j=i; j<s->schedulable_count-1; j++){
				s->schedulables[j]=s->schedulables[j+1];
			}
			s->schedulables[j]=NULL;
			s->schedulable_count--;
			return 0;
		}
	}
	//Not found
	return -1;
}


//Return 0 when something was done
int kn_sched_step(kn_sched_t *s){
	int i;
	if (!s){
		return -1;
	}

	for (i=0; i<s->schedulable_count; i++){
		kn_schedulable_t *se = s->schedulables[i];

		if (se->state==NEEDS_SCHEDULE || se->state==WAITING){
			kn_schedulable_return_t ret = se->schedule(se, se->user_data);
			if (ret == DID_SOMETHING){
				return 0;
			}
		}
	}
	return 1;
}

unsigned int sched_get_remaining_ticks(kn_sched_t *s){
	int i;
	int ticks=-1;
	if (!s){
		return -1;
	}

	for (i=0; i<s->schedulable_count; i++){
		kn_schedulable_t *se = s->schedulables[i];

		if (se->remaining_ticks && se->state == WAITING){
			int t = se->remaining_ticks(se, se->user_data);
			if (t >= 0 && t < ticks){
				ticks=t;
			}
		}
	}
	return ticks;
}