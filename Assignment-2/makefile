RM = rm -f
OBJECTS = ass2_20CS10039.o main.o
LIBRARY = libass2_20CS10039.a
TARGETS = $(OBJECTS) Library a.out

all: $(TARGETS)

ass2_20CS10039.o: ass2_20CS10039.c myl.h
	gcc -Wall -c ass2_20CS10039.c

main.o: main.c myl.h
	gcc -Wall -c main.c

Library: ass2_20CS10039.o 
	ar -rcs libass2_20CS10039.a ass2_20CS10039.o

a.out: main.o Library
	gcc -Wall main.o -L. -lass2_20CS10039

clean:
	$(RM) $(OBJECTS) $(LIBRARY) a.out