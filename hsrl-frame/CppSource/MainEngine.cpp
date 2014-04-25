#include "MainEngine.h"


namespace hsrl {


	MainEngine::MainEngine()
	{
	}


	MainEngine::~MainEngine()
	{
	}

	MainEngine * MainEngine::getInstance() {
		if (MainEngine::mMainEngine == NULL) {
			MainEngine::mMainEngine = new MainEngine();
		}
		return MainEngine::mMainEngine;
	}

}