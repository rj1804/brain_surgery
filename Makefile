ifeq (1,$(VERBOSE))
quiet =
else
quiet = quiet_
endif

-include local.mk

TARGET ?= bootloader

TCHAIN_PATH   ?=
TCHAIN_PREFIX = $(TCHAIN_PATH)/arm-none-eabi-
REMOVE_CMD = rm

CDEFS    += -DDEBUG
CDEFS    += -DDEBUG_BAUDRATE=115200

USE_THUMB_MODE = YES

ifeq ($(TARGET),bootloader)
MCU       = cortex-m7
CHIP      = STM32H750VBTx
BOARD     = H7_NUCLEO

CDEFS    += -mfpu=fpv5-d16 -mfloat-abi=hard -ffast-math
CDEFS    += -DSTM32 -DSTM32H7 -DSTM32H750xx 
CDEFS    += -DUSE_HAL_DRIVER
CDEFS    += -DARM_MATH_CM7 -D__FPU_PRESENT=1

HAL_DRV  = STM32H7xx
HAL_DRV2 = stm32h7xx

TGT_SRC += src/stm32h7xx_it.c
TGT_SRC += src/system_stm32h7xx.c
TGT_ASRC = gcc/startup_stm32h750xx.s
TGT_LD   = gcc/$(CHIP)_FLASH.ld
OOCD_TGT = stm32h7.cfg
endif

RUN_MODE=FLASH_RUN
#RUN_MODE=RAM_RUN

VECTOR_TABLE_LOCATION=VECT_TAB_FLASH
#VECTOR_TABLE_LOCATION=VECT_TAB_RAM

OUTDIR = build_$(TARGET)
DEPDIR = build_$(TARGET)/dep

# List C source files here
SRC  = $(TGT_SRC)

SRC += src/font.c
SRC += src/main.c
SRC += src/oled.c
SRC += src/paint.c
SRC += src/qspi_flash.c
SRC += src/spi.c
SRC += src/system.c
SRC += src/uart.c
SRC += src/usbd.c
SRC += src/usbd_conf.c
SRC += src/usbd_storage.c
SRC += src/usbd_desc_MSC.c
SRC += src/xprintf.c

HAL_PATH = lib/Drivers/$(HAL_DRV)_HAL_Driver/Src/$(HAL_DRV2)

SRC += $(HAL_PATH)_hal.c
SRC += $(HAL_PATH)_hal_adc.c
SRC += $(HAL_PATH)_hal_adc_ex.c
SRC += $(HAL_PATH)_hal_cortex.c
SRC += $(HAL_PATH)_hal_dma.c
SRC += $(HAL_PATH)_hal_dma2d.c
SRC += $(HAL_PATH)_hal_dsi.c
SRC += $(HAL_PATH)_hal_eth.c
SRC += $(HAL_PATH)_hal_flash.c
SRC += $(HAL_PATH)_hal_flash_ex.c
SRC += $(HAL_PATH)_hal_gpio.c
SRC += $(HAL_PATH)_hal_i2c.c
SRC += $(HAL_PATH)_hal_i2c_ex.c
SRC += $(HAL_PATH)_hal_i2s.c
SRC += $(HAL_PATH)_hal_ltdc.c
SRC += $(HAL_PATH)_hal_ltdc_ex.c
SRC += $(HAL_PATH)_hal_lptim.c
SRC += $(HAL_PATH)_hal_mdma.c
SRC += $(HAL_PATH)_hal_pwr.c
SRC += $(HAL_PATH)_hal_pwr_ex.c
SRC += $(HAL_PATH)_hal_qspi.c
SRC += $(HAL_PATH)_hal_rcc.c
SRC += $(HAL_PATH)_hal_rcc_ex.c
SRC += $(HAL_PATH)_hal_sdram.c
SRC += $(HAL_PATH)_hal_sai.c
SRC += $(HAL_PATH)_hal_sai_ex.c
SRC += $(HAL_PATH)_hal_sd.c
SRC += $(HAL_PATH)_ll_sdmmc.c
SRC += $(HAL_PATH)_ll_lptim.c
SRC += $(HAL_PATH)_hal_spi.c
SRC += $(HAL_PATH)_hal_tim.c
SRC += $(HAL_PATH)_hal_tim_ex.c
SRC += $(HAL_PATH)_hal_rng.c
SRC += $(HAL_PATH)_hal_uart.c
SRC += $(HAL_PATH)_ll_fmc.c
SRC += $(HAL_PATH)_hal_hcd.c
SRC += $(HAL_PATH)_hal_pcd.c
SRC += $(HAL_PATH)_hal_pcd_ex.c
SRC += $(HAL_PATH)_ll_usb.c

