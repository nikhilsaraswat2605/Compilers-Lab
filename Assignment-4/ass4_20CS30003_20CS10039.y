%{
    #include <stdio.h>

    void yyerror(char *);
    extern int lineNum;
    extern int yylex();
%}

%union {
    char *Identifier_Value;
    char *Char_Value;
    char *String_Value;
    float Float_Value;
    int Int_Value;
}

%token AUTO
%token DO
%token DOUBLE
%token ELSE
%token ENUM
%token BREAK
%token CASE
%token CHAR
%token INLINE
%token CONST
%token INT
%token EXTERN
%token FLOAT
%token FOR
%token GOTO
%token IF
%token LONG
%token REGISTER
%token RESTRICT
%token SWITCH
%token TYPEDEF
%token STATIC
%token _IMAGINARY
%token _COMPLEX
%token UNION
%token UNSIGNED
%token RETURN
%token CONTINUE
%token DEFAULT
%token SHORT
%token WHILE
%token _BOOL
%token STRUCT
%token VOID
%token VOLATILE
%token SIGNED
%token SIZEOF



%token ASTERISK
%token PLUS
%token MINUS
%token TILDE
%token EXCLAMATION
%token MODULO
%token RIGHT_SQUARE_BRACKET
%token LEFT_PARENTHESES
%token LEFT_CURLY_BRACKET
%token RIGHT_CURLY_BRACKET
%token DOT
%token COMMA
%token RIGHT_SHIFT
%token LESS_THAN
%token GREATER_THAN
%token LESS_EQUAL_THAN
%token GREATER_EQUAL_THAN
%token QUESTION_MARK
%token ASSIGNMENT
%token SEMI_COLON
%token ELLIPSIS
%token ASTERISK_ASSIGNMENT
%token SLASH_ASSIGNMENT
%token MODULO_ASSIGNMENT
%token COLON
%token MINUS_ASSIGNMENT
%token LEFT_SHIFT_ASSIGNMENT
%token HASH
%token DECREMENT
%token RIGHT_PARENTHESES
%token BITWISE_AND
%token SLASH
%token PLUS_ASSIGNMENT
%token BITWISE_XOR
%token BITWISE_OR
%token LOGICAL_AND
%token LEFT_SQUARE_BRACKET
%token INCREMENT
%token BITWISE_AND_ASSIGNMENT
%token BITWISE_OR_ASSIGNMENT
%token BITWISE_XOR_ASSIGNMENT
%token LOGICAL_OR
%token RIGHT_SHIFT_ASSIGNMENT
%token NOT_EQUALS
%token EQUALS
%token ARROW
%token LEFT_SHIFT

%token ERROR_TOKEN


%token<String_Value> STRING_LITERAL
%token<Char_Value> CHARACTER_CONST
%token<Int_Value> INTEGER_CONST
%token<Float_Value> FLOATING_CONST
%token<String_Value> IDENTIFIER

%nonassoc RIGHT_PARENTHESES
%nonassoc ELSE

%start translation_unit

%%



AND_expression:
                AND_expression BITWISE_AND equality_expression
                    { printf("[Line No. %d] :: AND_expression ---> AND_expression & equality_expression\n",lineNum); }
                |equality_expression
                    { printf("[Line No. %d] :: AND_expression ---> equality_expression\n",lineNum); }
                ;
relational_expression:
                         relational_expression GREATER_EQUAL_THAN shift_expression
                            { printf("[Line No. %d] :: relational_expression ---> relational_expression >= shift_expression\n",lineNum); }
                        | relational_expression LESS_EQUAL_THAN shift_expression
                            { printf("[Line No. %d] :: relational_expression ---> relational_expression <= shift_expression\n",lineNum); }
                        | relational_expression LESS_THAN shift_expression
                            { printf("[Line No. %d] :: relational_expression ---> relational_expression < shift_expression\n",lineNum); }
                        | relational_expression GREATER_THAN shift_expression
                            { printf("[Line No. %d] :: relational_expression ---> relational_expression > shift_expression\n",lineNum); }
                        |shift_expression
                            { printf("[Line No. %d] :: relational_expression ---> shift_expression\n",lineNum); }
                        ;

equality_expression:
                    equality_expression NOT_EQUALS relational_expression
                        { printf("[Line No. %d] :: equality_expression ---> equality_expression != relational_expression\n",lineNum); }
                    |relational_expression
                        { printf("[Line No. %d] :: equality_expression ---> relational_expression\n",lineNum); }
                    | equality_expression EQUALS relational_expression
                        { printf("[Line No. %d] :: equality_expression ---> equality_expression == relational_expression\n",lineNum); }
                    ;
