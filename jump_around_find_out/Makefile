CC ?= gcc
CXX ?= g++
CFLAGS ?= -Wall -Wextra -std=c11 -I./src -g
CXXFLAGS ?= -Wall -Wextra -std=c++17 -I./src -g
TARGET = jump_around_find_out

C_SRC = src/main.c
CPP_SRC = src/sha256_wrapper.cpp
OBJS = $(C_SRC:.c=.o) $(CPP_SRC:.cpp=.o)

all: generate $(TARGET)

generate:
	python3 scripts/generate_challenge.py

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) src/*.o

.PHONY: all generate clean
