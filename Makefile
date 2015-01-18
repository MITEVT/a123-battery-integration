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


MAP=-Wl,-Map=$(BUILD_DIR)/$(NAME).map

LDSCRIPTS=-L. -L $(NXP_BASE)/ldscripts -T gcc.ld

INC = -I $(NXP_BASE)/CMSIS/CM0/DeviceSupport/NXP/LPC11xx/ -I $(NXP_BASE)/CMSIS/CM0/CoreSupport/ -I $(UNITY_BASE)/src

LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)

TARGET=blink-CM0

all: $(BUILD_DIR)/$(TARGET).hex

%.hex: %.axf
	@arm-none-eabi-size $^;
	@arm-none-eabi-objcopy -O ihex $^ $@;

$(BUILD_DIR)/$(NAME)-$(CORE).axf: $(SRC_DIR)/$(NAME).c $(STARTUP) $(SYSTEM)
	$(CC) $^ $(CFLAGS) $(INC) $(LFLAGS) -o $@

clean:
	rm -f $(BUILD_DIR)/*.axf $(BUILD_DIR)/*.map $(BUILD_DIR)/*.o $(BUILD_DIR)/*.hex
