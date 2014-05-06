/*
* ImagePool.cpp
*
*  Created on: 2014-4-16
*      Author: wsds
*/

#include "ImagePool.hpp"
#include "./../../MainEngine.h"
#include "lodepng\lodepng.h"

#include <math.h>

#include <android/log.h>

namespace hsrl {

#define  LOG_TAG    "ImagePool"

#ifndef LOGI
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif /* LOGI */

	ImagePool* ImagePool::instance = NULL;

	ImagePool::ImagePool() {
		this->images = new MyMap<std::string, Image*>();
		Image* image = this->loadImage("loading.png");
		loadingTextureID = image->textureID;

		LOGI("One instance of {ImagePool} created");
	}

	ImagePool::~ImagePool(){

	}

	int ImagePool::getImage(std::string key){


		LOGI("Getting image(%s)", key.c_str());
		int textureID = loadingTextureID;
		Image** ppimage = this->images->get(key);

		if (ppimage == NULL) {
			textureID = loadingTextureID;
			// TODO load image in other thread.
			Image* image = this->loadImage(key);
		}
		else {
			Image* image = *ppimage;
			if (image->status != STATUS_ONGPU) {
				textureID = loadingTextureID;
			}
			else {
				textureID = image->textureID;
			}
		}
		return textureID;
	}


	Image* ImagePool::loadImage(std::string key){

		Image* image = new Image();
		image->key = &key;
		image->status = STATUS_PRELOAD;
		this->readImageFromApk(key.c_str(), image);
		image->status = STATUS_ONRAM;
		this->createTextureWithPixels(image);
		image->status = STATUS_ONGPU;
		this->images->put(key, image);

	}

	void ImagePool::readImageFromApk(const char* filename, Image* image){

		hsrl::MainEngine* mMainEngine = hsrl::MainEngine::getInstance();

		AAssetManager* assetManager = mMainEngine->assetManager;

		AAsset* file = AAssetManager_open(assetManager, filename, AASSET_MODE_BUFFER);

		const void* buffer = AAsset_getBuffer(file);
		unsigned char* bufferChar = (unsigned char*)buffer;

		size_t buffersize = AAsset_getLength(file);

		unsigned char* bitmap;

		unsigned errorCode = lodepng_decode32(&bitmap, &image->width, &image->height, bufferChar, buffersize);


		// The internal (real) size of the texture needs to be a power of two
		image->width2pow = pow(2, ceil(log10((double)image->width) / log10(2.0)));
		image->height2pow = pow(2, ceil(log10((double)image->height) / log10(2.0)));


		if (image->width == image->width2pow && image->height == image->height2pow) {
			image->bitmap = bitmap;
		}

		// Copy the original pixels into the upper left corner of a larger
		// (power of 2) pixel buffer, free the original pixels and return
		// the larger buffer
		else {
			image->bitmap = (unsigned char*)calloc(image->width2pow * image->height2pow * 4, sizeof(unsigned char*));

			for (int y = 0; y < image->height; y++) {
				memcpy(&image->bitmap[y*image->width2pow * 4], &bitmap[y*image->width * 4], image->width * 4);
			}

			free(bitmap);
		}

	}

	void ImagePool::test(){
		std::string filename = "emoji_1.png";
		Image* image = new Image();
		image->key = &filename;
		this->readImageFromApk(filename.c_str(), image);
		this->createTextureWithPixels(image);

		//*(*images)[filename] = image;

		images->put(filename, image);
	}


	void ImagePool::createTextureWithPixels(Image* image) {
		unsigned char * pixels = image->bitmap;
		// Release previous texture if we had one
		if (image->textureID) {
			glDeleteTextures(1, (const GLuint*)&(image->textureID));
			image->textureID = 0;
		}

		GLint maxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

		if (image->width2pow > maxTextureSize || image->height2pow > maxTextureSize) {

			LOGI("Warning: Image %s larger than MAX_TEXTURE_SIZE (%d)", image->key->c_str(), maxTextureSize);
		}
		GLenum format = GL_RGBA;

		bool wasEnabled = glIsEnabled(GL_TEXTURE_2D);
		int boundTexture = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, (GLuint*)&(image->textureID));
		glBindTexture(GL_TEXTURE_2D, image->textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, image->width2pow, image->height2pow, 0, format, GL_UNSIGNED_BYTE, pixels);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, boundTexture);

		if (!wasEnabled) {
			glDisable(GL_TEXTURE_2D);
		}

		free(image->bitmap);
	}
}
