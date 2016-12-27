/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */

#ifndef KN_FIFO_H
#define KN_FIFO_H

#include "kn_event.h"

// multi producer/single consumer queue

typedef struct kn_fifo_t kn_fifo_t;

struct kn_fifo_t{
	kn_event_t *buf;
	unsigned int size; //number of elements in buf[], not a size in bytes
	unsigned int read;
	unsigned int write;
	unsigned int mask;
};

//Value of authorized fifo sizes. Must be a power of 2, or the magic won't work
typedef enum {
	FIFO_SIZE_2=2,
	FIFO_SIZE_4=4,
	FIFO_SIZE_8=8,
	FIFO_SIZE_16=16,
	FIFO_SIZE_32=32,
	FIFO_SIZE_64=64,
	//etc
}kn_fifo_size_t;

//This macro only works if buf is a statically allocated array of a size defined in kn_fifo_size_t
//  If you use this macro for initialization, there is no need to call kn_fifo_init()
#define KN_FIFO_INIT(_buf) {\
	.buf=(_buf),\
	.size=sizeof(_buf)/sizeof((_buf)[0]),\
	.mask=(sizeof(_buf)/sizeof((_buf)[0]))-1,\
	.read=0,\
	.write=0}

int kn_fifo_init(kn_fifo_t*f, kn_event_t *buf, kn_fifo_size_t size); //size must be a power of 2

int kn_fifo_post(kn_fifo_t*f, kn_event_t *event);

int kn_fifo_get(kn_fifo_t*f, kn_event_t *event);

int kn_fifo_count(kn_fifo_t*f);

int kn_fifo_free(kn_fifo_t*f);

//TODO add some way to send events to an event_worker, and an event worker interface maybe

#endif //KN_FIFO_H