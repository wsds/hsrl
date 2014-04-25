#pragma once
#ifndef MAIN_ENGINE
#define MAIN_ENGINE
#include <EGL/egl.h>
#include "native_app_glue/android_native_app_glue.h"

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
	};

}
#endif // MAIN_ENGINE