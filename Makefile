# 
# This file is part of Kunu, a bare-metal task scheduler for microcontrollers.
# 
# Copyright (c) 2017 hyves42
# 
# This file is licensed according to the terms of the MIT license. 
# Full text of license is in /LICENSE file


all: linux unit_tests


linux: 
	cd arch/linux && make

unit_tests: linux
	cd unit_tests && make

distclean:
	cd arch/linux && make clean