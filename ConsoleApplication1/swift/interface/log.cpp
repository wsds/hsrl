#include <iostream>
#include "log.h"

void log(JSObject* object){

	if (object->type == JSSTRING){
		std::cout << ((JSObject*)object)->char_string;
		open::logBuf("1.");
		open::logBuf(((JSObject*)object)->char_string);
		open::logBufFlush();
	}
	else if (object->type == JSNUMBER){
		std::cout << ((JSObject*)object)->number;
		char num[12];
		parseNubmerToString(((JSObject*)object)->number, num);
		open::logBuf("1.");
		open::logBuf(num);
		open::logBufFlush();
	}
	else if (object->type == JSJSON){
		char* json_str = stringifyJSON((JSON*)object);
		std::cout << json_str;
		open::logBuf("1.");
		open::logBuf(json_str);
		open::logBufFlush();
	}
	else if (object->type == JSCLASS){
		char* json_str = stringifyJSON(((JSClass*)object)->children);
		std::cout << json_str;
		open::logBuf("1.");
		open::logBuf(json_str);
		open::logBufFlush();
	}

	std::cout << std::endl;
}

int add(int x, int y){
	int z = x + y;
	return z;
}

JSObject*  frame_log(JSON* parameter){
	JSON* result = NULL;
	log(parameter->pop());
	return result;
};

JSObject* frame_add(JSON* parameter){

	JSObject * resultNumber = new JSNumber();
	resultNumber->number = add(parameter->pop()->number, parameter->pop()->number);
	return resultNumber;
}

int add3(int x, int y, int z){
	int r = x + y + z;
	return r;
}

JSObject* frame_add3(JSON* parameter){

	JSObject * resultNumber = new JSNumber();
	resultNumber->number = add3(parameter->pop()->number, parameter->pop()->number, parameter->pop()->number);
	return resultNumber;
}

void regeditFunction_log(){


}