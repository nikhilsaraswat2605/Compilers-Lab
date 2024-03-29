#ifndef _TRANSLATOR_H
#define _TRANSLATOR_H

#include <list>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <string>
#include <map>
#include <vector>
using namespace std;

class SymbolType;
class SymbolTable;
class Symbol;
class Label;
class Quad;
class Expression;
class Array;
class Statement;

// Utility functions
string toString(float);  // returns the string representation of the given float
string toString(char);  // returns the string representation of the given character
string toString(int);  // returns the string representation of the given integer

// Emit functions for generating quads
void emit(string, string, string = "", string = "");  // emits a quad with the given parameters
void emit(string, string, int, string = "");  // emits a quad with the given parameters

// Symbol type class ( type safe representation for the type of a symbol )
class SymbolType {
    public:
        enum typeEnum {VOID, CHAR, INT, FLOAT, POINTER, FUNCTION, ARRAY, BLOCK} type;  // type of the symbol, scoped enum for safe comparisons
        int width;   // width of the symbol
        SymbolType *arrayType;  // type of the array elements

        int getSize();  // returns the size(width) of the symbol
        string toString(); // returns the string representation of the type
        SymbolType(typeEnum, SymbolType * = NULL, int = 1);  // constructor
};

// Symbol class ( represents a single symbol in the symbol table )
class Symbol {
    public:
                         // if it does represent a function the return type will be given by the type attribute
        Symbol *update(SymbolType *);  // updates the symbol with the given type
        Symbol *convert(SymbolType::typeEnum);  // converts the symbol to the given type
        Symbol(string, SymbolType::typeEnum = SymbolType::INT, string = "");  // constructor


        string name;  // name of the symbol
        int size, offset; // size and offset of the symbol
        SymbolType *type;  // type of the symbol
        SymbolTable *nestedTable;  // pointer to the symbol table if it is a nested entry
        string initialValue;  // initial value of the symbol
        bool isFunction; // flag to indicate if the symbol represents a function or not
};

// Expression attributes
class Expression {
    public:
        void toInt();  // converts the expression to an integer
        void toBool();  // converts the expression to a boolean

        Symbol *symbol;  // symbol of the expression
        enum typeEnum {NONBOOLEAN, BOOLEAN} type;  // type of the expression scoped enum
        list<int> trueList, falseList, nextList;  // lists of quad numbers for next, true and false jumps

};


// Array attributes
class Array {
    public:
        Symbol *temp;    // temporary used for computing the offset for the array reference
        enum typeEnum {OTHER, POINTER, ARRAY} type;    // Pointers, arrays and normal expressions are all stored using array attributes initially
        Symbol *symbol;  // pointer to the symbol table entry
        SymbolType *subArrayType;   // type of the sub-array generated by A
};


// Symbol table class
class SymbolTable {
    public:
        
        void print();  // prints the symbol table
        void update(); // updates the symbol table
        SymbolTable(string = "NULL", SymbolTable * = NULL); // constructor
        Symbol *lookup(string); // returns the symbol with the given name, adds new entry if not found
        
        string name;  // name of the symbol table
        map<string, Symbol> symbols;    // list of all symbols in this table, mapped by their names for fast access
        SymbolTable *parent;            // parent symbol table of this symbol table
};


// Quad class ( represents a 3-address quad )
class Quad {
    public:

        void print();  // prints the quad
        Quad(string, int, string = "=", string = ""); // constructor
        Quad(string, string, string = "=", string = "");  // constructor

        string op, arg1, arg2, result;  // parameters of the quad
};


// Statement attributes
class Statement {
    public:
        list<int> nextList;     // List of quads having dangling exits for this statement
};



// Other helper functions

int nextInstruction();  // returns the next instruction number
void changeTable(SymbolTable *);  // changes the current symbol table to the given one
Symbol *gentemp(SymbolType::typeEnum, string = "");  // generates a new temporary symbol

inline bool type_comp(SymbolType *a, SymbolType *b);  // returns true if the two types are compatible
// Type checking and conversions
bool typeCheck(Symbol *&, Symbol *&);  // checks if the two symbols have the same type



// Backpatching functions
void backpatch(list<int>, int);  // backpatches the list of quads with the given address
list<int> merge(list<int>, list<int>); // merges the two lists
list<int> makeList(int);  // makes a list with the given number



// Global variables
extern vector<Quad *> quadArray; // array of quads
extern SymbolTable *currentTable, *globalTable; // current and global symbol tables
extern Symbol *currentSymbol;  // current symbol
extern SymbolType::typeEnum currentType;  // current type
extern int tableCount, temporaryCount; // counters for symbol table and temporary symbols

extern int yyparse();

#endif
