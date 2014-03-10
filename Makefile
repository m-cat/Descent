DEPS = src/*.c

descent : $(DEPS)
	gcc $(DEPS) -Llib -Iinclude -ltcod-mingw -o $@

clean :
	rm -f -r *~ src/*~ src/*\# *.stackdump