SRC += lib/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c
SRC += lib/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c
SRC += lib/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c
SRC += lib/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc.c
SRC += lib/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_bot.c
SRC += lib/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_scsi.c
SRC += lib/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Src/usbd_msc_data.c

# List Assembler source files here.
ASRC = $(TGT_ASRC)

# List any extra directories to look for include files here.
EXTRAINCDIRS += src

EXTRAINCDIRS += lib/Drivers/CMSIS/Device/ST/$(HAL_DRV)/Include
EXTRAINCDIRS += lib/Drivers/CMSIS/Include
EXTRAINCDIRS += lib/Drivers/$(HAL_DRV)_HAL_Driver/Inc

EXTRAINCDIRS += lib/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
EXTRAINCDIRS += lib/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc

# List non-source files which should trigger build here
BUILDONCHANGE = Makefile $(TGT_LD)

OPT = 3

# Debugging format.
DEBUG = gdb

LOAD_FILE  = $(OUTDIR)/$(TARGET).elf

ifdef HLA_SERIAL
HLA_PREFIX = HLA_SERIAL=$(HLA_SERIAL)
else
HLA_PREFIX =
endif

OOCD_EXE = $(HLA_PREFIX) openocd

OOCD_PROG_CL  = -d0 -f $(OOCD_TGT) -c init -c "reset halt" -c targets -c "poll off"
OOCD_PROG_CL += -c "flash write_image erase $(LOAD_FILE)" -c "verify_image $(LOAD_FILE)"
OOCD_PROG_CL += -c "reset run" -c shutdown

OOCD_RESET_CL = -d0 -f $(OOCD_TGT) -c init -c "reset run" -c shutdown
OOCD_CHECK_CL = -d0 -f $(OOCD_TGT) -c init -c targets -c "reset run" -c shutdown 
OOCD_ERASE_CL = -d0 -f $(OOCD_TGT) -c init -c "reset halt" -c "stm32h7x mass_erase 0" -c shutdown

ifdef VECTOR_TABLE_LOCATION
CDEFS += -D$(VECTOR_TABLE_LOCATION)
ADEFS += -D$(VECTOR_TABLE_LOCATION)
endif

CDEFS += -D$(RUN_MODE) -D$(BOARD)
ADEFS += -D$(RUN_MODE) -D$(BOARD)

# Compiler flags.

ifeq ($(USE_THUMB_MODE),YES)
THUMB    = -mthumb
THUMB_IW = -mthumb-interwork
else 
THUMB    = 
THUMB_IW = 
endif

# Flags for C and C++ (arm-elf-gcc/arm-elf-g++)
CFLAGS +=  -g$(DEBUG)
CFLAGS += -O$(OPT)
CFLAGS += -mcpu=$(MCU) $(THUMB) $(THUMB_IW)
CFLAGS += $(CDEFS)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS)) -I.
# when using ".ramfunc"s without attribute longcall:
#CFLAGS += -mlong-calls
# -mapcs-frame is important if gcc's interrupt attributes are used
# (at least from my eabi tests), not needed if assembler-wrappers are used 

CFLAGS += -mapcs-frame 
CFLAGS += -ffunction-sections -fdata-sections -fstack-usage
CFLAGS += -fstrict-volatile-bitfields -fno-exceptions
CFLAGS += -Wall -Wdouble-promotion
CFLAGS += -Wpointer-arith
CFLAGS += -Wno-cast-qual
CFLAGS += -Wno-attributes
CFLAGS += -Wno-unused-function
CFLAGS += -Wno-strict-aliasing
CFLAGS += -Wno-stringop-truncation -Wno-misleading-indentation
CFLAGS += -Wa,-adhlns=$(addprefix $(OUTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))

