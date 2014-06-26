#include "Closure.h"

bool Closure::initialize()
{
	this->variables = new JSON();
	this->variables->initialize();
}