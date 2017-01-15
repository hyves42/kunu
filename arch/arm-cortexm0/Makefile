# 
# This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
# 
# Copyright (c) 2017 hyves42
# 
# This file is licensed according to the terms of the MIT license. 
# Full text of license is in /LICENSE file


TARGET_ARCH=arm-cortex-m0

PREFIX		?= arm-none-eabi
CC		:= $(PREFIX)-gcc
CXX		:= $(PREFIX)-g++
LD		:= $(PREFIX)-gcc
AR		:= $(PREFIX)-ar
AS		:= $(PREFIX)-as
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB		:= $(PREFIX)-gdb

CFLAGS =  -msoft-float -mthumb -mcpu=cortex-m0

KUNU_TOPDIR=../..
include ../../kunu.files.mk


CFLAGS += -I$(KUNU_INC_DIR) -I$(KUNU_TOPDIR)/src -I.

KUNU_ARCH_SRC=kn_platform.c kn_sleep.c

SRC= $(KUNU_ARCH_SRC) $(KUNU_SRC)
OBJS = $(SRC:.c=.$(TARGET_ARCH).o)


TARGET=libkunu.$(TARGET_ARCH).a


all: $(TARGET)


%.$(TARGET_ARCH).o: %.c $(KUNU_HEADERS)
	$(CC) $(CFLAGS) -o $@ -c $<


libkunu.$(TARGET_ARCH).a: $(OBJS)
	ar rcs $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)