%{
    #include "ass6_20CS30003_20CS10039_translator.h"
    int lineCount = 1;
    #include "y.tab.h"
    #include <stdlib.h>
    #include <stdio.h>
%}

%option noyywrap




ESCAPE_CHAR_SEQUENCE    "\\f"|"\\n"|"\\r"|"\\t"|"\\v"|"\\b"|"\\a"|"\\\\"|"\\?"|"\\\""|"\\\'"



DIGIT                   [0-9]
DIGITS                  {DIGIT}{DIGIT}*
NONZERO_DIGIT           [1-9]
INTEGER_CONSTANT           [0]|{NONZERO_DIGIT}{DIGIT}*

ALPHA_                  [A-Za-z_]
ALPHA_NUM_              ({ALPHA_}|{DIGIT})
IDENTIFIER              {ALPHA_}{ALPHA_NUM_}*

SIGN                    [+-]
EXPONENT_PART           [Ee]({SIGN}?)({DIGIT}{DIGIT}*)
FRACTIONAL_CONSTANT     {DIGITS}?\.{DIGITS}|{DIGITS}\.
FLOATING_CONSTANT       {FRACTIONAL_CONSTANT}({EXPONENT_PART}?)|{DIGITS}{EXPONENT_PART}

ENUMERATION_CONSTANT    {IDENTIFIER}

