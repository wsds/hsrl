#include "tools.h"

#include <android/log.h>

#include "./../MainEngine.h"

//#include "libs/testso.h"
//#include "add.h"

#include <string>
#include <map>

extern "C"
{
#include "./../prebuilt\add.h"
#include "./../prebuilt\sooo1.h"
}
#include "./../MainEngine.h"

#include "imagePools\lodepng\lodepng.h"

#include "hsrl\imagePools\ImagePool.hpp"

#include ".\..\prelibs\mySTL\MyMap.h"

namespace hsrl {

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "tools", __VA_ARGS__))


	void test8(){

		MyMap<std::string, std::string>* images12 = new MyMap<std::string, std::string>();

		images12->put("a", "safasfsafasf");

		//(*children)["a"] = 12;

		std::string value = images12->get("a");

		LOGI("children['a'] is %s", value.c_str());

	}

	void test7(){
		hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();

		ImagePool* imagePool = mMainEngine->imagePool;

		imagePool->test();

	}

	void test5(){

		hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();
		struct android_app* app = mMainEngine->app;
		struct ANativeActivity* activity = app->activity;

		AAssetManager* assetManager = activity->assetManager;

		AAsset* test1File = AAssetManager_open(assetManager, "emoji_1.png", AASSET_MODE_BUFFER);

		const void* buffer = AAsset_getBuffer(test1File);
		unsigned char* bufferChar = (unsigned char*)buffer;

		size_t buffersize = AAsset_getLength(test1File);

		//LOGI("AAsset_getBuffer is %s", bufferChar);

		unsigned width = 0;
		unsigned height = 0;

		unsigned char* out;

		unsigned errorCode = lodepng_decode32(&out, &width, &height, bufferChar, buffersize);



		const char* internalDataPath = activity->internalDataPath;

		//std::string internalDataPathStr = internalDataPath;

		//std::string packetNameStr = internalDataPathStr.substr(11, internalDataPathStr.length() - 17);

		//LOGI("internalDataPath is %s", internalDataPathStr.c_str());
		//LOGI("packetName is %s", packetNameStr.c_str());

	}


	void test6(){
		std::map<std::string, int>* children = new std::map<std::string, int>();

		(*children)["a"] = 12;

		LOGI("children['a'] is %d", (*children)["a"]);


		//std::map<std::string, int> mymap;

		//mymap.at("alpha") = 10;
		//mymap.at("beta") = 20;
		//mymap.at("gamma") = 30;


		//LOGI("mymap['alpha'] is %d", mymap['alpha']);

	}

	void test4(){

		int y = 10;
		//y = add(10, 20);
		y = add2(3254);
		LOGI("You have done %d", y);
	}


	void test3(){
		int y = 23;
		//y = add(10, 20);
		//y = add2(30);
		LOGI("MyTestFunction%d", y);
	}

	void test() {
		SpaceHolder* spaceHolder = new SpaceHolder();
		spaceHolder->test();
		hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();

	}

	int logCount = 0;
	void log_information(const char *text_format, int param) {

		char logCountText[200];
		//	sprintf(logCountText, "log 16:�?d�?---", logCount++);

		LOGI("log 15:Hello%d", logCount++);
		if (logCount == 10){
			//test3();
			test8();
		}
	}

}
