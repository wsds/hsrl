#ifndef JSFUNCTION_H
#define JSFUNCTION_H


#include "../../data_core/data_core.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class JSFunction : JSObject
{
public:
	JSFunction();

	void free();

	char* function_name;

	JSON* (*function)(JSON* parameter);
};


#endif /* JSFUNCTION_H */

