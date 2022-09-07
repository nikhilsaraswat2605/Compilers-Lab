%{
    #define ERROR_TOKEN             100
    #define SINGLELINE_COMMENT      99
    #define MULTILINE_COMMENT       98
    #define PUNCTUATOR              97
    #define STRING_LITERAL          96
    #define CHARACTER_CONST         95
    #define ENUMERATION_CONST       94
    #define FLOATING_CONST          93
    #define INTEGER_CONST           92
    #define IDENTIFIER              91
    #define KEYWORD                 90
    #define WHITESPACE              89
    #define NEWLINE                 88
    #define SPECIAL_SYMBOL          87
    #define OPERATOR                86
    #define ESCAPE_CHAR_SEQUENCE    85
%}



PUNCTUATOR              "#"|","|"..."|";"|":"|"?"|"."
OPERATOR                "|="|"^="|"&="|">>="|"<<="|"-="|"+="|"%="|"/="|"*="|"="|"||"|"&&"|"|"|"^"|"!="|"=="|">="|"<="|">"|"<"|">>"|"<<"|"%"|"/"|"!"|"~"|"-"|"+"|"*"|"&"|"--"|"++"|"->"
SPECIAL_SYMBOL          "}"|"{"|")"|"("|"]"|"["
ESCAPE_CHAR_SEQUENCE    "\\f"|"\\n"|"\\r"|"\\t"|"\\v"|"\\b"|"\\a"|"\\\\"|"\\?"|"\\\""|"\\\'"
KEYWORD                 "auto"|"enum"|"restrict"|"unsigned"|"_Imaginary"|"_Complex"|"_Bool"|"while"|"volatile"|"void"|"union"|"typedef"|"switch"|"struct"|"static"|"sizeof"|"signed"|"short"|"return"|"register"|"long"|"int"|"inline"|"if"|"goto"|"for"|"float"|"extern"|"else"|"double"|"do"|"default"|"continue"|"const"|"char"|"case"|"break"


DIGIT                   [0-9]
NONZERO_DIGIT           [1-9]
DIGITS                  {DIGIT}{DIGIT}*
INTEGER_CONST           [0]|{NONZERO_DIGIT}{DIGIT}*
ALPHA_                  [A-Za-z_]
ALPHA_NUM_              ({ALPHA_}|{DIGIT})
IDENTIFIER              {ALPHA_}{ALPHA_NUM_}*
SIGN                    [+-]
EXPONENT_PART           [Ee]({SIGN}?)({DIGIT}{DIGIT}*)
DECIMAL                 [\.]
FRACTIONAL_CONST        {DIGITS}{DECIMAL}|({DIGIT}{DIGIT}*)?{DECIMAL}{DIGITS}
FLOATING_CONST          ({DIGIT}{DIGIT}*)([Ee]({SIGN}?)({DIGIT}{DIGIT}*))|{FRACTIONAL_CONST}({EXPONENT_PART}?)
ENUMERATION_CONST       {IDENTIFIER}
WS                      [ \t]
NEWLINE                 [\n]

ALL_S_CHARS             [^\"\\\n]
S_CHAR                  {ESCAPE_CHAR_SEQUENCE}|{ALL_S_CHARS}
S_CHAR_SEQUENCE         {S_CHAR}{S_CHAR}*
DOUBLE_QUOTE            [\"]
STRING_LITERAL          {DOUBLE_QUOTE}({S_CHAR_SEQUENCE}?){DOUBLE_QUOTE}

ALL_CHARS               [^\'\\\n]
C_CHAR                  {ESCAPE_CHAR_SEQUENCE}|{ALL_CHARS}
C_CHAR_SEQUENCE         {C_CHAR}{C_CHAR}*
SINGLE_QUOTE            [\']
CHARACTER_CONST         {SINGLE_QUOTE}{C_CHAR_SEQUENCE}{SINGLE_QUOTE}



%x BODY_OF_SINGLELINE_COMMENT


%x BODY_OF_MULTILINE_COMMENT




%%

{PUNCTUATOR}                     return PUNCTUATOR;
{SPECIAL_SYMBOL}                 return SPECIAL_SYMBOL;
{OPERATOR}                       return OPERATOR;
{INTEGER_CONST}                  return INTEGER_CONST;
{FLOATING_CONST}                 return FLOATING_CONST;
{KEYWORD}                        return KEYWORD;
{ESCAPE_CHAR_SEQUENCE}           return ESCAPE_CHAR_SEQUENCE;
{IDENTIFIER}                     return IDENTIFIER;
{CHARACTER_CONST}                return CHARACTER_CONST;
{STRING_LITERAL}                 return STRING_LITERAL;
{NEWLINE}                        return NEWLINE;

{WS}                             /*ignores delimiters */;

"/"+"/"                                     { BEGIN(BODY_OF_SINGLELINE_COMMENT); }
<BODY_OF_SINGLELINE_COMMENT>.                  {/* ignores comment */ }
<BODY_OF_SINGLELINE_COMMENT><<EOF>>            { BEGIN(INITIAL); return SINGLELINE_COMMENT; }
<BODY_OF_SINGLELINE_COMMENT>"\n"               { BEGIN(INITIAL); return SINGLELINE_COMMENT; }



"/"+"*"                                     { BEGIN(BODY_OF_MULTILINE_COMMENT); }
<BODY_OF_MULTILINE_COMMENT>"\n"                { /*ignore chars of types \n   */ }
<BODY_OF_MULTILINE_COMMENT>"*"+[^*/\n]*        { /*ignore chars of types *****   */ }
<BODY_OF_MULTILINE_COMMENT>[^*\n]*             {/*ignore chars except \n and * */ }
<BODY_OF_MULTILINE_COMMENT>"*"+"/"             { BEGIN(INITIAL); return MULTILINE_COMMENT; }

.                                           { return ERROR_TOKEN; }

%%
