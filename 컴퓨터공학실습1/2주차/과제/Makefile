cc = gcc
target = starwars
objects = main.o calc.o print.o

$(target): $(objects)
	$(cc) -o $(target) $(objects)

$(objects) : header.h

.PHONY : clean
clean :
	rm $(target) $(objects)
