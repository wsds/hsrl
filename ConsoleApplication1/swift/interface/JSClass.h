#ifndef JSCLASS_H
#define JSCLASS_H


#include "../../data_core/data_core.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class JSClass : JSObject
{
public:
	JSClass();
	char* key;
	JSObject* value;

	void free();
};


#endif /* JSCLASS_H */

