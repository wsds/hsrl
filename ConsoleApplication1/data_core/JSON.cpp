#include "JSON.h"

JSON::JSON(){
	this->type = JSJSON;
	this->initialize();
}


bool JSON::initialize()
{
	this->list = new LIST();//to optimize the memory use, get it from a managed pool instead of new it.//to do
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


//list
JSObject* JSON::pop()
{
	JSObject* object = this->list->pop();
	if (object != NULL&&object->type == JSKEYVALUE){
		this->hashTable->del(((JSKeyValue*)object)->key);
	}
	this->length--;
	return object;
}

bool JSON::push(JSObject* object)
{
	this->list->push(object);
	if (object->type == JSKEYVALUE){
		this->hashTable->set(((JSKeyValue*)object)->key, object);
	}
	this->length++;
	return true;
}

bool JSON::insert(JSObject* object, int index)
{
	this->list->insert(object, index);
	if (object->type == JSKEYVALUE){
		this->hashTable->set(((JSKeyValue*)object)->key, object);
	}
	this->length++;
	return true;
}

bool JSON::replace(JSObject* object, int index)
{
	this->list->replace(object, index);
	if (object->type == JSKEYVALUE){
		this->hashTable->set(((JSKeyValue*)object)->key, object);
	}
	return true;
}

JSObject*  JSON::del(int index)
{
	JSObject* object = this->list->del(index);

	if (object != NULL)//deleted
	{
		if (object->type == JSKEYVALUE){
			this->hashTable->del(((JSKeyValue*)object)->key);
		}
		this->length--;
	}
	else //not found
	{
	}
	return object;
}

JSObject* JSON::find(int index)
{
	JSObject* object = this->list->find(index);
	return object;
}


//hash
JSObject* JSON::get(char* key)
{
	JSObject* object = this->hashTable->get(key);
	return object;
}

bool JSON::set(char* key, JSObject* value)
{
	JSKeyValue* object = new JSKeyValue();
	object->key = key;
	object->value = value;

	int status = this->hashTable->set(((JSKeyValue*)object)->key, (JSObject*)object);

	if (status == 2)//new entry
	{
		this->list->push((JSObject*)object);
		this->length++;
	}
	else if (status == 1)//replace entry
	{
		int index = this->list->findValue((JSObject*)object);
		this->list->replace((JSObject*)object, index);
	}

	return true;
}

JSObject* JSON::del(char* key)
{
	JSObject* object = this->hashTable->del(key);

	if (object != NULL)//deleted
	{
		int index = this->list->findValue((JSObject*)object);
		this->list->del(index);
		this->length--;
	}
	else //not found
	{
	}
	return object;
}

char* JSON::stringify()
{
	return NULL;
}

bool JSON::parse(char* string)
{
	return true;
}
