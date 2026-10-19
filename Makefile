#---------------------------------------------------------------------------------
# Bombchu Benchmark
#
#   make                 Wii build      -> bombchu-wii.dol
#   make PLATFORM=gc     GameCube build -> bombchu-gc.dol
#   make OGC=$(DEVKITPRO)/libogc        build against stock libogc instead of libogc2
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error DEVKITPRO is not set)
endif

ifeq ($(strip $(DEVKITPPC)),)
$(error DEVKITPPC is not set)
endif

include $(DEVKITPPC)/base_tools

#---------------------------------------------------------------------------------
# Platform
#---------------------------------------------------------------------------------

PLATFORM ?= wii

ifeq ($(PLATFORM),wii)
MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
OGC_LIBDIR := wii
else ifeq ($(PLATFORM),gc)
MACHDEP := -DGEKKO -mogc -mcpu=750 -meabi -mhard-float
OGC_LIBDIR := cube
else
$(error PLATFORM must be wii or gc)
endif

OGC ?= $(DEVKITPRO)/libogc2

TARGET  := bombchu-$(PLATFORM)
BUILD   := build/$(PLATFORM)
SOURCES := src benchmarks ui ui/themes
INCLUDES := include ui ui/themes

#---------------------------------------------------------------------------------
# Flags
#---------------------------------------------------------------------------------

CFLAGS  := -Wall -O2 -g $(MACHDEP)
CFLAGS  += $(foreach dir,$(INCLUDES),-I$(dir)) -I$(OGC)/include
LDFLAGS := -g $(MACHDEP) -Wl,-Map,$(BUILD)/$(TARGET).map
LIBS    := -L$(OGC)/lib/$(OGC_LIBDIR) -lfat -logc -lm

#---------------------------------------------------------------------------------
# Sources
#---------------------------------------------------------------------------------

CFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
OFILES := $(addprefix $(BUILD)/,$(CFILES:.c=.o))
DEPS   := $(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# Targets
#---------------------------------------------------------------------------------

.PHONY: all clean

all: $(TARGET).dol

$(TARGET).dol: $(BUILD)/$(TARGET).elf
	elf2dol $< $@

$(BUILD)/$(TARGET).elf: $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) $(LIBS) -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf build bombchu-wii.dol bombchu-gc.dol

-include $(DEPS)
