RM=rm -f
files_to_remove=ass5_20CS30003_20CS10039_translator.o lex.yy.* y.tab.* y.output *.out
CXX_FILE=ass5_20CS30003_20CS10039_translator.cxx
GCC=g++
LEX_FILE=ass5_20CS30003_20CS10039.l
BISON_FILE=ass5_20CS30003_20CS10039.y
TEST1_C_FILE=ass5_20CS30003_20CS10039_test1.c
TEST2_C_FILE=ass5_20CS30003_20CS10039_test2.c
TEST3_C_FILE=ass5_20CS30003_20CS10039_test3.c
TEST4_C_FILE=ass5_20CS30003_20CS10039_test4.c
TEST5_C_FILE=ass5_20CS30003_20CS10039_test5.c
TEST6_C_FILE=ass5_20CS30003_20CS10039_test6.c
TEST7_C_FILE=ass5_20CS30003_20CS10039_test7.c
TEST8_C_FILE=ass5_20CS30003_20CS10039_test8.c
TEST1_QUAD_FILE=ass5_20CS30003_20CS10039_quads1.out
TEST2_QUAD_FILE=ass5_20CS30003_20CS10039_quads2.out
TEST3_QUAD_FILE=ass5_20CS30003_20CS10039_quads3.out
TEST4_QUAD_FILE=ass5_20CS30003_20CS10039_quads4.out
TEST5_QUAD_FILE=ass5_20CS30003_20CS10039_quads5.out
TEST6_QUAD_FILE=ass5_20CS30003_20CS10039_quads6.out
TEST7_QUAD_FILE=ass5_20CS30003_20CS10039_quads7.out
TEST8_QUAD_FILE=ass5_20CS30003_20CS10039_quads8.out
FLAG=-lfl
parser.out: lex.yy.o y.tab.o ass5_20CS30003_20CS10039_translator.o
	$(GCC) lex.yy.o y.tab.o ass5_20CS30003_20CS10039_translator.o $(FLAG) -o parser.out

ass5_20CS30003_20CS10039_translator.o: ass5_20CS30003_20CS10039_translator.cxx ass5_20CS30003_20CS10039_translator.h
	$(GCC) -c $(CXX_FILE)

lex.yy.o: lex.yy.c
	$(GCC) -c lex.yy.c

y.tab.o: y.tab.c
	$(GCC) -c y.tab.c

lex.yy.c: ass5_20CS30003_20CS10039.l y.tab.h ass5_20CS30003_20CS10039_translator.h
	flex $(LEX_FILE)

y.tab.c y.tab.h: ass5_20CS30003_20CS10039.y
	bison -dty --report=all $(BISON_FILE)

clean:
	$(RM) parser.out $(files_to_remove)

test: parser.out
	@echo "\n\nRunning test 1 (Arithmetic, shift, and bit expressions)"
	./parser.out < $(TEST1_C_FILE) > $(TEST1_QUAD_FILE)
	@echo "Test 1 complete\n"

	@echo "Running test 2 (Unary expressions and operators)"
	./parser.out < $(TEST2_C_FILE) > $(TEST2_QUAD_FILE)
	@echo "Test 2 complete\n"

	@echo "Running test 3 (Relational and boolean operators with if-else blocks)"
	./parser.out < $(TEST3_C_FILE) > $(TEST3_QUAD_FILE)
	@echo "Test 3 complete\n"

	@echo "Running test 4 (Arrays and pointers)"
	./parser.out < $(TEST4_C_FILE) > $(TEST4_QUAD_FILE)
	@echo "Test 4 complete\n"

	@echo "Running test 5 (Declarations and assignments)"
	./parser.out < $(TEST5_C_FILE) > $(TEST5_QUAD_FILE)
	@echo "Test 5 complete\n"

	@echo "Running test 6 (Loops)"
	./parser.out < $(TEST6_C_FILE) > $(TEST6_QUAD_FILE)
	@echo "Test 6 complete\n"

	@echo "Running test 7 (Function calls)"
	./parser.out < $(TEST7_C_FILE) > $(TEST7_QUAD_FILE)
	@echo "Test 7 complete\n"

	@echo "Running test 8 (General file)"
	./parser.out < $(TEST8_C_FILE) > $(TEST8_QUAD_FILE)
	@echo "Test 8 complete\n"
	