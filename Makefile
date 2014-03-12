DEPS = src/*.c

descent : $(DEPS)
	gcc $(DEPS) -Llib -Iinclude -ltcod-mingw -Wall -Werror -o $@

clean :
	rm -f -r *~ src/*~ src/*\# *.stackdump
