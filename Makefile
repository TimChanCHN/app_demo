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
$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/src/stm32f10x_fsmc.c	\
$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/src/stm32f10x_sdio.c	\
$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/src/stm32f10x_dma.c	\
$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/src/stm32f10x_exti.c	\
$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/src/stm32f10x_flash.c	\

#APP
SRC_APP = \
handler/letter_shell \
handler/nt_shell \
gui/lvgl_app/lv_demos/src/lv_ex_get_started \
gui/lvgl_app/lv_ui							\
# gui/lvgl_app/lv_demos/src/lv_demo_widgets \
# handler/cmd_management \

SRC_NTSHELL = \
$(SDK_DIR)/components/ntshell/core		\
$(SDK_DIR)/components/ntshell/util		\
$(SDK_DIR)/components/ntshell/usrcmd	\


SRC_LVGL = \
$(SDK_DIR)/components/lvgl/porting	\
$(SDK_DIR)/components/lvgl/src/lv_core	\
$(SDK_DIR)/components/lvgl/src/lv_draw	\
$(SDK_DIR)/components/lvgl/src/lv_font	\
$(SDK_DIR)/components/lvgl/src/lv_gpu	\
$(SDK_DIR)/components/lvgl/src/lv_hal	\
$(SDK_DIR)/components/lvgl/src/lv_misc	\
$(SDK_DIR)/components/lvgl/src/lv_themes	\
$(SDK_DIR)/components/lvgl/src/lv_widgets	\

#第三方库
SRC_COMPONENTS = \
$(SDK_DIR)/components/trace		\
$(SDK_DIR)/components/app_scheduler	\
$(SDK_DIR)/components/app_timer		\
$(SDK_DIR)/components/letter-shell/src	\
$(SDK_DIR)/components/cmbacktrace	\
$(SRC_LVGL)


#对应的底层接口
SRC_CUSTOMIZED = \
$(SDK_DIR)/customized/hal/usart		\
$(SDK_DIR)/customized/hal/tftlcd		\
$(SDK_DIR)/customized/hk_lib/f1/sys		\
$(SDK_DIR)/customized/hk_lib/f1/usart	\
$(SDK_DIR)/customized/hk_lib/f1/gpio	\
$(SDK_DIR)/customized/hk_lib/f1/systick	\
$(SDK_DIR)/customized/hk_lib/f1/timer	\
$(SDK_DIR)/customized/hk_lib/f1/fsmc	\
$(SDK_DIR)/customized/hk_lib/f1/i2c		\
$(SDK_DIR)/customized/hk_lib/f1/sdio	\
$(SDK_DIR)/customized/hk_lib/f1/exit	\
$(SDK_DIR)/customized/hk_lib/f1/flash	\

#driver
SRC_DRIVERS = \
$(SDK_DIR)/drivers/tftlcd/st7789		\
$(SDK_DIR)/drivers/tftlcd/nt35510		\
$(SDK_DIR)/drivers/tftlcd/ili9341		\
$(SDK_DIR)/drivers/touch				\
$(SDK_DIR)/drivers/eeprom				\
$(SDK_DIR)/drivers/encoder				\

SRC_EXTERNAL = \
$(SDK_DIR)/external/fatfs/src		\
$(SDK_DIR)/external/fatfs/src/option\
# $(SDK_DIR)/external/fatfs/exfuns	\


SRCDIRS	:= \
$(SRC_APP) \
$(SRC_COMPONENTS) \
$(SRC_CUSTOMIZED) \
$(SRC_DRIVERS)	  \
$(SRC_EXTERNAL)

CFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
CFILENDIR	:= $(notdir  $(CFILES))

C_SOURCES =  			\
src/main.c 				\
src/hk_peripheral.c 	\
src/stm32f10x_it.c  	\
core/system_stm32f10x.c	\
$(SRC_STD_LIB)	\
$(CFILENDIR)	\
# $(SRC_LV_WIDGETS)	\

VPATH			:= $(SRCDIRS)

