#---------------------------------------------------------------------------------
# Bombchu Benchmark - Flat header layout
#---------------------------------------------------------------------------------

TARGET := bombchu

SOURCES := src benchmarks ui

#---------------------------------------------------------------------------------
# Toolchain sanity
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPRO)),)
$(error DEVKITPRO is not set)
endif

ifeq ($(strip $(DEVKITPPC)),)
$(error DEVKITPPC is not set)
endif

#---------------------------------------------------------------------------------
# Source discovery
#---------------------------------------------------------------------------------

CFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
OFILES := $(CFILES:.c=.o)

#---------------------------------------------------------------------------------
# libogc2 rules
#---------------------------------------------------------------------------------

include $(DEVKITPRO)/libogc2/wii_rules

#---------------------------------------------------------------------------------
# Flags
#---------------------------------------------------------------------------------

CFLAGS += -Wall -O2 -g
CFLAGS += -Isrc -Ibenchmarks -Iui
CFLAGS += -I$(DEVKITPRO)/libogc2/include
CFLAGS += -DGEKKO -DHW_RVL -mrvl

#---------------------------------------------------------------------------------
# Targets
#---------------------------------------------------------------------------------

.PHONY: all clean

all: $(TARGET).dol

$(TARGET).elf: $(OFILES)
	$(CC) $(OFILES) $(LDFLAGS) $(LIBS) -o $@

$(TARGET).dol: $(TARGET).elf
	$(ELF2DOL) $< $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

benchmarks/%.o: benchmarks/%.c
	$(CC) $(CFLAGS) -c $< -o $@

ui/%.o: ui/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES) $(TARGET).elf $(TARGET).dol
