#---------------------------------------------------------------------------------
# Bombchu Benchmark - Clean devkitPPC Makefile
#---------------------------------------------------------------------------------

TARGET := bombchu
BUILD  := build

SOURCES := src benchmarks
INCLUDES := include

TARGET_SYSTEM ?= wii

CFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
OFILES := $(CFILES:.c=.o)

CFLAGS := -g -Wall -O2 -fomit-frame-pointer
CFLAGS += -I$(INCLUDES)

LDFLAGS :=
LIBS := -logc -lm

#---------------------------------------------------------------------------------
# devkitPro setup
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITPPC)),)
$(error DEVKITPPC is not set)
endif

include $(DEVKITPPC)/wii_rules

#---------------------------------------------------------------------------------
# Build rules
#---------------------------------------------------------------------------------

.PHONY: all clean run

all: $(TARGET).dol

$(TARGET).elf: $(OFILES)
	$(CC) $(CFLAGS) $(OFILES) $(LDFLAGS) $(LIBS) -o $@

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
