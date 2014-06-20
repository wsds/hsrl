#include "JSON.h"


bool JSON::initialize()
{
	this->list = new LIST();
	this->list->initialize();
	this->hashTable = new HashTable();
	this->hashTable->initialize();

	this->length = 0;
	this->is_initialized = true;

	return true;
}

bool JSON::free()
{
	//JSFree(this->elements);
	return true;
}



JSObject* JSON::pop()
{

}

bool JSON::push(JSObject* object)
{

}

bool JSON::insert(JSObject* object, int index)
{

}

bool JSON::replace(JSObject* object, int index)
{

}

bool JSON::del(int index)
{

}

JSObject* JSON::find(int index)
{

}

JSObject* JSON::get(char* key)
{

}

bool JSON::set(char* key, JSObject* value)
{

}

bool JSON::del(char* key)
{

}

char* JSON::stringify()
{

}

bool JSON::parse(char* string)
{

}
