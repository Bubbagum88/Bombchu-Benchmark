#---------------------------------------------------------------------------------
# Bombchu Benchmark - Modern Makefile
#---------------------------------------------------------------------------------

# Ensure DEVKITPPC is set
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path>/devkitPPC")
endif

#---------------------------------------------------------------------------------
# Project settings
#---------------------------------------------------------------------------------

TARGET        := bombchu
BUILD         := build

# Source directories
SOURCES       := src benchmarks
INCLUDES      := include

#---------------------------------------------------------------------------------
# Compiler flags
#---------------------------------------------------------------------------------

CFLAGS        := -g -Wall -O2 -fomit-frame-pointer
CFLAGS        += -I$(INCLUDES)
CXXFLAGS      := $(CFLAGS)

LIBS          := -logc -lm

#---------------------------------------------------------------------------------
# Platform selection
#---------------------------------------------------------------------------------

TARGET_SYSTEM ?= wii

ifeq ($(TARGET_SYSTEM),gamecube)
include $(DEVKITPRO)/libogc2/gamecube_rules
else
include $(DEVKITPRO)/libogc2/wii_rules
endif

#---------------------------------------------------------------------------------
# Automatic source discovery
#---------------------------------------------------------------------------------

# Find all .c files in src/ and benchmarks/
CFILES        := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
OFILES        := $(CFILES:.c=.o)

#---------------------------------------------------------------------------------
# Build rules
#---------------------------------------------------------------------------------

all: $(TARGET).dol
$(TARGET).dol: $(TARGET).elf
<TAB>$(ELF2DOL) $< $@

$(TARGET).elf: $(OFILES)
<TAB>$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

src/%.o: src/%.c
<TAB>$(CC) $(CFLAGS) -c $< -o $@

benchmarks/%.o: benchmarks/%.c
<TAB>$(CC) $(CFLAGS) -c $< -o $@

clean:
<TAB>@echo "Cleaning..."
<TAB>@rm -f $(OFILES) $(TARGET).elf $(TARGET).dol

run: $(TARGET).dol
<TAB>wiiload $(TARGET).dol
