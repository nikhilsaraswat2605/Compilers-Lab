#include "ass5_20CS30003_20CS10039_translator.h"

// Global Variables
vector<Quad *> quadArray;  // Quad Array to store the quads
SymbolTable *currentTable, *globalTable, *parentTable;  // Symbol Tables for the current scope, global scope and parent scope
Symbol *currentSymbol;  // Current Symbol 
SymbolType::typeEnum currentType;  // Current Type
int tableCount, temporaryCount;  // Counts of number of tables and number of temps generated

// utility functions
// Implementation of utility functions
// overloaded toString function to maintain semantic consistency 
// convert int to string
string toString(int i)
{
    return to_string(i); // convert int to string
}
// converts float to string
string toString(float f)
{
    return to_string(f); // convert float to string
}
// converts char to string
string toString(char c)
{
    return string(1, c); // convert char to string
}


// change current table to specified table
void changeTable(SymbolTable *table) 
{
    currentTable = table; 
}

// code to check if a and b are of the same type, promotes to the higher type if feasible and if that makes the type of both the same
inline bool type_comp(SymbolType *a, SymbolType *b)
{
    if(!a and !b){ // if both are null pointers
        return true;
    }else if (!a or !b or a->type != b->type){ // if either of them is null pointer or they are of different types
        return false;
    }else{ 
        return type_comp(a->arrayType, b->arrayType); // recursively check for array types
    }
}

bool typeCheck(Symbol *&a, Symbol *&b){ // check if the types of a and b are the same
    // if the types are same return true
    if(type_comp(a->type, b->type)) // if the types are same
        return true;
    // if the types are not same but can be cast safely according the rules, then cast and return
    else if(a->type->type == SymbolType::FLOAT or b->type->type == SymbolType::FLOAT) { // if either of them is float
        if(a->type->type == SymbolType::FLOAT){ // if a is float 
            b = b->convert(SymbolType::FLOAT); // convert b to float
        }else{ // if b is float
            a = a->convert(SymbolType::FLOAT); // convert a to float
        }
        return true;
    }
    else if(a->type->type == SymbolType::INT or b->type->type == SymbolType::INT) { // if either of them is int
        if(a->type->type == SymbolType::INT) // if a is int
            b = b->convert(SymbolType::INT); // convert b to int
        else    // if b is int
            a = a->convert(SymbolType::INT); // convert a to int
        return true;
    }else{ // if the types are not same and cannot be cast safely
        return false;
    }
}

// Implementation of other helper functions
int nextInstruction(){// returns the next instruction number
    return quadArray.size() + 1; 
}



list<int> makeList(int base){ // returns list with the base address as its only value
    return {base};
}

list<int> merge(list<int> first, list<int> second){// merge two lists
    first.insert(first.end(), second.begin(), second.end()); // insert second list at the end of first list
    return first;
}

// Implementation of emit funtions
void emit(string op, string result, string arg1, string arg2){ // emit quad with 3 arguments 
    Quad *q = new Quad(result, arg1, op, arg2); // create a new quad with the given arguments
    quadArray.push_back(q); // push the quad into the quad array
}

void emit(string op, string result, int arg1, string arg2){ // emit quad with 3 arguments (one int)
    Quad *q = new Quad(result, arg1, op, arg2); // create a new quad with the given arguments
    quadArray.push_back(q); // push the quad into the quad array
}

// Implementation of backpatching functions
void backpatch(list<int> list_, int addr){// for all the addresses in the list, add the target address 
    for (auto &i : list_){
        quadArray[i-1]->result = toString(addr); // set the result of the quad at the address to the target address
    }
}

