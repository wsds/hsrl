#ifndef JSKEYVALUE_H
#define JSKEYVALUE_H


#include "base/JSObject.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class JSKeyValue : JSObject
{
public:
	JSKeyValue();
	char* key;
	JSObject* value;
};


#endif /* JSKEYVALUE_H */

