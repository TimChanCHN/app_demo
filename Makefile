TARGET = app

DEBUG = 1
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


######################################
# source
######################################
# C sources
#标准库函数
SRC_STD_LIB = 	\
$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/src/stm32f10x_gpio.c	\
$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/src/stm32f10x_tim.c	\
$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/src/misc.c	\
$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/src/stm32f10x_rcc.c	\
$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/src/stm32f10x_usart.c	\

#APP
SRC_APP = \
handler/cmd_management \
handler/letter_shell \
handler/nt_shell \

#第三方库
SRC_COMPONENTS = \
$(SDK_DIR)/components/trace		\
$(SDK_DIR)/components/app_scheduler	\
$(SDK_DIR)/components/app_timer		\
$(SDK_DIR)/components/letter-shell/src	\
$(SDK_DIR)/components/ntshell/core		\
$(SDK_DIR)/components/ntshell/util		\
$(SDK_DIR)/components/ntshell/usrcmd	\

#对应的底层接口
SRC_CUSTOMIZED = \
$(SDK_DIR)/customized/hal/usart		\
$(SDK_DIR)/customized/hk_lib/f1/usart	\
$(SDK_DIR)/customized/hk_lib/f1/gpio	\
$(SDK_DIR)/customized/hk_lib/f1/systick	\
$(SDK_DIR)/customized/hk_lib/f1/timer	\	

SRCDIRS	:= \
$(SRC_APP) \
$(SRC_COMPONENTS) \
$(SRC_CUSTOMIZED)

CFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CFILENDIR	:= $(notdir  $(CFILES))

C_SOURCES =  			\
src/main.c 				\
src/hk_peripheral.c 	\
src/stm32f10x_it.c  	\
core/system_stm32f10x.c	\
$(SRC_STD_LIB)	\
$(CFILENDIR)	\

VPATH			:= $(SRCDIRS)

####################################################################
# C includes
# system include
INC_SYSTEM = \
-I$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/inc \
-I$(SDK_DIR)/platform/hk/HK32F103/CMSIS/CM3/DeviceSupport \
-I$(SDK_DIR)/platform/hk/HK32F103/CMSIS/CM3/CoreSupport	\

# APP include
INC_APP = \
-Ihandler/cmd_management	\
-Ihandler/letter_shell	\
-Ihandler/nt_shell	\

# 第三方库头文件
INC_COMPONENTS = \
-I$(SDK_DIR)/components/util	\
-I$(SDK_DIR)/components/lib_err	\
-I$(SDK_DIR)/components/letter-shell/src \
-I$(SDK_DIR)/components/ntshell/core \
-I$(SDK_DIR)/components/ntshell/usrcmd	\
-I$(SDK_DIR)/components/ntshell/util \
-I$(SDK_DIR)/components/trace	\
-I$(SDK_DIR)/components/app_timer	\
-I$(SDK_DIR)/components/queue	\
-I$(SDK_DIR)/components/app_scheduler	\

INC_CUSTOMIZE = \
-I$(SDK_DIR)/customized/hal/gpio	\
-I$(SDK_DIR)/customized/hal/systick	\
-I$(SDK_DIR)/customized/hal/usart	\
-I$(SDK_DIR)/customized/hal/timer	\
-I$(SDK_DIR)/customized/hk_lib/f1/usart	\
-I$(SDK_DIR)/customized/hk_lib/f1/gpio	\
-I$(SDK_DIR)/customized/hk_lib/f1/systick	\
-I$(SDK_DIR)/customized/hk_lib/f1/timer	\

C_INCLUDES =  \
-Isrc \
-Ibsp \
-Icore \
-Ihandler \
$(INC_APP)	\
$(INC_SYSTEM)	\
$(INC_COMPONENTS)	\
$(INC_CUSTOMIZE)	\

# ASM sources
ASM_SOURCES =  \
linker/startup_stm32f103_gcc.s

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
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

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
	@echo $(CFILENDIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)


include cmd.mk

# *** EOF ***