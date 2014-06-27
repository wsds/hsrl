#include "JSInstance.h"


JSInstance::JSInstance(){
	this->type = JSINSTANCE;
	(*(this->function))(NULL,NULL);
}

