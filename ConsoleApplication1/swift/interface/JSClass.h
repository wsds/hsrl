#ifndef JSCLASS_H
#define JSCLASS_H


#include "../../data_core/data_core.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class JSClass : public JSObject
{
public:
	JSClass();

	char* className;
	JSON* children;

	void free();
};


#endif /* JSCLASS_H */

