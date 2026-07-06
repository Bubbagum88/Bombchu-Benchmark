TARGET := bombchu

SOURCES := src benchmarks
INCLUDES := include

CFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
OFILES := $(CFILES:.c=.o)

ifeq ($(strip $(DEVKITPPC)),)
$(error DEVKITPPC not set)
endif

include $(DEVKITPPC)/wii_rules

# Force correct include path (IMPORTANT FIX)
CFLAGS += -I$(DEVKITPRO)/libogc/include -I$(INCLUDES)

.PHONY: all clean

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
