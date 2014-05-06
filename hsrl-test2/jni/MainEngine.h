#pragma once
#ifndef MAIN_ENGINE
#define MAIN_ENGINE
#include <EGL/egl.h>
#include "prelibs\native_app_glue\android_native_app_glue.h"
#include "hsrl\imagePools\ImagePool.hpp"
#include "hsrl\SpaceHolder.hpp"

namespace hsrl {

	class MainEngine
	{

	public:
		static MainEngine *instance;

		static MainEngine * getInstance()
		{
			if (instance == NULL) { //判断是否第一次调用  
				instance = new MainEngine();
			}
			return instance;
		}

		struct android_app* app;
		struct ANativeActivity* activity;
		AAssetManager* assetManager;

		bool isReady;

		//ASensorManager* sensorManager;
		//const ASensor* accelerometerSensor;
		//ASensorEventQueue* sensorEventQueue;

		int animating;
		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;
		int32_t width;
		int32_t height;

		MainEngine();
		~MainEngine();
		void initialize(struct android_app* app);

		ImagePool* imagePool;
		SpaceHolder* spaceHolder;
	};

}
#endif // MAIN_ENGINE