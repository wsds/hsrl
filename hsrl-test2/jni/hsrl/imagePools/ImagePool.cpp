/*
* ImagePool.cpp
*
*  Created on: 2014-4-16
*      Author: wsds
*/

#include "ImagePool.hpp"
#include "./../../MainEngine.h"
#include "lodepng\lodepng.h"

#include "prelibs\mySTL\MyMap.h"

//extern template class MyMap<long, int>;
//template class MyMap<long, int>;


namespace hsrl {





	ImagePool* ImagePool::instance = NULL;

	ImagePool::ImagePool() {
		// TODO Auto-generated constructor stub
		//this->images = new MyMap<std::string, Image*>();
		//this->images = new MyMap<int>();
		//MyMap<long> test;
		//MyMap<int,int>* images121 = new MyMap<int,int>();
		//template class MyMap<long, int>;
		//MyMap<long, int>* images12 = new MyMap<long, int>();


	}

	ImagePool::~ImagePool() {
		// TODO Auto-generated destructor stub
	}

	void ImagePool::readImageFromApk(const char* filename, Image* image){

		hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();

		AAssetManager* assetManager = mMainEngine->assetManager;

		AAsset* test1File = AAssetManager_open(assetManager, filename, AASSET_MODE_BUFFER);

		const void* buffer = AAsset_getBuffer(test1File);
		unsigned char* bufferChar = (unsigned char*)buffer;

		size_t buffersize = AAsset_getLength(test1File);

		unsigned errorCode = lodepng_decode32(&image->bitmap, &image->width, &image->height, bufferChar, buffersize);

	}

	void ImagePool::test(){
		const char* filename = "emoji_1.png";
		Image* image = new Image();
		readImageFromApk(filename, image);

		//(*this->images)[filename] = image;
	}

}
