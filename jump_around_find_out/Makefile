CC ?= gcc
CXX ?= g++
CFLAGS ?= -Wall -Wextra -std=c11 -I./src
CXXFLAGS ?= -Wall -Wextra -std=c++17 -I./src
TARGET = jump-around-find-out

C_SRC = src/main.c
CPP_SRC = src/sha256_wrapper.cpp
OBJS = $(C_SRC:.c=.o) $(CPP_SRC:.cpp=.o)

all: $(TARGET)

build: $(TARGET)

generate:
	python3 scripts/generate_challenge.py

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)
	mkdir -p challenge/public
	cp $(TARGET) challenge/public/$(TARGET)
	@if [ -e /etc/NIXOS ] || ( [ -f /etc/os-release ] && grep -qi 'nixos' /etc/os-release ); then \
		patchelf --set-interpreter /lib64/ld-linux-x86-64.so.2 challenge/public/$(TARGET) && \
		patchelf --remove-rpath challenge/public/$(TARGET) ; \
	fi

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

compile_commands.json:
	bear -- $(MAKE) clean all

clean:
	rm -f $(TARGET) challenge/public/$(TARGET) src/*.o compile_commands.json

.PHONY: all build generate clean compile_commands.json
