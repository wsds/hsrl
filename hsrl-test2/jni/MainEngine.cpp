#include "MainEngine.h"

#include <android/log.h>

#define  LOG_TAG    "MainEngine"

#ifndef LOGI
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif /* LOGI */

namespace hsrl {

	MainEngine::MainEngine()
	{
		isReady = false;
		LOGI("One instance of {MainEngine} created");
	}


	MainEngine::~MainEngine()
	{
	}

	void MainEngine::initialize(struct android_app* app){
		this->app = app;
		this->activity = this->app->activity;
		this->assetManager = this->activity->assetManager;

		this->imagePool = ImagePool::getInstance();
		LOGI("{MainEngine} is initialized");
	}

	MainEngine* MainEngine::instance = NULL;

}