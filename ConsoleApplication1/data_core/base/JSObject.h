#ifndef JSOBJECT_H
#define JSOBJECT_H


#ifndef NULL
#define NULL 0
#endif /* NULL */

#define JSBOOL 0
#define JSNUMBER 1
#define JSJSON 2
#define JSKEYVALUE 3
#define JSSTRING 4

class JSObject
{
public:
	char type;
};


#endif /* JSOBJECT_H */

