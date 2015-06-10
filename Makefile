#=============================================================================#
# ARM makefile
#
# author: Freddie Chopin, http://www.freddiechopin.info/
# last change: 2012-01-08
#
# this makefile is based strongly on many examples found in the network
#=============================================================================#

#=============================================================================#
# toolchain configuration
#=============================================================================#

TOOLCHAIN = arm-none-eabi-

CXX_CROSS = $(TOOLCHAIN)g++
CC_CROSS = $(TOOLCHAIN)gcc
AS_CROSS = $(TOOLCHAIN)gcc -x assembler-with-cpp
OBJCOPY_CROSS = $(TOOLCHAIN)objcopy
OBJDUMP_CROSS = $(TOOLCHAIN)objdump
SIZE_CROSS = $(TOOLCHAIN)size
RM = rm -f

#=============================================================================#
# test configuration
#=============================================================================#

UNITY_BASE=../Unity
CXX_TEST = g++
CC_TEST = gcc
AS_TEST = gcc -x assembler-with-cpp
SIZE_TEST = size

#=============================================================================#
# project configuration
#=============================================================================#

# project name
PROJECT = BCM

# core type
CORE = cortex-m0

# linker script
LD_SCRIPT = gcc.ld

# output folder (absolute or relative path, leave empty for in-tree compilation)
OUT_DIR = bin

# test out folder
OUT_DIR_TEST = testbin

# directories for testing sources
TEST_SRCS_DIRS = test $(UNITY_BASE)/src $(UNITY_BASE)/extras/fixture/src

