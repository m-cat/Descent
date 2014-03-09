DEPS = src/*.c

descent : $(DEPS)
	@mkdir -p bin
	@cp -r data bin
	gcc $(DEPS) -lncurses -m32 -o bin/$@
#	gcc $(DEPS) -lncurses -m64 -o bin/$@-64

clean :
	rm -f -r */*~ *.stackdump bin
