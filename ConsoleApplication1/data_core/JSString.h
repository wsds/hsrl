#ifndef JSSTRING_H
#define JSSTRING_H


#include "base/JSObject.h"
#include "base/List.h"
#include "base/HashTable.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class JSString : JSObject
{
public:
	JSString();
	~JSString();

};


#endif /* JSSTRING_H */

