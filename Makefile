# Makefile for a generic medium-sized C program.
# Author: Marcin Swieczkowski
# Notes:
#  * All warnings enabled and all warnings are treated as errors.
#  * Errors are sent to the file "errors".

# Configure these options:

NAME    = descent
O       = -O1

# Leave these alone:

DEPS_C  = src/*.c
DEPS_H  = src/*.h
SRC  = $(DEPS_C) $(DEPS_H)

BACKUP  = backup
DIR     = src
ERRORS  = errors
LINTS   = lints
OUTPUT  = $(NAME)

DEBUG   = -m32 -g -fno-inline -fno-omit-frame-pointer
CF_OPTS = -style="{BasedOnStyle: llvm, IndentWidth: 4}"
LIBS    = -Llib -Iinclude -ltcod-mingw
OC_OPTS = -enable-clang-static-analyzer -no-analytics -o $(LINTS)
WARN    = -Wall -Werror
WARNALL = -Wall -Wextra

GARBAGE = *~ *.stackdump src/*.tmp src/*~ src/*\#

package: format build

build: compile clean
	./$(OUTPUT)

compile: $(SRC)
	gcc $(DEPS_C) $(LIBS) $(WARN) $(O) -o $(OUTPUT) 2>&1 | tee $(ERRORS)
#	gtags

analyze:
	oclint $(OC_OPTS) $(SRC) -- $(LIBS) -c; rm *.plist

format: backup
	clang-format $(CF_OPTS) -i $(SRC)

backup:
	mkdir -p $(BACKUP)
	cp -r -f $(SRC) $(BACKUP)

clean:
	rm -f -r $(GARBAGE)

debug: $(SRC)
	gcc $(SRC) $(LIBS) $(O) $(DEBUG) -o $(OUTPUT)-debug
	gdb $(OUTPUT)-debug
