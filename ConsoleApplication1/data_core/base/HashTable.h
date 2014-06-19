#ifndef HASHTABLE_H
#define HASHTABLE_H


#include "JSObject.h"
#include "MemoryManagement.h"

#include "../JSNumber.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class HashEntry
{
public:
	char* key;
	JSNumber* value;
	unsigned int hash;

	int level;

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

	//get O(1)
	JSObject* get(char* key);

	//set O(1)
	bool set(char* key, JSObject* value);

	//del O(1)
	bool del(char* key);

	//resize O(n)
	bool resize();

	//initialize default size=8;
	bool initialize();

	bool free();
};

unsigned int dictGenHashFunction(const void *key, int len);

int strlen(char *str);
int strcmp(char *src, char *dst);

#endif /* HASHTABLE_H */

