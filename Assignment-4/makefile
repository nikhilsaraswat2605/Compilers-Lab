RM = rm -f
OUTPUT_FILE = ass4_20CS30003_20CS10039_output.txt
SRC_FILE = ass4_20CS30003_20CS10039_main.c
TEST_FILE = ass4_20CS30003_20CS10039_test.c
LEX_FILE = ass4_20CS30003_20CS10039.l
YACC_FILE = ass4_20CS30003_20CS10039.y
OBJ_FILE = ass4_20CS30003_20CS10039_main.o
LEX_SRC = lex.yy.c
PARSER = parser
CC = gcc
LEX_COMPILER = flex
BISON_COMPIILER = bison
LEX_FLAG = -lfl
BISON_FLAG = -dty --report=all


ass4_20CS30003_20CS10039_output.txt: parser ass4_20CS30003_20CS10039_test.c
	./$(PARSER) < $(TEST_FILE) > $(OUTPUT_FILE)

parser: lex.yy.o y.tab.o ass4_20CS30003_20CS10039_main.o
	$(CC) lex.yy.o y.tab.o $(OBJ_FILE) $(LEX_FLAG) -o $(PARSER)

ass4_20CS30003_20CS10039_main.o: ass4_20CS30003_20CS10039_main.c
	$(CC) -c $(SRC_FILE)

lex.yy.o: lex.yy.c
	$(CC) -c $(LEX_SRC)

y.tab.o: y.tab.c
	$(CC) -c y.tab.c

lex.yy.c: ass4_20CS30003_20CS10039.l y.tab.h
	$(LEX_COMPILER) $(LEX_FILE)

y.tab.c y.tab.h: ass4_20CS30003_20CS10039.y
	$(BISON_COMPIILER) $(BISON_FLAG) $(YACC_FILE)

clean:
	$(RM) $(PARSER) $(OBJ_FILE) lex.yy.* y.tab.* y.output
