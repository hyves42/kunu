/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "kn_timer.h"
#include "kn_tick.h"
#include "kn_sched.h"

 //TODO add a 'class method' to init a local sched and thck controller
 // instead of calling  kn_sched_get_default() and kn_tick_controller_get_default() always






int kn_timer_init(kn_timer_t *t, void (*timer_cb)(kn_timer_t *timer, void *user_data), void *user_data, int priority){
	if (!t){
		return -1;
	}
	// static init. Can be done using macro KN_TIMER_INIT() as well

	//setup schedulable interface
	kn_schedulable_t *s=&t->schedulable;
	s->schedule=kn_timer_schedule;
	s->remaining_ticks=kn_timer_remaining_ticks;
	s->priority=priority;
	s->state=SLEEPING;
	s->user_data=t;

	//setup tick_client interface
	kn_tick_client_t *ti=&t->tick;
	ti->tick=kn_timer_tick;
	ti->user_data=t;

	t->user_data=user_data;
	t->timer_cb=timer_cb;

	t->tick_interval=0;
	t->tick_remaining=0;
	t->is_armed=false;
	t->must_repeat=false;

	return kn_timer_runtime_init(t);
}


int kn_timer_runtime_init(kn_timer_t *t){
	int ret=0;
	if (!t){
		return -1;
	}

	// Register schedulable and tick_client interfaces
	ret = kn_sched_add_schedulable(kn_sched_get_default(), &t->schedulable);
	if (ret<0) goto fail;

	ret = kn_tick_register_client(kn_tick_controller_get_default(), &t->tick);
	if (ret<0) goto fail;

	return ret;
fail:
	kn_sched_remove_schedulable(kn_sched_get_default(), &t->schedulable);
	kn_tick_unregister_client(kn_tick_controller_get_default(), &t->tick);
	return ret;
}

int kn_timer_start(kn_timer_t *t, int ms_interval){
	if (!t){
		return -1;
	}
	int tick_interval=MS_TO_TICK(ms_interval);
	if (tick_interval == 0){
		return -1;
	}

	t->tick_interval=tick_interval;
	t->tick_remaining=tick_interval;
	t->is_armed=true;
	t->must_repeat=true;
	t->schedulable.state=(t->tick_remaining>0)?SLEEPING:NEEDS_SCHEDULE;
	return 0;
}

int kn_timer_stop(kn_timer_t *t){
	if (!t){
		return -1;
	}
	t->is_armed=false;
	t->schedulable.state=SLEEPING;
	return 0;
}

int kn_timer_start_one_shot(kn_timer_t *t, int ms_interval){
	int ret=kn_timer_start(t, ms_interval);
	if (!ret){
		t->must_repeat=false;	
	}
	return ret;
}



//

void kn_timer_tick(int num_tick, void *user_data){
	kn_timer_t *t=(kn_timer_t*)user_data;
	if (!t || !t->is_armed){
		return;
	}
	t->tick_remaining-=num_tick;
	if (t->tick_remaining <=0){
		t->schedulable.state=NEEDS_SCHEDULE;
	}
}
kn_schedulable_return_t kn_timer_schedule(kn_schedulable_t *s, void* user_data){
	kn_timer_t *t=(kn_timer_t*)user_data;

	if (!t || !t->timer_cb){
		return DID_NOTHING;
	}	
	if (t->tick_remaining >0){
		return DID_NOTHING;
	}

	t->timer_cb(t, t->user_data);

	if (t->must_repeat){
		t->tick_remaining += t->tick_interval;
		t->schedulable.state=(t->tick_remaining>0)?SLEEPING:NEEDS_SCHEDULE;
	}
	else{
		t->is_armed=false;
		t->schedulable.state=SLEEPING;
	}
	return DID_SOMETHING;
}

// return the number of remaining ticks
// If the scheduler is calling us too late and the remaining time is negative, return 0 for schedule now
int kn_timer_remaining_ticks(kn_schedulable_t *s, void* user_data){
	kn_timer_t *t=(kn_timer_t*)user_data;
	if (!t || !t->is_armed){
		return -1;
	}
	int rem=t->tick_remaining;
	return (rem<0)?0:rem;
}
