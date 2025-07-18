ARCH = -target armv6m-none-eabi -mthumb -mcpu=cortex-m0+ -flto
CC = clang
CXX = clang
FLAGS = $(ARCH) -O3 -Wall -MMD -MP -Iinc/ -ffreestanding -fno-exceptions -fno-unwind-tables -ffixed-point
CFLAGS = $(FLAGS) -std=c2y
CXXFLAGS = $(FLAGS) -std=c++2c

LD = clang -fuse-ld=lld
LDFLAGS = $(ARCH) -Tflash.ld -nostartfiles -nostdlib -static
LDFLAGS += -Wl,-X,-s,-S,--strip-all,--as-needed,--gc-sections,--icf=all,--lto-O3,--lto-whole-program-visibility,--exclude-libs=ALL

SRCS = $(wildcard src/*)
OBJS = $(addprefix obj/,$(notdir $(SRCS:.cpp=.o)))
OBJS := $(OBJS:.c=.o)
DEPS = $(OBJS:.o=.d)

TARGET = obj/main

all: $(TARGET).bin
	sudo python flash.py

build: $(TARGET).bin

asm: $(OBJS:.o=.s)

$(TARGET).bin: $(TARGET).elf
	llvm-objcopy -O binary $< $@

$(TARGET).elf: $(OBJS) flash.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.s: src/%.cpp
	$(CXX) $(CXXFLAGS) -S $< -o $@

obj/%.s: src/%.c
	$(CC) $(CFLAGS) -S $< -o $@

-include $(DEPS)

clean:
	rm -rf obj/*

.PHONY: all asm build clean

