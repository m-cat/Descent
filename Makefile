DEPS_C  = src/*.c 
DEPS_H  = src/defs.h
DEPS    = $(DEPS_C) $(DEPS_H)
DIR     = src
WARNALL = -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
WARN    = -Wall -Werror
OUTPUT  = descent

descent : backup beautify compile clean
	./$(OUTPUT)

compile : $(DEPS)
	gcc $(DEPS) -Llib -Iinclude -ltcod-mingw $(WARN) -o $(OUTPUT)
	
analyze : $(DEPS)
	/cppcheck/cppcheck $(DIR) --enable=warning,performance,information --inconclusive
	gcc $(DEPS) -Llib -Iinclude -ltcod-mingw $(WARNALL) -o /dev/null
	
beautify : $(DIR)
	gc -dir-$(DIR)

backup : $(DEPS)
	mkdir backup -p
	cp -r $? backup
	
clean :
	rm -f -r *~ *.stackdump src/*.tmp src/*~ src/*\#
