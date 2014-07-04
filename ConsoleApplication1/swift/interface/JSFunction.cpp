#include "JSFunction.h"


JSFunction::JSFunction(){
	this->type = JSFUNCTION;
	this->char_string = NULL;
	this->function = NULL;
	this->functionDefinition = NULL;
}

void JSFunction::free(){

}
