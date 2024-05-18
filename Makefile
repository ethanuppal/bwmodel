# Copyright (C) 2024 Ethan Uppal. All rights reserved.

SRCDIR		:= ./src
INCLUDEDIR	:= ./src
TESTSDIR	:= ./tests

CC			:= $(shell which g++ || which clang)
LDFLAGS		:= -Lefsw -lefsw
CFLAGS		:= -std=c++17 -pedantic -Wall -Wextra -I $(INCLUDEDIR)
CDEBUG		:= -g
CRELEASE	:= -O2 -DRELEASE_BUILD
TARGET		:= main

SRC			:= $(shell find $(SRCDIR) -name "*.cpp" -type f)
OBJ			:= $(SRC:.cpp=.o)

.PHONY: build 
build: library 
	make $(TARGET)

$(TARGET): main.cpp $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	@echo 'Compiling $@'
	$(CC) $(CFLAGS) -MMD -MP $< -c -o $@

.PHONY: library
library:
	cmake -DBUILD_SHARED_LIBS=OFF efsw
	make -C efsw

.PHONY: clean
clean:
	rm -rf $(OBJ) $(TARGET) $(shell find . -name "*.dSYM")
	make -C efsw clean

.PHONY: run
run: build
	./$(TARGET)

.PHONY: test
test: CFLAGS += -DNO_LOGGING
test: $(OBJ)
	@for f in $(TESTSDIR)/*.cpp; do \
		echo "  testing $$f"; \
		$(CC) $(CFLAGS) "$$f" $^ -o $(TESTSDIR)/tmpexec; \
		$(TESTSDIR)/tmpexec && printf "\033[32m+ test $$f passed\033[m\n" || printf "\033[31m- test $$f failed\033[m\n"; \
	done; \
	find $(TESTSDIR) -type f -not -name "*.cpp" -delete; \
	rm -rf $(TESTSDIR)/tmpexec.dSYM
