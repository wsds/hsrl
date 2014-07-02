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
	char *keyword;
	char *name;
	char code_operator;
	int number;

	int index;
};


class CodeLine{
public:
	//CodeElement** codeElements;
	CodeElement* codeElements[10];
	int element_index;
};

class Assignment{
public:
	bool isNew;
	CodeElement* name;
	CodeElement* codeOperator;
	CodeElement* value;
};
void resolveCodeLine(char* line);

#endif /* INTERPRETMAIN_H */