cast_expression:
                LEFT_PARENTHESES type_name RIGHT_PARENTHESES cast_expression
                    { printf("[Line No. %d] :: cast_expression ---> ( type_name ) cast_expression\n",lineNum); }
                |unary_expression
                    { printf("[Line No. %d] :: cast_expression ---> unary_expression\n",lineNum); }
                ;

exclusive_OR_expression:
                        exclusive_OR_expression BITWISE_XOR AND_expression
                            { printf("[Line No. %d] :: exclusive_OR_expression ---> exclusive_OR_expression ^ AND_expression\n",lineNum); }
                        |AND_expression
                            { printf("[Line No. %d] :: exclusive_OR_expression ---> AND_expression\n",lineNum); }
                        ;



additive_expression:
                    additive_expression MINUS multiplicative_expression
                        { printf("[Line No. %d] :: additive_expression ---> additive_expression - multiplicative_expression\n",lineNum); }
                    |multiplicative_expression
                        { printf("[Line No. %d] :: additive_expression ---> multiplicative_expression\n",lineNum); }
                    | additive_expression PLUS multiplicative_expression
                        { printf("[Line No. %d] :: additive_expression ---> additive_expression + multiplicative_expression\n",lineNum); }
                    ;

multiplicative_expression:
                            multiplicative_expression MODULO cast_expression
                                { printf("[Line No. %d] :: multiplicative_expression ---> multiplicative_expression %% cast_expression\n",lineNum); }
                            | multiplicative_expression SLASH cast_expression
                                { printf("[Line No. %d] :: multiplicative_expression ---> multiplicative_expression / cast_expression\n",lineNum); }
                            | multiplicative_expression ASTERISK cast_expression
                                { printf("[Line No. %d] :: multiplicative_expression ---> multiplicative_expression * cast_expression\n",lineNum); }
                            |cast_expression
                                { printf("[Line No. %d] :: multiplicative_expression ---> cast_expression\n",lineNum); }
                            ;


assignment_expression:
                        unary_expression assignment_operator assignment_expression
                            { printf("[Line No. %d] :: assignment_expression ---> unary_expression assignment_operator assignment_expression\n",lineNum); }
                        |conditional_expression
                            { printf("[Line No. %d] :: assignment_expression ---> conditional_expression\n",lineNum); }
                        ;

shift_expression:
                     shift_expression RIGHT_SHIFT additive_expression
                        { printf("[Line No. %d] :: shift_expression ---> shift_expression >> additive_expression\n",lineNum); }
                    |additive_expression
                        { printf("[Line No. %d] :: shift_expression ---> additive_expression\n",lineNum); }
                    | shift_expression LEFT_SHIFT additive_expression
                        { printf("[Line No. %d] :: shift_expression ---> shift_expression << additive_expression\n",lineNum); }
                    ;
expression:
            expression COMMA assignment_expression
                { printf("[Line No. %d] :: expression ---> expression , assignment_expression\n",lineNum); }
            |assignment_expression
                { printf("[Line No. %d] :: expression ---> assignment_expression\n",lineNum); }
            ;

assignment_operator:
                    RIGHT_SHIFT_ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> >>=\n",lineNum); }
                    | BITWISE_AND_ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> &=\n",lineNum); }
                    | BITWISE_XOR_ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> ^=\n",lineNum); }
                    | BITWISE_OR_ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> |=\n",lineNum); }
                    |ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> =\n",lineNum); }
                    | MINUS_ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> -= \n",lineNum); }
                    | LEFT_SHIFT_ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> <<=\n",lineNum); }
                    | ASTERISK_ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> *=\n",lineNum); }
                    | SLASH_ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> /=\n",lineNum); }
                    | MODULO_ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> %%=\n",lineNum); }
                    | PLUS_ASSIGNMENT
                        { printf("[Line No. %d] :: assignment_operator ---> += \n",lineNum); }
                    ;


inclusive_OR_expression:
                        inclusive_OR_expression BITWISE_OR exclusive_OR_expression
                            { printf("[Line No. %d] :: inclusive_OR_expression ---> inclusive_OR_expression | exclusive_OR_expression\n",lineNum); }
                        |exclusive_OR_expression
                            { printf("[Line No. %d] :: inclusive_OR_expression ---> exclusive_OR_expression\n",lineNum); }
                        ;

constant_expression:
                    conditional_expression
                        { printf("[Line No. %d] :: constant_expression ---> conditional_expression\n",lineNum); }
                    ;
logical_OR_expression:
                        logical_OR_expression LOGICAL_OR logical_AND_expression
                            { printf("[Line No. %d] :: logical_OR_expression ---> logical_OR_expression || logical_AND_expression\n",lineNum); }
                        |logical_AND_expression
                            { printf("[Line No. %d] :: logical_OR_expression ---> logical_AND_expression\n",lineNum); }
                        ;