# Compiler flags to generate dependency files:
CFLAGS += -MMD -MP -MF $(OUTDIR)/dep/$(@F).d

# flags only for C
CONLYFLAGS += $(CSTANDARD)

# Assembler flags.
ASFLAGS  = -mcpu=$(MCU) $(THUMB) $(THUMB_IW) -I. -x assembler-with-cpp
ASFLAGS += -D__ASSEMBLY__ $(ADEFS)
ASFLAGS += -Wa,-adhlns=$(addprefix $(OUTDIR)/, $(notdir $(addsuffix .lst, $(basename $<))))
ASFLAGS += -Wa,-g$(DEBUG)
ASFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))

# Linker flags.
LDFLAGS = -Wl,-Map=$(OUTDIR)/$(TARGET).map,--cref,--gc-sections
LDFLAGS += --specs=nano.specs -lc -lm -lnosys
LDFLAGS += $(patsubst %,-L%,$(EXTRA_LIBDIRS))
LDFLAGS += $(patsubst %,-L%,$(LINKERSCRIPTINC))
LDFLAGS += $(patsubst %,-l%,$(EXTRA_LIBS)) 
LDFLAGS += $(EXTRA_LDFLAGS)

# Set linker-script name depending on selected run-mode and chip
ifeq ($(RUN_MODE),RAM_RUN)
LDFLAGS +=-T./$(CHIP)_ram.ld
else 
LDFLAGS +=-T./$(TGT_LD)
endif

# Autodetect environment
SHELL   = sh
REMOVE_CMD:=rm

# Define programs and commands.
CC      = $(TCHAIN_PREFIX)g++
AR      = $(TCHAIN_PREFIX)ar
OBJCOPY = $(TCHAIN_PREFIX)objcopy
OBJDUMP = $(TCHAIN_PREFIX)objdump
SIZE    = $(TCHAIN_PREFIX)size
NM      = $(TCHAIN_PREFIX)nm
GDB     = $(TCHAIN_PREFIX)gdb
REMOVE  = $(REMOVE_CMD) -f

      CMD_CC_O_C =  @$(CC) -c $(THUMB) $$(CFLAGS) $$(CONLYFLAGS) $$< -o $$@
quiet_CMD_CC_O_C = "[CC] $$<"

      CMD_AS_T_O_S =  @$(CC) -c $(THUMB) $$(ASFLAGS) $$< -o $$@
quiet_CMD_AS_T_O_S = "[AS] $$<"

      CMD_LD     =  @$(CC) $(THUMB) $(CFLAGS) $(ALLOBJ) --output $@  $(LDFLAGS)
quiet_CMD_LD     = "[LD] $@"

# Define Messages
MSG_COMPILING    = [CC]
MSG_ASSEMBLING   = [AS]
MSG_LINKING      = [LD]
MSG_SYMBOL_TABLE = [SY]
MSG_LISTING      = [LS]
MSG_HEX_FILE     = [HX]
MSG_BIN_FILE     = [BN]
MSG_SIZE_AFTER   = [Size after build]
MSG_CLEANING     = [Cleaning project]

# List of all source files.
ALLSRC     = $(ASRC) $(SRC) $(CPPSRC)

# List of all source files without directory and file-extension.
ALLSRCBASE = $(notdir $(basename $(ALLSRC)))

# Define all object files.
ALLOBJ     = $(addprefix $(OUTDIR)/, $(addsuffix .o, $(ALLSRCBASE)))

# Define all listing files (used for make clean).
LSTFILES   = $(addprefix $(OUTDIR)/, $(addsuffix .lst, $(ALLSRCBASE)))
# Define all depedency-files (used for make clean).
DEPFILES   = $(addprefix $(OUTDIR)/dep/, $(addsuffix .o.d, $(ALLSRCBASE)))

# Default target.
all: build

