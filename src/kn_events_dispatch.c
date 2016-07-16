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
static kn_event_reg_entry_t* map=NULL;
static int map_size=0;
static int map_elements_count=0;


int kn_events_init(kn_event_reg_entry_t* a_map, int a_map_size){
	int i=0;

	if (!a_map || a_map_size==0){
		return -1;
	}
	map=a_map;
	map_size=a_map_size;
	for (; i<map_size; i++){
		if (map[i].id==0 || map[i].reg==NULL) break;
	}
	map_elements_count=i;
}

int kn_events_register_subscriber_array(kn_event_reg_t* array, int elt_count){
	int i=0;
	int ret=0;
	if (!array){
		return -1;
	}

	for (i=0; i<elt_count; i++){
		ret=kn_events_register_subscriber(&array[i]);
		if (ret<0){
			return ret;
		}
	}
	return 0;
}

int kn_events_register_subscriber(kn_event_reg_t* event_reg){
	int i=0, j=0;
	int id;
	if (!event_reg){
		return -1;
	}
	if (map_elements_count>=map_size){
		//no room left
		return -1;
	}
	id=event_reg->id;

	if (map[0].id==0){
		map[0].id=id;
		map[0].reg=event_reg;
		map_elements_count=1;
		return 0;
	}

	for (i=1; i<map_size; i++){
		if ((map[i].id==0)
			||((map[i].id>=id) && (map[i-1].id<=id))){
			
			//Make room
			for (j=map_elements_count; j>i; --j){
				map[j]=map[j-1]; //yup that's legal
			}
			map_elements_count++;

			map[i].id=id;
			map[i].reg=event_reg;
			return 0;
		}
	}
	//Should not be reached
	return -1;
}


int kn_events_get_registered_count(void){
	return map_elements_count;
}



int kn_events_broadcast(int id, void *data){
	int i;
	int min=0, max=map_elements_count;
	if (!map || !map_elements_count){
		return 0;
	}

	//printf("Looking for id %d\n", id);

	// Find 1st occurence of id in ordered map by dichotomy
	// Not a classic dichotomy because id may be present several times
	while ((max-min)>1){
		if (map[min].id == id){
			//found it trivially
			i=min;
			//printf("->found=%d\n", i);
			break;
		}
		i=min+(max-min)/2;
		//printf("min %d, max %d, i %d, id[i] %d\n", min, max, i, map[i].id);
		if (map[i].id == id){
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
		else if (map[i].id < id){
			min=i;
			//printf("->min=%d\n", min);
		}
		else{
			max=i;
			//printf("->max=%d\n", max);
		}
	}
	while(map[i].id==id){
		if (map[i].reg && map[i].reg->handler){
			map[i].reg->handler(id, data, map[i].reg->user_data);
		}
		i++;
	}
	return 0;
}