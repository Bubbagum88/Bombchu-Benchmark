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

ifeq ($(strip $(DEVKITPRO)),)
$(error DEVKITPRO is not set. Please install devkitPro correctly.)
endif

#---------------------------------------------------------------------------------
# Source discovery
#---------------------------------------------------------------------------------

CFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
OFILES := $(CFILES:.c=.o)

#---------------------------------------------------------------------------------
# libogc2 rules (IMPORTANT)
#---------------------------------------------------------------------------------

include $(DEVKITPRO)/libogc2/wii_rules

#---------------------------------------------------------------------------------
# Compiler flags
#---------------------------------------------------------------------------------

CFLAGS += -Wall -O2 -g
CFLAGS += -I$(INCLUDES)
CFLAGS += -I$(DEVKITPRO)/libogc2/include
CFLAGS += -DGEKKO -DHW_RVL -mrvl

#---------------------------------------------------------------------------------
# Targets
#---------------------------------------------------------------------------------

.PHONY: all clean run

all: $(TARGET).dol

$(TARGET).elf: $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) $(LIBS) -o $@

$(TARGET).dol: $(TARGET).elf
	$(ELF2DOL) $< $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

benchmarks/%.o: benchmarks/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES) $(TARGET).elf $(TARGET).dol

run: $(TARGET).dol
	wiiload $(TARGET).dol
