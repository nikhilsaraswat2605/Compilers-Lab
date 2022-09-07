/*
 *   // Amit Kumar  - 20CS30003
 *   // Nikhil Saraswat - 20CS10039
 */
#include <stdio.h>

extern int yylex();
extern int yylineno;
extern char *yytext;

int main()
{
        int token = yylex();
        while (token)
        {
                if (token == KEYWORD)
                {
                        printf("<KEYWORD, %s> ", yytext);
                }
                else if (token == INTEGER_CONST)
                {
                        printf("<CONSTANT, %s> ", yytext);
                }
                else if (token == IDENTIFIER)
                {
                        printf("<IDENTIFIER, %s> ", yytext);
                }
                else if (token == FLOATING_CONST)
                {
                        printf("<CONSTANT, %s> ", yytext);
                }
                else if (token == CHARACTER_CONST)
                {
                        printf("<CONSTANT, %s> ", yytext);
                }
                else if (token == NEWLINE)
                {
                        printf("\n");
                }
                else if (token == STRING_LITERAL)
                {
                        printf("<STRING-LITERAL, %s> ", yytext);
                }
                else if (token == MULTILINE_COMMENT)
                {
                        printf("<MULTILINE-COMMENT> ");
                }
                else if (token == SINGLELINE_COMMENT)
                {
                        printf("<SINGLELINE-COMMENT>\n");
                }
                else if (token == PUNCTUATOR)
                {
                        printf("<PUNCTUATOR, %s> ", yytext);
                }
                else if (token == OPERATOR)
                {
                        printf("<PUNCTUATOR, %s> ", yytext);
                }
                else if (token == SPECIAL_SYMBOL)
                {
                        printf("<PUNCTUATOR, %s> ", yytext);
                }
                else if (token == ESCAPE_CHAR_SEQUENCE){
                        printf("<CONSTANT, %s> ", yytext);
                }
                else
                {
                        printf("<ERROR-TOKEN, %s> ", yytext);
                }
                token = yylex();
        }
        return 0;
}
int yywrap()
{
    return 1;
}
