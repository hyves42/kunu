#!/bin/sh
rm -f test
gcc ../src/kn_state.c ../src/kn_events_dispatch.c ../src/kn_events_fifo.c\
	../src/kn_sched.c ../src/kn_power_sched.c ../src/kn_timer.c\
	../src/kn_thread.c\
	tests_state_machine.c tests_events_dispatch.c\
	tests_events_fifo.c tests_sched.c tests_thread.c\
	tests_timer.c\
	../arch/linux/kn_sleep.c\
	main.c -o test\
	-I. -I../inc `pkg-config --cflags check` `pkg-config --libs check`