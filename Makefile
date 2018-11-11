EXE ?= test.elf

DEFS += -DCLOCK_SETUP=1

INCLUDE_DIRS := -Isystem/include -Iinclude
SRCS := $(wildcard src/*.c)

INCLUDE_DIRS += -Imscan/cfg -Imscan/inc
SRCS += $(wildcard mscan/src/*.c)

ARCH_FLAGS := -mthumb -mcpu=cortex-m0 -mfloat-abi=soft
LD_LIBS := -Tsystem/src/SKEAZ_flash.ld # linker script
OBJS := $(patsubst %.c,%.o,$(SRCS))
OBJS += system/src/system_SKEAZ1284.o system/src/startup_SKEAZ1284.S.o

PREFIX ?= arm-none-eabi

CC := $(PREFIX)-gcc
CXX := $(PREFIX)-g++
LD := $(PREFIX)-gcc
AR := $(PREFIX)-ar
AS := $(PREFIX)-as
OBJCOPY := $(PREFIX)-objcopy
OBJDUMP := $(PREFIX)-objdump

CFLAGS ?= -g3 -O0
CFLAGS := -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -fno-common $(ARCH_FLAGS) $(CFLAGS)
CPPFLAGS := -MD $(INCLUDE_DIRS) $(DEFS) $(CPPFLAGS)
LDFLAGS := --static --specs=nosys.specs $(LD_LIBS) $(ARCH_FLAGS) $(LDFLAGS)


# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
NULL := 2>/dev/null
endif

%.S.o: %.S
	@printf "  ASM     $(*).S\n"
	$(Q)$(CC) -x assembler-with-cpp -o $(*).S.o -c $(*).S

%.o: %.c
	@printf "  CC      $(*).c\n"
	$(Q)$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $(*).o -c $(*).c

$(EXE): $(OBJS)
	@printf "  LD      $(@)\n"
	$(Q)$(LD) $(LDFLAGS) $(^) -o $(@)

all: $(EXE)

clean:
	@printf "  CLEAN\n"
	$(Q)find -name \*.o -delete
	$(Q)find -name \*.d -delete
	$(Q)rm -f $(EXE)
#$(Q)find -name \*.elf -delete

.PHONY: all clean
.SECONDARY:

-include $(OBJS:.o=.d)
