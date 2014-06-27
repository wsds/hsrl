#ifndef JSINSTANCE_H
#define JSINSTANCE_H


#include "base/JSObject.h"
#include "JSON.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class JSInstance : JSObject
{
public:
	JSInstance();
	char* name;
	JSON* children;

	void(*function)(JSObject* parameter, JSObject* result);
};


#endif /* JSINSTANCE_H */