Symbol *SymbolTable::lookup(string name){ // lookup a symbol in the symbol table

    // If the symbol is present in the symbols, return it
    if (symbols.find(name) != symbols.end()){ // if the symbol is present in the symbol table
        auto it = (this)->symbols.find(name); // find the symbol in the symbol table
        // return &symbols[name];
        return &(it->second); // return the symbol
    }
    else if(parent && parent->lookup(name) != NULL) // if the symbol is not present in the symbol table but is present in the parent symbol table
        return parent->lookup(name); // return the symbol from the parent symbol table
    else if(this == currentTable) { // if the symbol is not present in the symbol table and is not present in the parent symbol table but is present in the current symbol table
        // insert in the symbols
        this->symbols.insert({name, *(new Symbol(name))}); // insert the symbol in the symbol table
        return &((this)->symbols.find(name)->second); // return the symbol
    }
    return NULL;
}

// Update the symbol table and its children with offsets
void SymbolTable::update(){
    vector<SymbolTable *> visited; // vector to keep track of children tables to visit
    int offset = 0; // offset of the current symbol
    for (auto &map_entry : (this)->symbols)  // for all symbols in the table
    {
        map_entry.second.offset = offset; // set the offset of the symbol
        offset += map_entry.second.size; // increment the offset by the size of the symbol
        
        if (map_entry.second.nestedTable)  // remember children table
        {
            visited.push_back(map_entry.second.nestedTable); // add the nested table to the visited vector
            map_entry.second.nestedTable->update(); // update the nested table
        }
    }
}

// Function to print the symbol table and its children
void SymbolTable::print()
{
    // pretty print 
    cout << string(140, '=') << endl;
    cout << "Table Name: " << this->name <<"\t\t Parent Name: "<< ((this->parent)?this->parent->name:"None") << endl;
    cout << string(140, '=') << endl;
    cout << setw(20) << "Name" << setw(40) << "Type" << setw(20) << "Initial Value" << setw(20) << "Offset" << setw(20) << "Size" << setw(20) << "Child" << "\n" << endl;

    // print all the symbols in the table
    for (auto &map_entry : (this)->symbols)
    {
        cout << setw(20) << map_entry.first;
        fflush(stdout);
        cout << setw(40) << map_entry.second.type->toString();
        cout << setw(20);
        if(map_entry.second.category == Symbol::LOCAL)
            cout << "local";
        else if(map_entry.second.category == Symbol::GLOBAL)
            cout << "global";
        else if(map_entry.second.category == Symbol::FUNCTION)
            cout << "function";
        else if(map_entry.second.category == Symbol::PARAMETER)
            cout << "parameter";
        else if(map_entry.second.category == Symbol::TEMPORARY)
            cout << "temporary";
        cout << setw(20) << map_entry.second.initialValue << setw(20) << map_entry.second.offset << setw(20) << map_entry.second.size;
        cout << setw(20) << (map_entry.second.nestedTable ? map_entry.second.nestedTable->name : "NULL") << endl;
    }
    cout << string(140, '-') << endl;
    cout << "\n" << endl;
    for(auto &map_entry : (this)->symbols){
        if (map_entry.second.nestedTable)
        {
            map_entry.second.nestedTable->print();
        } 
    }
}


// Implementation of quad class
Quad::Quad(string result, string arg1, string op, string arg2) : result(result), op(op), arg1(arg1), arg2(arg2) {}
Quad::Quad(string result, int arg1, string op, string arg2) : result(result), op(op), arg1(toString(arg1)), arg2(arg2) {}

