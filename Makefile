#---------------------------------------------------------------------------------
# Bombchu Benchmark - Clean devkitPPC Build
#---------------------------------------------------------------------------------

TARGET := bombchu

SOURCES := src benchmarks
INCLUDES := include

#---------------------------------------------------------------------------------
# Toolchain sanity check
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPPC)),)
$(error DEVKITPPC is not set. Please install devkitPro correctly.)
endif

#---------------------------------------------------------------------------------
# Source discovery
#---------------------------------------------------------------------------------

CFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
OFILES := $(CFILES:.c=.o)

#---------------------------------------------------------------------------------
# devkitPro Wii rules (this defines CC, LD, ELF2DOL, etc.)
#---------------------------------------------------------------------------------

include $(DEVKITPPC)/wii_rules

#---------------------------------------------------------------------------------
# Compiler flags
#---------------------------------------------------------------------------------

CFLAGS += -Wall -O2 -g
CFLAGS += -I$(INCLUDES)
CFLAGS += -I$(DEVKITPRO)/libogc/include
CFLAGS += -DGEKKO -mrvl

#---------------------------------------------------------------------------------
# Targets
#---------------------------------------------------------------------------------

.PHONY: all clean run

all: $(TARGET).dol

# Link ELF
$(TARGET).elf: $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) $(LIBS) -o $@

# Convert ELF -> DOL
$(TARGET).dol: $(TARGET).elf
	$(ELF2DOL) $< $@

# Compile C files (src)
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C files (benchmarks)
benchmarks/%.o: benchmarks/%.c
	$(CC) $(CFLAGS) -c $< -o $@

#---------------------------------------------------------------------------------
# Cleanup
#---------------------------------------------------------------------------------

clean:
	rm -f $(OFILES) $(TARGET).elf $(TARGET).dol

#---------------------------------------------------------------------------------
# Run on Wii (optional)
#---------------------------------------------------------------------------------

run: $(TARGET).dol
	wiiload $(TARGET).dol
