#ifndef JSFUNCTION_H
#define JSFUNCTION_H


#include "../../data_core/data_core.h"
#include "../interpret_main.h"
#include "../FunctionsClosures/Closure.h"
#ifndef NULL
#define NULL 0
#endif /* NULL */

class JSFunction : public JSObject
{
public:
	JSFunction();

	void free();

	char* function_name;

	JSObject* (*function)(JSON* parameter);

	FunctionDefinition * functionDefinition;

	Closure* closure;
};


#endif /* JSFUNCTION_H */