// print the quad 
void Quad::print()
{
    if (this->op == "=")
    {
        cout << "\t" << this->result << " = " << this->arg1 << endl;
    }
    else if (this->op == "goto")
    {
        cout << "\tgoto " << this->result << endl;
    }
    else if (this->op == "param")
    {
        cout << "\t" << "param " << this->result << endl;
    }
    else if (this->op == "return")
    {
        cout << "\treturn " << this->result << endl;
    }
    else if (this->op == "call")
    {
        cout << "\t" << this->result << " = call " << this->arg1 << ", " << this->arg2 << endl;
    }
    else if (this->op == "label")
    {
        cout << this->result << endl;
    }
    else if (this->op == "=[]")
    {
        cout << "\t" << this->result << " = " << this->arg1 << "[" << this->arg2 << "]" << endl;
    }
    else if (this->op == "[]=")
    {
        cout << "\t" << this->result << "[" << this->arg1 << "] = " << this->arg2 << endl;
    }
    else if (this->op == "+" or this->op == "-" or this->op == "*" or this->op == "/" or this->op == "%" or this->op == "|" or this->op == "^" or this->op == "&" or this->op == "<<" or this->op == ">>")
    {
        // binary_print();
        cout<<"\t"<<this->result<<" = "<<this->arg1<<" "<<this->op<<" "<<this->arg2<<endl;
    }
    else if (this->op == "==" or this->op == "!=" or this->op == "<" or this->op == ">" or this->op == "<=" or this->op == ">=")
    {
        cout<<"\tif "<<this->arg1<<" "<<this->op<<" "<<this->arg2<<" goto "<<this->result<<endl;
    }
    else if(this->op == "*=")
    {
        cout << "\t" << "*" << this->result << " = " << this->arg1 << endl;
    }
    else if (this->op == "=-")
    {
        cout << "\t" << this->result << " = - " << this->arg1 << endl;
    }
    else if (this->op == "~")
    {
        cout<<"\t"<<this->result<<" = ~"<<this->arg1<<endl;
    }
    else if (this->op == "!")
    {
        cout<<"\t"<<this->result<<" = !"<<this->arg1<<endl;
    }
    else if (this->op == "=&" or this->op == "=*")
    {
        cout<<"\t"<<this->result<<" "<<this->op[0]<<" "<<this->arg1<<endl;
    }
    else
    {
        // if none of the above operators
        cout << this->op << this->arg1 << this->arg2 << this->result << endl;
        cout << "OPERATOR INVALID!\n";
    }
}



// Implementation of symbol type class
SymbolType::SymbolType(typeEnum type, SymbolType *arrayType, int width) : type(type), width(width), arrayType(arrayType) {}

// Implementation of sizes for symbol types
int SymbolType::getSize()
{
    switch (type) 
    {
    case INT:
        return 4;
    case FLOAT:
        return 8;
    case CHAR:
        return 1;
    case VOID:
        return 0;
    case POINTER:
        return 4;
    case ARRAY:
        return arrayType->getSize() * width;
    default:
        return 0;
    }
}

// Function to print symbol type
string SymbolType::toString()
{
    switch (this->type)
    {
    case INT:
        return "int";
    case FLOAT:
        return "float";
    case CHAR:
        return "char";
    case VOID:
        return "void";
    case POINTER:
        return "ptr(" + this->arrayType->toString() + ")";
    case ARRAY:
        return "array(" + to_string(this->width) + ", " + this->arrayType->toString() + ")";
    case FUNCTION:
        return "function";
    case BLOCK:
        return "block";
    default:
        return "unknown";
    }
}

// Implementation of symbol table class
SymbolTable::SymbolTable(string name, SymbolTable *parent) :  parent(parent), name(name) {}


