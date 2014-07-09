#ifndef INTERPRETMAIN_H
#define INTERPRETMAIN_H
#include <iostream>

#include "../data_core/data_core.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

void interpret_test();
void interpret_main();

class KeyWords{
public:
	KeyWords();
	char* string_func;


};

#define KEYWORD 0
#define NAME 1
#define CODEOPERATOR  2
#define CODE_NUMBER 3
#define CODE_STRING 4
#define CODE_JSON 5
#define DELIMITER 6
#define BRACKET 7

#define SKIP 10;
class CodeElement{
public:
	int type;
	char * keyword;
	char * variable_name;
	char code_operator;
	char code_operator2;

	int number;
	char * char_string;
	char * jsonstr;

	int index;
};


class CodeLine{
public:
	//CodeElement** codeElements;
	CodeElement* codeElements[20];
	int element_index;
};

#define ASSIGNMENT 1
#define FUNCTIONCALL 2
#define FUNCTIONDEFINITION 3

#define CONDITION 4
#define IFBLOCK 5

class Executable{
public:
	int type;
};

class Assignment : public Executable{
public:
	Assignment();
	bool isNew;
	CodeElement* left;
	CodeElement* codeOperator;
	CodeElement* right;
};


class FunctionCall : public Executable{
public:
	FunctionCall();
	CodeElement* functionName;
	CodeElement* variables;
};

class FunctionDefinition : public Executable{
public:
	FunctionDefinition();
	CodeElement* functionName;
	CodeElement* variables;

	Executable* executables[10];
	int executable_index;
};


class Condition : public Executable{
public:
	Condition();
	CodeElement* left;
	CodeElement* codeOperator;
	CodeElement* right;
};

class IfBlock : public Executable{
public:
	IfBlock();
	Condition * conditions[5]; 
	int conditions_index;

	Executable * executables[10];
	int executable_index;

	IfBlock* next;

	Executable * else_executables[10];
	int else_executable_index;
};


class ForBlock : public Executable{
public:
	ForBlock();
	Executable * pre_executable;

	Condition * conditions[5];
	int conditions_index;

	Executable * last_executable;

	Executable * executables[10];
	int executable_index;

	bool isDo;
};

class ForInBlock : public Executable{
public:
	ForInBlock();

	JSON* iterator;
	CodeElement* keyName;
	CodeElement* valueName;

	Executable * executables[10];
	int executable_index;

};

void interpret_main();
void resolveCodeLine(char* line);
void excute(Assignment * assignment);
void excute(FunctionCall * functionCall);
void excute(FunctionDefinition * functionDefinition);

void excuteFunction(FunctionDefinition * functionDefinition, JSON* parameter);

void getAllVariablesToString();

#endif /* INTERPRETMAIN_H */

