OBJECTS = ass2_20CS10039.o main.o
LIBRARY = ass2_20CS10039.a
TARGETS = $(OBJECTS) $(LIBRARY) a.out

all: $(TARGETS)

ass2_20CS10039.o: ass2_20CS10039.c myl.h
	gcc -Wall -c ass2_20CS10039.c

ass2_20CS10039.a: ass2_20CS10039.o 
	ar -rcs libass2_20CS10039.a ass2_20CS10039.o

main.o: main.c myl.h
	gcc -Wall -c main.c

a.out: main.o ass2_20CS10039.a
	gcc main.o -L. -lass2_20CS10039

clean:
	rm -f *.o *.a a.out