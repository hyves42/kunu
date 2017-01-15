/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "kunu.h"

#include <stdio.h>



kn_sched_t scheduler=KN_SCHED_INIT_VALUE;
kn_tick_controller_t tic = {};


#define EVENT_ID_RND 0x42

struct random_event_t{
	kn_event_t super;
	long int rnd;
};


struct random_event_t random_event={
	.super={
		.user_data=&random_event,
		.id=EVENT_ID_RND
	}
};




// Component 1 declaration
typedef struct main_component_t main_component_t;

struct main_component_t{
	kn_component_t super;
	kn_event_t fifo_buf[FIFO_SIZE_8];
	kn_event_reg_t output_dispatcher_map[10];
	kn_event_reg_t input_dispatcher_map[1];
	
	kn_timer_t rnd_timer;
};



int main_component_init(main_component_t *c);
void main_component_timer_cb(kn_timer_t *timer, void *user_data);


main_component_t main_component={
	.super=KN_COMPONENT_INIT(main_component.super, 
		main_component.fifo_buf, 
		main_component.input_dispatcher_map, 
		main_component.output_dispatcher_map,
		1,
		&main_component),
	.input_dispatcher_map={
	},
	.rnd_timer=KN_TIMER_INIT(main_component.rnd_timer, 1, main_component_timer_cb, &main_component)
};




// Component 2 declaration



#define STATE_MACHINE_STATE_OFF 0
#define STATE_MACHINE_STATE_ON 1

typedef struct main_component2_t main_component2_t;


struct main_component2_t{
	kn_component_t super;
	kn_event_t fifo_buf[FIFO_SIZE_8];
	kn_event_reg_t output_dispatcher_map[10];
	kn_event_reg_t input_dispatcher_map[1];
	void* user_data;

	kn_state_machine_t state_machine;
	kn_state_t states[2];
};

void component2_on_random_event(kn_event_t *event, void *user_data);

void top_state_init(kn_state_t *self, kn_state_machine_t *machine);  
void top_state_enter(kn_state_t *self, kn_state_machine_t *machine);
void top_state_quit(kn_state_t *self, kn_state_machine_t *machine);
int top_state_on_event(kn_state_t *self, kn_state_machine_t *machine, kn_event_t *event);


void off_state_init(kn_state_t *self, kn_state_machine_t *machine);  
void off_state_enter(kn_state_t *self, kn_state_machine_t *machine);
void off_state_quit(kn_state_t *self, kn_state_machine_t *machine);
int off_state_on_event(kn_state_t *self, kn_state_machine_t *machine, kn_event_t *event);

void on_state_init(kn_state_t *self, kn_state_machine_t *machine);  
void on_state_enter(kn_state_t *self, kn_state_machine_t *machine);
void on_state_quit(kn_state_t *self, kn_state_machine_t *machine);
int on_state_on_event(kn_state_t *self, kn_state_machine_t *machine, kn_event_t *event);


main_component2_t main_component2={
	.super=KN_COMPONENT_INIT(main_component2.super, 
		main_component2.fifo_buf, 
		main_component2.input_dispatcher_map, 
		main_component2.output_dispatcher_map,
		1,
		&main_component2),
	.input_dispatcher_map={
		KN_EVENT_REGISTER(EVENT_ID_RND, component2_on_random_event, &main_component2),
	},
	.states={
		[STATE_MACHINE_STATE_OFF]=KN_STATE_INIT(off_state),
		[STATE_MACHINE_STATE_ON]=KN_STATE_INIT(on_state),
	},
	.state_machine=KN_STATE_MACHINE_INIT(top_state, main_component2.states)
};








int main(void){
	kn_platform_init();
	// FIXME j'aime pas trop devoir appeler ,ca chaque fois
	kn_sched_set_default(&scheduler);
	kn_tick_controller_set_default(&tic);
	// FIXME power_sched n'a pas une API coh'erente avec tick controller et sched
	kn_power_sched_init(&scheduler, &tic);


	main_component_init(&main_component);
	main_component2_init(&main_component2);



	kn_component_add_listener_comp(&main_component.super, EVENT_ID_RND, &main_component2.super);


	kn_platform_linux_start_tick_thread();

	kn_power_sched_run_forever();

	return 0;
}


//Main component
int main_component_init(main_component_t *c){
	if (!c){
		return -1;
	}

	kn_sched_add_schedulable(&scheduler, &c->super.schedulable_if);

	kn_timer_runtime_init(&(c->rnd_timer));
	kn_timer_start(&c->rnd_timer, 100);

	return 0;
}

void main_component_timer_cb(kn_timer_t *timer, void *user_data){
	long int rnd=random();
	random_event.rnd=rnd;
	main_component_t *c=(main_component_t*)user_data;

	printf("publish %ld!\n", random_event.rnd);
	kn_component_publish_evt(&c->super, &random_event.super);
}


int main_component2_init(main_component2_t *c){
	if (!c){
		return -1;
	}

	kn_sched_add_schedulable(&scheduler, &c->super.schedulable_if);
	kn_machine_init(&c->state_machine, STATE_MACHINE_STATE_OFF);


	return 0;
}


void component2_on_random_event(kn_event_t *event, void *user_data){
	main_component2_t *c=(main_component2_t *)user_data;
	
	struct random_event_t *rnd_evt=(struct random_event_t *)event->user_data;
	printf("received %ld!\n", rnd_evt->rnd);
	
	kn_machine_send_event(&c->state_machine, event);

}


void top_state_init(kn_state_t *self, kn_state_machine_t *machine){}
void top_state_enter(kn_state_t *self, kn_state_machine_t *machine){}
void top_state_quit(kn_state_t *self, kn_state_machine_t *machine){}
int top_state_on_event(kn_state_t *self, kn_state_machine_t *machine, kn_event_t *event){
	printf("top_state_on_event ... event was not handled by sub states\n");
	
	return KN_EVENT_UNHANDLED;
}

void off_state_init(kn_state_t *self, kn_state_machine_t *machine){}
void off_state_enter(kn_state_t *self, kn_state_machine_t *machine){}
void off_state_quit(kn_state_t *self, kn_state_machine_t *machine){}
int off_state_on_event(kn_state_t *self, kn_state_machine_t *machine, kn_event_t *event){
	printf("off_state_on_event\n");
	if (event->id==EVENT_ID_RND){
		struct random_event_t *rnd_evt=(struct random_event_t *)event->user_data;
		if (rnd_evt->rnd >	1000000000){
			printf("switch to state ON\n");
			kn_machine_set_state(machine, STATE_MACHINE_STATE_ON);
			return KN_EVENT_HANDLED;
		}
	}
	return KN_EVENT_UNHANDLED;
}

void on_state_init(kn_state_t *self, kn_state_machine_t *machine){}
void on_state_enter(kn_state_t *self, kn_state_machine_t *machine){}
void on_state_quit(kn_state_t *self, kn_state_machine_t *machine){}
int on_state_on_event(kn_state_t *self, kn_state_machine_t *machine, kn_event_t *event){
	printf("on_state_on_event\n");
	if (event->id==EVENT_ID_RND){
		struct random_event_t *rnd_evt=(struct random_event_t *)event->user_data;
		if (rnd_evt->rnd <	1000000000){
			printf("switch to state OFF\n");
			kn_machine_set_state(machine, STATE_MACHINE_STATE_OFF);
			return KN_EVENT_HANDLED;
		}
	}
	return KN_EVENT_UNHANDLED;
}