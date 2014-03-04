DEPS = src/*.c

descent : $(DEPS)
	@mkdir -p bin
	gcc $(DEPS) -lncurses -g -o bin/$@

clean :
	rm -f -r */*~ */*.stackdump bin
