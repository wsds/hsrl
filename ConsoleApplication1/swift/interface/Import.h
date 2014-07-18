#ifndef IMPORT_H
#define IMPORT_H


//functions and classes that will be imported to swift context, goes here.
#include "log.h"

#include "pause.h"

//

#include "../../data_core/data_core.h"
#include "../FunctionsClosures/Closure.h"

#include "JSFunction.h"
#include "JSClass.h"
#include "JSInstance.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class Import
{
public:
	Import();
	void free();

	static Import *instance;

	static Import * getInstance()
	{
		if (instance == NULL) {
			instance = new Import();
		}
		return instance;
	}

	Closure * rootClosure;

	void regedit(char * function_name, JSObject* (*function)(JSON* parameter));

	void regiditFunctions();

};


#endif /* IMPORT_H */

