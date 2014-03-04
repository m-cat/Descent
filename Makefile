DEPS = *.c

descent : $(DEPS)
	gcc $(DEPS) -lncurses -g -o $@

clean :
	rm -f -r *~ *.stackdump descent
