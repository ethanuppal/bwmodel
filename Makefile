# Copyright (C) 2024 Ethan Uppal. All rights reserved.

SRCDIR		:= ./src
INCLUDEDIR	:= ./src

CC			:= $(shell which g++-13 || \
                       which g++ || which clang)
CFLAGS		:= -std=c++17 -pedantic -Wall -Wextra -I $(INCLUDEDIR)
CDEBUG		:= -g
CRELEASE	:= -O2 -DRELEASE_BUILD
TARGET		:= main

# CFLAGS 		+= $(CRELEASE)
CFLAGS 		+= $(CDEBUG)

SRC			:= $(shell find $(SRCDIR) -name "*.cpp" -type f)
OBJ			:= $(SRC:.cpp=.o)

$(TARGET): main.cpp $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	@echo 'Compiling $@'
	$(CC) $(CFLAGS) -MMD -MP $< -c -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ) $(TARGET) $(shell find . -name "*.dSYM")

.PHONY: run
run: $(TARGET)
	./$(TARGET)
