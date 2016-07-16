/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#include "kn_events_fifo.h"

#include <stdio.h> //for debug printf



int kn_fifo_init(kn_fifo_t*f, kn_event_t *buf, kn_fifo_size_t size){
	if (!f || !buf){
		return -1;
	}
	f->buf=buf;
	f->size=size;
	f->mask=size-1;//this little trick only works for size=2^n
	f->read=0;
	f->write=0;
	return 0;
}


int kn_fifo_post(kn_fifo_t*f, kn_event_t *event){
	if (!f || !event){
		return -1;
	}

	int  new_w;
	new_w = (f->write+1) & (f->mask);
	//printf("read=%d, write=%d, new_w=%d\n", f->read, f->write, new_w);
	if (new_w == f->read){
		//not enough room
		return -1;
	}
	f->buf[f->write]=*event;

	f->write=new_w;
	return 0;
}


int kn_fifo_get(kn_fifo_t *f, kn_event_t *event){
	if (!f || !event){
		return -1;
	}

	if (f->read==f->write){
		//empty queue
		return -1;
	}

	*event = f->buf[f->read];
	f->read = (f->read+1) & (f->mask);
	return 0;
}

int kn_fifo_count(kn_fifo_t*f){
	if (!f){
		return -1;
	}
	return (f->size + f->write - f->read) & f->mask;
}

int kn_fifo_free(kn_fifo_t*f){
	if (!f){
		return -1;
	}
	return f->size - kn_fifo_count(f)-1; 
}