#!/bin/sh
rm -f test
gcc ../src/kn_state.c ../src/kn_events_dispatch.c ../src/kn_events_fifo.c\
	../src/kn_sched.c ../src/kn_power_sched.c ../src/kn_timer.c\
	../src/kn_thread.c ../src/kn_tick.c ../src/kn_component.c\
	../arch/linux/kn_sleep.c ../arch/linux/kn_platform.c\
	main.c -o test_project\
	-I. -I../inc -I../arch/linux `pkg-config --cflags check` `pkg-config --libs check` -lpthread -lrt -g