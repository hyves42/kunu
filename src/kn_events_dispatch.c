/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */
 
#include "kn_events_dispatch.h"
#include <stdio.h> //for debug printf and NULL

static void kn_eventdisp_on_event(kn_event_worker_t *worker, kn_event_t *event);


int kn_eventdisp_init(kn_event_dispatcher_t *disp, kn_event_reg_entry_t* a_map, int a_map_size){
	int i=0;

	if (!disp || !a_map || a_map_size==0){
		return -1;
	}
	disp->map=a_map;
	disp->map_size=a_map_size;

	disp->worker.on_event=kn_eventdisp_on_event;
	disp->worker.user_data=disp;



	// if a_map is already an ordered map of subscribers. accept it as is
	// Just perform some basic safety tests
	int last_id=0;
	for (; i<a_map_size; i++){
		int id = a_map[i].id;
		if (id==0 || a_map[i].reg==NULL) break;
		if (id<last_id){
			disp->map_elements_count=0;
			// This is forbidden, a_map should either be empty or correctly ordered
			return -1;
		}
		last_id=id;
	}
	disp->map_elements_count=i;
	return 0;
}

int kn_eventdisp_register_subscriber_array(kn_event_dispatcher_t *disp, kn_event_reg_t* array, int elt_count){
	int i=0;
	int ret=0;
	if (!disp || !array){
		return -1;
	}

	for (i=0; i<elt_count; i++){
		ret=kn_eventdisp_register_subscriber(disp, &array[i]);
		if (ret<0){
			return ret;
		}
	}
	return 0;
}

int kn_eventdisp_register_subscriber(kn_event_dispatcher_t *disp, kn_event_reg_t* event_reg){
	int i=0, j=0;
	int id;
	if (!disp || !event_reg){
		return -1;
	}
	if (disp->map_elements_count>=disp->map_size){
		//no room left
		return -1;
	}
	id=event_reg->id;

	if (disp->map[0].id==0){
		disp->map[0].id=id;
		disp->map[0].reg=event_reg;
		disp->map_elements_count=1;
		return 0;
	}

	for (i=1; i<disp->map_size; i++){
		if ((disp->map[i].id==0)
			||((disp->map[i].id>=id) && (disp->map[i-1].id<=id))){
			
			//Make room
			for (j=disp->map_elements_count; j>i; --j){
				disp->map[j]=disp->map[j-1]; //yup that's legal
			}
			disp->map_elements_count++;

			disp->map[i].id=id;
			disp->map[i].reg=event_reg;
			return 0;
		}
	}
	//Should not be reached
	return -1;
}


int kn_eventdisp_get_registered_count(kn_event_dispatcher_t *disp){
	if (!disp){
		return -1;
	}
	return disp->map_elements_count;
}


void kn_eventdisp_on_event(kn_event_worker_t *worker, kn_event_t *event){
	kn_event_dispatcher_t *disp=(kn_event_dispatcher_t *)worker->user_data;
	if (disp){
		kn_eventdisp_broadcast(disp, event);
	}
}



int kn_eventdisp_broadcast(kn_event_dispatcher_t *disp, kn_event_t *event){
	if (!disp || !disp->map){
		return -1;
	}
	if (!disp->map_elements_count){
		return 0;
	}

	int i=0;
	int min=0, max=disp->map_elements_count;
	int id=event->id;

	//printf("Looking for id %d\n", id);

	// Find 1st occurence of id in ordered map by dichotomy
	// Not a classic dichotomy because id may be present several times
	while ((max-min)>1){
		if (disp->map[min].id == id){
			//found it trivially
			i=min;
			//printf("->found=%d\n", i);
			break;
		}
		i=min+(max-min)/2;
		//printf("min %d, max %d, i %d, id[i] %d\n", min, max, i, map[i].id);
		if (disp->map[i].id == id){
			// We are close to the solution, so dealing with edge cases
			if (i==(min+1)){
				// That's it, we found the first occurence of id
				//printf("->found=%d\n", i);
				break;
			}else{
				max=i+1;// That is perfectly safe, compute yourself if you don't trust me
				//printf("->max=%d\n", max);
			}
		}
		else if (disp->map[i].id < id){
			min=i;
			//printf("->min=%d\n", min);
		}
		else{
			max=i;
			//printf("->max=%d\n", max);
		}
	}

	// index i is noow the 1st occurence of 'id' in the map
	// Broadcast message to all occurences
	while(disp->map[i].id==id){
		if (disp->map[i].reg && disp->map[i].reg->handler){
			disp->map[i].reg->handler(event, disp->map[i].reg->user_data);
		}
		i++;
	}
	return 0;
}