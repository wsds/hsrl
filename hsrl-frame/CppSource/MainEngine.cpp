#include "MainEngine.h"


namespace hsrl {

	MainEngine::MainEngine()
	{
		isReady = false;
	}


	MainEngine::~MainEngine()
	{
	}

	MainEngine* MainEngine::instance = NULL;

}