DEPS = src/*.c src/defs.h

descent : $(DEPS)
	gcc $(DEPS) -Llib -Iinclude -ltcod-mingw -Wall -Werror -o $@

clean :
	rm -f -r *~ *.stackdump src/*.tmp src/*~ src/*\#
