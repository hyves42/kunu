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
	unsigned int size;
	unsigned int read;
	unsigned int write;
	unsigned int mask;
};

//Valud fifo sizes. Must be a power of 2
typedef enum {
	FIFO_SIZE_2=2,
	FIFO_SIZE_4=4,
	FIFO_SIZE_8=8,
	FIFO_SIZE_16=16,
	FIFO_SIZE_32=32,
	FIFO_SIZE_64=64,
	//etc
}kn_fifo_size_t;

int kn_fifo_init(kn_fifo_t*f, kn_event_t *buf, kn_fifo_size_t size); //size must be a power of 2

int kn_fifo_post(kn_fifo_t*f, kn_event_t *event);

int kn_fifo_get(kn_fifo_t*f, kn_event_t *event);

int kn_fifo_count(kn_fifo_t*f);

int kn_fifo_free(kn_fifo_t*f);

#endif //KN_FIFO_H