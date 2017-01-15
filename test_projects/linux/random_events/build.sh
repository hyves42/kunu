#!/bin/sh
rm -f test
TOP=../../..
gcc $TOP/src/kn_state.c $TOP/src/kn_events_dispatch.c $TOP/src/kn_events_fifo.c\
	$TOP/src/kn_sched.c $TOP/src/kn_power_sched.c $TOP/src/kn_timer.c\
	$TOP/src/kn_thread.c $TOP/src/kn_tick.c $TOP/src/kn_component.c\
	$TOP/arch/linux/kn_sleep.c $TOP/arch/linux/kn_platform.c\
	main.c -o test_project\
	-I. -I$TOP/inc -I$TOP/arch/linux `pkg-config --cflags check` `pkg-config --libs check` -lpthread -lrt -g