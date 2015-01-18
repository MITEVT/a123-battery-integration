UNITY_BASE=../Unity
NXP_BASE=../lpc11cx4-io
SRC_DIR=src
BUILD_DIR=build

include $(NXP_BASE)/makefile.conf
#setup for unity
TEST_C_COMPILER=gcc
TEST_TARGET=$(BUILD_DIR)/all_tests.out
TEST_INC=-Isrc -Itest -I$(UNITY_BASE)/src -I$(UNITY_BASE)/extras/fixture/src
TEST_SRC=\
  $(UNITY_BASE)/src/unity.c \
  $(UNITY_BASE)/extras/fixture/src/unity_fixture.c \
  test/TestProductionCode.c \
  test/test_runners/TestProductionCode_Runner.c \
  test/test_runners/all_tests.c

NAME=blink

STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main -DRAM_MODE=1

# Need following option for LTO as LTO will treat retarget functions as
# unused without following option
CFLAGS+=-fno-builtin


MAP=-Wl,-Map=$(BUILD_DIR)/$(NAME).map

LDSCRIPTS=-L. -L $(NXP_BASE)/ldscripts -T gcc.ld

INC = -I $(NXP_BASE)/CMSIS/CM0/DeviceSupport/NXP/LPC11xx/ -I $(NXP_BASE)/CMSIS/CM0/CoreSupport/

LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)

TARGET=blink-CM0

all: $(BUILD_DIR)/$(TARGET).hex test

.PHONY: test
test:
	$(TEST_C_COMPILER) $(TEST_INC) $(TEST_SYMBOLS) $(TEST_SRC) -o $(TEST_TARGET)
	./$(TEST_TARGET)

%.hex: %.axf
	@arm-none-eabi-size $^;
	@arm-none-eabi-objcopy -O ihex $^ $@;

$(BUILD_DIR)/$(NAME)-$(CORE).axf: $(SRC_DIR)/$(NAME).c $(STARTUP) $(SYSTEM)
	$(CC) $^ $(CFLAGS) $(INC) $(LFLAGS) -o $@

clean:
	rm -f $(BUILD_DIR)/*.axf $(BUILD_DIR)/*.map $(BUILD_DIR)/*.o $(BUILD_DIR)/*.hex