logical_AND_expression:
                        logical_AND_expression LOGICAL_AND inclusive_OR_expression
                            { printf("[Line No. %d] :: logical_AND_expression ---> logical_AND_expression && inclusive_OR_expression\n",lineNum); }
                        |inclusive_OR_expression
                            { printf("[Line No. %d] :: logical_AND_expression ---> inclusive_OR_expression\n",lineNum); }
                        ;


postfix_expression:
                    postfix_expression LEFT_SQUARE_BRACKET expression RIGHT_SQUARE_BRACKET
                        { printf("[Line No. %d] :: postfix_expression ---> postfix_expression [ expression ]\n",lineNum); }
                    | LEFT_PARENTHESES type_name RIGHT_PARENTHESES LEFT_CURLY_BRACKET initialiser_list COMMA RIGHT_CURLY_BRACKET
                        { printf("[Line No. %d] :: postfix_expression ---> ( type_name ) { initialiser_list , }\n",lineNum); }
                    | LEFT_PARENTHESES type_name RIGHT_PARENTHESES LEFT_CURLY_BRACKET initialiser_list RIGHT_CURLY_BRACKET
                        { printf("[Line No. %d] :: postfix_expression ---> ( type_name ) { initialiser_list }\n",lineNum); }
                    | postfix_expression ARROW IDENTIFIER
                        { printf("[Line No. %d] :: postfix_expression ---> postfix_expression -> IDENTIFIER\n",lineNum);
                        printf("\t\t\t\tIDENTIFIER = %s\n", $3); }
                    | postfix_expression INCREMENT
                        { printf("[Line No. %d] :: postfix_expression ---> postfix_expression ++\n",lineNum); }
                    | postfix_expression DOT IDENTIFIER
                        { printf("[Line No. %d] :: postfix_expression ---> postfix_expression . IDENTIFIER\n",lineNum);
                        printf("\t\t\t\tIDENTIFIER = %s\n", $3); }
                    | postfix_expression DECREMENT
                        { printf("[Line No. %d] :: postfix_expression ---> postfix_expression --\n",lineNum); }
                    | postfix_expression LEFT_PARENTHESES argument_expression_list_opt RIGHT_PARENTHESES
                        { printf("[Line No. %d] :: postfix_expression ---> postfix_expression ( argument_expression_list_opt )\n",lineNum); }
                    |primary_expression
                        { printf("[Line No. %d] :: postfix_expression ---> primary_expression\n",lineNum); }
                    ;


conditional_expression:
                        logical_OR_expression QUESTION_MARK expression COLON conditional_expression
                            { printf("[Line No. %d] :: conditional_expression ---> logical_OR_expression ? expression : conditional_expression\n",lineNum); }
                        |logical_OR_expression
                            { printf("[Line No. %d] :: conditional_expression ---> logical_OR_expression\n",lineNum); }
                        ;
argument_expression_list:
                            argument_expression_list COMMA assignment_expression
                                { printf("[Line No. %d] :: argument_expression_list ---> argument_expression_list , assignment_expression\n",lineNum); }
                            |assignment_expression
                                { printf("[Line No. %d] :: argument_expression_list ---> assignment_expression\n",lineNum); }
                            ;

argument_expression_list_opt:
                                
                                    { printf("[Line No. %d] :: argument_expression_list_opt ---> epsilon\n",lineNum); }
                                |argument_expression_list
                                    { printf("[Line No. %d] :: argument_expression_list_opt ---> argument_expression_list\n",lineNum); }
                                ;
unary_operator:
                EXCLAMATION
                    { printf("[Line No. %d] :: unary_operator ---> !\n",lineNum); }
                | TILDE
                    { printf("[Line No. %d] :: unary_operator ---> ~\n",lineNum); }
                | MINUS
                    { printf("[Line No. %d] :: unary_operator ---> -\n",lineNum); }
                | PLUS
                    { printf("[Line No. %d] :: unary_operator ---> +\n",lineNum); }
                | ASTERISK
                    { printf("[Line No. %d] :: unary_operator ---> *\n",lineNum); }
                |BITWISE_AND
                    { printf("[Line No. %d] :: unary_operator ---> &\n",lineNum); }
                ;


unary_expression:
                     SIZEOF LEFT_PARENTHESES type_name RIGHT_PARENTHESES
                        { printf("[Line No. %d] :: unary_expression ---> sizeof ( type_name )\n",lineNum); }
                    | SIZEOF unary_expression
                        { printf("[Line No. %d] :: unary_expression ---> sizeof unary_expression\n",lineNum); }
                    | DECREMENT unary_expression
                        { printf("[Line No. %d] :: unary_expression ---> -- unary_expression\n",lineNum); }
                    | INCREMENT unary_expression
                        { printf("[Line No. %d] :: unary_expression ---> ++ unary_expression\n",lineNum); }
                    | unary_operator cast_expression
                        { printf("[Line No. %d] :: unary_expression ---> unary_operator cast_expression\n",lineNum); }
                    |postfix_expression
                        { printf("[Line No. %d] :: unary_expression ---> postfix_expression\n",lineNum); }
                    ;


