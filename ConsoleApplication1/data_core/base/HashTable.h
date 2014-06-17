#ifndef HASHTABLE_H
#define HASHTABLE_H


#include "JSObject.h"
#include "MemoryManagement.h"

#ifndef NULL
#define NULL 0;
#endif /* NULL */

class HashEntry
{
public:
	void* key;
	void* value;

	HashEntry* next;
};

class HashTable
{
public:

	HashEntry** elements;//~~~~~~~~~~~~~~~Memory Management~~~~~~~~~~~~~~~~~

	int length;

	int max_size;
	int threshold;


	bool is_initialized = false;

	//find O(1)
	JSObject* find(char* key);

	//set O(1)
	bool set(char* key, JSObject* object);

	//del O(1)
	bool del(char* key);

	//resize O(n)
	bool resize();

	//initialize default size=8;
	bool initialize();

	bool free();
};


#endif /* HASHTABLE_H */

