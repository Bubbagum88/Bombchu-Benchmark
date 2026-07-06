#---------------------------------------------------------------------------------
# Bombchu-Benchmark Makefile
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

# Target name
TARGET      := bombchu

# Build directory
BUILD       := build

# Source directories
SOURCES     := src benchmarks
INCLUDES    := include

#---------------------------------------------------------------------------------
# Options
#---------------------------------------------------------------------------------
CFLAGS      := -g -Wall -O2 -fomit-frame-pointer
CFLAGS      += -Iinclude
CXXFLAGS    := $(CFLAGS)

# Libraries
LIBS        := -logc -lm

# Platform selection (default: both, or specify make TARGET_SYSTEM=gamecube)
TARGET_SYSTEM ?= auto

# Include rules
ifeq ($(TARGET_SYSTEM),gamecube)
    include $(DEVKITPRO)/libogc2/gamecube_rules
else ifeq ($(TARGET_SYSTEM),wii)
    include $(DEVKITPRO)/libogc2/wii_rules
else
    # Default: build both if possible, or set one
    include $(DEVKITPPC)/gamecube_rules
endif

#---------------------------------------------------------------------------------
# Automatic source discovery
#---------------------------------------------------------------------------------
# Find all .c files in src/ and benchmarks/
CFILES      := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
OFILES      := $(CFILES:.c=.o)
OUTPUT      := $(TARGET)

#---------------------------------------------------------------------------------
# Build rules
#---------------------------------------------------------------------------------
all: $(OUTPUT).dol

$(OUTPUT).dol: $(OUTPUT).elf
    $(ELF2DOL) $< $@

$(OUTPUT).elf: $(OFILES)
    $(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

%.o: $(SOURCES)/%.c
    $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
    @echo clean ...
    @rm -fr $(BUILD) $(OFILES) $(OUTPUT).elf $(OUTPUT).dol

run: $(OUTPUT).dol
    wiiload $(OUTPUT).dol
