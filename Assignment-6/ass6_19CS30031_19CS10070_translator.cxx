#include "ass6_19CS30031_19CS10070_translator.h"

// Global Variables
int tableCount, temporaryCount;  // Counts of number of tables and number of temps generated
vector<Quad *> quadArray;  // Quad Array
Symbol *currentSymbol;  // Current Symbol
vector<string> stringLiterals;
SymbolTable *currentTable, *globalTable;  // Symbol Tables
SymbolType::typeEnum currentType;  // Current Type

// Implementation of activation record class
ActivationRecord::ActivationRecord() : totalDisplacement(0), displacement(map<string, int>()) {}  // start with an initial displacement of 0

// Implementation of symbol type class
SymbolType::SymbolType(typeEnum type, SymbolType *arrayType, int width) : type(type), width(width), arrayType(arrayType) {}



// Implementation of utility functions
// overloaded toString function to maintain semantic consistency 
// convert int to string
string toString(int i)
{
    return to_string(i);
}
// converts float to string
string toString(float f)
{
    return to_string(f);
}
// converts char to string
string toString(char c)
{
    return string(1, c);
}

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
        return 8;
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
SymbolTable::SymbolTable(string name, SymbolTable *parent) : name(name), parent(parent) {}

Symbol *SymbolTable::lookup(string name)
{

    // If the symbol is present in the current table, return it
    auto it = (this)->symbols.find(name);
    if (it != (this)->symbols.end())
        return &(it->second);
    else{
        // If the symbol is not present in the current table, check in the parent table
    }
    
    // If the symbol is not present in the current table, check the parent table
    Symbol *ret_ptr = nullptr;
    if (this->parent != NULL)
        ret_ptr = this->parent->lookup(name);
    else{
        // If the symbol is not present in the parent table, return NULL
    }

    // if the symbol is not present in the parent table, insert it in the current table and return
    if (this == currentTable && !ret_ptr)
    {
        ret_ptr = new Symbol(name);
        ret_ptr->category = Symbol::LOCAL;
        if(currentTable == globalTable)
            ret_ptr->category = Symbol::GLOBAL;
        this->symbols.insert({name, *ret_ptr});
        return &((this)->symbols.find(name)->second);
    }else{
        // If the symbol is present in the parent table, return it
    }
    return ret_ptr;
}

// Update the symbol table and its children with offsets
void SymbolTable::update()
{
    // first update the current table
    int offset;
    // now prepare activation record for the current table

    activationRecord = new ActivationRecord();
    vector<SymbolTable *> visited; // vector to keep track of children tables to visit
    for (auto &map_entry : (this)->symbols)  // for all symbols in the table
    {
        if (map_entry.first != (this->symbols).begin()->first)     // else update the offset of the symbol and update the offset by adding the symbols width
        {
            map_entry.second.offset = offset;
            offset += map_entry.second.size;
        }
        else  // if the symbol is the first one in the table then set offset of it to 0
        {
            map_entry.second.offset = 0;
            offset = map_entry.second.size;
        }
        if (map_entry.second.nestedTable)  // remember children table
        {
            visited.push_back(map_entry.second.nestedTable);
        }else {
            // do nothing
        }
    }

    // first stack the parameters
    for (auto map_entry : (this)->symbols)
    {
        if (map_entry.second.category == Symbol::PARAMETER)
        {
            if(map_entry.second.size != 0)
            {
                activationRecord->totalDisplacement -= map_entry.second.size;
                activationRecord->displacement[map_entry.second.name] = activationRecord->totalDisplacement;
            }else{
                //do nothing
            }
        }else{
            //do nothing
        }
    }
    // now stack the local variables and the temporaries
    for (auto map_entry : (this)->symbols)
    {
        if ((map_entry.second.category == Symbol::LOCAL && map_entry.second.name != "return") || map_entry.second.category == Symbol::TEMPORARY)
        {
            if(map_entry.second.size != 0)
            {
                activationRecord->totalDisplacement -= map_entry.second.size;
                activationRecord->displacement[map_entry.second.name] = activationRecord->totalDisplacement;
            }
            else
            {
                //do nothing
            }
        }else
        {
            //do nothing
        }
    }
    
    // update children table
    for (auto &table : visited)
    {
        table->update();
    }
}