/* External definitions */


function_definition:
                    declaration_specifiers declarator declaration_list_opt compound_statement
                        { printf("[Line No. %d] :: function_definition ---> declaration_specifiers declarator declaration_list_opt compound_statement\n",lineNum); }
                    ;

declaration_list:
                     declaration_list declaration
                        { printf("[Line No. %d] :: declaration_list ---> declaration_list declaration\n",lineNum); }
                    | declaration
                        { printf("[Line No. %d] :: declaration_list ---> declaration\n",lineNum); }
                    ;

primary_expression: 
                    LEFT_PARENTHESES expression RIGHT_PARENTHESES
                        { printf("[Line No. %d] :: primary_expression ---> ( expression )\n",lineNum); }
                    | CHARACTER_CONST 
                        { printf("[Line No. %d] :: primary_expression ---> CHARACTER_CONSTANT\n",lineNum);
                        printf("\t\t\t\tCHARACTER_CONSTANT = %s\n", $1); }
                    | STRING_LITERAL 
                        { printf("[Line No. %d] :: primary_expression ---> STRING_LITERAL\n",lineNum);
                        printf("\t\t\t\tSTRING_LITERAL = %s\n", $1); }
                    | IDENTIFIER 
                        { printf("[Line No. %d] :: primary_expression ---> IDENTIFIER\n",lineNum);
                        printf("\t\t\t\tIDENTIFIER = %s\n", $1); }
                    | INTEGER_CONST 
                        { printf("[Line No. %d] :: primary_expression ---> INTEGER_CONSTANT\n",lineNum);
                        printf("\t\t\t\tINTEGER_CONSTANT = %d\n", $1); }
                    | FLOATING_CONST 
                        { printf("[Line No. %d] :: primary_expression ---> FLOATING_CONSTANT\n",lineNum);
                        printf("\t\t\t\tFLOATING_CONSTANT = %f\n", $1); }
                    ;
external_declaration:
                        declaration
                            { printf("[Line No. %d] :: external_declaration ---> declaration\n",lineNum); }
                        | function_definition
                            { printf("[Line No. %d] :: external_declaration ---> function_definition\n",lineNum); }
                        ;


declaration_list_opt:
                        
                            { printf("[Line No. %d] :: declaration_list_opt ---> epsilon\n",lineNum); }
                        | declaration_list
                            { printf("[Line No. %d] :: declaration_list_opt ---> declaration_list\n",lineNum); }
                        ;
translation_unit:
                    translation_unit external_declaration
                        { printf("[Line No. %d] :: translation_unit ---> translation_unit external_declaration\n",lineNum); }
                    | external_declaration
                        { printf("[Line No. %d] :: translation_unit ---> external_declaration\n",lineNum); }
                    ;

/* Statements */



compound_statement:
                    LEFT_CURLY_BRACKET block_item_list_opt RIGHT_CURLY_BRACKET
                        { printf("[Line No. %d] :: compound_statement ---> { block_item_list_opt }\n",lineNum); }
                    ;

block_item_list_opt:
                    
                        { printf("[Line No. %d] :: block_item_list_opt ---> epsilon\n",lineNum); }
                    | block_item_list
                        { printf("[Line No. %d] :: block_item_list_opt ---> block_item_list\n",lineNum); }
                    ;

block_item_list:
                block_item_list block_item
                    { printf("[Line No. %d] :: block_item_list ---> block_item_list block_item\n",lineNum); }
                | block_item
                    { printf("[Line No. %d] :: block_item_list ---> block_item\n",lineNum); }
                ;

expression_opt:
                
                    { printf("[Line No. %d] :: expression_opt ---> epsilon\n",lineNum); }
                | expression
                    { printf("[Line No. %d] :: expression_opt ---> expression\n",lineNum); }
                ;

labeled_statement:
                    CASE constant_expression COLON statement
                        { printf("[Line No. %d] :: labeled_statement ---> case constant_expression : statement\n",lineNum); }    
                    | IDENTIFIER COLON statement
                        { printf("[Line No. %d] :: labeled_statement ---> IDENTIFIER : statement\n",lineNum);
                        printf("\t\t\t\tIDENTIFIER = %s\n", $1); }
                    | DEFAULT COLON statement
                        { printf("[Line No. %d] :: labeled_statement ---> default : statement\n",lineNum); }
                    ;
