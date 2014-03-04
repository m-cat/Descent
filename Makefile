DEPS = src/*.c

descent : $(DEPS)
	gcc $(DEPS) -lncurses -g -o bin/$@

clean :
	rm -f -r */*~ */*.stackdump bin