# c files for testing
C_SRCS_TEST = $(wildcard $(patsubst %, %/*.$(C_EXT), . $(TEST_SRCS_DIRS))) src/charge.c

# C++ definitions (e.g. "-Dsymbol_with_value=0xDEAD -Dsymbol_without_value")
CXX_DEFS =

# C definitions
C_DEFS = -DCORE_M0

# ASM definitions
AS_DEFS = -D__STARTUP_CLEAR_BSS -D__START=main -DRAM_MODE=1

# include directories (absolute or relative paths to additional folders with
# headers, current folder is always included)
INC_DIRS_CROSS = ../lpc11cx4-library/lpc_chip_11cxx_lib/inc ../lpc11cx4-library/evt_lib/inc inc

# library directories (absolute or relative paths to additional folders with
# libraries)
LIB_DIRS = 

# libraries (additional libraries for linking, e.g. "-lm -lsome_name" to link
# math library libm.a and libsome_name.a)
LIBS =

# additional directories with source files (absolute or relative paths to
# folders with source files, current folder is always included)
SRCS_DIRS = ../lpc11cx4-library/lpc_chip_11cxx_lib/src ../lpc11cx4-library/evt_lib/src src

# include directories for test
INC_DIRS_TEST = $(INC_DIRS_CROSS) $(SRCS_DIRS) test $(UNITY_BASE)/src $(UNITY_BASE)/extras/fixture/src

# extension of C++ files
CXX_EXT = cpp

# wildcard for C++ source files (all files with CXX_EXT extension found in
# current folder and SRCS_DIRS folders will be compiled and linked)
CXX_SRCS = $(wildcard $(patsubst %, %/*.$(CXX_EXT), . $(SRCS_DIRS)))

# extension of C files
C_EXT = c

# wildcard for C source files (all files with C_EXT extension found in current
# folder and SRCS_DIRS folders will be compiled and linked)
C_SRCS = $(wildcard $(patsubst %, %/*.$(C_EXT), . $(SRCS_DIRS)))

# extension of ASM files
AS_EXT = S

# wildcard for ASM source files (all files with AS_EXT extension found in
# current folder and SRCS_DIRS folders will be compiled and linked)
AS_SRCS = $(wildcard $(patsubst %, %/*.$(AS_EXT), . $(SRCS_DIRS)))

# optimization flags ("-O0" - no optimization, "-O1" - optimize, "-O2" -
# optimize even more, "-Os" - optimize for size or "-O3" - optimize yet more) 
OPTIMIZATION = -Os

# set to 1 to optimize size by removing unused code and data during link phase
REMOVE_UNUSED = 1

# set to 1 to compile and link additional code required for C++
USES_CXX = 0

# define warning options here
CXX_WARNINGS = -Wall -Wextra
C_WARNINGS = -Wall -Wstrict-prototypes -Wextra

# C++ language standard ("c++98", "gnu++98" - default, "c++0x", "gnu++0x")
CXX_STD = gnu++98

# C language standard ("c89" / "iso9899:1990", "iso9899:199409",
# "c99" / "iso9899:1999", "gnu89" - default, "gnu99")
C_STD = gnu89

#=============================================================================#
# Write Configuration
#=============================================================================#

# COMPORT = /dev/tty.usbserial-FTH7MMTD
COMPORT = $(word 1, $(wildcard /dev/tty.usbserial-*) $(wildcard /dev/ttyUSB*))
# ifeq ($(strip $(foo)),)
# 	COMPORT = /dev/tty.usbserial-FTH7MMTD
# endif
BAUDRATE = 115200
CLOCK_OSC = 12000

#=============================================================================#
# set the VPATH according to SRCS_DIRS
#=============================================================================#

VPATH = $(SRCS_DIRS) test $(UNITY_BASE)/extras/fixture/src $(UNITY_BASE)/src devices

#=============================================================================#
# when using output folder, append trailing slash to its name
#=============================================================================#

ifeq ($(strip $(OUT_DIR)), )
	OUT_DIR_F =
else
	OUT_DIR_F = $(strip $(OUT_DIR))/
endif

#=============================================================================#
# when using output folder, append trailing slash to its name
#=============================================================================#

ifeq ($(strip $(OUT_DIR_TEST)), )
	OUT_DIR_TEST_F =
else
	OUT_DIR_TEST_F = $(strip $(OUT_DIR_TEST))/
endif

#=============================================================================#
# various compilation flags
#=============================================================================#

# core flags
CORE_FLAGS = -mcpu=$(CORE) -mthumb

# flags for C++ compiler
CXX_FLAGS = -std=$(CXX_STD) -g -ggdb3 -fno-rtti -fno-exceptions -fverbose-asm -Wa,-ahlms=$(OUT_DIR_F)$(notdir $(<:.$(CXX_EXT)=.lst))

# flags for C compiler
C_FLAGS = -std=$(C_STD) -g -ggdb3 -fverbose-asm -Wa,-ahlms=$(OUT_DIR_F)$(notdir $(<:.$(C_EXT)=.lst))

# flags for assembler
AS_FLAGS = -g -ggdb3 -Wa,-amhls=$(OUT_DIR_F)$(notdir $(<:.$(AS_EXT)=.lst))

# flags for linker
LD_FLAGS = -T$(LD_SCRIPT) -g -Wl,-Map=$(OUT_DIR_F)$(PROJECT).map,--cref,--no-warn-mismatch

# process option for removing unused code
ifeq ($(REMOVE_UNUSED), 1)
	LD_FLAGS += -Wl,--gc-sections
	OPTIMIZATION += -ffunction-sections -fdata-sections
endif

# if __USES_CXX is defined for ASM then code for global/static constructors /
# destructors is compiled; if -nostartfiles option for linker is added then C++
# initialization / finalization code is not linked
ifeq ($(USES_CXX), 1)
	AS_DEFS += -D__USES_CXX
else
	LD_FLAGS += -nostartfiles
endif

#=============================================================================#
# do some formatting
#=============================================================================#

CXX_OBJS_TEST = $(addprefix $(OUT_DIR_TEST_F), $(notdir $(CXX_SRCS_TEST:.$(CXX_EXT)=.o)))
C_OBJS_TEST = $(addprefix $(OUT_DIR_TEST_F), $(notdir $(C_SRCS_TEST:.$(C_EXT)=.o)))
AS_OBJS_TEST = $(addprefix $(OUT_DIR__TESTF), $(notdir $(AS_SRCS_TEST:.$(AS_EXT)=.o)))

TEST_OBJS = $(AS_OBJS_TEST) $(C_OBJS_TEST) $(CXX_OBJS_TEST)

CXX_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(CXX_SRCS:.$(CXX_EXT)=.o)))
C_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(C_SRCS:.$(C_EXT)=.o)))
AS_OBJS = $(addprefix $(OUT_DIR_F), $(notdir $(AS_SRCS:.$(AS_EXT)=.o)))
OBJS_F = $(AS_OBJS) $(C_OBJS) $(CXX_OBJS) $(USER_OBJS)
DEPS = $(OBJS:.o=.d)
INC_DIRS_F_CROSS = -I. $(patsubst %, -I%, $(INC_DIRS_CROSS))
LIB_DIRS_F_CROSS = $(patsubst %, -L%, $(LIB_DIRS))

INC_DIRS_F_TEST = -I. $(patsubst %, -I%, $(INC_DIRS_TEST))

ELF = $(OUT_DIR_F)$(PROJECT).elf
HEX = $(OUT_DIR_F)$(PROJECT).hex
BIN = $(OUT_DIR_F)$(PROJECT).bin
LSS = $(OUT_DIR_F)$(PROJECT).lss
DMP = $(OUT_DIR_F)$(PROJECT).dmp

TEST_TARGET = $(OUT_DIR_TEST_F)$(PROJECT)

# format final flags for tools, request dependancies for C++, C and asm
CXX_FLAGS_F_CROSS = $(CORE_FLAGS) $(OPTIMIZATION) $(CXX_WARNINGS) $(CXX_FLAGS)  $(CXX_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F_CROSS)
C_FLAGS_F_CROSS = $(CORE_FLAGS) $(OPTIMIZATION) $(C_WARNINGS) $(C_FLAGS) $(C_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F_CROSS)
AS_FLAGS_F_CROSS = $(CORE_FLAGS) $(AS_FLAGS) $(AS_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F_CROSS)
LD_FLAGS_F_CROSS = $(CORE_FLAGS) $(LD_FLAGS) $(LIB_DIRS_F_CROSS)

CXX_FLAGS_F_TEST = $(OPTIMIZATION) $(CXX_WARNINGS) $(CXX_FLAGS) $(CXX_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F_TEST)
C_FLAGS_F_TEST =  $(OPTIMIZATION) $(C_WARNINGS) $(C_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F_TEST)
AS_FLAGS_F_TEST = $(AS_FLAGS) $(AS_DEFS) -MD -MP -MF $(OUT_DIR_F)$(@F:.o=.d) $(INC_DIRS_F_TEST)
LD_FLAGS_F_TEST = $(LIB_DIRS_F_TEST)

#contents of output directory
GENERATED = $(wildcard $(patsubst %, $(OUT_DIR_F)*.%, bin d dmp elf hex lss lst map o)) $(wildcard $(OUT_DIR_TEST_F)*)


#=============================================================================#
# make all
#=============================================================================#
all : cross

cross : CXX 		= $(CXX_CROSS)
cross : CC 			= $(CC_CROSS)
cross : AS 			= $(AS_CROSS)
cross : OBJCOPY 	= $(OBJCOPY_CROSS)
cross : OBJDUMP 	= $(OBJDUMP_CROSS)
cross : SIZE 		= $(SIZE_CROSS)
cross : CXX_FLAGS_F = $(CXX_FLAGS_F_CROSS)
cross : C_FLAGS_F 	= $(C_FLAGS_F_CROSS)
cross : AS_FLAGS_F 	= $(AS_FLAGS_F_CROSS)
cross : LD_FLAGS_F 	= $(LD_FLAGS_F_CROSS)

cross : make_output_dir $(ELF) $(LSS) $(DMP) $(HEX) $(BIN) print_size

test : CXX 		= $(CXX_TEST)
test : CC 			= $(CC_TEST)
test : AS 			= $(AS_TEST)
test : OBJCOPY 	= $(OBJCOPY_TEST)
test : OBJDUMP 	= $(OBJDUMP_TEST)
test : SIZE 		= $(SIZE_TEST)
test : CXX_FLAGS_F = $(CXX_FLAGS_F_TEST)
test : C_FLAGS_F 	= $(C_FLAGS_F_TEST)
test : AS_FLAGS_F 	= $(AS_FLAGS_F_TEST)
test : LD_FLAGS_F 	= $(LD_FLAGS_F_TEST)

.PHONY: test
test : make_test_output_dir $(TEST_TARGET)
	./$(TEST_TARGET)

# make object files dependent on Makefile
$(OBJS_F) : Makefile
# make .elf file dependent on linker script
$(ELF) : $(LD_SCRIPT)

#-----------------------------------------------------------------------------#
# test_linking - objects -> elf
#-----------------------------------------------------------------------------#
$(TEST_TARGET) : $(TEST_OBJS)	
	echo $(C_SRCS_TEST)
	@echo 'Linking test target: $(TEST_TARGET)'
	$(CXX) $(LD_FLAGS_F_TEST) $(TEST_OBJS) $(LIBS) -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# linking - objects -> elf
#-----------------------------------------------------------------------------#

$(ELF) : $(OBJS_F)
	@echo 'Linking target: $(ELF)'
	$(CXX) $(LD_FLAGS_F) $(OBJS_F) $(LIBS) -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# compiling - C++ source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(CXX_EXT)
	@echo 'Compiling file: $<'
	$(CXX) -c $(CXX_FLAGS_F) $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# compiling - C source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(C_EXT)
	@echo 'Compiling file: $<'
	$(CC) -c $(C_FLAGS_F) $< -o $@
	@echo ' '

$(OUT_DIR_TEST_F)%.o : %.$(C_EXT)
	@echo 'Compiling file: $<'
	$(CC) -c $(C_FLAGS_F_TEST) $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# assembling - ASM source -> objects
#-----------------------------------------------------------------------------#

$(OUT_DIR_F)%.o : %.$(AS_EXT)
	@echo 'Assembling file: $<'
	$(AS) -c $(AS_FLAGS_F) $< -o $@
	@echo ' '

#-----------------------------------------------------------------------------#
# memory images - elf -> hex, elf -> bin
#-----------------------------------------------------------------------------#

$(HEX) : $(ELF)
	@echo 'Creating IHEX image: $(HEX)'
	$(OBJCOPY) -O ihex $< $@
	@echo ' '

$(BIN) : $(ELF)
	@echo 'Creating binary image: $(BIN)'
	$(OBJCOPY) -O binary $< $@
	@echo ' '

#-----------------------------------------------------------------------------#
# memory dump - elf -> dmp
#-----------------------------------------------------------------------------#

$(DMP) : $(ELF)
	@echo 'Creating memory dump: $(DMP)'
	$(OBJDUMP) -x --syms $< > $@
	@echo ' '

#-----------------------------------------------------------------------------#
# extended listing - elf -> lss
#-----------------------------------------------------------------------------#

$(LSS) : $(ELF)
	@echo 'Creating extended listing: $(LSS)'
	$(OBJDUMP) -S $< > $@
	@echo ' '

#-----------------------------------------------------------------------------#
# print the size of the objects and the .elf file
#-----------------------------------------------------------------------------#

print_size :
	@echo 'Size of modules:'
	$(SIZE) -B -t --common $(OBJS_F) $(USER_OBJS)
	@echo ' '
	@echo 'Size of target .elf file:'
	$(SIZE) -B $(ELF)
	@echo ' '

#-----------------------------------------------------------------------------#
# create the desired output directory
#-----------------------------------------------------------------------------#

make_output_dir :
	$(shell mkdir $(OUT_DIR_F) 2>/dev/null)

make_test_output_dir :
	$(shell mkdir $(OUT_DIR_TEST_F) 2>/dev/null)

#-----------------------------------------------------------------------------#
# Write to flash of chip
#-----------------------------------------------------------------------------#

writeflash: all
	@echo "Writing to" $(COMPORT)
	@lpc21isp -NXPARM $(HEX) $(COMPORT) $(BAUDRATE) $(CLOCK_OSC)

#-----------------------------------------------------------------------------#
# Opening Picocom
#-----------------------------------------------------------------------------#

com:
	@echo "Opening" $(COMPORT)
	@picocom -b 9600 $(COMPORT)

#=============================================================================#
# make clean
#=============================================================================#

clean:
ifeq ($(strip $(OUT_DIR_F)), )
	@echo 'Removing all generated output files'
else
	@echo 'Removing all generated output files from output directory: $(OUT_DIR_F)'
endif
ifeq ($(strip $(OUT_DIR_TEST_F)), )
	@echo 'Removing all generated output files'
else
	@echo 'Removing all generated output files from output directory: $(OUT_DIR_TEST_F)'
endif
ifneq ($(strip $(GENERATED)), )
	$(RM) $(GENERATED)
else
	@echo 'Nothing to remove...'
endif

#=============================================================================#
# global exports
#=============================================================================#

.PHONY: all clean dependents writeflash

.SECONDARY:

# include dependancy files
-include $(DEPS)
