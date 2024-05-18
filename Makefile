# Copyright (C) 2024 Ethan Uppal. All rights reserved.

SRCDIR		:= ./src
INCLUDEDIR	:= ./src
TESTSDIR	:= ./tests

C_CC		:= $(shell which gcc || which clang)
CPP_CC		:= $(shell which g++ || which clang)
LDFLAGS		:= -Lbuild -lefsw
ifeq ($(shell uname -s),Darwin)
	LDFLAGS += -framework CoreFoundation -framework CoreServices
endif
CFLAGS		:= -std=c99 -pedantic -Wall -Wextra -I $(INCLUDEDIR)
CPPFLAGS	:= -std=c++17 -pedantic -Wall -Wextra -I $(INCLUDEDIR)
CDEBUG		:= -g
CRELEASE	:= -O2 -DRELEASE_BUILD
TARGET		:= main

SRC			:= $(shell find $(SRCDIR) -name "*.cpp" -type f)
OBJ			:= $(SRC:.cpp=.o)

.PHONY: build 
build: library 
	make $(TARGET)

$(TARGET): main.cpp $(OBJ)
	$(CPP_CC) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	@echo 'Compiling $@'
	$(CPP_CC) $(CPPFLAGS) -MMD -MP $< -c -o $@

.PHONY: library
library:
	git submodule update --init --recursive
	CMAKE_C_COMPILER=$(C_CC) cmake -DBUILD_SHARED_LIBS=OFF -Bbuild efsw
	cd build && make

.PHONY: clean
clean:
	rm -rf $(OBJ) $(TARGET) $(shell find . -name "*.dSYM")
	cd efsw && make clean

.PHONY: run
run: build
	./$(TARGET)

.PHONY: test
test: CFLAGS += -DNO_LOGGING
test: $(OBJ)
	@for f in $(TESTSDIR)/*.cpp; do \
		echo "  testing $$f"; \
		$(CPP_CC) $(CPPFLAGS) "$$f" $^ -o $(TESTSDIR)/tmpexec; \
		$(TESTSDIR)/tmpexec && printf "\033[32m+ test $$f passed\033[m\n" || printf "\033[31m- test $$f failed\033[m\n"; \
	done; \
	find $(TESTSDIR) -type f -not -name "*.cpp" -delete; \
	rm -rf $(TESTSDIR)/tmpexec.dSYM
