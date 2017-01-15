# 
# This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
# 
# Copyright (c) 2017 hyves42
# 
# This file is licensed according to the terms of the MIT license. 
# Full text of license is in /LICENSE file
#

KUNU_TOPDIR ?= .

KUNU_SRC = $(KUNU_TOPDIR)/src/kn_state.c \
	$(KUNU_TOPDIR)/src/kn_events_dispatch.c \
	$(KUNU_TOPDIR)/src/kn_events_fifo.c \
	$(KUNU_TOPDIR)/src/kn_sched.c \
	$(KUNU_TOPDIR)/src/kn_power_sched.c \
	$(KUNU_TOPDIR)/src/kn_timer.c \
	$(KUNU_TOPDIR)/src/kn_thread.c \
	$(KUNU_TOPDIR)/src/kn_tick.c \
	$(KUNU_TOPDIR)/src/kn_component.c 

KUNU_INC_DIR = $(KUNU_TOPDIR)/inc

KUNU_HEADERS = $(wildcard $(KUNU_TOPDIR)/inc/*.h) $(wildcard $(KUNU_TOPDIR)/src/*.h)
	