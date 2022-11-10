
#include "ass6_20CS30003_20CS10039_translator.h"

// points to current function's activation record 
ActivationRecord *currentAR; 
// assembly file
ofstream ASSEMBLY_FILE_WRITE; 
// names of the files 
string FILE_NAME_INPUT, FILE_NAME_ASSEMBLY;

// maps to generate assembly code for a given function
map<char, int> ESCAPE_CHARACTER_ASCII_VALUE = { 
    {'n', 10}, 
    {'t', 9},
    {'r', 13}, 
    {'b', 8}, 
    {'f', 12}, 
    {'v', 11}, 
    {'a', 7}, 
    {'0', 0}
};
map<int, string> arg4SizeRegMap = { {1, "cl"}, {4, "ecx"}, {8, "rcx"} };
map<int, string> Maparg3SizeReg = { {1, "dl"}, {4, "edx"}, {8, "rdx"} };
map<int, string> Maparg2SizeReg = { {1, "sil"}, {4, "esi"}, {8, "rsi"} };
map<int, string> Maparg1SizeReg = { {1, "dil"}, {4, "edi"}, {8, "rdi"} };
map<int, string> MapretSizeReg = { {1, "al"}, {4, "eax"}, {8, "rax"} };
map<int, map<int, string>> MapargNum2Reg = { {1, Maparg1SizeReg}, {2, Maparg2SizeReg}, {3, Maparg3SizeReg}, {4, arg4SizeRegMap} };


// function to get the register name for a given argument
string getReg(string paramName, int paramNum, int size) {
    return "%" + MapargNum2Reg[paramNum][size];
}


string getStackLoc(string paramName) {
    // get the offset of the parameter from the base pointer

    if(currentAR->displacement.count(paramName)){
        string name1 =  toString(currentAR->displacement[paramName]) + "(%rbp)";
        return name1;
    }
    // incase it is  global variable
    return paramName;
}



void storeParam(string paramName, int paramNum) {
    // store the parameter from the stack to the register
    // symbol table entry for the parameter
    Symbol *symbol = currentTable->lookup(paramName);
    
    // size of the parameter
    int size = symbol->size;
    
    // type of the parameter
    SymbolType::typeEnum type = symbol->type->type;
    
    
    string movIns = "";
    
    if (size == 1) {
        movIns = "movb";
    }
    if (size == 4) {
        movIns = "movl";
    }
    if (size == 8) {
        movIns = "movq";
    }
    // if it is an array just store the address
    if(type == SymbolType::ARRAY) {
        size = 8;
        movIns = "leaq";
    }
    // get the register name for the parameter
    string reg = getReg(paramName, paramNum, size);
    // write the assembly instruction
    
    ASSEMBLY_FILE_WRITE << "\t" ;
    ASSEMBLY_FILE_WRITE << setw(8) ;
    ASSEMBLY_FILE_WRITE << movIns ;
    ASSEMBLY_FILE_WRITE << getStackLoc(paramName) ;
    ASSEMBLY_FILE_WRITE << ", " ;
    ASSEMBLY_FILE_WRITE << reg ;
    ASSEMBLY_FILE_WRITE << endl;
}

void loadParam(string paramName, int paramNum) {
    // load the parameter from the register to the stack

    // symbol table entry for the parameter
    Symbol *symbol = currentTable->lookup(paramName);
    
    // size of the parameter
    int size = symbol->size;
    
    // type of the parameter
    SymbolType::typeEnum type = symbol->type->type;
    
    string movIns = "";

    
    if (size == 1) {
        movIns = "movb";
    }
    if (size == 4) {
        movIns = "movl";
    }
    if (size == 8) {
        movIns = "movq";
    }
    // if it is an array just store the address
    if(type == SymbolType::ARRAY) {
        size = 8;
        movIns = "movq";
    }

    // get the register name for the parameter
    string reg = getReg(paramName, paramNum, size);

    // write the assembly instruction
    ASSEMBLY_FILE_WRITE << "\t" ;
    ASSEMBLY_FILE_WRITE << setw(8) ;
    ASSEMBLY_FILE_WRITE << movIns ;
    ASSEMBLY_FILE_WRITE << reg ;
    ASSEMBLY_FILE_WRITE << ", " ;
    ASSEMBLY_FILE_WRITE << getStackLoc(paramName) ;
    ASSEMBLY_FILE_WRITE << endl;
}





