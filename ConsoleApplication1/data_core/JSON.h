#ifndef JSON_H
#define JSON_H


#include "base/JSObject.h"
#include "base/List.h"
#include "base/HashTable.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class JSON : JSObject
{
public:
	JSON();
	~JSON();

	LIST *list;
	HashTable *hashTable;

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
	bool del(int index);
	JSObject* find(int index);

	//hash
	JSObject* get(char* key);
	bool set(char* key, JSObject* value);
	bool del(char* key);

	//serialize
	char* stringify();
	bool parse(char* string);
};


#endif /* JSON_H */

