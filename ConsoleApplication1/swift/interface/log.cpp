#include <iostream>
#include "log.h"

void log(JSObject* object){

	if (object->type == JSSTRING){
		std::cout << ((JSObject*)object)->char_string;
		open::log(((JSObject*)object)->char_string);
	}
	else if (object->type == JSNUMBER){
		std::cout << ((JSObject*)object)->number;
		char num[12];
		parseNubmerToString(((JSObject*)object)->number,num);
		open::log(num);
	}
	else if (object->type == JSJSON){
		std::cout << stringifyJSON((JSON*)object);
		open::log(stringifyJSON((JSON*)object));
	}

	std::cout << std::endl;
}

int add(int x, int y){
	int z = x + y;
	return z;
}

JSON*  frame_log(JSON* parameter){
	JSON* result = NULL;
	log(parameter->pop());
	return result;
};

JSON* frame_add(JSON* parameter){
	JSON* result = new JSON();
	result->initialize();

	JSObject * resultNumber = new JSObject();
	resultNumber->number = add(parameter->pop()->number, parameter->pop()->number);
	return result;
}


void regeditFunction_log(){


}