// function to get Ascii value
int getAsciiValue(string charConst) {
    
    if(charConst.length() != 3) {
        // check if it is an escape character 
        if(ESCAPE_CHARACTER_ASCII_VALUE.find(charConst[2]) != ESCAPE_CHARACTER_ASCII_VALUE.end()) {
            return ESCAPE_CHARACTER_ASCII_VALUE[charConst[2]];
        }
        return (int)charConst[2];        
    }

    return (int)charConst[1];
}



void translate() {
    // translate the input file to assembly file

    // open the assembly input file
    ASSEMBLY_FILE_WRITE.open(FILE_NAME_ASSEMBLY);

    // write the assembly 
    ASSEMBLY_FILE_WRITE << left;

    // string literals in the rodata section
    if(stringLiterals.size() > 0) {
        ASSEMBLY_FILE_WRITE << "\t.section\t.rodata" ;
        ASSEMBLY_FILE_WRITE << endl;
        int i = 0;
        for(auto &stringLiteral : stringLiterals) {
            ASSEMBLY_FILE_WRITE << ".LC" << i++ << ":" ;
            ASSEMBLY_FILE_WRITE << endl;
            
            ASSEMBLY_FILE_WRITE << "\t.string\t" << stringLiteral ;
            ASSEMBLY_FILE_WRITE << endl;
        }
    }

    // global variables (uninitialised)
    for(auto &symbol:globalTable->symbols) {
        if(symbol.second.initialValue.empty() && symbol.second.category == Symbol::GLOBAL) {
            ASSEMBLY_FILE_WRITE << "\t.comm\t" << symbol.first ;
            ASSEMBLY_FILE_WRITE << "," << symbol.second.size ;
            ASSEMBLY_FILE_WRITE << "," << symbol.second.size << endl;
        }
    }

    // convert tac labels to assembly labels
    map<int, string> labelMap;
    int quadNum = 1, labelNum = 0;
    for(auto &quad:quadArray) {
        string label = "label";
        string labelend = "labelend";
        if(quad->op == labelend) {
            string ok = ".LFE" + toString(labelNum);
            labelMap[quadNum] = ok;
            labelNum+=1;
        }else if(quad->op == label) {
            string ok = ".LFB" + toString(labelNum);
            labelMap[quadNum] = ok;
        } 
        quadNum+=1;
    }

    // write the assembly code for each quad
    for(auto &quad:quadArray) {
        string goto_ = "goto";
        string equal_to = "==";
        string not_equal_to = "!=";
        string less_than = "<";
        string greater_than = ">";
        string less_than_equal_to = "<=";
        string greater_than_equal_to = ">=";
        if(quad->op == goto_ || quad->op == equal_to || quad->op == not_equal_to || quad->op == less_than || quad->op == greater_than || quad->op == less_than_equal_to || quad->op == greater_than_equal_to) {
            int loc = stoi(quad->result);
            if(labelMap.find(loc) == labelMap.end()) {
                string ok = ".L" + toString(labelNum);;
                labelMap[loc] = ok;
                labelNum+=1;
            }
        }
    }

    // we need to know this to decide whether to use the global symbol table or the local symbol table
    bool inTextSpace = false; 
    string globalStringTemp;
    // for char simply hold the ascii value
    int globalIntTemp, globalCharTemp; 
    string functionEndLabel;
    // stack to store the params for function calls, especially helpful for nested function calls
    // of the type fun1(fun2(arg1, arg2), fun3(arg3, arg4))
    stack<string> params; 
                            
    quadNum = 1;
    for(auto &quad:quadArray) {
        string label = "label";
        string labelend = "labelend";
        
        if(quad->op == labelend) {
            // function epilogue
            ASSEMBLY_FILE_WRITE << labelMap[quadNum] << ":" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movq" << "%rbp, %rsp" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "popq" << "%rbp" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t" << ".cfi_def_cfa 7, 8" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t" << "ret" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t" << ".cfi_endproc" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".size" << quad->result << ", .-" << quad->result ;
            ASSEMBLY_FILE_WRITE << endl;

            inTextSpace = false;

        }
        else if(quad->op == label) {
            if(inTextSpace==0) {
                ASSEMBLY_FILE_WRITE << "\t.text" ;
                ASSEMBLY_FILE_WRITE << endl;
                inTextSpace = true;
            }

            // current table
            currentTable = globalTable->lookup(quad->result)->nestedTable;
            // current activation name
            currentAR = currentTable->activationRecord;
            
            // 
            functionEndLabel = labelMap[quadNum];
            // 
            functionEndLabel[3] = 'E';

            // function prologue
            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".globl" << quad->result ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".type" << quad->result << ", @function" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << quad->result << ":" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << labelMap[quadNum] << ":" ;
            ASSEMBLY_FILE_WRITE  << endl;
            ASSEMBLY_FILE_WRITE << "\t" << ".cfi_startproc" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "pushq" << "%rbp" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t.cfi_def_cfa_offset 16" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t.cfi_offset 6, -16" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movq" << "%rsp, %rbp" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t.cfi_def_cfa_register 6" ;
            ASSEMBLY_FILE_WRITE << endl;
            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "subq" << "$" << -currentAR->totalDisplacement << ", %rsp" ;
            ASSEMBLY_FILE_WRITE << endl;

            // store the params
            int paramNum = 1;
            for(auto param:currentTable->parameters) {
                loadParam(param, paramNum);
                paramNum+=1;
            }

        }

         else {
            if(inTextSpace!=0) {
                // process the function instructions
                string arg1 = quad->arg1;
                string arg2 = quad->arg2;
                string result = quad->result;
                string op = quad->op;

                if(labelMap.count(quadNum)) {
                    ASSEMBLY_FILE_WRITE << labelMap[quadNum] << ":" ;
                    ASSEMBLY_FILE_WRITE << endl;
                }

                string equal_ = "=";
                if(op == equal_) {
                    // first check if arg1 is a constant
                    if(isdigit(arg1[0])) {
                        // integer constant
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) ;
                        ASSEMBLY_FILE_WRITE << "movl" << "$" << arg1 ;
                        ASSEMBLY_FILE_WRITE << ", " << getStackLoc(result) ;
                        ASSEMBLY_FILE_WRITE << endl;
                    } else if(arg1[0] == '\''){
                        // character constant
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) ;
                        ASSEMBLY_FILE_WRITE << "movb" << "$" << getAsciiValue(arg1) ;
                        ASSEMBLY_FILE_WRITE << ", " << getStackLoc(result) ;
                        ASSEMBLY_FILE_WRITE << endl;
                    } else {
                        // variable
                        // check the size of arg1 in the current table and apply the corresponding move from arg1 to result via the appropriate register
                        if(currentTable->lookup(arg1)->size == 4) {
                            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" ;
                            ASSEMBLY_FILE_WRITE << getStackLoc(arg1) << ", %eax" ;
                            ASSEMBLY_FILE_WRITE << endl;
                            
                            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" ;
                            ASSEMBLY_FILE_WRITE << "%eax, " << getStackLoc(result) ;
                            ASSEMBLY_FILE_WRITE << endl;
                        }else if(currentTable->lookup(arg1)->size == 1) {
                            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movb" ;
                            ASSEMBLY_FILE_WRITE << getStackLoc(arg1) << ", %al" ;
                            ASSEMBLY_FILE_WRITE << endl;
                            
                            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movb" ;
                            ASSEMBLY_FILE_WRITE << "%al, " << getStackLoc(result) ;
                            ASSEMBLY_FILE_WRITE << endl;
                        } else if(currentTable->lookup(arg1)->size == 8) {
                            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movq" ;
                            ASSEMBLY_FILE_WRITE << getStackLoc(arg1) << ", %rax" ;
                            ASSEMBLY_FILE_WRITE << endl;
                            
                            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movq" << "%rax, " ;
                            ASSEMBLY_FILE_WRITE << getStackLoc(result) ;
                            ASSEMBLY_FILE_WRITE << endl;
                        }
                    }
                } 
                string str_="=str";
                if(op == str_) {
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movq" << "$.LC" << arg1 << ", " << getStackLoc(result) << endl;
                }
                string param_ = "param";
                if(op == param_) {
                    params.push(result);
                }
                string call_= "call";
                if(op == call_) {
                    // call function
                    int paramCount = stoi(arg2);
                    int tmp1=paramCount;
                    for(int i=0;i<tmp1;i++) {
                        storeParam(params.top(), paramCount);
                        paramCount-=1;
                        params.pop();
                    }
                    
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "call" << arg1 << endl;
                    // check the size of result in the current table and apply the corresponding move from appropriate return register to result
                    
                    if(currentTable->lookup(result)->size == 1) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movb" << "%al, " << getStackLoc(result) << endl;
                    } 
                     if(currentTable->lookup(result)->size == 4) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%eax, " << getStackLoc(result) << endl;
                    } 
                     if(currentTable->lookup(result)->size == 8) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movq" << "%rax, " << getStackLoc(result) << endl;
                    }
                } 
                string return_ = "return";
                 if(op == return_) {
                    // return from function
                    if(!result.empty()) {
                        // check the size of result in the current table and apply the corresponding move from result to return register
                        
                        if(currentTable->lookup(result)->size == 1) {
                            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movb" << getStackLoc(result) << ", %al" << endl;
                        } 
                         if(currentTable->lookup(result)->size == 4) {
                            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(result) << ", %eax" << endl;
                        } 
                         if(currentTable->lookup(result)->size == 8) {
                            ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movq" << getStackLoc(result) << ", %rax" << endl;
                        }
                    }
                    // if the next quad is not a labelend, then we need to jump to the function end
                    if(quadArray[quadNum]->op != labelend) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "jmp" << functionEndLabel << endl;
                    }
                } 
                string goto_ = "goto";
                 if(op == goto_) {
                    // unconditional jump
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "jmp" << labelMap[stoi(result)] << endl;
                } 
                string equal_to = "==";
                string not_equal_to = "!=";
                string less_than = "<";
                string greater_than = ">";
                string less_than_equal_to = "<=";
                string greater_than_equal_to = ">=";
                
                if(op == equal_to || op == not_equal_to || op == less_than || op == less_than_equal_to || op == greater_than || op == greater_than_equal_to) {
                    // check if arg1 == arg2
                    
                    string movins;
                    string cmpins;
                    string  movreg;
                    if(currentTable->lookup(arg1)->size == 1) {
                        movreg = "%al";
                        cmpins = "cmpb";
                        movins = "movb";
                    } 
                     if(currentTable->lookup(arg1)->size == 4) {
                        movreg = "%eax";
                        cmpins = "cmpl";
                        movins = "movl";
                    } 
                     if(currentTable->lookup(arg1)->size == 8) {
                        movreg = "%rax";
                        cmpins = "cmpq";
                        movins = "movq";
                    }
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << movins << getStackLoc(arg2) << ", " << movreg << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << cmpins << movreg << ", " << getStackLoc(arg1) << endl;
                    if(op == equal_to) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "je" << labelMap[stoi(result)] << endl;
                    } 
                     if(op == not_equal_to) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "jne" << labelMap[stoi(result)] << endl;
                    } 
                     if(op == less_than) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "jl" << labelMap[stoi(result)] << endl;
                    } 
                     if(op == less_than_equal_to) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "jle" << labelMap[stoi(result)] << endl;
                    } 
                     if(op == greater_than) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "jg" << labelMap[stoi(result)] << endl;
                    } 
                     if(op == greater_than_equal_to) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "jge" << labelMap[stoi(result)] << endl;
                    }
                } 
                string plus_ = "+";
                 if(op == plus_) {
                    // result = arg1 + arg2
                    if(result == arg1) {
                        // increment arg1
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "incl" << getStackLoc(arg1) << endl;
                    } else {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg1) << ", " << "%eax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "addl" << getStackLoc(arg2) << ", " << "%eax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%eax" << ", " << getStackLoc(result) << endl;
                    }
                } 
                string sub_ = "-";
                 if(op == sub_) {
                    // result = arg1 - arg2
                    if(result == arg1) {
                        // decrement arg1
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "decl" << getStackLoc(arg1) << endl;
                    }
                    else {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg1) << ", " << "%eax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "subl" << getStackLoc(arg2) << ", " << "%eax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%eax" << ", " << getStackLoc(result) << endl;
                    }
                } 
                string mul_ = "*";
                 if(op == mul_) {
                    // result = arg1 * arg2
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg1) << ", " << "%eax" << endl;
                    if(isdigit(arg2[0])) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "imull" << "$" + getStackLoc(arg2) << ", " << "%eax" << endl;
                    } else { 
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "imull" << getStackLoc(arg2) << ", " << "%eax" << endl;
                    }
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%eax" << ", " << getStackLoc(result) << endl;
                } 
                string div_ = "/";
                 if(op == div_) {
                    // result = arg1  / arg2
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg1) << ", " << "%eax" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "cdq" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "idivl" << getStackLoc(arg2) << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%eax" << ", " << getStackLoc(result) << endl;
                } 
                string mod_ = "%";
                 if(op == mod_) {
                    // result = arg1 % arg2
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg1) << ", " << "%eax" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "cdq" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "idivl" << getStackLoc(arg2) << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%edx" << ", " << getStackLoc(result) << endl;
                } 
                string equal_bracket = "=[]";
                 if(op == equal_bracket) {
                    // result = arg1[arg2]
                    Symbol *symbol = currentTable->lookup(arg1);
                    if(symbol->category == Symbol::PARAMETER) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg2) << ", " << "%eax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "cltq" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "addq" << getStackLoc(arg1) << ", " << "%rax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "(%rax)" << ", " << "%eax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%eax" << ", " << getStackLoc(result) << endl;
                    } else {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg2) << ", " << "%eax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "cltq" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << currentAR->displacement[arg1] << "(%rbp, %rax, 1)" << ", " << "%eax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%eax" << ", " << getStackLoc(result) << endl;
                    }
                } 
                string bracket_equal = "[]=";
                 if(op == bracket_equal) {
                    // result[arg1] = arg2
                    Symbol *symbol = currentTable->lookup(result);
                    if(symbol->category == Symbol::PARAMETER) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg1) << ", " << "%eax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "cltq" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "addq" << getStackLoc(result) << ", " << "%rax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg2) << ", " << "%ebx" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%ebx" << ", " << "(%rax)" << endl;
                    } else {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg1) << ", " << "%eax" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "cltq" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg2) << ", " << "%ebx" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%ebx" << ", " << currentAR->displacement[result] << "(%rbp, %rax, 1)" << endl;
                    }
                } 
                string equal_and = "=&";
                 if(op == equal_and) {
                    // result = &arg1
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "leaq" << getStackLoc(arg1) << ", " << "%rax" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movq" << "%rax" << ", " << getStackLoc(result) << endl;
                } 
                string equal_mul ="=*";
                 if(op == equal_mul) {
                    // result = *arg1
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movq" << getStackLoc(arg1) << ", " << "%rax" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "(%rax)" << ", " << "%eax" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%eax" << ", " << getStackLoc(result) << endl;
                } 
                string equal_sub = "=-";
                if(op == equal_sub) {
                    // result = -arg1
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg1) << ", " << "%eax" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "negl" << "%eax" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%eax" << ", " << getStackLoc(result) << endl;
                } 
                string mul_equal = "*=";
                if(op == mul_equal) {
                    // *result = arg1
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << getStackLoc(arg1) << ", " << "%eax" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movq" << getStackLoc(result) << ", " << "%rbx" << endl;
                    ASSEMBLY_FILE_WRITE << "\t" << setw(8) << "movl" << "%eax" << ", " << "(%rbx)" << endl;
                }

            } if(inTextSpace==0) {
                // process the global assignments
                currentSymbol = globalTable->lookup(quad->result);

                // first store the assignment value, this removes the need of temporaries, sort of a peep hole optimisation
                if(currentSymbol->category == Symbol::TEMPORARY) {
                    if(currentSymbol->type->type == SymbolType::INT) {
                        globalIntTemp = stoi(quad->arg1);
                    } 
                     if(currentSymbol->type->type == SymbolType::CHAR) {
                        globalCharTemp = getAsciiValue(quad->arg1);
                    } 
                     if(currentSymbol->type->type == SymbolType::POINTER) {
                        globalStringTemp = ".LC" + quad->arg1;
                    }
                } else {
                    if(currentSymbol->type->type == SymbolType::INT) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".globl" << currentSymbol->name << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".data" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".align" << 4 << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".type" << currentSymbol->name << ", @object" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".size" << currentSymbol->name << ", 4" << endl;
                        ASSEMBLY_FILE_WRITE << currentSymbol->name << ":" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".long" << globalIntTemp << endl;
                    } 
                     if(currentSymbol->type->type == SymbolType::CHAR) {
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".globl" << currentSymbol->name << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".data" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".type" << currentSymbol->name << ", @object" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".size" << currentSymbol->name << ", 1" << endl;
                        ASSEMBLY_FILE_WRITE << currentSymbol->name << ":" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".byte" << globalCharTemp << endl;
                    } 
                     if(currentSymbol->type->type == SymbolType::POINTER) {
                        ASSEMBLY_FILE_WRITE << "\t" << ".section	.data.rel.local" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".align" << 8 << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".type" << currentSymbol->name << ", @object" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".size" << currentSymbol->name << ", 8" << endl;
                        ASSEMBLY_FILE_WRITE << currentSymbol->name << ":" << endl;
                        ASSEMBLY_FILE_WRITE << "\t" << setw(8) << ".quad" << globalStringTemp << endl;
                    }
                }
            }
        }

        quadNum+=1;
    }


    ASSEMBLY_FILE_WRITE.close();
}

// main function
int main(int argc, char const *argv[]) {
    
    // input file name
    FILE_NAME_INPUT = string(argv[1]) + ".c";
    // assembly file name
    FILE_NAME_ASSEMBLY = string(argv[1]) + ".s";

    // intialise the global variables
    tableCount = 0;
    temporaryCount = 0;
    globalTable = new SymbolTable("global");
    currentTable = globalTable;
    cout << left;


    // parsing
    yyin = fopen(FILE_NAME_INPUT.c_str(), "r");
    
    yyparse();

    // symbol table updation 
    // generate activation records
    globalTable->update();
    
    // symbol table printing
    globalTable->print();

    // backpatching
    finalBackpatch();

    // counter for the number of quads
    int ins = 1;
    // print the quads
    for(auto it : quadArray) {
        cout<<setw(4)<<ins++<<": "; it->print();
    }

    // assembly file generation
    translate();

    return 0;
}