ALL_CHARS               [^\'\\\n]
C_CHAR                  {ESCAPE_CHAR_SEQUENCE}|{ALL_CHARS}
C_CHAR_SEQUENCE         {C_CHAR}{C_CHAR}*
SINGLE_QUOTE            [\']
CHARACTER_CONSTANT         {SINGLE_QUOTE}{C_CHAR_SEQUENCE}{SINGLE_QUOTE}

ALL_S_CHARS             [^\"\\\n]
S_CHAR                  {ESCAPE_CHAR_SEQUENCE}|{ALL_S_CHARS}
S_CHAR_SEQUENCE         {S_CHAR}{S_CHAR}*
DOUBLE_QUOTE            [\"]
STRING_LITERAL          {DOUBLE_QUOTE}({S_CHAR_SEQUENCE}?){DOUBLE_QUOTE}

WS                       [ \t]
NEWLINE                  [\n]


%x BODY_OF_MULTI_LINE_COMMENT
%x BODY_OF_SINGLE_LINE_COMMENT

%%

"auto"                          { return AUTO; }
"break"                         { return BREAK; }
"case"                          { return CASE; }
"char"                          { return CHARTYPE; }
"const"                         { return CONST; }
"continue"                      { return CONTINUE; }
"default"                       { return DEFAULT; }
"do"                            { return DO; }
"double"                        { return DOUBLE; }
"else"                          { return ELSE; }
"enum"                          { return ENUM; }
"extern"                        { return EXTERN; }
"float"                         { return FLOATTYPE; }
"for"                           { return FOR; }
"goto"                          { return GOTO; }
"if"                            { return IF; }
"inline"                        { return INLINE; }
"int"                           { return INTTYPE; }
"long"                          { return LONG; }
"register"                      { return REGISTER; }
"restrict"                      { return RESTRICT; }
"return"                        { return RETURN; }
"short"                         { return SHORT; }
"signed"                        { return SIGNED; }
"sizeof"                        { return SIZEOF; }
"static"                        { return STATIC; }
"struct"                        { return STRUCT; }
"switch"                        { return SWITCH; }
"typedef"                       { return TYPEDEF; }
"union"                         { return UNION; }
"unsigned"                      { return UNSIGNED; }
"void"                          { return VOIDTYPE; }
"volatile"                      { return VOLATILE; }
"while"                         { return WHILE; }
"_Bool"                         { return _BOOL; }
"_Complex"                      { return _COMPLEX; }
"_Imaginary"                    { return _IMAGINARY; }


{IDENTIFIER}                    { yylval.symbol = currentTable->lookup(yytext);; return IDENTIFIER; }
{INTEGER_CONSTANT}              { yylval.intVal = atoi(yytext); return INTEGER_CONSTANT; }
{FLOATING_CONSTANT}             { yylval.floatVal = strdup(yytext); return FLOATING_CONSTANT; }
{CHARACTER_CONSTANT}            { yylval.charVal = strdup(yytext); return CHARACTER_CONSTANT; }
{STRING_LITERAL}                { yylval.stringVal = strdup(yytext); return STRING_LITERAL; }

"["                             { return LEFT_SQUARE_BRACKET; }
"++"                            { return INCREMENT; }
"/"                             { return SLASH; }
"?"                             { return QUESTION_MARK; }
"="                             { return ASSIGNMENT; }
","                             { return COMMA; }
"]"                             { return RIGHT_SQUARE_BRACKET; }
"("                             { return LEFT_PARENTHESES; }
"{"                             { return LEFT_CURLY_BRACKET; }
"}"                             { return RIGHT_CURLY_BRACKET; }
"."                             { return DOT; }
"->"                            { return ARROW; }
"*"                             { return ASTERISK; }
"+"                             { return PLUS; }
"-"                             { return MINUS; }
"~"                             { return TILDE; }
"!"                             { return EXCLAMATION; }
"%"                             { return MODULO; }
"<<"                            { return LEFT_SHIFT; }
">>"                            { return RIGHT_SHIFT; }
"<"                             { return LESS_THAN; }
">"                             { return GREATER_THAN; }
"<="                            { return LESS_EQUAL_THAN; }
">="                            { return GREATER_EQUAL_THAN; }
":"                             { return COLON; }
";"                             { return SEMI_COLON; }
"..."                           { return ELLIPSIS; }
"*="                            { return ASTERISK_ASSIGNMENT; }
"/="                            { return SLASH_ASSIGNMENT; }
"%="                            { return MODULO_ASSIGNMENT; }
"+="                            { return PLUS_ASSIGNMENT; }
"-="                            { return MINUS_ASSIGNMENT; }
"<<="                           { return LEFT_SHIFT_ASSIGNMENT; }
"#"                             { return HASH; }
"--"                            { return DECREMENT; }
")"                             { return RIGHT_PARENTHESES; }
"&"                             { return BITWISE_AND; }
"=="                            { return EQUALS; }
"^"                             { return BITWISE_XOR; }
"|"                             { return BITWISE_OR; }
"&&"                            { return LOGICAL_AND; }
"||"                            { return LOGICAL_OR; }
">>="                           { return RIGHT_SHIFT_ASSIGNMENT; }
"!="                            { return NOT_EQUALS; }
"&="                            { return BITWISE_AND_ASSIGNMENT; }
"|="                            { return BITWISE_OR_ASSIGNMENT; }
"^="                            { return BITWISE_XOR_ASSIGNMENT; }

"/*"                            { BEGIN(BODY_OF_MULTI_LINE_COMMENT); }
<BODY_OF_MULTI_LINE_COMMENT>"*/"   { BEGIN(INITIAL); }
<BODY_OF_MULTI_LINE_COMMENT>.      { }
<BODY_OF_MULTI_LINE_COMMENT>"\n"   { lineCount++; /* printf("\n-------------------- Parsing Line No. %d  --------------------\n\n", lineCount); */ }

"//"                            { BEGIN(BODY_OF_SINGLE_LINE_COMMENT); }
<BODY_OF_SINGLE_LINE_COMMENT>"\n"  { BEGIN(INITIAL); /* lineCount++; printf("\n-------------------- Parsing Line No. %d  --------------------\n\n", lineCount); */ }
<BODY_OF_SINGLE_LINE_COMMENT>.     { }

{WS}                    { }
{NEWLINE}                       { lineCount++; /* printf("\n-------------------- Parsing Line No. %d  --------------------\n\n", lineCount); */ }
.                               { printf("ERROR [Line %d] : Invalid token %s\n", lineCount, yytext); return INVALID_TOKEN; }

%%