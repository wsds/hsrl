/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <errno.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <android/sensor.h>
#include <android/log.h>
#include "libs/native_app_glue/android_native_app_glue.h"

#include <android/native_window_jni.h>
#include "libs/cpufeatures/cpu-features.h"

#include "hsrl/tools.h"

#define  LOG_TAG    "mainEntry"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

//#include "TeapotRenderer.h"
//#include "libs/ndk_helper/NDKHelper.h"

//-------------------------------------------------------------------------
//Preprocessor
//-------------------------------------------------------------------------
#define HELPER_CLASS_NAME "com/sample/helper/NDKHelper" //Class name of helper function
//-------------------------------------------------------------------------
//Shared state for our app.
//-------------------------------------------------------------------------
struct android_app;
class Engine
{
	//TeapotRenderer renderer_;

	//ndk_helper::GLContext* gl_context_;

	bool initialized_resources_;
	bool has_focus_;

	//ndk_helper::DoubletapDetector doubletap_detector_;
	//ndk_helper::PinchDetector pinch_detector_;
	//ndk_helper::DragDetector drag_detector_;
	//ndk_helper::PerfMonitor monitor_;

	//ndk_helper::TapCamera tap_camera_;

	android_app* app_;

	ASensorManager* sensor_manager_;
	const ASensor* accelerometer_sensor_;
	ASensorEventQueue* sensor_event_queue_;

	void UpdateFPS(float fFPS);
	void ShowUI();
	//void TransformPosition( ndk_helper::Vec2& vec );

public:
	static void HandleCmd(struct android_app* app,
		int32_t cmd);
	static int32_t HandleInput(android_app* app,
		AInputEvent* event);

	Engine();
	~Engine();
	void SetState(android_app* state);
	int InitDisplay();
	void LoadResources();
	void UnloadResources();
	void DrawFrame();
	void TermDisplay();
	void TrimMemory();
	bool IsReady();

	void UpdatePosition(AInputEvent* event,
		int32_t iIndex,
		float& fX,
		float& fY);

	void InitSensors();
	void ProcessSensors(int32_t id);
	void SuspendSensors();
	void ResumeSensors();
};

//-------------------------------------------------------------------------
//Ctor
//-------------------------------------------------------------------------
Engine::Engine() :
initialized_resources_(false),
has_focus_(false),
app_(NULL),
sensor_manager_(NULL),
accelerometer_sensor_(NULL),
sensor_event_queue_(NULL)
{
	//gl_context_ = ndk_helper::GLContext::GetInstance();
}

//-------------------------------------------------------------------------
//Dtor
//-------------------------------------------------------------------------
Engine::~Engine()
{
}

/**
 * Load resources
 */
void Engine::LoadResources()
{
	//renderer_.Init();
	//renderer_.Bind( &tap_camera_ );
}

/**
 * Unload resources
 */
void Engine::UnloadResources()
{
	//renderer_.Unload();
}

/**
 * Initialize an EGL context for the current display.
 */
int Engine::InitDisplay()
{

}

/**
 * Just the current frame in the display.
 */
void Engine::DrawFrame()
{

}

/**
 * Tear down the EGL context currently associated with the display.
 */
void Engine::TermDisplay()
{
	//gl_context_->Suspend();

}

void Engine::TrimMemory()
{
	LOGI("Trimming memory");
	//gl_context_->Invalidate();
}
/**
 * Process the next input event.
 */
int x = 0;
int y = 0;
int32_t Engine::HandleInput(android_app* app, AInputEvent* event)
{
	struct engine* engine = (struct engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		x = AMotionEvent_getX(event, 0);
		y = AMotionEvent_getY(event, 0);
		LOGI("engine_handle_input: x=%d y=%d", x, y);

		hsrl::log_information("hello", 10);
		return 1;
	}
	return 0;

	return 0;
}

/**
 * Process the next main command.
 */