elf: $(OUTDIR)/$(TARGET).elf
lst: $(OUTDIR)/$(TARGET).lst 
sym: $(OUTDIR)/$(TARGET).sym
hex: $(OUTDIR)/$(TARGET).hex
bin: $(OUTDIR)/$(TARGET).bin

# Target for the build-sequence.
build: folders elf hex bin lst sym sizeafter

# Display sizes of sections.
ELFSIZE = $(SIZE) -B  $(OUTDIR)/$(TARGET).elf 

sizeafter: elf
	@echo $(MSG_SIZE_AFTER)
	@$(ELFSIZE)

p: program

c: check

r: reset

program: build
	@echo "Programming with OPENOCD"
	-$(OOCD_EXE) $(OOCD_PROG_CL)

reset: 
	@echo "RESET!"
	-$(OOCD_EXE) $(OOCD_RESET_CL)

check: 
	@echo "CHECK!"
	-$(OOCD_EXE) $(OOCD_CHECK_CL)

erase: 
	@echo "ERASE!"
	@echo "WARNING! uncomment the next line in the Makefile to really do it!"
	#-$(OOCD_EXE) $(OOCD_ERASE_CL)

# Create final output file in ihex format from ELF output file (.hex).
%.hex: %.elf
	@echo $(MSG_HEX_FILE) $@
	@$(OBJCOPY) -O ihex $< $@
	
# Create final output file in raw binary format from ELF output file (.bin)
%.bin: %.elf
	@echo $(MSG_BIN_FILE) $@
	@$(OBJCOPY) -O binary $< $@

# Create extended listing file/disassambly from ELF output file.
# using objdump (testing: option -C)
%.lst: %.elf
	@echo $(MSG_LISTING) $@
	@$(OBJDUMP) -h -S -C -r $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo $(MSG_SYMBOL_TABLE) $@
	@$(NM) -n $< > $@

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(ALLOBJ)
%.elf:  $(ALLOBJ) $(BUILDONCHANGE)
	@echo $($(quiet)CMD_LD)
	@$(CMD_LD)

$(OUTDIR):
	mkdir  $(OUTDIR)
$(DEPDIR):
	mkdir  $(DEPDIR)

folders: $(OUTDIR) $(DEPDIR)
	
# Assemble: create object files from assembler source files.
define ASSEMBLE_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1) $(BUILDONCHANGE) 
	@echo $($(quiet)CMD_AS_T_O_S)
	@$(CMD_AS_T_O_S)
endef
$(foreach src, $(ASRC), $(eval $(call ASSEMBLE_TEMPLATE, $(src)))) 

# Compile: create object files from C source files.
define COMPILE_C_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1) $(BUILDONCHANGE) 
	@echo $($(quiet)CMD_CC_O_C)
	@$(CMD_CC_O_C)
endef
$(foreach src, $(SRC), $(eval $(call COMPILE_C_TEMPLATE, $(src)))) 

# Compile: create object files from C++ source files.
define COMPILE_CPP_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1) $(BUILDONCHANGE) 
	@echo $($(quiet)CMD_CC_O_C)
	@$(CMD_CC_O_C)
endef
$(foreach src, $(CPPSRC), $(eval $(call COMPILE_CPP_TEMPLATE, $(src)))) 

# Target: clean project.
clean:
	@echo $(MSG_CLEANING)
	$(REMOVE) $(OUTDIR)/$(TARGET).map
	$(REMOVE) $(OUTDIR)/$(TARGET).elf
	$(REMOVE) $(OUTDIR)/$(TARGET).hex
	$(REMOVE) $(OUTDIR)/$(TARGET).bin
	$(REMOVE) $(OUTDIR)/$(TARGET).sym
	$(REMOVE) $(OUTDIR)/$(TARGET).lst
	$(REMOVE) $(ALLOBJ)
	$(REMOVE) $(LSTFILES)
	$(REMOVE) $(DEPFILES)

allclean: clean
	$(REMOVE) -r ./build_*

# Include the dependency files.
-include $(wildcard $(OUTDIR)/dep/*.d)

# Listing of phony targets.
.PHONY : folders all sizeafter build elf hex bin lst sym clean program

