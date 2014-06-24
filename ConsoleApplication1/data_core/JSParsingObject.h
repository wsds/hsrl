#ifndef JSPARSINGOBJECT_H
#define JSPARSINGOBJECT_H

#include "base/JSObject.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

#define JSBOOL 0
#define JSNUMBER 1
#define JSJSON 2
#define JSKEYVALUE 3
#define JSSTRING 4




class JSONIndicator
{
public:
	int head = -1;
	int tail = -1;


	JSObject* object = NULL;
};



class JSParsingObject : JSObject
{
public:



};


#endif /* JSPARSINGOBJECT_H */

