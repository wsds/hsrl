#include "HashTable.h"


JSObject* HashTable::find(char* key)
{
	return NULL;
}

bool HashTable::set(char* key, JSObject* object)
{
	return true;
}

bool HashTable::del(char* key)
{
	return true;
}

bool HashTable::resize()
{
	return true;
}

bool HashTable::initialize()
{
	this->max_size = 8;
	this->length = 0;
	this->threshold = this->max_size * 0.8;

	int mem_size = this->max_size*sizeof(void*);

	this->elements = (HashEntry**)JSMalloc(mem_size);

	this->is_initialized = true;

	return true;


}