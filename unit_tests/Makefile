# 
# This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
# 
# Copyright (c) 2017 hyves42
# 
# This file is licensed according to the terms of the MIT license. 
# Full text of license is in /LICENSE file


CC		:= gcc
CXX		:= g++
LD		:= gcc
AR		:= ar
AS		:= as
OBJCOPY		:= objcopy
OBJDUMP		:= objdump
GDB		:= gdb


KUNU_TOPDIR=..
include ../kunu.files.mk

CFLAGS=-I$(KUNU_INC_DIR) -I. -I$(KUNU_TOPDIR)/arch/linux
LDFLAGS=`pkg-config --cflags check` `pkg-config --libs check` -lpthread -lrt
LDFLAGS+= -lkunu.x86-linux -L$(KUNU_TOPDIR)/arch/linux

SRC=tests_component.c \
	tests_state_machine.c \
	tests_events_dispatch.c \
	tests_events_fifo.c \
	tests_sched.c \
	tests_thread.c \
	tests_timer.c \
	tests_linux_simul.c \
	tests_system.c \
	thread_helper.c \
	main.c

OBJS = $(SRC:.c=.o)



TARGET=unit_tests


all: $(TARGET) run_tests


run_tests: $(TARGET)
	echo "Running unit tests"
	./$(TARGET)


$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c $(KUNU_HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<


kunu.x86-linux.a: $(OBJS)
	ar rcs $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)