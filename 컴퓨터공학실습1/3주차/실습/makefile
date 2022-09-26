cc = g++
target = main
objects = main.o array.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

$(objects) : array.h

.PHONY : clean
clean :
	rm $(target) $(objects)