selection_statement:
                    SWITCH LEFT_PARENTHESES expression RIGHT_PARENTHESES statement
                        { printf("[Line No. %d] :: selection_statement ---> switch ( expression ) statement\n",lineNum); }
                    | IF LEFT_PARENTHESES expression RIGHT_PARENTHESES statement ELSE statement
                        { printf("[Line No. %d] :: selection_statement ---> if ( expression ) statement else statement\n",lineNum); }
                    | IF LEFT_PARENTHESES expression RIGHT_PARENTHESES statement
                        { printf("[Line No. %d] :: selection_statement ---> if ( expression ) statement\n",lineNum); }
                    ;

iteration_statement:
                    FOR LEFT_PARENTHESES declaration expression_opt SEMI_COLON expression_opt RIGHT_PARENTHESES statement
                        { printf("[Line No. %d] :: iteration_statement ---> for ( declaration expression_opt ; expression_opt ) statement\n",lineNum); }
                    | FOR LEFT_PARENTHESES expression_opt SEMI_COLON expression_opt SEMI_COLON expression_opt RIGHT_PARENTHESES statement
                        { printf("[Line No. %d] :: iteration_statement ---> for ( expression_opt ; expression_opt ; expression_opt ) statement\n",lineNum); }
                    | WHILE LEFT_PARENTHESES expression RIGHT_PARENTHESES statement
                        { printf("[Line No. %d] :: iteration_statement ---> while ( expression ) statement\n",lineNum); }
                    | DO statement WHILE LEFT_PARENTHESES expression RIGHT_PARENTHESES SEMI_COLON
                        { printf("[Line No. %d] :: iteration_statement ---> do statement while ( expression ) ;\n",lineNum); }
                    ;
statement:
            jump_statement
                { printf("[Line No. %d] :: statement ---> jump_statement\n",lineNum); }
            | expression_statement
                { printf("[Line No. %d] :: statement ---> expression_statement\n",lineNum); }
            | iteration_statement
                { printf("[Line No. %d] :: statement ---> iteration_statement\n",lineNum); }
            | compound_statement
                { printf("[Line No. %d] :: statement ---> compound_statement\n",lineNum); }
            | labeled_statement
                { printf("[Line No. %d] :: statement ---> labeled_statement\n",lineNum); }
            | selection_statement
                { printf("[Line No. %d] :: statement ---> selection_statement\n",lineNum); }
            ;
jump_statement:
                CONTINUE SEMI_COLON
                    { printf("[Line No. %d] :: jump_statement ---> continue ;\n",lineNum); }
                | BREAK SEMI_COLON
                    { printf("[Line No. %d] :: jump_statement ---> break ;\n",lineNum); }
                | GOTO IDENTIFIER SEMI_COLON
                    { printf("[Line No. %d] :: jump_statement ---> goto IDENTIFIER ;\n",lineNum);
                    printf("\t\t\t\tIDENTIFIER = %s\n", $2); }    
                | RETURN expression_opt SEMI_COLON
                    { printf("[Line No. %d] :: jump_statement ---> return expression_opt ;\n",lineNum); }
                ;
block_item:
            statement
                { printf("[Line No. %d] :: block_item ---> statement\n",lineNum); }
            | declaration
                { printf("[Line No. %d] :: block_item ---> declaration\n",lineNum); }
            ;

expression_statement:
                        expression_opt SEMI_COLON
                            { printf("[Line No. %d] :: expression_statement ---> expression_opt ;\n",lineNum); }
                        ;



initialiser_list:
                    initialiser_list COMMA designation_opt initialiser
                        { printf("[Line No. %d] :: initialiser_list ---> initialiser_list , designation_opt initialiser\n",lineNum); }
                    | designation_opt initialiser
                        { printf("[Line No. %d] :: initialiser_list ---> designation_opt initialiser\n",lineNum); }
                    ;

designation_opt:
                
                    { printf("[Line No. %d] :: designation_opt ---> epsilon\n",lineNum); }
                | designation
                    { printf("[Line No. %d] :: designation_opt ---> designation\n",lineNum); }
                ;

            ;
designator_list:
                designator_list designator
                    { printf("[Line No. %d] :: designator_list ---> designator_list designator\n",lineNum); }
                | designator
                    { printf("[Line No. %d] :: designator_list ---> designator\n",lineNum); }
                ;

designator:
            DOT IDENTIFIER
                { printf("[Line No. %d] :: designator ---> . IDENTIFIER\n",lineNum);
                printf("\t\t\t\tIDENTIFIER = %s\n", $2); }   
            | LEFT_SQUARE_BRACKET constant_expression RIGHT_SQUARE_BRACKET
                { printf("[Line No. %d] :: designator ---> [ constant_expression ]\n",lineNum); }

