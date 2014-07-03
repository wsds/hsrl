#include "Closure.h"

bool Closure::initialize()
{
	this->variables = new JSON();
	this->variables->initialize();
	return true;
}

bool Closure::set(char* key, JSObject* jsObject){
	return this->variables->set(key, jsObject);
}

JSObject* Closure::get(char* key) {
	if (key == NULL){
		return NULL;
	}
	return this->variables->get(key);
}


JSObject* Closure::lookup(char* key) {
	if (key == NULL){
		return NULL;
	}
	JSObject* jsObject = this->variables->get(key);
	if (jsObject == NULL){
		if (this->previous != NULL){
			jsObject = this->previous->lookup(key);
		}
		else{
			jsObject = NULL;
		}
	}
	return jsObject;
}