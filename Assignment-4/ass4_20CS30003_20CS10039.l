%{
    #include "y.tab.h"
    #include <stdlib.h>
    #include <stdio.h>
    int lineNum = 1;
%}


ESCAPE_CHAR_SEQUENCE    "\\f"|"\\n"|"\\r"|"\\t"|"\\v"|"\\b"|"\\a"|"\\\\"|"\\?"|"\\\""|"\\\'"



DIGIT                   [0-9]
DIGITS                  {DIGIT}{DIGIT}*
NONZERO_DIGIT           [1-9]
INTEGER_CONST           [0]|{NONZERO_DIGIT}{DIGIT}*

ALPHA_                  [A-Za-z_]
ALPHA_NUM_              ({ALPHA_}|{DIGIT})
IDENTIFIER              {ALPHA_}{ALPHA_NUM_}*

SIGN                    [+-]
EXPONENT_PART           [Ee]({SIGN}?)({DIGIT}{DIGIT}*)
FRACTIONAL_CONST     {DIGITS}?\.{DIGITS}|{DIGITS}\.
FLOATING_CONST       {FRACTIONAL_CONST}({EXPONENT_PART}?)|{DIGITS}{EXPONENT_PART}

ENUMERATION_CONST    {IDENTIFIER}

ALL_CHARS               [^\'\\\n]
C_CHAR                  {ESCAPE_CHAR_SEQUENCE}|{ALL_CHARS}
C_CHAR_SEQUENCE         {C_CHAR}{C_CHAR}*
SINGLE_QUOTE            [\']
CHARACTER_CONST         {SINGLE_QUOTE}{C_CHAR_SEQUENCE}{SINGLE_QUOTE}

ALL_S_CHARS             [^\"\\\n]
S_CHAR                  {ESCAPE_CHAR_SEQUENCE}|{ALL_S_CHARS}
S_CHAR_SEQUENCE         {S_CHAR}{S_CHAR}*
DOUBLE_QUOTE            [\"]
STRING_LITERAL          {DOUBLE_QUOTE}({S_CHAR_SEQUENCE}?){DOUBLE_QUOTE}

WS                       [ \t]
NEWLINE                  [\n]

%x BODY_OF_MULTILINE_COMMENT

%x BODY_OF_SINGLELINE_COMMENT

%%

"while"                          return WHILE; 
"volatile"                       return VOLATILE; 
"_Complex"                       return _COMPLEX; 
"_Bool"                          return _BOOL; 
"default"                        return DEFAULT; 
"_Imaginary"                     return _IMAGINARY; 
"double"                         return DOUBLE; 
"do"                             return DO; 
"restrict"                       return RESTRICT; 
"register"                       return REGISTER; 
"short"                          return SHORT; 
"return"                         return RETURN; 
"sizeof"                         return SIZEOF; 
"signed"                         return SIGNED; 
"struct"                         return STRUCT; 
"static"                         return STATIC; 
"typedef"                        return TYPEDEF; 
"switch"                         return SWITCH; 
"enum"                           return ENUM; 
"else"                           return ELSE; 
"continue"                       return CONTINUE; 
"goto"                           return GOTO; 
"inline"                         return INLINE; 
"if"                             return IF; 
"long"                           return LONG; 
"int"                            return INT; 
"unsigned"                       return UNSIGNED; 
"void"                           return VOID; 
"union"                          return UNION; 
"extern"                         return EXTERN; 
"float"                          return FLOAT; 
"auto"                           return AUTO; 
"break"                          return BREAK; 
"for"                            return FOR; 
"char"                           return CHAR; 
"case"                           return CASE; 
"const"                          return CONST; 

{CHARACTER_CONST}            { yylval.Char_Value = strdup(yytext); return CHARACTER_CONST; }
{STRING_LITERAL}                { yylval.String_Value = strdup(yytext); return STRING_LITERAL; }
{INTEGER_CONST}              { yylval.Int_Value = atoi(yytext); return INTEGER_CONST; }
{FLOATING_CONST}             { yylval.Float_Value = atof(yytext); return FLOATING_CONST; }
{IDENTIFIER}                    { yylval.Identifier_Value = strdup(yytext); return IDENTIFIER; }

"~"                              return TILDE; 
"-"                              return MINUS; 
"%"                              return MODULO; 
"!"                              return EXCLAMATION; 
">>"                             return RIGHT_SHIFT; 
"<<"                             return LEFT_SHIFT; 
">"                              return GREATER_THAN; 
"<"                              return LESS_THAN; 
">="                             return GREATER_EQUAL_THAN; 
"<="                             return LESS_EQUAL_THAN; 
"||"                             return LOGICAL_OR; 
"&="                             return BITWISE_AND_ASSIGNMENT; 
"!="                             return NOT_EQUALS; 
"^="                             return BITWISE_XOR_ASSIGNMENT; 
"|="                             return BITWISE_OR_ASSIGNMENT; 
";"                              return SEMI_COLON; 
"..."                            return ELLIPSIS; 
":"                              return COLON; 
"/="                             return SLASH_ASSIGNMENT; 
"*="                             return ASTERISK_ASSIGNMENT; 
"+="                             return PLUS_ASSIGNMENT; 
"%="                             return MODULO_ASSIGNMENT; 
"<<="                            return LEFT_SHIFT_ASSIGNMENT; 
"-="                             return MINUS_ASSIGNMENT; 
"--"                             return DECREMENT; 
"#"                              return HASH; 
"&"                              return BITWISE_AND; 
")"                              return RIGHT_PARENTHESES; 
"^"                              return BITWISE_XOR; 
"=="                             return EQUALS; 
"&&"                             return LOGICAL_AND; 
"|"                              return BITWISE_OR; 
">>="                            return RIGHT_SHIFT_ASSIGNMENT; 
"++"                             return INCREMENT; 
"["                              return LEFT_SQUARE_BRACKET; 
"?"                              return QUESTION_MARK; 
"/"                              return SLASH; 
","                              return COMMA; 
"="                              return ASSIGNMENT; 
"("                              return LEFT_PARENTHESES; 
"]"                              return RIGHT_SQUARE_BRACKET; 
"}"                              return RIGHT_CURLY_BRACKET; 
"{"                              return LEFT_CURLY_BRACKET; 
"->"                             return ARROW; 
"."                              return DOT; 
"+"                              return PLUS; 
"*"                              return ASTERISK; 

{WS}                    /*ignore chars of types *****   */


"/"+"*"                                     { BEGIN(BODY_OF_MULTILINE_COMMENT); }
<BODY_OF_MULTILINE_COMMENT>"\n"                { lineNum++; printf("\n-------------------- Line No. %d getting parsed  --------------------\n\n", lineNum); }
<BODY_OF_MULTILINE_COMMENT>"*"+[^*/\n]*        { /*ignore chars of types *****   */ }
<BODY_OF_MULTILINE_COMMENT>[^*\n]*             {/*ignore chars except \n and * */ }
<BODY_OF_MULTILINE_COMMENT>"*"+"/"             { BEGIN(INITIAL); }

"/"+"/"                                     { BEGIN(BODY_OF_SINGLELINE_COMMENT); }
<BODY_OF_SINGLELINE_COMMENT>.                  {/* ignores comment */ }
<BODY_OF_SINGLELINE_COMMENT><<EOF>>            { BEGIN(INITIAL); lineNum++; printf("\n-------------------- Line No. %d getting parsed  --------------------\n\n", lineNum); }
<BODY_OF_SINGLELINE_COMMENT>"\n"               { BEGIN(INITIAL); lineNum++; printf("\n-------------------- Line No. %d getting parsed  --------------------\n\n", lineNum); }



{NEWLINE}                       { lineNum++; printf("\n-------------------- Line No. %d getting parsed  --------------------\n\n", lineNum); }
.                               { printf("ERROR [Line %d] : Invalid token %s\n", lineNum, yytext); return ERROR_TOKEN; }

%%