parameter_declaration:
                        declaration_specifiers declarator
                            { printf("[Line No. %d] :: parameter_declaration ---> declaration_specifiers declarator\n",lineNum); }
                        | declaration_specifiers
                            { printf("[Line No. %d] :: parameter_declaration ---> declaration_specifiers\n",lineNum); }
                        ;
identifier_list_opt:
                    
                        { printf("[Line No. %d] :: identifier_list_opt ---> epsilon\n",lineNum); }
                    | identifier_list
                        { printf("[Line No. %d] :: identifier_list_opt ---> identifier_list\n",lineNum); }
                    ;

pointer:
        ASTERISK type_qualifier_list_opt pointer
            { printf("[Line No. %d] :: pointer ---> * type_qualifier_list_opt pointer\n",lineNum); }
        | ASTERISK type_qualifier_list_opt
            { printf("[Line No. %d] :: pointer ---> * type_qualifier_list_opt\n",lineNum); }
        ;

direct_declarator:
                    direct_declarator LEFT_SQUARE_BRACKET type_qualifier_list_opt assignment_expression_opt RIGHT_SQUARE_BRACKET
                        { printf("[Line No. %d] :: direct_declarator ---> direct_declarator [ type_qualifier_list_opt assignment_expression_opt ]\n",lineNum); }
                    | direct_declarator LEFT_SQUARE_BRACKET type_qualifier_list_opt ASTERISK RIGHT_SQUARE_BRACKET
                        { printf("[Line No. %d] :: direct_declarator ---> direct_declarator [ type_qualifier_list_opt * ]\n",lineNum); }
                    | direct_declarator LEFT_PARENTHESES identifier_list_opt RIGHT_PARENTHESES
                        { printf("[Line No. %d] :: direct_declarator ---> direct_declarator ( identifier_list_opt )\n",lineNum); }
                    | IDENTIFIER 
                        { printf("[Line No. %d] :: direct_declarator ---> IDENTIFIER\n",lineNum);
                        printf("\t\t\t\tIDENTIFIER = %s\n", $1); }
                    | direct_declarator LEFT_PARENTHESES parameter_type_list RIGHT_PARENTHESES
                        { printf("[Line No. %d] :: direct_declarator ---> direct_declarator ( parameter_type_list )\n",lineNum); }
                    | LEFT_PARENTHESES declarator RIGHT_PARENTHESES
                        { printf("[Line No. %d] :: direct_declarator ---> ( declarator )\n",lineNum); }
                    | direct_declarator LEFT_SQUARE_BRACKET type_qualifier_list STATIC assignment_expression RIGHT_SQUARE_BRACKET
                        { printf("[Line No. %d] :: direct_declarator ---> direct_declarator [ type_qualifier_list static assignment_expression ]\n",lineNum); }
                    | direct_declarator LEFT_SQUARE_BRACKET STATIC type_qualifier_list_opt assignment_expression RIGHT_SQUARE_BRACKET
                        { printf("[Line No. %d] :: direct_declarator ---> direct_declarator [ static type_qualifier_list_opt assignment_expression ]\n",lineNum); }
                    ;
type_qualifier_list:
                    type_qualifier_list type_qualifier
                        { printf("[Line No. %d] :: type_qualifier_list ---> type_qualifier_list type_qualifier\n",lineNum); }
                    | type_qualifier
                        { printf("[Line No. %d] :: type_qualifier_list ---> type_qualifier\n",lineNum); }
                    ;
type_qualifier_list_opt:
                        
                            { printf("[Line No. %d] :: type_qualifier_list_opt ---> epsilon\n",lineNum); }
                        | type_qualifier_list
                            { printf("[Line No. %d] :: type_qualifier_list_opt ---> type_qualifier_list\n",lineNum); }
                        ;

assignment_expression_opt:
                            
                                { printf("[Line No. %d] :: assignment_expression_opt ---> epsilon\n",lineNum); }
                            | assignment_expression
                                { printf("[Line No. %d] :: assignment_expression_opt ---> assignment_expression\n",lineNum); }
                            ;

parameter_type_list:
                    parameter_list COMMA ELLIPSIS
                        { printf("[Line No. %d] :: parameter_type_list ---> parameter_list , ...\n",lineNum); }
                    | parameter_list
                        { printf("[Line No. %d] :: parameter_type_list ---> parameter_list\n",lineNum); }
                    ;

parameter_list:
                parameter_list COMMA parameter_declaration
                    { printf("[Line No. %d] :: parameter_list ---> parameter_list , parameter_declaration\n",lineNum); }
                | parameter_declaration
                    { printf("[Line No. %d] :: parameter_list ---> parameter_declaration\n",lineNum); }
                ;
