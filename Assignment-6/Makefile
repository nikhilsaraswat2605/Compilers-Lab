RM=rm -f
files_to_remove=*.a *.o lex.yy.* y.tab.* y.output test* *.out *.s
CXX_FILE=ass6_20CS30003_20CS10039_translator.cxx
GCC=g++
LEX_FILE=ass6_20CS30003_20CS10039.l
BISON_FILE=ass6_20CS30003_20CS10039.y
TEST2_C_FILE=ass6_20CS30003_20CS10039_test2
TEST1_C_FILE=ass6_20CS30003_20CS10039_test1
TEST3_C_FILE=ass6_20CS30003_20CS10039_test3
TEST4_C_FILE=ass6_20CS30003_20CS10039_test4
TEST5_C_FILE=ass6_20CS30003_20CS10039_test5
TEST6_C_FILE=ass6_20CS30003_20CS10039_test6
TEST7_C_FILE=ass6_20CS30003_20CS10039_test7
ASM2_S_FILE=ass6_20CS30003_20CS10039_test2.s
ASM1_S_FILE=ass6_20CS30003_20CS10039_test1.s
ASM3_S_FILE=ass6_20CS30003_20CS10039_test3.s
ASM4_S_FILE=ass6_20CS30003_20CS10039_test4.s
ASM5_S_FILE=ass6_20CS30003_20CS10039_test5.s
ASM6_S_FILE=ass6_20CS30003_20CS10039_test6.s
ASM7_S_FILE=ass6_20CS30003_20CS10039_test7.s
TEST1_QUAD_FILE=ass6_20CS30003_20CS10039_quads1.out
TEST2_QUAD_FILE=ass6_20CS30003_20CS10039_quads2.out
TEST3_QUAD_FILE=ass6_20CS30003_20CS10039_quads3.out
TEST4_QUAD_FILE=ass6_20CS30003_20CS10039_quads4.out
TEST5_QUAD_FILE=ass6_20CS30003_20CS10039_quads5.out
TEST6_QUAD_FILE=ass6_20CS30003_20CS10039_quads6.out
TEST7_QUAD_FILE=ass6_20CS30003_20CS10039_quads7.out
TEST8_QUAD_FILE=ass6_20CS30003_20CS10039_quads8.out
FLAG=-lfl
lass2=lass2_20CS30003_20CS10039
translator=ass6_20CS30003_20CS10039_translator.o
traget_translator=ass6_20CS30003_20CS10039_target_translator.o
target_translator_CXX=ass6_20CS30003_20CS10039_target_translator.cxx
BISON_FLAG=-dty --report=all
parser.out: lex.yy.o y.tab.o ass6_20CS30003_20CS10039_translator.o ass6_20CS30003_20CS10039_target_translator.o
	$(GCC) lex.yy.o y.tab.o $(translator) $(traget_translator) $(FLAG) -o parser.out

ass6_20CS30003_20CS10039_target_translator.o: ass6_20CS30003_20CS10039_target_translator.cxx ass6_20CS30003_20CS10039_translator.h
	$(GCC) -c $(target_translator_CXX)

ass6_20CS30003_20CS10039_translator.o: ass6_20CS30003_20CS10039_translator.cxx ass6_20CS30003_20CS10039_translator.h
	$(GCC) -c $(CXX_FILE)

lex.yy.o: lex.yy.c
	$(GCC) -c lex.yy.c

y.tab.o: y.tab.c
	$(GCC) -c y.tab.c

lex.yy.c: ass6_20CS30003_20CS10039.l y.tab.h ass6_20CS30003_20CS10039_translator.h
	flex $(LEX_FILE)

y.tab.c y.tab.h: ass6_20CS30003_20CS10039.y
	bison $(BISON_FLAG) $(BISON_FILE)

libass2_20CS30003_20CS10039.a: ass2_20CS30003_20CS10039.o
	ar -rcs libass2_20CS30003_20CS10039.a ass2_20CS30003_20CS10039.o

ass2_20CS30003_20CS10039.o: ass2_20CS30003_20CS10039.c myl.h
	gcc -c ass2_20CS30003_20CS10039.c

clean:
	$(RM) $(files_to_remove)

test: parser.out libass2_20CS30003_20CS10039.a
	@echo "--------Generating assembly files--------\n"
	./parser.out $(TEST1_C_FILE) > $(TEST1_QUAD_FILE)
	./parser.out $(TEST2_C_FILE) > $(TEST2_QUAD_FILE)
	./parser.out $(TEST3_C_FILE) > $(TEST3_QUAD_FILE)
	./parser.out $(TEST4_C_FILE) > $(TEST4_QUAD_FILE)
	./parser.out $(TEST5_C_FILE) > $(TEST5_QUAD_FILE)
	./parser.out $(TEST6_C_FILE) > $(TEST6_QUAD_FILE)
	./parser.out $(TEST7_C_FILE) > $(TEST7_QUAD_FILE)
	@echo "--------Compiling assembly files--------\n"
	gcc $(ASM1_S_FILE) -L. -$(lass2) -no-pie -o test1
	gcc $(ASM2_S_FILE) -L. -$(lass2) -no-pie -o test2
	gcc $(ASM3_S_FILE) -L. -$(lass2) -no-pie -o test3
	gcc $(ASM4_S_FILE) -L. -$(lass2) -no-pie -o test4
	gcc $(ASM5_S_FILE) -L. -$(lass2) -no-pie -o test5
	gcc $(ASM6_S_FILE) -L. -$(lass2) -no-pie -o test6
	gcc $(ASM7_S_FILE) -L. -$(lass2) -no-pie -o test7
	@echo "--------Binaries generated. Run using ./test<num>\n\n"
	@echo "--------Running test1:\n"
	@./test1
	@echo "--------Running test2:\n"
	@./test2
	@echo "--------Running test3:\n"
	@./test3
	@echo "--------Running test4:\n"
	@./test4
	@echo "--------Running test5:\n"
	@./test5
	@echo "--------Running test6:\n"
	@./test6
	@echo "--------Running test7:\n"
	@./test7
