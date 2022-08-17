# RM = rm -f
# OBJECTS = ass2_20CS10039.o main.o
# SRCFILES = ass2_20CS10039.c main.c 
# LIBRARY = libass2_20CS10039.a
# TARGETS = $(OBJECTS) Library a.out

# all: $(TARGETS)

# ass2_20CS10039.o: ass2_20CS10039.c myl.h
# 	gcc -Wall -c ass2_20CS10039.c

# main.o: main.c myl.h
# 	gcc -Wall -c main.c

# Library: ass2_20CS10039.o 
# 	ar -rcs libass2_20CS10039.a ass2_20CS10039.o

# a.out: main.o Library
# 	gcc -Wall main.o -L. -lass2_20CS10039

# clean:
# 	$(RM) $(OBJECTS) $(LIBRARY) a.out


RM = rm -f
OBJECTS = ass2_20CS10039_printStr.o ass2_20CS10039_printInt.o ass2_20CS10039_printFlt.o ass2_20CS10039_readInt.o ass2_20CS10039_readFlt.o
SRCFILES = ass2_20CS10039_printStr.c ass2_20CS10039_printInt.c ass2_20CS10039_printFlt.c ass2_20CS10039_readInt.c ass2_20CS10039_readFlt.c
LIBRARY = libass2_20CS10039.a
TARGETS = $(OBJECTS) Library a.out

all: $(TARGETS)

ass2_20CS10039_printStr.o: myl.h $(SRCFILES) 
	gcc -Wall -c ass2_20CS10039_printStr.c

ass2_20CS10039_printInt.o: myl.h $(SRCFILES)
	gcc -Wall -c ass2_20CS10039_printInt.c

ass2_20CS10039_printFlt.o: myl.h $(SRCFILES)
	gcc -Wall -c ass2_20CS10039_printFlt.c

ass2_20CS10039_readInt.o: myl.h $(SRCFILES)
	gcc -Wall -c ass2_20CS10039_readInt.c

ass2_20CS10039_readFlt.o: myl.h $(SRCFILES)
	gcc -Wall -c ass2_20CS10039_readFlt.c

main.o: main.c myl.h
	gcc -Wall -c main.c

Library: $(OBJECTS)
	ar -rcs libass2_20CS10039.a $(OBJECTS)

a.out: main.o Library
	gcc -Wall main.o -L. -lass2_20CS10039

clean:
	$(RM) $(OBJECTS) $(LIBRARY) a.out main.o