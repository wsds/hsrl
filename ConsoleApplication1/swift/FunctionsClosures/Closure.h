#ifndef CLOSURE_H
#define CLOSURE_H



#include "../../data_core/JSON.h"

#ifndef NULL
#define NULL 0
#endif /* NULL */

class Closure
{
public:
	Closure* previous;
	JSON* variables;
	Closure* next;

	bool initialize();
	bool free();

	Closure* forward();
	Closure* backward();
};


#endif /* CLOSURE_H */

