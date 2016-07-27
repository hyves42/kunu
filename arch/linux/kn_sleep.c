/* 
 * This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
 * 
 * Copyright (c) 2016 hyves42
 * 
 * This file is licensed according to the terms of the MIT license. 
 * Full text of license is in /LICENSE file
 */


// This file provides an emulation of the sleep mode of a microcontroller
// Emulating sleep needs two threads : a main kunu thread and a simulation thread
// The kunu thread can be put to sleep while the other thread will simulate 
// hw interrupts or tick interrupts by writing into a pipe, thus waking the other thread

#include "kn_linux_sleep.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/poll.h>

static int pipe_fd[2]={-1,-1};

static const int read_id=0;
static const int write_id=1;

// we will differentiate hw interrupts and simple ticks by writing a different character into the pipe
static const char tick_char='t';
static const char int_char='i';

int kn_platform_linux_sleep_init(void){
	//open pipe fds
	return pipe2(pipe_fd, O_NONBLOCK);
}


int kn_platform_linux_sleep_exit(void){
	close(pipe_fd[write_id]);
	close(pipe_fd[read_id]);
	return 0;
}

int kn_platform_sleep_for(int ticks){
	struct pollfd ufds[1]={{
		.fd=pipe_fd[read_id],
		.events = POLLIN,
	}};
	int count=0;


	if (pipe_fd[read_id]<0){
		return -1;
	}

	while (1==poll(ufds, 1, 0)){
		char c;
		while(1==read(pipe_fd[read_id], &c, 1)){
			if (c==tick_char){
				count++;
			}

			if (count>=ticks || c==int_char){
				return 0;
			}
		}
	}
	return -1;
}

int kn_platform_sleep_forever(void){
	struct pollfd ufds[1]={{
		.fd=pipe_fd[read_id],
		.events = POLLIN,
	}};
	int count=0;


	if (pipe_fd[read_id]<0){
		return -1;
	}

	while (1==poll(ufds, 1, 0)){
		char c;
		while(1==read(pipe_fd[read_id], &c, 1)){
			if (c==int_char){
				return 0;
			}
		}
	}
	return -1;
}

int kn_platform_linux_simulate_interrupt(void){
	if (pipe_fd[write_id]<0){
		return -1;
	}
	return (1==write(pipe_fd[write_id], &int_char, sizeof(int_char)));
}

int kn_platform_linux_simulate_tick(void){
	if (pipe_fd[write_id]<0){
		return -1;
	}
	return (1==write(pipe_fd[write_id], &tick_char, sizeof(tick_char)));
}