void Engine::HandleCmd(struct android_app* app,
	int32_t cmd)
{
	Engine* eng = (Engine*)app->userData;
	switch (cmd)
	{
	case APP_CMD_SAVE_STATE:
		break;
	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		if (app->window != NULL)
		{
			eng->InitDisplay();
			eng->DrawFrame();
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		eng->TermDisplay();
		eng->has_focus_ = false;
		break;
	case APP_CMD_STOP:
		break;
	case APP_CMD_GAINED_FOCUS:
		eng->ResumeSensors();
		//Start animation
		eng->has_focus_ = true;
		break;
	case APP_CMD_LOST_FOCUS:
		eng->SuspendSensors();
		// Also stop animating.
		eng->has_focus_ = false;
		eng->DrawFrame();
		break;
	case APP_CMD_LOW_MEMORY:
		//Free up GL resources
		eng->TrimMemory();
		break;
	}
}

//-------------------------------------------------------------------------
//Sensor handlers
//-------------------------------------------------------------------------
void Engine::InitSensors()
{
	sensor_manager_ = ASensorManager_getInstance();
	accelerometer_sensor_ = ASensorManager_getDefaultSensor(sensor_manager_,
		ASENSOR_TYPE_ACCELEROMETER);
	sensor_event_queue_ = ASensorManager_createEventQueue(sensor_manager_, app_->looper,
		LOOPER_ID_USER, NULL, NULL);
}

void Engine::ProcessSensors(int32_t id)
{
	// If a sensor has data, process it now.
	if (id == LOOPER_ID_USER)
	{
		if (accelerometer_sensor_ != NULL)
		{
			ASensorEvent event;
			while (ASensorEventQueue_getEvents(sensor_event_queue_, &event, 1) > 0)
			{
			}
		}
	}
}

void Engine::ResumeSensors()
{
	// When our app gains focus, we start monitoring the accelerometer.
	if (accelerometer_sensor_ != NULL)
	{
		ASensorEventQueue_enableSensor(sensor_event_queue_, accelerometer_sensor_);
		// We'd like to get 60 events per second (in us).
		ASensorEventQueue_setEventRate(sensor_event_queue_, accelerometer_sensor_,
			(1000L / 60) * 1000);
	}
}

void Engine::SuspendSensors()
{
	// When our app loses focus, we stop monitoring the accelerometer.
	// This is to avoid consuming battery while not being used.
	if (accelerometer_sensor_ != NULL)
	{
		ASensorEventQueue_disableSensor(sensor_event_queue_, accelerometer_sensor_);
	}
}

//-------------------------------------------------------------------------
//Misc
//-------------------------------------------------------------------------
void Engine::SetState(android_app* state)
{
	app_ = state;
}

bool Engine::IsReady()
{
	if (has_focus_)
		return true;

	return false;
}


void Engine::ShowUI()
{
	//    JNIEnv *jni;
	//    app_->activity->vm->AttachCurrentThread( &jni, NULL );
	//
	//    //Default class retrieval
	//    jclass clazz = jni->GetObjectClass( app_->activity->clazz );
	//    jmethodID methodID = jni->GetMethodID( clazz, "showUI", "()V" );
	//    jni->CallVoidMethod( app_->activity->clazz, methodID );
	//
	//    app_->activity->vm->DetachCurrentThread();
	//    return;
}

void Engine::UpdateFPS(float fFPS)
{
	//    JNIEnv *jni;
	//    app_->activity->vm->AttachCurrentThread( &jni, NULL );
	//
	//    //Default class retrieval
	//    jclass clazz = jni->GetObjectClass( app_->activity->clazz );
	//    jmethodID methodID = jni->GetMethodID( clazz, "updateFPS", "(F)V" );
	//    jni->CallVoidMethod( app_->activity->clazz, methodID, fFPS );
	//
	//    app_->activity->vm->DetachCurrentThread();
	return;
}

Engine g_engine;

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(android_app* state)
{
	app_dummy();

	g_engine.SetState(state);

	//Init helper functions
	//ndk_helper::JNIHelper::Init( state->activity, HELPER_CLASS_NAME );

	state->userData = &g_engine;
	state->onAppCmd = Engine::HandleCmd;
	state->onInputEvent = Engine::HandleInput;

#ifdef USE_NDK_PROFILER
	monstartup("libTeapotNativeActivity.so");
#endif

	// Prepare to monitor accelerometer
	g_engine.InitSensors();

	// loop waiting for stuff to do.
	while (1)
	{
		// Read all pending events.
		int id;
		int events;
		android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((id = ALooper_pollAll(g_engine.IsReady() ? 0 : -1, NULL, &events, (void**)&source))
			>= 0)
		{
			// Process this event.
			if (source != NULL)
				source->process(state, source);

			g_engine.ProcessSensors(id);

			// Check if we are exiting.
			if (state->destroyRequested != 0)
			{
				g_engine.TermDisplay();
				return;
			}
		}

		if (g_engine.IsReady())
		{
			// Drawing is throttled to the screen update rate, so there
			// is no need to do timing here.
			g_engine.DrawFrame();
		}
	}
}
