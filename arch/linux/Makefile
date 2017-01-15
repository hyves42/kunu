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


KUNU_TOPDIR=../..
include ../../kunu.files.mk


CFLAGS=-I$(KUNU_INC_DIR) -I$(KUNU_TOPDIR)/src -I.

KUNU_LINUX_SRC=kn_platform.c kn_sleep.c

SRC= $(KUNU_LINUX_SRC) $(KUNU_SRC)
OBJS = $(SRC:.c=.x86-linux.o)



TARGET=libkunu.x86-linux.a


all: $(TARGET)


%.x86-linux.o: %.c $(KUNU_HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<


libkunu.x86-linux.a: $(OBJS)
	ar rcs $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)