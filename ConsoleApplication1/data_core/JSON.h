#ifndef JSON_H
#define JSON_H


#include "base/JSObject.h"
#include "base/List.h"
#include "base/HashTable.h"

#include "JSKeyValue.h"
//#include "JSParsingObject.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */


class JSONIndicator
{
public:
	int head = -1;
	int tail = -1;


	JSON* json = NULL;
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



int checkJSONGrammar(char* string, int from);
bool checkJSON(char* string);
bool checkParenthesesMatching(char* string);

void testJSONParse();
int parseJSON(char* string, JSONIndicator* json_indicator);

#endif /* JSON_H */