####################################################################
# C includes
# system include
INC_SYSTEM = \
-I$(SDK_DIR)/platform/hk/HK32F103/STD_LIB/inc \
-I$(SDK_DIR)/platform/hk/HK32F103/CMSIS/CM3/DeviceSupport \
-I$(SDK_DIR)/platform/hk/HK32F103/CMSIS/CM3/CoreSupport	\

INC_LVGL = \
-I$(SDK_DIR)/components/lvgl			\
-I$(SDK_DIR)/components/lvgl/src		\
-I$(SDK_DIR)/components/lvgl/src/lv_core	\
-I$(SDK_DIR)/components/lvgl/src/lv_draw	\
-I$(SDK_DIR)/components/lvgl/src/lv_font	\
-I$(SDK_DIR)/components/lvgl/src/lv_gpu	\
-I$(SDK_DIR)/components/lvgl/src/lv_misc	\
-I$(SDK_DIR)/components/lvgl/src/lv_hal	\
-I$(SDK_DIR)/components/lvgl/src/lv_themes	\
-I$(SDK_DIR)/components/lvgl/src/lv_widgets	\
-I$(SDK_DIR)/components/lvgl/porting	\

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
-I$(SDK_DIR)/components/letter-shell/src \
-I$(SDK_DIR)/components/ntshell/core \
-I$(SDK_DIR)/components/ntshell/usrcmd	\
-I$(SDK_DIR)/components/ntshell/util \
-I$(SDK_DIR)/components/trace	\
-I$(SDK_DIR)/components/app_timer	\
-I$(SDK_DIR)/components/queue	\
-I$(SDK_DIR)/components/app_scheduler	\
-I$(SDK_DIR)/components/cmbacktrace	\
-I$(SDK_DIR)/components/cmbacktrace/Languages	\
$(INC_LVGL) 

INC_CUSTOMIZE = \
-I$(SDK_DIR)/customized/hal/gpio	\
-I$(SDK_DIR)/customized/hal/systick	\
-I$(SDK_DIR)/customized/hal/usart	\
-I$(SDK_DIR)/customized/hal/timer	\
-I$(SDK_DIR)/customized/hal/tftlcd	\
-I$(SDK_DIR)/customized/hal/i2c	\
-I$(SDK_DIR)/customized/hal/touch	\
-I$(SDK_DIR)/customized/hal/sdio	\
-I$(SDK_DIR)/customized/hal/exit	\
-I$(SDK_DIR)/customized/hal/flash	\
-I$(SDK_DIR)/customized/hk_lib/f1/sys	\
-I$(SDK_DIR)/customized/hk_lib/f1/usart	\
-I$(SDK_DIR)/customized/hk_lib/f1/gpio	\
-I$(SDK_DIR)/customized/hk_lib/f1/systick	\
-I$(SDK_DIR)/customized/hk_lib/f1/timer	\
-I$(SDK_DIR)/customized/hk_lib/f1/fsmc	\
-I$(SDK_DIR)/customized/hk_lib/f1/i2c	\
-I$(SDK_DIR)/customized/hk_lib/f1/sdio	\
-I$(SDK_DIR)/customized/hk_lib/f1/exit	\
-I$(SDK_DIR)/customized/hk_lib/f1/flash	\

INC_DRIVER = \
-I$(SDK_DIR)/drivers/tftlcd/st7789 \
-I$(SDK_DIR)/drivers/tftlcd/nt35510 \
-I$(SDK_DIR)/drivers/tftlcd/ili9341		\
-I$(SDK_DIR)/drivers/touch		\
-I$(SDK_DIR)/drivers/eeprom		\
-I$(SDK_DIR)/drivers/encoder				\

INC_EXTERNAL = \
-I$(SDK_DIR)/external/fatfs/src	\
-I$(SDK_DIR)/external/fatfs/exfuns	\
# $(SDK_DIR)/external/fatfs			\

C_INCLUDES =  \
-Isrc \
-Ibsp \
-Icore \
-Ihandler \
$(INC_APP)	\
$(INC_SYSTEM)	\
$(INC_COMPONENTS)	\
$(INC_CUSTOMIZE)	\
$(INC_DRIVER) 		\
$(INC_EXTERNAL)		

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