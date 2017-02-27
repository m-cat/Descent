# C makefile for a generic moderate-sized program
# Author: Marcin Swieczkowski
# Notes:
#  * All warnings enabled and all warnings are treated as errors.
#  * Errors are sent to the file errors

DEPS_C  = src/*.c
DEPS_H  = src/defs.h
DEPS    = $(DEPS_C) $(DEPS_H)
DIR     = src
WARNALL = -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused
WARN    = -Wall -Werror
OUTPUT  = descent
ERRORS  = errors
LIBS    = -Llib -Iinclude -ltcod-mingw
O       = -O1
DEBUG   = -m32 -g -fno-inline -fno-omit-frame-pointer
BACKUP  = backup

package : build

build : compile clean
	./$(OUTPUT)

compile : $(DEPS)
	gcc $(DEPS) $(LIBS) $(WARN) $(O) -o $(OUTPUT) 2>&1 | tee $(ERRORS)
#	gtags

analyze : $(DEPS)
	/cppcheck/cppcheck $(DIR) --enable=warning,performance,information --inconclusive
	gcc $(DEPS) $(LIBS) $(WARNALL) $(O) $(DEBUG) -o $(OUTPUT)-debug 2>&1 | tee $(ERRORS)

# beautify : $(DEPS)
#	gc -dir-$(DIR)

backup : $(DEPS)
	mkdir $(BACKUP) -p
	cp -r $? $(BACKUP)

clean :
	rm -f -r *~ *.stackdump src/*.tmp src/*~ src/*\#

debug : $(DEPS)
	gcc $(DEPS) $(LIBS) $(O) $(DEBUG) -o $(OUTPUT)-debug
	gdb $(OUTPUT)-debug
