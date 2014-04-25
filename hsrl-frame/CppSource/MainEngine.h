#pragma once
#ifndef MAIN_ENGINE
#define MAIN_ENGINE
#include <EGL/egl.h>
#include "native_app_glue/android_native_app_glue.h"

namespace hsrl {

	class MainEngine
	{

	public:
		MainEngine()   //���캯����˽�е�  
		{
		}
		static MainEngine *m_pInstance;
		static int test;
	public:
		static int test1;
		static MainEngine * getInstance()
		{

			//test = test + 1;
			//m_pInstance++;

			if (m_pInstance == NULL) { //�ж��Ƿ��һ�ε���  
				//test = test + 1;
				m_pInstance = new MainEngine();
			}
			//return m_pInstance;

			//if (m_pInstance == NULL)  //�ж��Ƿ��һ�ε���  
			//	m_pInstance = new MainEngine();
			//return m_pInstance;
			return m_pInstance;
		}
		static MainEngine * getInstance1(){
			//return GetInstance();
			return NULL;
		}

	public:
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

		//MainEngine();
		~MainEngine();
	};

}
#endif // MAIN_ENGINE