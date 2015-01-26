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


###############

###
# Directory Structure
BINDIR=bin
SRCDIR=src
BUILDDIR=build

DRIVERS= 	gpio_11xx_2.c \

DRIVERS_SRC=$(addprefix $(CHIP_SRC)/, $(DRIVERS)) 

ASOURCES=$(shell find -L $(SRCDIR) -name '*.S')
ASOURCES+=$(STARTUP)
CSOURCESI=$(shell find -L $(SRCDIR) -name '*.c')
CSOURCES=$(CSOURCESI) $(DRIVERS_SRC)

OBJECTS=$(ASOURCES:%.S=%.o)
OBJECTS+=$(CSOURCES:%.c=%.o)



BINELF=outp.elf
BINHEX=outp.hex

include $(NXP_BASE)/makefile.conf

STARTUP_DEFS=-D__STARTUP_CLEAR_BSS -D__START=main -DRAM_MODE=1


# Need following option for LTO as LTO will treat retarget functions as
# unused without following option
CFLAGS+=-fno-builtin -DCORE_M0

LDSCRIPTS=-L. -L $(NXP_BASE)/ldscripts -T gcc.ld

INC = $(CHIP_INC)

DEFINE= -DCORE_M0
CFLAGS= -c $(MCFLAGS) $(INC) $(DEFINE) $(USE_NOHOST)

LFLAGS=$(USE_NANO) $(USE_NOHOST) $(LDSCRIPTS) $(GC) $(MAP)


# System code
SYSTEM=sysinit.c

TARGET=$(NAME)-CM0

all: release

release: $(BINDIR)/$(BINHEX)

$(BINDIR)/$(BINHEX): $(BINDIR)/$(BINELF)
	$(CP) -O ihex $< $@
	@echo "Objcopy from ELF to IHEX complete!\n"

$(BINDIR)/$(BINELF): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!\n"
	$(SIZE) $(BINDIR)/$(BINELF)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled "$<"!\n"

%.o: %.S
	$(CC) $(CFLAGS) $< -o $@
	@echo "Assambled "$<"!\n"

clean:
	rm -f $(OBJECTS) $(BINDIR)/$(BINELF) $(BINDIR)/$(BINHEX) $(BINDIR)/output.map


.PHONY: test
test:
	$(TEST_C_COMPILER) $(TEST_INC) $(TEST_SYMBOLS) $(TEST_SRC) -o $(TEST_TARGET)
	./$(TEST_TARGET)
