#include "mainOpenGL.h"

#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "prelibs/native_app_glue/android_native_app_glue.h"
#include "prelibs/math/Matrix4.h"

#include "hsrl/tools.h"
#include "MainEngine.h"


#define  LOG_TAG    "mainOpenGL"
#ifndef LOGI
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif /* LOGI */
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

namespace hsrl {
	static void printGLString(const char *name, GLenum s) {
		const char *v = (const char *)glGetString(s);
		LOGI("GL %s = %s\n", name, v);
	}

	static void checkGlError(const char* op) {
		for (GLint error = glGetError(); error; error = glGetError()) {
			LOGI("after %s() glError (0x%x)\n", op, error);
		}
	}

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
		LOGE("shader loadeed.");
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

		LOGE("program created.");
		return program;
	}

	GLuint gProgram;
	GLuint gvPositionHandle;


	GLuint mProjectionViewMatrixHandle;
	GLuint mModelMatrixHandle;
	GLuint maPositionHandle;
	GLuint maTextureHandle;

	std::string VertexShaderFilename = "hsrl/VertexShader.js";
	std::string FragmentShaderFilename = "hsrl/FragmentShader.js";

	static const char gVertexShader[] =
		"attribute vec4 aPosition; \n"
		"attribute vec2 aTextureCoord; \n"
		"varying vec2 vTextureCoord; \n"
		"uniform mat4 projectionView; \n"
		"uniform mat4 model; \n"
		"void main() {\n"
		"gl_Position = projectionView * model * aPosition; \n"
		"vTextureCoord = aTextureCoord; \n"
		"}\n";


	static const char gFragmentShader[] =
		"precision mediump float;\n"
		"varying vec2 vTextureCoord;\n"
		"uniform sampler2D sTexture;\n"
		"void main() {\n"
		"gl_FragColor = texture2D(sTexture, vTextureCoord);\n"
		"}\n";


	bool setupProgram(){

		hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();

		AAssetManager* assetManager = mMainEngine->assetManager;

		AAsset*  mVertexShaderFile = AAssetManager_open(assetManager, VertexShaderFilename.c_str(), AASSET_MODE_BUFFER);
		const void* mVertexShaderBuffer = AAsset_getBuffer(mVertexShaderFile);
		char* mVertexShaderBufferChar = (char*)mVertexShaderBuffer;


		AAsset*  mFragmentShaderFile = AAssetManager_open(assetManager, FragmentShaderFilename.c_str(), AASSET_MODE_BUFFER);
		const void* mFragmentShaderBuffer = AAsset_getBuffer(mFragmentShaderFile);
		char* mFragmentShaderBufferChar = (char*)mFragmentShaderBuffer;


		//gProgram = createProgram(mVertexShaderBufferChar, mFragmentShaderBufferChar);

		gProgram = createProgram(gVertexShader, gFragmentShader);
		if (!gProgram) {
			LOGE("Could not create program.");
			return false;
		}
		//gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
		//checkGlError("glGetAttribLocation");
		//LOGI("glGetAttribLocation(\"vPosition\") = %d\n", gvPositionHandle);

		maPositionHandle = glGetAttribLocation(gProgram, "aPosition");
		maTextureHandle = glGetAttribLocation(gProgram, "aTextureCoord");
		mProjectionViewMatrixHandle = glGetUniformLocation(gProgram, "projectionView");
		mModelMatrixHandle = glGetUniformLocation(gProgram, "model");

		return true;
	}

	//float mModelMatrixMove[16];
	//float mProjectionViewMatrix[16];
	//float mProjectionMatrix[16];
	//float mViewMatrix[16];

	// mModelMatrix模型矩阵；mViewMatrix视图矩阵；mProjectionMatrix透视矩阵；
	Matrix4* mModelMatrixMove;
	Matrix4* mProjectionViewMatrix;
	Matrix4* mProjectionMatrix;
	Matrix4* mViewMatrix;
	Matrix4Helper* mMatrix4Helper;

	void setupProjectionView(int width, int height){
		mMatrix4Helper = Matrix4Helper::getInstance();

		glViewport(0, 0, width, height);
		checkGlError("glViewport");

		float ratio = (float)width / height;

		mViewMatrix = mMatrix4Helper->setLookAtM(0.0f, 0.0f, 1.0f/* look at z=1 */, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		mProjectionMatrix = mMatrix4Helper->OrthoM(-1.0f, 1.0f, -1.0f / ratio, 1.0f / ratio, 1.0f, 11.0f);// 正交投影//near at z=1//far at z=11
		mProjectionViewMatrix = mMatrix4Helper->multiplyMM(mProjectionMatrix, mViewMatrix);

		GLfloat* value = mMatrix4Helper->getDataCopy(mProjectionViewMatrix);
		glUniformMatrix4fv(mProjectionViewMatrixHandle, 1, false, value);
	}

	SpaceHolder* spaceHolder;
	bool setupGraphics(int width, int height) {
		printGLString("Version", GL_VERSION);
		printGLString("Vendor", GL_VENDOR);
		printGLString("Renderer", GL_RENDERER);
		printGLString("Extensions", GL_EXTENSIONS);

		LOGI("setupGraphics(%d, %d)", width, height);

		if (!setupProgram()){
			return false;
		}

		setupProjectionView(width, height);

		spaceHolder = new SpaceHolder();
		spaceHolder->initialize(width, height, mModelMatrixHandle);

		LOGE("setupGraphics done.");
		return true;
	}

	const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
		0.5f, -0.5f };
	const GLfloat mTriangleVerticesData[] = {
		// X, Y, Z, U, V
		// 1
		0.0f, -1.0f, 0, 0.0f, 1.0f,
		// 2
		1.0f, -1.0f, 0, 1.0f, 1.0f,
		// 3
		0.0f, 0.0f, 0, 0.0f, 0.0f,
		// 4
		1.0f, 0.0f, 0, 1.0f, 0.0f

	};

	int testFlag = 600;
	int count = 0;
	void test1(){
		testFlag--;
		if (testFlag < 0){
			testFlag = 60;
			LOGI("renderFrame : %d", count++);
		}
	}
	long renderCount = 0;
	bool isInitailized = false;

	const int FLOAT_SIZE_BYTES = 4;
	const int TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
	const int TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;
	const int TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;

	void renderFrame() {
		hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();
		if (mMainEngine->display == NULL) {
			// No display.
			LOGI("No display");
			return;
		}

		renderCount++;

		// GLES20.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClearColor(0.0f, 0.6f, 0.804f, 1.0f);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		if (isInitailized == false) {

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glActiveTexture(GL_TEXTURE0);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);

			glUseProgram(gProgram);

			const GLfloat* mTriangleVertices = mTriangleVerticesData + TRIANGLE_VERTICES_DATA_POS_OFFSET;

			glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT, false, TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
			glEnableVertexAttribArray(maPositionHandle);

			const GLfloat* mTriangleVerticesUV = mTriangleVerticesData + TRIANGLE_VERTICES_DATA_UV_OFFSET;

			glVertexAttribPointer(maTextureHandle, 2, GL_FLOAT, false, TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVerticesUV);
			glEnableVertexAttribArray(maTextureHandle);


			//GLfloat* value = mProjectionViewMatrix->Ptr();
			//glUniformMatrix4fv(mProjectionViewMatrixHandle, 1, false, value);
			//glUniformMatrix4fv(mProjectionViewMatrixHandle, 1, false, mProjectionViewMatrix->Ptr());//重复？
			GLfloat* value = mMatrix4Helper->getDataCopy(mProjectionViewMatrix);
			glUniformMatrix4fv(mProjectionViewMatrixHandle, 1, false, value);
			isInitailized = true;
		}

		////// spaceHolder.drawImage("snow_d_blur.jpg", 0, 0, spaceHolder.width,
		////// spaceHolder.height);

		////// spaceHolder.drawImage("test2.png", 0, 0, 50, 50);
		////// spaceHolder.drawImage("face_man.png", 100, 200, 250, 250);
		////// for (int i = 1; i < 20; i++) {
		////// spaceHolder.drawImage("test2.png", 0, i * 60, 50, 50);
		////// }

		////Matrix.setIdentityM(mModelMatrixMove, 0);

		////float offset_x_move = 0;
		////float offset_y_move = 0;

		////offset_x_move = spaceHolder.screen_offset_x + (x_move - 48) * spaceHolder.Pi;
		////offset_y_move = spaceHolder.screen_offset_y - (y_move - 48) * spaceHolder.Pi;

		////// offset_x_move = x_move * spaceHolder.Pi;
		//////
		////// offset_y_move = -y_move * spaceHolder.Pi;

		//////Matrix.translateM(mModelMatrixMove, 0, offset_x_move, offset_y_move, -8.0f);
		//////spaceHolder.renderWorld();
		spaceHolder->drawImage("emoji_normal.png", 100, 600, 0, 96, 96);

		eglSwapBuffers(mMainEngine->display, mMainEngine->surface);


	}

	//hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();

	/**
	* Initialize an EGL context for the current display.
	*/
	int engine_init_display() {
		hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();
		//	 initialize OpenGL ES and EGL

		/*
		* Here specify the attributes of the desired configuration.
		* Below, we select an EGLConfig with at least 8 bits per color
		* component compatible with on-screen windows
		*/
		const EGLint attribs[] = { EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE,
			8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_NONE };
		EGLint w, h, dummy, format;
		EGLint numConfigs;
		EGLConfig config;
		EGLSurface surface;
		EGLContext context;

		EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(display, 0, 0);

		/* Here, the application chooses the configuration it desires. In this
		* sample, we have a very simplified selection process, where we pick
		* the first EGLConfig that matches our criteria */
		eglChooseConfig(display, attribs, &config, 1, &numConfigs);

		/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		* As soon as we picked a EGLConfig, we can safely reconfigure the
		* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
		eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

		ANativeWindow_setBuffersGeometry(mMainEngine->app->window, 0, 0, format);

		surface = eglCreateWindowSurface(display, config, mMainEngine->app->window, NULL);


		EGLint contextAtt[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
		context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAtt);

		if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
			LOGI("Unable to eglMakeCurrent");
			return -1;
		}

		eglQuerySurface(display, surface, EGL_WIDTH, &w);
		eglQuerySurface(display, surface, EGL_HEIGHT, &h);

		mMainEngine->display = display;
		mMainEngine->context = context;
		mMainEngine->surface = surface;
		mMainEngine->width = w;
		mMainEngine->height = h;
		mMainEngine->isReady = true;

		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		//Initialize GL state.
		/*	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
			glEnable(GL_CULL_FACE);
			glShadeModel(GL_SMOOTH);
			glDisable(GL_DEPTH_TEST);*/
		LOGE("engine_init_display done.");

		hsrl::setupGraphics(w, h);
		//hsrl::renderFrame();

		return 0;
	}

	void engine_term_display() {
		hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();

		if (mMainEngine->display != EGL_NO_DISPLAY) {
			eglMakeCurrent(mMainEngine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			if (mMainEngine->context != EGL_NO_CONTEXT) {
				eglDestroyContext(mMainEngine->display, mMainEngine->context);
			}
			if (mMainEngine->surface != EGL_NO_SURFACE) {
				eglDestroySurface(mMainEngine->display, mMainEngine->surface);
			}
			eglTerminate(mMainEngine->display);
		}
		mMainEngine->animating = 0;
		mMainEngine->display = EGL_NO_DISPLAY;
		mMainEngine->context = EGL_NO_CONTEXT;
		mMainEngine->surface = EGL_NO_SURFACE;
		mMainEngine->isReady = false;

		LOGE("engine_term_displays.");
	}

}