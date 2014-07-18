#include "Import.h"

Import* Import::instance = NULL;
Import::Import(){
	rootClosure = new Closure();
	rootClosure->initialize();
}

void Import::free(){

}

void Import::regedit(char * function_name, JSObject* (*function)(JSON* parameter)){

	JSFunction* jsFunction = new JSFunction();
	jsFunction->function_name = function_name;
	jsFunction->function = function;
	this->rootClosure->set(function_name, (JSObject*)jsFunction);

}

void Import::regiditFunctions(){
	Import* import = Import::getInstance();

	import->regedit("log", frame_log);
	import->regedit("add", frame_add);
	import->regedit("pause", frame_pause);

	import->regedit("add3", frame_add3);
}

