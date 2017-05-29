
PREFIX		?= arm-none-eabi
CC		:= $(PREFIX)-gcc
CXX		:= $(PREFIX)-g++
LD		:= $(PREFIX)-gcc
AR		:= $(PREFIX)-ar
AS		:= $(PREFIX)-as
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB		:= $(PREFIX)-gdb



systick-sleep_SRC = systick-sleep.c
systick-sleep_OBJS = $(systick-sleep_SRC:.c=.o)

uart-blocking_SRC = uart-blocking.c
uart-blocking_OBJS = $(uart-blocking_SRC:.c=.o)

uart-interrupt_SRC = uart-interrupt.c
uart-interrupt_OBJS = $(uart-interrupt_SRC:.c=.o)

wut-sleep_SRC = wut-sleep.c
wut-sleep_OBJS = $(wut-sleep_SRC:.c=.o)

TARGET = systick-sleep.elf uart-blocking.elf uart-interrupt.elf wut-sleep.elf
TARGET_BIN = $(patsubst %.elf,%.bin, $(TARGET))
TARGET_HEX = $(patsubst %.elf,%.hex, $(TARGET))

OPENCM3_DIR=libopencm3
LDSCRIPT = ./stm32f030f4.ld


OPENCM3_LDFLAGS	= -lopencm3_stm32f0 -L$(OPENCM3_DIR)/lib
OPENCM3_CFLAGS	= -I$(OPENCM3_DIR)/include -DSTM32F0

CFLAGS =  -msoft-float -mthumb -mcpu=cortex-m0
CFLAGS += -O2
CFLAGS += -Wextra -Wshadow -Wimplicit-function-declaration
CFLAGS += -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
CFLAGS += -fno-common -ffunction-sections -fdata-sections

CFLAGS += $(OPENCM3_CFLAGS)
CFLAGS += -g



LDFLAGS	= --static -nostartfiles -g -L.
LDFLAGS	+= -T$(LDSCRIPT)
LDFLAGS += $(OPENCM3_LDFLAGS)
LDFLAGS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group 
LDFLAGS += -msoft-float -mthumb -mcpu=cortex-m0


OPENOCD = /usr/bin/openocd
OCD_SHARE = /usr/share/openocd
# Flash with cheap ST-link from ebay
OPENOCD_CONF = -f $(OCD_SHARE)/scripts/interface/stlink-v2.cfg -f $(OCD_SHARE)/scripts/target/stm32f0x.cfg
# Flash with ST-Link part of nucleo board
#OPENOCD_CONF = -f stlink-v2-1.cfg -f $(OCD_SHARE)/scripts/target/stm32f0x.cfg



.PHONY: clean flash

all: $(TARGET) $(TARGET_BIN) $(TARGET_HEX)


%.bin: %.elf
	$(OBJCOPY) -Obinary $< $@

%.hex: %.elf
	$(OBJCOPY) -Oihex $< $@




%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

systick-sleep.elf: $(systick-sleep_OBJS)
	$(LD) $^ $(LDFLAGS)  -o $@


uart-blocking.elf: $(uart-blocking_OBJS)
	$(LD) $^ $(LDFLAGS)  -o $@

wut-sleep.elf: $(wut-sleep_OBJS)
	$(LD) $^ $(LDFLAGS)  -o $@


uart-interrupt.elf: $(uart-interrupt_OBJS)
	$(LD) $^ $(LDFLAGS)  -o $@


flash:
	$(OPENOCD) $(OPENOCD_CONF) -c "init" -c "reset halt" -c "flash write_image erase $(hex)" -c "reset" -c "shutdown"


gdb:
	$(OPENOCD) $(OPENOCD_CONF) -c "init" -c "reset halt" 

clean:
	rm -f $(TARGET) $(TARGET_BIN) $(TARGET_HEX) $(OBJS)