#ifndef INTERPRETMAIN_H
#define INTERPRETMAIN_H
#include <iostream>

#include "../data_core/data_core.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

void interpret_test();
void interpret_main();

#define KEYWORD 0
#define NAME 1
#define CODEOPERATOR  2
#define CODE_NUMBER 3
#define CODE_STRING 4
#define CODE_JSON 5

class CodeElement{
public:
	int type;
	char * keyword;
	char * variable_name;
	char code_operator;

	int number;
	char * char_string;
	char * jsonstr;

	int index;
};


class CodeLine{
public:
	//CodeElement** codeElements;
	CodeElement* codeElements[10];
	int element_index;
};

#define ASSIGNMENT 1
#define FUNCTIONCALL 2
#define FUNCTIONDEFINITION 3

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

void interpret_main();
void resolveCodeLine(char* line);
void excute(Assignment * assignment);
void excute(FunctionCall * functionCall);
void excute(FunctionDefinition * functionDefinition);

void excuteFunction(FunctionDefinition * functionDefinition, JSON* parameter);

#endif /* INTERPRETMAIN_H */

