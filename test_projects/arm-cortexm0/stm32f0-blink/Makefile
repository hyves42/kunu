
PREFIX		?= arm-none-eabi
CC		:= $(PREFIX)-gcc
CXX		:= $(PREFIX)-g++
LD		:= $(PREFIX)-gcc
AR		:= $(PREFIX)-ar
AS		:= $(PREFIX)-as
OBJCOPY		:= $(PREFIX)-objcopy
OBJDUMP		:= $(PREFIX)-objdump
GDB		:= $(PREFIX)-gdb



SRC = main.c
OBJS = $(SRC:.c=.o)

TARGET = miniblink.elf
TARGET_BIN = $(TARGET:.elf=.bin)
TARGET_HEX = $(TARGET:.elf=.hex)

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



LDFLAGS	= --static -nostartfiles -g -L.
LDFLAGS	+= -T$(LDSCRIPT)
LDFLAGS += $(OPENCM3_LDFLAGS)
LDFLAGS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group


OPENOCD = /usr/local/bin/openocd
OCD_SHARE = /usr/local/share/openocd
# Flash with cheap ST-link from ebay
OPENOCD_CONF = -f $(OCD_SHARE)/scripts/interface/stlink-v2.cfg -f $(OCD_SHARE)/scripts/target/stm32f0x.cfg
# Flash with ST-Link part of nucleo board
#OPENOCD_CONF = -f stlink-v2-1.cfg -f $(OCD_SHARE)/scripts/target/stm32f0x.cfg



.PHONY: clean flash


$(TARGET_BIN): $(TARGET)
	$(OBJCOPY) -Obinary $< $@

$(TARGET_HEX): $(TARGET)
	$(OBJCOPY) -Oihex $< $@


clean:
	rm -f $(TARGET) $(TARGET_BIN) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET): $(OBJS) 
	$(LD) $(OBJS) $(LDFLAGS)  -o $@


all: $(TARGET) $(TARGET_BIN)

flash:
	$(OPENOCD) $(OPENOCD_CONF) -c "init" -c "reset halt" -c "flash write_image erase $(TARGET)" -c "reset" -c "shutdown"

