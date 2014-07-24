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

	static KeyWords *instance;

	static KeyWords * getInstance()
	{
		if (instance == NULL) {
			instance = new KeyWords();
		}
		return instance;
	}

	KeyWords();
	char* string_var;
	char* string_new;
	char* string_func;
	char* string_if;
	char* string_else;

	char* string_for;
	char* string_in;
	char* string_while;
	char* string_do;


	char* string_return;

	char* string_class;
	char* string_instance;
	char* string_this;
	char* string_self;

	HashTable * keyWordMap;

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
#define CHILDNAME 11
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

	char bracket;
	int nextBracketIndex;
	int preBracketIndex;

	//char * delimiter;
	bool isResolvedDelimiter;

	CodeElement * child;
};


class CodeLine{
public:
	//CodeElement** codeElements;
	CodeElement* codeElements[30];
	int element_index;
};
#define META 0
#define ASSIGNMENT 1
#define FUNCTIONCALL 2
#define FUNCTIONDEFINITION 3

#define CONDITION 4
#define IFBLOCK 5
#define FORBLOCK 6
#define FORINBLOCK 7

#define EXPRESSION 9
#define EXCUTEABLEBLOCK 10

#define OPERATOR 11
#define FUNCTIONRETURN 12
#define CLASSDEFINITION 13
#define INSTANCEDEFINITION 14

#define EXCUTED 20
class Executable{
public:
	int type;
};
class ExecutableBlock : public Executable{
public:
	ExecutableBlock();

	Executable * holder;
	bool isHolded;

	Executable* executables[20];
	int executable_index;

	JSObject* result;
};
class MetaExecutable : public Executable{
public:
	MetaExecutable();
	CodeElement* codeElement;
};


class ExcutedExecutable : public Executable{
public:
	ExcutedExecutable();
	JSObject* result;
};


class Operator : public Executable{
public:
	Operator();
	char code_operator;
	char code_operator2;
	
	Executable* left;
	Executable* right;
};
class Expression : public Executable{
public:
	Expression();
	bool isNew;
	Executable* executables[40];
	int executable_index;

	JSObject* result;
};

class FunctionCall : public Executable{
public:
	FunctionCall();
	char* functionName;

	CodeElement* functionNameChain;
	bool isNew;
	Executable* variables[5];
	int variable_index;

	JSObject* result;
};

class FunctionReturn : public Executable{
public:
	FunctionReturn();
	Executable* variables[5];
	int variable_index;

	JSObject* result;
};
class FunctionDefinition : public Executable{
public:
	FunctionDefinition();
	char* functionName;
	Executable* variables[5];
	int variable_index;

	ExecutableBlock* executableBlock;
};


class ClassDefinition : public Executable{
public:
	ClassDefinition();
	char* className;
	Executable* variables[5];
	int variable_index;

	ExecutableBlock* executableBlock;
};

class InstanceDefinition : public Executable{
public:
	InstanceDefinition();
	char* instanceName;
	Executable* variables[5];
	int variable_index;

	ExecutableBlock* executableBlock;
};

class IfBlock : public Executable{
public:
	IfBlock();

	bool isElse;

	Executable * condition;

	ExecutableBlock* executableBlock;

	IfBlock* next;

	ExecutableBlock* else_executableBlock;

};


class ForBlock : public Executable{
public:
	ForBlock();
	Executable * pre_executable;

	Executable * conditions[5];
	int conditions_index;

	Executable * last_executable;

	ExecutableBlock* executableBlock;

	bool isDo;
};

class ForInBlock : public Executable{
public:
	ForInBlock();

	JSON* iterator;
	CodeElement* iteratorName;
	CodeElement* keyName;
	CodeElement* valueName;

	ExecutableBlock* executableBlock;
};

void interpret_main();
void resolveCodeLine(char* line);

JSObject* excute(Executable * executable);
JSObject* excute(ExecutableBlock * executableBlock);

JSObject* excute(Expression * expression);

JSObject* excute(FunctionCall * functionCall);
JSObject* excute(FunctionDefinition * functionDefinition);
JSObject* excute(ClassDefinition * classDefinition);

JSObject* excute(IfBlock * ifBlock);
JSObject* excute(ForBlock * forBlock);
JSObject* excute(ForInBlock * forInBlock);

JSObject* excute(FunctionReturn* functionReturn);

JSObject* excuteAssignment(Executable * target, MetaExecutable * source, bool isNew);
JSObject* excuteFunction(FunctionDefinition * functionDefinition, JSON* parameter);

void getAllVariablesToString();
JSKeyValue * getFromClosure(CodeElement* codeElement);



class DEBUGExecutable{
public:
	MetaExecutable* metaExecutable;
	Operator* codeOperator;
	Expression* expression;
	FunctionCall* functionCall;
	FunctionDefinition* functionDefinition;
	ExecutableBlock* executableBlock;
	ForInBlock* forInBlock;
	ForBlock* forBlock;
	IfBlock* ifBlock;

	DEBUGExecutable * children[20];
	int children_index;
};

DEBUGExecutable * debugExecutable(Executable * executable);

#endif /* INTERPRETMAIN_H */



