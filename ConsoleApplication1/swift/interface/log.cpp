#include <iostream>
#include "log.h"

void log(JSObject* object){



	if (object->type == JSSTRING){
		std::cout << ((JSString*)object)->char_string;
	}
	else if (object->type == JSNUMBER){
		std::cout << ((JSNumber*)object)->number;
	}
	else if (object->type == JSJSON){
		std::cout << stringifyJSON((JSON*)object);
	}



	std::cout << std::endl;
}