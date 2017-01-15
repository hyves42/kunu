/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "kn_state.h"
#include <stdio.h>


int kn_machine_init(kn_state_machine_t *machine, unsigned int default_state){
	int i;
	if (!machine || !machine->states || default_state >= machine->states_count){
		return -1;
	}
	if (machine->top_level_state.interface.init){
		machine->top_level_state.interface.init(&machine->top_level_state, machine);
	}

	for (i=0; i < machine->states_count; i++){
		// States that don't have a parent must have top level as parent
		if (!machine->states[i].parent){
			machine->states[i].parent=&machine->top_level_state;
		}
		if (machine->states[i].interface.init){
			machine->states[i].interface.init(&machine->states[i], machine);
		}
	}

	machine->current_state=&machine->states[default_state];
	if (machine->current_state->interface.enter){
		machine->current_state->interface.enter(machine->current_state, machine);
	}

	return 0;
}


void kn_machine_send_event(kn_state_machine_t *machine, kn_event_t *event){
	kn_state_t *target=machine->current_state;

	while (target != NULL){
		int status = KN_EVENT_UNHANDLED;
		if (target->interface.on_event){
			status = target->interface.on_event(target, machine, event);
		}
		if (status == KN_EVENT_HANDLED){
			return;
		}
		else {
			target = target->parent;
		}
	}	

}

#define MAX_PATH_LEN 10
//To be called only from inside the state machine implementation
// or not ?
int kn_machine_set_state(kn_state_machine_t *machine, unsigned int state){
	int i=0;
	kn_state_t *old_path[MAX_PATH_LEN]={};
	kn_state_t *new_path[MAX_PATH_LEN]={};
	int old_path_len =0;
	int new_path_len =0;

	if ((!machine) || (state >= machine->states_count)){
		return -1;
	}

	kn_state_t *old_state = machine->current_state;
	kn_state_t *new_state = &machine->states[state];

	if (!old_state || !new_state){
		return -1;
	}

	kn_state_t *s=NULL;
	//compute old state path
	for (i=0, s=old_state; s!=NULL; i++, s=s->parent){
		if (i==MAX_PATH_LEN){
			return -1;
		}
		old_path[i]=s;
	}
	old_path_len=i; //>=1
	// Compute new state path
	for (i=0, s=new_state; s!=NULL; i++, s=s->parent){
		if (i==MAX_PATH_LEN){
			return -1;
		}
		new_path[i]=s;
	}
	new_path_len=i; //>=1
	//now go back in reverse to find where they diverge
	for (i=0; i<old_path_len && i<new_path_len; i++){
		if (old_path[old_path_len-i-1]!=new_path[new_path_len-i-1]){
			break;
		}
	}
	old_path_len-=i;
	new_path_len-=i;

	// exit notification for all the exited states
	for (i=0; i<old_path_len; i++){
		if (old_path[i]->interface.quit){
			old_path[i]->interface.quit(old_path[i], machine);
		}
	}

	// enter notification for all the entered states
	for (i=new_path_len-1; i>=0; i--){
		if (new_path[i]->interface.enter){
			new_path[i]->interface.enter(new_path[i], machine);
		}
	}

	machine->current_state=new_state;
	return 0;
}

