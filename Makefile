UNITY_BASE=../Unity
NXP_BASE=../lpc11cx4-io
SRC_DIR=src
BUILD_DIR=build

include $(NXP_BASE)/makefile.conf
NAME=blink

STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main -DRAM_MODE=1

# Need following option for LTO as LTO will treat retarget functions as
# unused without following option
CFLAGS+=-fno-builtin

LDSCRIPTS=-L. -L $(NXP_BASE)/ldscripts -T gcc.ld

INC = -I $(NXP_BASE)/CMSIS/CM0/DeviceSupport/NXP/LPC11xx/ -I $(NXP_BASE)/CMSIS/CM0/CoreSupport/ -I $(UNITY_BASE)/src

LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)

TARGET=blink-CM0

all: $(TARGET).hex

%.hex: %.axf
	@arm-none-eabi-size $^;
	@arm-none-eabi-objcopy -O ihex $^ $@;

$(NAME)-$(CORE).axf: $(SRC_DIR)/$(NAME).c $(STARTUP) $(SYSTEM)
	$(CC) $^ $(CFLAGS) $(INC) $(LFLAGS) -o $@

clean:
	rm -f *.axf *.map *.o *.hex
