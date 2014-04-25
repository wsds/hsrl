#include "mainOpenGL.h"

#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "native_app_glue/android_native_app_glue.h"

#include "hsrl/tools.h"
#include "MainEngine.h"


#define  LOG_TAG    "mainOpenGL"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
namespace hsrl {
	static void printGLString(const char *name, GLenum s) {
		const char *v = (const char *)glGetString(s);
		LOGI("GL %s = %s\n", name, v);
	}

	static void checkGlError(const char* op) {
		for (GLint error = glGetError(); error; error
			= glGetError()) {
			LOGI("after %s() glError (0x%x)\n", op, error);
		}
	}

	static const char gVertexShader[] =
		"attribute vec4 vPosition;\n"
		"void main() {\n"
		"  gl_Position = vPosition;\n"
		"}\n";

	static const char gFragmentShader[] =
		"precision mediump float;\n"
		"void main() {\n"
		"  gl_FragColor = vec4(1.0, 1.0, 0.0, 0.0);\n"
		"}\n";

	GLuint loadShader(GLenum shaderType, const char* pSource) {
		GLuint shader = glCreateShader(shaderType);
		if (shader) {
			glShaderSource(shader, 1, &pSource, NULL);
			glCompileShader(shader);
			GLint compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
			if (!compiled) {
				GLint infoLen = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
				if (infoLen) {
					char* buf = (char*)malloc(infoLen);
					if (buf) {
						glGetShaderInfoLog(shader, infoLen, NULL, buf);
						LOGE("Could not compile shader %d:\n%s\n",
							shaderType, buf);
						free(buf);
					}
					glDeleteShader(shader);
					shader = 0;
				}
			}
		}
		return shader;
	}

	GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
		GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
		if (!vertexShader) {
			return 0;
		}

		GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
		if (!pixelShader) {
			return 0;
		}

		GLuint program = glCreateProgram();
		if (program) {
			glAttachShader(program, vertexShader);
			checkGlError("glAttachShader");
			glAttachShader(program, pixelShader);
			checkGlError("glAttachShader");
			glLinkProgram(program);
			GLint linkStatus = GL_FALSE;
			glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
			if (linkStatus != GL_TRUE) {
				GLint bufLength = 0;
				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
				if (bufLength) {
					char* buf = (char*)malloc(bufLength);
					if (buf) {
						glGetProgramInfoLog(program, bufLength, NULL, buf);
						LOGE("Could not link program:\n%s\n", buf);
						free(buf);
					}
				}
				glDeleteProgram(program);
				program = 0;
			}
		}
		return program;
	}

	GLuint gProgram;
	GLuint gvPositionHandle;

	bool setupGraphics(int w, int h) {
		printGLString("Version", GL_VERSION);
		printGLString("Vendor", GL_VENDOR);
		printGLString("Renderer", GL_RENDERER);
		printGLString("Extensions", GL_EXTENSIONS);

		LOGI("setupGraphics(%d, %d)", w, h);
		gProgram = createProgram(gVertexShader, gFragmentShader);
		if (!gProgram) {
			LOGE("Could not create program.");
			return false;
		}
		gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
		checkGlError("glGetAttribLocation");
		LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
			gvPositionHandle);

		glViewport(0, 0, w, h);
		checkGlError("glViewport");
		return true;
	}

	const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
		0.5f, -0.5f };


	int testFlag = 600;
	void test1(){
		testFlag--;
		if (testFlag < 0){
			testFlag = 60;
			hsrl::log_information("hello", 10);
		}
	}

	void renderFrame() {
		static float grey;
		grey += 0.01f;
		if (grey > 1.0f) {
			grey = 0.0f;
		}
		glClearColor(grey, grey, grey, 1.0f);
		checkGlError("glClearColor");
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		checkGlError("glClear");

		glUseProgram(gProgram);
		checkGlError("glUseProgram");

		glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
		checkGlError("glVertexAttribPointer");
		glEnableVertexAttribArray(gvPositionHandle);
		checkGlError("glEnableVertexAttribArray");
		glDrawArrays(GL_TRIANGLES, 0, 3);
		checkGlError("glDrawArrays");
		test1();
	}

	//hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();

	/**
	* Initialize an EGL context for the current display.
	*/
	int engine_init_display() {
		//hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();
		// initialize OpenGL ES and EGL

		/*
		* Here specify the attributes of the desired configuration.
		* Below, we select an EGLConfig with at least 8 bits per color
		* component compatible with on-screen windows
		*/
	//	const EGLint attribs[] = { EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE,
	//		8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_NONE };
	//	EGLint w, h, dummy, format;
	//	EGLint numConfigs;
	//	EGLConfig config;
	//	EGLSurface surface;
	//	EGLContext context;

	//	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	//	eglInitialize(display, 0, 0);

	//	/* Here, the application chooses the configuration it desires. In this
	//	* sample, we have a very simplified selection process, where we pick
	//	* the first EGLConfig that matches our criteria */
	//	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	//	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	//	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	//	* As soon as we picked a EGLConfig, we can safely reconfigure the
	//	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	//	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	//	ANativeWindow_setBuffersGeometry(mMainEngine->app->window, 0, 0, format);

	//	surface = eglCreateWindowSurface(display, config, mMainEngine->app->window,
	//		NULL);
	//	context = eglCreateContext(display, config, NULL, NULL);

	//	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
	//		LOGI("Unable to eglMakeCurrent");
	//		return -1;
	//	}

	//	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	//	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	//	mMainEngine->display = display;
	//	mMainEngine->context = context;
	//	mMainEngine->surface = surface;
	//	mMainEngine->width = w;
	//	mMainEngine->height = h;

	//	 Initialize GL state.
	///*	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//	glEnable(GL_CULL_FACE);
	//	glShadeModel(GL_SMOOTH);
	//	glDisable(GL_DEPTH_TEST);*/

		return 0;
	}

	void engine_term_display() {
		//hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();

		//if (mMainEngine->display != EGL_NO_DISPLAY) {
		//	eglMakeCurrent(mMainEngine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		//	if (mMainEngine->context != EGL_NO_CONTEXT) {
		//		eglDestroyContext(mMainEngine->display, mMainEngine->context);
		//	}
		//	if (mMainEngine->surface != EGL_NO_SURFACE) {
		//		eglDestroySurface(mMainEngine->display, mMainEngine->surface);
		//	}
		//	eglTerminate(mMainEngine->display);
		//}
		//mMainEngine->animating = 0;
		//mMainEngine->display = EGL_NO_DISPLAY;
		//mMainEngine->context = EGL_NO_CONTEXT;
		//mMainEngine->surface = EGL_NO_SURFACE;
	}

}