designation:
            designator_list ASSIGNMENT
                { printf("[Line No. %d] :: designation ---> designator_list =\n",lineNum); }
            ;

identifier_list:
                identifier_list COMMA IDENTIFIER
                    { printf("[Line No. %d] :: identifier_list ---> identifier_list , IDENTIFIER\n",lineNum);
                    printf("\t\t\t\tIDENTIFIER = %s\n", $3); }
                | IDENTIFIER 
                    { printf("[Line No. %d] :: identifier_list ---> IDENTIFIER\n",lineNum);
                    printf("\t\t\t\tIDENTIFIER = %s\n", $1); }
                ;

type_name:
            specifier_qualifier_list
                { printf("[Line No. %d] :: type_name ---> specifier_qualifier_list\n",lineNum); }
            ;

initialiser:
            LEFT_CURLY_BRACKET initialiser_list COMMA RIGHT_CURLY_BRACKET
                { printf("[Line No. %d] :: initialiser ---> { initialiser_list , }\n",lineNum); }
            | LEFT_CURLY_BRACKET initialiser_list RIGHT_CURLY_BRACKET
                { printf("[Line No. %d] :: initialiser ---> { initialiser_list }\n",lineNum );}  
            | assignment_expression
                { printf("[Line No. %d] :: initialiser ---> assignment_expression\n",lineNum); }
            ;


/* Declarations */

declaration_specifiers_opt:
                            
                                { printf("[Line No. %d] :: declaration_specifiers_opt ---> epsilon \n",lineNum); }
                            |declaration_specifiers
                                { printf("[Line No. %d] :: declaration_specifiers_opt ---> declaration_specifiers\n",lineNum); }
                            ;


declaration_specifiers:
                        function_specifier declaration_specifiers_opt
                            { printf("[Line No. %d] :: declaration_specifiers ---> function_specifier declaration_specifiers_opt\n",lineNum); }
                        | type_specifier declaration_specifiers_opt
                            { printf("[Line No. %d] :: declaration_specifiers ---> type_specifier declaration_specifiers_opt\n",lineNum); }
                        |storage_class_specifier declaration_specifiers_opt
                            { printf("[Line No. %d] :: declaration_specifiers ---> storage_class_specifier declaration_specifiers_opt\n",lineNum); }
                        | type_qualifier declaration_specifiers_opt
                            { printf("[Line No. %d] :: declaration_specifiers ---> type_qualifier declaration_specifiers_opt\n",lineNum); }
                        ;
init_declarator:
                declarator ASSIGNMENT initialiser
                    { printf("[Line No. %d] :: init_declarator ---> declarator = initialiser\n",lineNum); }
                |declarator
                    { printf("[Line No. %d] :: init_declarator ---> declarator\n",lineNum); }
                ;


enum_specifier:
                 ENUM IDENTIFIER
                    { printf("[Line No. %d] :: enum_specifier ---> enum IDENTIFIER\n",lineNum);
                    printf("\t\t\t\tIDENTIFIER = %s\n", $2); }
                | ENUM identifier_opt LEFT_CURLY_BRACKET enumerator_list COMMA RIGHT_CURLY_BRACKET
                    { printf("[Line No. %d] :: enum_specifier ---> enum identifier_opt { enumerator_list , }\n",lineNum); }
                |ENUM identifier_opt LEFT_CURLY_BRACKET enumerator_list RIGHT_CURLY_BRACKET 
                    { printf("[Line No. %d] :: enum_specifier ---> enum identifier_opt { enumerator_list }\n",lineNum); }
                ;


init_declarator_list:
                         init_declarator_list COMMA init_declarator
                            { printf("[Line No. %d] :: init_declarator_list ---> init_declarator_list , init_declarator\n",lineNum); }
                        |init_declarator
                            { printf("[Line No. %d] :: init_declarator_list ---> init_declarator\n",lineNum); }
                        ;

init_declarator_list_opt:
                            
                                { printf("[Line No. %d] :: init_declarator_list_opt ---> epsilon\n",lineNum); }
                            |init_declarator_list
                                { printf("[Line No. %d] :: init_declarator_list_opt ---> init_declarator_list\n",lineNum); }
                            ;

declaration:
            declaration_specifiers init_declarator_list_opt SEMI_COLON
                { printf("[Line No. %d] :: declaration ---> declaration_specifiers init_declarator_list_opt ;\n",lineNum); }
            ;

identifier_opt:
                 
                    { printf("[Line No. %d] :: identifier_opt ---> epsilon\n",lineNum); }
                |IDENTIFIER 
                    { printf("[Line No. %d] :: identifier_opt ---> IDENTIFIER\n",lineNum);
                    printf("\t\t\t\tIDENTIFIER = %s\n", $1); }
                ;

