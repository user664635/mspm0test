ARCH = -target armv6m-none-eabi -mthumb -mcpu=cortex-m0+
CC = clang
CXX = clang++
FLAGS = $(ARCH) -O3 -Wall -MMD -MP -Iinc/ -ffreestanding 
CFLAGS = $(FLAGS) -std=c2y
CXXFLAGS = $(FLAGS) -std=c++2c

LD = clang -fuse-ld=lld
LDFLAGS = $(ARCH) -Tflash.ld -nostartfiles -nostdlib -static
LDFLAGS += -Wl,-X,-s,-S,--as-needed,--gc-sections,--icf=all

SRCS = $(wildcard src/*)
OBJS = $(addprefix obj/,$(notdir $(SRCS:.cpp=.o)))
OBJS := $(OBJS:.c=.o)
DEPS = $(OBJS:.o=.d)

TARGET = obj/main

all: $(TARGET).bin
	sudo python flash.py

$(TARGET).bin: $(TARGET).elf
	llvm-objcopy -O binary $< $@

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf obj/*

.PHONY: all clean

