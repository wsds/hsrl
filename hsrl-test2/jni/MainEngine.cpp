#include "MainEngine.h"


namespace hsrl {

	MainEngine::MainEngine()
	{
		isReady = false;
		this->imagePool = ImagePool::getInstance();
	}


	MainEngine::~MainEngine()
	{
	}

	void MainEngine::initialize(struct android_app* app){
		this->app = app;
		this->activity = this->app->activity;
		this->assetManager = this->activity->assetManager;
	}

	MainEngine* MainEngine::instance = NULL;

}