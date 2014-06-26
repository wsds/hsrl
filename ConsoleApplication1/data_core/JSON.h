#ifndef JSON_H
#define JSON_H


#include "base/JSObject.h"
#include "base/List.h"
#include "base/HashTable.h"

#include "JSKeyValue.h"
//#include "JSParsingObject.h"

#include "JSString.h"
#include "JSNumber.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */


class JSONIndicator
{
public:
	int head;
	int tail;
	int quotes_count;//[0,1,3]

	JSObject* json;
};

class JSON : JSObject
{
public:
	JSON();
	~JSON();

	LIST *list;
	HashTable *hashTable;

	JSONIndicator* json_indicator;

	int length;
	bool is_initialized = false;

	bool initialize();
	bool free();


	//API
	//list
	JSObject* pop();
	bool push(JSObject* object);
	bool insert(JSObject* object, int index);
	bool replace(JSObject* object, int index);
	JSObject*  del(int index);
	JSObject* find(int index);

	//hash
	JSObject* get(char* key);
	bool set(char* key, JSObject* value);
	JSObject*  del(char* key);

	//serialize
	char* stringify();
	bool parse(char* string);
};

int parseStringToNubmer(char* string, int length);
int parseNubmerToString(int number, char * target);

void testJSONParse();
JSON* parseJSON(char* string);
JSObject*  parseObject(char* string, JSONIndicator* object_indicator, bool isJSKeyValue);

#endif /* JSON_H */

