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

#include "imagePools\ImagePool.hpp"

#include ".\..\prelibs\mySTL\MyMap.h"

namespace hsrl {

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "tools", __VA_ARGS__))



	////test for ImagePool

	void test15(){
		ImagePool * imagePool = ImagePool::getInstance();

		imagePool->test();
	}

	////test for MyMap
	void test14(){
		MyMap<std::string, std::string> images11;

		//insert
		images11.put("b", "a12");

		//get
		std::string* value1 = images11["b"];
		std::string* value2 = images11["c"];//value2==NULL
		LOGI("children['b'] is %s", value1->c_str());


		//modify
		if (images11["b"] != NULL){
			*images11["b"] = "a24";
			LOGI("children['b'] is %s", images11["b"]->c_str());
		}
		//delete
		images11.put("c", "a36");
		images11.put("d", "a48");

		images11.del("d");

		if (images11["c"] != NULL){
			LOGI("children['c'] is %s", images11["c"]->c_str());
		}
		if (images11["d"] == NULL){
			LOGI("children['d'] is NULL");
		}


		images11.put("c", "a60");
		images11.put("d", "a72");

		images11.del("c");
		if (images11["d"] != NULL){
			LOGI("children['d'] is %s", images11["d"]->c_str());
		}
		if (images11["c"] == NULL){
			LOGI("children['c'] is NULL");
		}

	}

	void test13(){
		MyMap<std::string, float> images11;

		//images11["b"] = "a12";
		images11.put("b", 0.12);

		float* value1 = images11.get("b");
		LOGI("children['b'] is %f", *value1);

		float* value2 = images11.get("c");
		if (NULL != value2){
			LOGI("children['c'] is %f", *value2);
		}
		else{
			LOGI("children['c'] is NULL");
		}

		(*value1) = 0.24;

		float* value3 = images11.get("b");
		LOGI("children['b'] is %f", *value3);

	}

	void test12(){
		MyMap<std::string, std::string> images11;

		//images11["b"] = "a12";
		images11.put("b", "a12");

		std::string* value1 = images11.get("b");
		LOGI("children['b'] is %s", value1->c_str());

		std::string* value2 = images11.get("c");
		if (NULL != value2){
			LOGI("children['c'] is %s", value2->c_str());
		}
		else{
			LOGI("children['c'] is NULL");
		}

		(*value1) = "a24";

		std::string* value3 = images11.get("b");
		LOGI("children['b'] is %s", value3->c_str());

	}

	void test11(){
		MyMap<std::string, std::string> images11;

		//images11["b"] = "a12";
		images11.put("b", "a12");

		bool hasb = images11.exist("b");
		bool hasc = images11.exist("c");

		std::string* value1 = images11["b"];


		LOGI("children['b'] is %s", value1->c_str());


		std::string* value2 = images11["c"];

		if (value2 != NULL){
			LOGI("children['c'] is %s", value2->c_str());
		}

	}

	void test10(){

		MyMap<std::string, std::string> images11;

		//*images11["b"] = "a12";
		images11.put("b", "a12");

		std::string* value1 = images11["b"];

		LOGI("children['b'] is %s", value1->c_str());

		*images11["b"] = "a24";

		std::string* value2 = images11["b"];

		LOGI("children['b'] is %s", value2->c_str());


		std::string* value3 = images11["c"];

		if (value3 != NULL){
			LOGI("children['c'] is %s", value3->c_str());
		}



	}



	void test9(){

		int a1 = 10;
		int* p = &a1;
		int a = *p;
		LOGI("a is %d", a);

		int b1 = 50;
		p = &b1;
		int b = *p;
		LOGI("b is %d", b);

		LOGI("a is %d; b is %d", a, b);

		*p = 980;
		LOGI("a is %d; b is %d", a, b);

		LOGI("*p is %d; b1 is %d", *p, b1);
		//std::map<std::string, int>* map;
	}


	void test8(){
		//test <string, int>
		MyMap<std::string, int>* images12 = new MyMap<std::string, int>();

		images12->put("a", 12);

		////(*children)["a"] = 12;

		int* value = images12->get("a");

		LOGI("children['a'] is %d", *value);

		*value = 24;

		int* value2 = images12->get("a");

		LOGI("children['a'] is %d", *value2);


		//test <string, string>
		MyMap<std::string, std::string>* images13 = new MyMap<std::string, std::string>();

		images13->put("b", "a12");

		std::string* value3 = images13->get("b");

		LOGI("children['b'] is %s", value3->c_str());

		*value3 = "a24";

		std::string* value4 = images13->get("b");

		LOGI("children['b'] is %s", value4->c_str());

		//test <string, string> operator[]
		MyMap<std::string, std::string>* images14 = new MyMap<std::string, std::string>();

		images14->put("b", "a36");

		std::string* value5 = (*images14)["b"];

		LOGI("children['b'] is %s", value5->c_str());

		*(*images14)["b"] = "a48";

		std::string* value6 = (*images14)["b"];

		LOGI("children['b'] is %s", value6->c_str());


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
			//test8();
			//test10();
			//test11();
			//test12();
			//test13();

			test15();
			logCount = 0;
		}
	}

}
