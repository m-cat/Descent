DEPS_C = src/*.c 
DEPS_H = src/defs.h
DEPS   = $(DEPS_C) $(DEPS_H)
DIR    = src

descent : backup beautify compile

compile : $(DEPS)
	gcc $(DEPS) -Llib -Iinclude -ltcod-mingw -Wall -Werror -o $@
	
beautify : $(DEPS_C) $(DEPS_H)
	gc -dir-src

backup : $(DEPS)
	mkdir backup -p
	cp -r $? backup
	
clean :
	rm -f -r *~ *.stackdump src/*.tmp src/*~ src/*\#