// Implementation of symbol class
Symbol::Symbol(string name, SymbolType::typeEnum type, string init) : name(name), type(new SymbolType(type)), offset(0), nestedTable(NULL), initialValue(init), isFunction(false)
{
    size = this->type->getSize(); // size of symbol
}
// update type of the symbol
Symbol *Symbol::update(SymbolType *type)
{
    size = type->getSize(); // update size
    this->type = type;     // update type
    return this;
}
// convert the present symbol to different type, return old symbol if conversion not possible 
Symbol *Symbol::convert(SymbolType::typeEnum type_)
{

    // if the symbol is already of the required type, return the symbol
    if (this->type->type == type_) 
        return this;
    Symbol *newSymbol = gentemp(type_); // create a new symbol of the required type
    if(this->type->type==SymbolType::typeEnum::FLOAT && type_==SymbolType::typeEnum::INT){ // if conversion from float to int
        emit("=", newSymbol->name, "Float_TO_Int(" + this->name + ")");
        return newSymbol;
    }else if(this->type->type==SymbolType::typeEnum::INT && type_==SymbolType::typeEnum::FLOAT){ // if conversion from int to float
        // Symbol *newSymbol = new Symbol(this->name, type_, this->initialValue);
        emit("=", newSymbol->name, "Int_TO_Float(" + this->name + ")");
        return newSymbol;
    }
    else if(this->type->type==SymbolType::typeEnum::CHAR && type_==SymbolType::typeEnum::INT){ // if conversion from char to int
        // Symbol *newSymbol = new Symbol(this->name, type_, this->initialValue);
        emit("=", newSymbol->name, "Char_TO_Int(" + this->name + ")");
        return newSymbol;
    }else if(this->type->type==SymbolType::typeEnum::INT && type_==SymbolType::typeEnum::CHAR){ // if conversion from int to char
        // Symbol *newSymbol = new Symbol(this->name, type_, this->initialValue);
        emit("=", newSymbol->name, "Int_TO_Char(" + this->name + ")");
        return newSymbol;
    }
    else if(this->type->type==SymbolType::typeEnum::CHAR && type_==SymbolType::typeEnum::FLOAT){ // if conversion from char to float
        // Symbol *newSymbol = new Symbol(this->name, type_, this->initialValue);
        emit("=", newSymbol->name, "Char_TO_Float(" + this->name + ")");
        return newSymbol;
    }else if(this->type->type==SymbolType::typeEnum::FLOAT && type_==SymbolType::typeEnum::CHAR){ // if conversion from float to char
        // Symbol *newSymbol = new Symbol(this->name, type_, this->initialValue);
        emit("=", newSymbol->name, "Float_TO_Char(" + this->name + ")");
        return newSymbol;
    }
    else
    { // if conversion not possible
        cout << "Cannot convert " << this->type->toString() << " to " << toString(type_) << endl;
        return NULL;
    }
}

void Expression::toInt()
{
    // if the expression type is boolean
    if (this->type == Expression::typeEnum::BOOLEAN)
    {
        // generate a new temporary symbol
        Symbol *temp = gentemp(SymbolType::typeEnum::INT);
        // update true list and false list
        backpatch(this->trueList, static_cast<int>(quadArray.size()+1)); // update the true list
        // emit quad
        emit("=", this->symbol->name, "true");
        // emit go to quad
        emit("goto", toString(static_cast<int>(quadArray.size() + 2)));
        backpatch(this->falseList, static_cast<int>(quadArray.size()+1));  // update the false list
        // emit quad
        emit("=", this->symbol->name, "false");
    }
}

void Expression::toBool()
{
    // if the expression type is non boolean

    if (this->type != Expression::typeEnum::BOOLEAN)
    {
        // update true list and false list
        this->trueList = makeList(static_cast<int>(quadArray.size()) + 1); // if true then goto next quad
        this->falseList = makeList(static_cast<int>(quadArray.size()) + 2); // if false then goto next to next quad
        // emit quad
        emit("==", "", this->symbol->name, "0");
        // emit go to quad
        emit("goto", "");
    }
}


// generates temporary of given type with given value s
Symbol *gentemp(SymbolType::typeEnum type, string s)
{
    Symbol *temp = new Symbol("t" + toString(temporaryCount++), type, s); // create a new symbol
    currentTable->symbols.insert({temp->name, *temp});                 // insert the symbol in the current symbol table
    return temp;
}

int main() {
    // initialization of global variables
    cout << left; // left allign
    globalTable = new SymbolTable("global");
    currentTable = globalTable;
    int ins = 1;
    tableCount = 0;
    temporaryCount = 0;
    yyparse();
    globalTable->update();
    globalTable->print();
    for(auto it : quadArray) {
        cout<<setw(4)<<ins++<<": "; it->print();
    }
    return 0;
}
