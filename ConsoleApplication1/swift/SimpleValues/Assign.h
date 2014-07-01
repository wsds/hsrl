#ifndef ASSIGN_H
#define ASSIGN_H



#include "../../data_core/JSON.h"
#include "../interpret_main.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class Assignment{
	CodeElement* name;
	CodeElement* codeOperator;
	CodeElement* value;
};

class Assign
{
public:

	bool initialize();
	bool free();

};


void resolveAssign();


#endif /* ASSIGN_H */