// Function to print the symbol table and its children
void SymbolTable::print()
{

    // pretty print 
    cout << string(140, '=') << endl;
    cout << "Table Name: " << this->name <<"\t\t Parent Name: "<< ((this->parent)?this->parent->name:"None") << endl;
    cout << string(140, '=') << endl;
    cout << setw(20) << "Name" << setw(40) << "Type" << setw(20) << "Category" << setw(20) << "Initial Value" << setw(20) << "Offset" << setw(20) << "Size" << setw(20) << "Child" << "\n" << endl;
    // cout<<"Name\t Type\t InitialValue\t Offset\t Size\n";
    vector<SymbolTable *> tovisit;

    // print all the symbols in the table
    for (auto &map_entry : (this)->symbols)
    {
        cout << setw(20) << map_entry.first;
        fflush(stdout);
        cout << setw(40) << map_entry.second.type->toString();
        cout << setw(20);
        if(map_entry.second.category == Symbol::LOCAL)
            cout << "local";
        else if(map_entry.second.category == Symbol::TEMPORARY)
            cout << "temporary";
        else if(map_entry.second.category == Symbol::FUNCTION)
            cout << "function";
        else if(map_entry.second.category == Symbol::PARAMETER)
            cout << "parameter";
        else if(map_entry.second.category == Symbol::GLOBAL)
            cout << "global";
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

// Implementation of symbol class
Symbol::Symbol(string name, SymbolType::typeEnum type, string init) : name(name), type(new SymbolType(type)), offset(0), nestedTable(NULL), initialValue(init)
{
    int temp = 0;
    temp = this->type->getSize();
    size = temp;
}
// update type of the symbol
Symbol *Symbol::update(SymbolType *type)
{
    size = type->getSize();
    this->type = type;
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

// Implementation of quad class
Quad::Quad(string result, string arg1, string op, string arg2) : result(result), op(op), arg1(arg1), arg2(arg2) {}
Quad::Quad(string result, int arg1, string op, string arg2) : result(result), op(op), arg1(toString(arg1)), arg2(arg2) {}

// print the quad 
void Quad::print()
{
    // if shift operators
    auto shift_print = [this]()
    {
        cout << "\t" << this->result << " " << this->op[0] << " " << this->op[1] << this->arg1 << endl;
    };
    // if special type of operators
    auto shift_print_ = [this](string tp)
    {
        cout << "\t" << this->result << " " << tp << " " << this->arg1 << endl;
    };
    // if binary operations
    auto binary_print = [this]()
    {
        cout << "\t" << this->result << " = " << this->arg1 << " " << this->op << " " << this->arg2 << endl;
    };
    // if relational operators
    auto relation_print = [this]()
    {
        cout << "\tif " << this->arg1 << " " << this->op << " " << this->arg2 << " goto " << this->result << endl;
    };

    /* we define the printing format for all operators */
    if (this->op == "=")
    {
        cout << "\t" << this->result << " = " << this->arg1 << endl;
    }
    else if (this->op == "param")
    {
        cout << "\t" << "param " << this->result << endl;
    }
    else if (this->op == "label")
    {
        cout << "Function start: " << this->result << endl;
    }
    else if (this->op == "call")
    {
        cout << "\t" << this->result << " = call " << this->arg1 << ", " << this->arg2 << endl;
    }
    else if (this->op == "goto")
    {
        cout << "\tgoto " << this->result << endl;
    }
    else if (this->op == "return")
    {
        cout << "\treturn " << this->result << endl;
    }
    else if (this->op == "=[]")
    {
        cout << "\t" << this->result << " = " << this->arg1 << "[" << this->arg2 << "]" << endl;
    }
    else if (this->op == "[]=")
    {
        cout << "\t" << this->result << "[" << this->arg1 << "] = " << this->arg2 << endl;
    }
    else if (this->op == "labelend")
    {
        cout << "Function end: " << this->result << endl;
    }
    else if (this->op == "==" or this->op == "!=" or this->op == "<" or this->op == ">" or this->op == "<=" or this->op == ">=")
    {
        relation_print();
    }
    else if (this->op == "=&" or this->op == "=*")
    {
        shift_print();
    }
    else if(this->op == "*=")
    {
        cout << "\t" << "*" << this->result << " = " << this->arg1 << endl;
    }
    else if (this->op == "+" or this->op == "-" or this->op == "*" or this->op == "/" or this->op == "%" or this->op == "|" or this->op == "^" or this->op == "&" or this->op == "<<" or this->op == ">>")
    {
        binary_print();
    }
    else if (this->op == "=str")
    {
        cout << "\t" << this->result << " = " << stringLiterals[atoi(this->arg1.c_str())] << endl;
    }
    else if (this->op == "~")
    {
        shift_print_("= ~");
    }
    else if (this->op == "=-")
    {
        shift_print_("= -");
    }
    else if (this->op == "!")
    {
        shift_print_("= !");
    }
    else
    {
        // if none of the above operators
        cout << this->op << this->arg1 << this->arg2 << this->result << endl;
        cout << "INVALID OPERATOR\n";
    }
}

// Implementation of emit funtions
void emit(string op, string result, string arg1, string arg2)
{
    Quad *new_quad = new Quad(result, arg1, op, arg2);
    quadArray.push_back(new_quad);
}
void emit(string op, string result, int arg1, string arg2)
{
    Quad *new_quad = new Quad(result, arg1, op, arg2);
    quadArray.push_back(new_quad);
}

// Implementation of backpatching functions
void backpatch(list<int> list_, int addr)
{
    // for all the addresses in the list, add the target address 
    for (auto &i : list_)
    {
        quadArray[i-1]->result = toString(addr);
    }
}

void finalBackpatch()
{
    // any dangling exits for void type functions are sent to function end
    int lastExit = -1;
    int curPos = quadArray.size();
    for(auto it = quadArray.rbegin(); it != quadArray.rend(); it++) {
        string op = (*it)->op;
        if(op == "goto" or op == "==" or op == "!=" or op == "<" or op == ">" or op == "<=" or op == ">=") {
            if((*it)->result.empty()) 
            {
                (*it)->result = toString(lastExit);
            }
            else {
                // do nothing
            }
        }
        else if(op == "labelend") 
        {
            lastExit = curPos;
        } else{
            // do nothing
        }
        curPos--;
    }
}

list<int> makeList(int base)
{
    // returns list with the base address as its only value
    auto res = {base};
    return res;
}

list<int> merge(list<int> first, list<int> second)
{
    // merge two lists
    list<int> ret = first;
    ret.merge(second);
    return ret;
}
// Implementation of Expression class functions

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

// Implementation of other helper functions
int nextInstruction()
{
    // returns the next instruction number
    int ans = quadArray.size();
    return ans + 1;
}

// generates temporary of given type with given value s
Symbol *gentemp(SymbolType::typeEnum type, string s)
{
    Symbol *temp = new Symbol("t" + toString(temporaryCount++), type, s);
    temp->category = Symbol::TEMPORARY;
    currentTable->symbols.insert({temp->name, *temp});
    return temp;
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