enumerator_list:
                 enumerator_list COMMA enumerator
                    { printf("[Line No. %d] :: enumerator_list ---> enumerator_list , enumerator\n",lineNum); }
                |enumerator 
                    { printf("[Line No. %d] :: enumerator_list ---> enumerator\n",lineNum); }
                ;


type_qualifier:
                VOLATILE
                    { printf("[Line No. %d] :: type_qualifier ---> volatile\n",lineNum); }
                | RESTRICT
                    { printf("[Line No. %d] :: type_qualifier ---> restrict\n",lineNum); }
                |CONST
                    { printf("[Line No. %d] :: type_qualifier ---> const\n",lineNum); }
                ;

function_specifier:
                    INLINE
                        { printf("[Line No. %d] :: function_specifier ---> inline\n",lineNum); }
                    ;
enumerator:
             IDENTIFIER ASSIGNMENT constant_expression
                { printf("[Line No. %d] :: enumerator ---> ENUMERATION_CONSTANT = constant_expression\n",lineNum);
                printf("\t\t\t\tENUMERATION_CONSTANT = %s\n", $1); }
            |IDENTIFIER 
                { printf("[Line No. %d] :: enumerator ---> ENUMERATION_CONSTANT\n",lineNum);
                printf("\t\t\t\tENUMERATION_CONSTANT = %s\n", $1); }
            ;


declarator:
            pointer_opt direct_declarator
                { printf("[Line No. %d] :: declarator ---> pointer_opt direct_declarator\n",lineNum); }
            ;

pointer_opt:
            
                { printf("[Line No. %d] :: pointer_opt ---> epsilon\n",lineNum); }
            |pointer
                { printf("[Line No. %d] :: pointer_opt ---> pointer\n",lineNum); }
            ;

type_specifier:
                 _IMAGINARY
                    { printf("[Line No. %d] :: type_specifier ---> _Imaginary\n",lineNum); }
                | enum_specifier 
                    { printf("[Line No. %d] :: type_specifier ---> enum_specifier\n",lineNum); }
                | _BOOL
                    { printf("[Line No. %d] :: type_specifier ---> _Bool\n",lineNum); }
                | _COMPLEX
                    { printf("[Line No. %d] :: type_specifier ---> _Complex\n",lineNum); }
                | SIGNED
                    { printf("[Line No. %d] :: type_specifier ---> signed\n",lineNum); }
                | UNSIGNED
                    { printf("[Line No. %d] :: type_specifier ---> unsigned\n",lineNum); }
                | DOUBLE
                    { printf("[Line No. %d] :: type_specifier ---> double\n",lineNum); }
                | LONG
                    { printf("[Line No. %d] :: type_specifier ---> long\n",lineNum); }
                | FLOAT
                    { printf("[Line No. %d] :: type_specifier ---> float\n",lineNum); }
                | SHORT
                    { printf("[Line No. %d] :: type_specifier ---> short\n",lineNum); }
                | INT
                    { printf("[Line No. %d] :: type_specifier ---> int\n",lineNum); }
                |VOID
                    { printf("[Line No. %d] :: type_specifier ---> void\n",lineNum); }
                | CHAR
                    { printf("[Line No. %d] :: type_specifier ---> char\n",lineNum); }
                ;

storage_class_specifier://
                        EXTERN
                            { printf("[Line No. %d] :: storage_class_specifier ---> extern\n",lineNum); }
                        | STATIC
                            { printf("[Line No. %d] :: storage_class_specifier ---> static\n",lineNum); }
                        | AUTO
                            { printf("[Line No. %d] :: storage_class_specifier ---> auto\n",lineNum); }
                        | REGISTER
                            { printf("[Line No. %d] :: storage_class_specifier ---> register\n",lineNum); }
                        ;
specifier_qualifier_list_opt:
                                 
                                    { printf("[Line No. %d] :: specifier_qualifier_list_opt ---> epsilon\n",lineNum); }
                                |specifier_qualifier_list
                                    { printf("[Line No. %d] :: specifier_qualifier_list_opt ---> specifier_qualifier_list\n",lineNum); }
                                ;


specifier_qualifier_list:
                            type_qualifier specifier_qualifier_list_opt
                                { printf("[Line No. %d] :: specifier_qualifier_list ---> type_qualifier specifier_qualifier_list_opt\n",lineNum); }
                            |type_specifier specifier_qualifier_list_opt
                                { printf("[Line No. %d] :: specifier_qualifier_list ---> type_specifier specifier_qualifier_list_opt\n",lineNum); }
                            ;

%%

void yyerror(char* s) {
    printf("[Line No. %d] :: %s\n", lineNum, s);
}

