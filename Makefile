TARGET = app

DEBUG = 0
# optimization
OPT = -O3 -g

# Build path
BUILD_DIR = build

# sdk
SDK_DIR   = ../sdk
SDK_BRANCH = master
ifeq ($(SDK_DIR), $(wildcard $(SDK_DIR)))
else
$(shell cd ../ && git clone -b $(SDK_BRANCH) git@github.com:leadercxn/sdk.git)
endif

# 版本脚本
$(shell python $(SDK_DIR)/scripts/preBuild.py)
VERSION = $(shell python $(SDK_DIR)/scripts/getVersion.py)_$(shell git rev-parse --short HEAD)

# 时间戳
TIMESTAMP = $(shell date +%Y%m%d%H%M)


# 系统平台
ifeq ($(OS),Windows_NT)
	PLATFORM="Windows"
else
	ifeq ($(shell uname),Darwin)
  		PLATFORM="MacOS"
 	else
  		PLATFORM="Unix-Like"
 	endif
endif

$(info PLATFORM: $(PLATFORM))

# 文件添加在sdk.mk中实现
include ../sdk/sdk.mk

######################################
# source
######################################
# C sources

#APP
SRC_APP = \
handler/letter_shell \
gui/lvgl_app/lv_ui							\
# gui/lvgl_app/lv_demos/src/lv_ex_get_started \
# gui/lvgl_app/lv_demos/src/lv_demo_widgets \
# handler/cmd_management \
# handler/nt_shell \

SRC_NTSHELL = \
$(SDK_DIR)/components/ntshell/core		\
$(SDK_DIR)/components/ntshell/util		\
$(SDK_DIR)/components/ntshell/usrcmd	\

SRCDIRS	:= \
$(SRC_APP) \

CFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CFILENDIR	:= $(notdir  $(CFILES))

C_SOURCES =  			\
src/main.c 				\
src/hk_peripheral.c 	\
src/stm32f10x_it.c  	\
core/system_stm32f10x.c	\
$(CFILENDIR)	\
$(CSRCS)


VPATH			+= $(SRCDIRS)

####################################################################
# C includes


# APP include
INC_APP = \
-Ihandler/cmd_management	\
-Ihandler/letter_shell	\
-Ihandler/nt_shell	\
-Igui/lvgl_app/		\
-Igui/lvgl_app/lv_demos		\
-Igui/lvgl_app/lv_demos/src/lv_ex_get_started		\
-Igui/lvgl_app/lv_ui								\


# 第三方库头文件
INC_COMPONENTS = \
-I$(SDK_DIR)/components/util	\
-I$(SDK_DIR)/components/lib_err	\
-I$(SDK_DIR)/components/queue	\

C_INCLUDES =  \
-Isrc \
-Ibsp \
-Icore \
-Ihandler \
$(INC_APP)	\
# $(INC_COMPONENTS)	\

# ASM sources
ASM_SOURCES =  \
linker/startup_stm32f103_gcc.s	\
$(SDK_DIR)/components/cmbacktrace/cmb_fault.s	\

##############################################################
# macros for gcc
# AS defines
AS_DEFS = 

# C 宏
C_DEFS =  \
-DUSE_STDPERIPH_DRIVER \
-DHK32F103 \
-DVERSION="${VERSION}"	\
-DBOARD_V002	\
-DSTM32F10X_HD	\
-DTRACE_ENABLE	\
-DTRACE_LEVEL=5


#######################################
# binaries
#######################################
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.

PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# CC = armcc
# AS = armasm
# LD = armlink
# AR = armar


# LOG Message
$(info VERSION: $(VERSION))
$(info sdk path: $(SDK_DIR))
$(info TIMESTAMP: $(TIMESTAMP))


#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)



# AS includes
AS_INCLUDES = 


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = linker/STM32F103VETx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS)  $(IFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS)  $(IFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

echo:
	@echo $(IFLAGS)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)


include cmd.mk

# *** EOF ***