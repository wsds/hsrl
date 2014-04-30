/*
* ImagePool.h
*
*  Created on: 2014-4-16
*      Author: wsds
*/

#ifndef IMAGEPOOL_H_
#define IMAGEPOOL_H_

//#define NULL 0

//#include <vector>
#include <string>
#include "prelibs\mySTL\MyMap.h"

namespace hsrl {

#define STATUS_PRELOAD 0
#define STATUS_ONRAM 1
#define STATUS_ONGPU 2
#define STATUS_ONGPUFRAMEBUFER 3

	class Image {
	public:

		unsigned width;
		unsigned height;
		int textureID;
		//Bitmap bitmap;
		unsigned char* bitmap;

		int status;

		float u1;
		float v1;
		float u2;
		float v2;
	};



	class ImagePool {
	public:
		ImagePool();
		virtual ~ImagePool();

		static ImagePool *instance;

		static ImagePool * getInstance()
		{
			if (instance == NULL) { //�ж��Ƿ��һ�ε���  
				instance = new ImagePool();
			}
			return instance;
		}


		int loadingTextureID;
		//MyMap<std::string, Image*>* images;
		//MyMap<int, int>* images;

		void readImageFromApk(const char* filename, Image* image);

		void test();

	};
}

#endif /* IMAGEPOOL_H_ */
