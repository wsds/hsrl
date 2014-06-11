/*
 * SpaceHolder.cpp
 *
 *  Created on: 2014-4-16
 *      Author: wsds
 */

#include "SpaceHolder.hpp"
#include "StringTools.h"

namespace hsrl {

#define  LOG_TAG    "SpaceHolder"
#ifndef LOGI
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif /* LOGI */


	SpaceHolder::SpaceHolder() {
		this->imagePool = ImagePool::getInstance();
		this->mMatrix4Helper = Matrix4Helper::getInstance();
		LOGI("One instance of {SpaceHolder} created");
	}
	SpaceHolder::~SpaceHolder(){

	}

	void SpaceHolder::initialize(float width, float height, int mModelMatrixHandle){

		this->width = width;
		this->height = height;
		float ratio = (float)width / height;;
		this->ratio = ratio;
		this->Pi = 2.0f / width;

		this->screen_offset_x = -1.0f;
		this->screen_offset_y = 1.0f / ratio;

		this->mModelMatrixHandle = mModelMatrixHandle;

		mModelMatrixMove = Matrix4Helper::getInstance()->Identity();
		angle = 0.0f;
		offset_x = 0.0f;
		offset_y = 0.0f;
		offset_z = 0.0f;
		offset_w = 0.0f;
		offset_h = 0.0f;
		LOGI("{SpaceHolder} is initialized");
	}


	//Mat4* mModelMatrixMove;
	void SpaceHolder::drawImage(std::string key, float left, float top, float z, float width, float height){
		int textureID = this->imagePool->getImage(key);
		glBindTexture(GL_TEXTURE_2D, textureID);

		offset_x = this->screen_offset_x + left * this->Pi;
		offset_y = this->screen_offset_y - top * this->Pi;
		offset_z = -10.0f + z / 10.0f;

		offset_w = 2 * width / this->width;
		offset_h = 2 * height / this->width;

		mMatrix4Helper->setRotateM(mModelMatrixMove, angle, 0.0f, 0.0f, 1.0f);
		mMatrix4Helper->translateM(mModelMatrixMove, offset_x, offset_y, offset_z);
		mMatrix4Helper->scaleM(mModelMatrixMove, offset_w, offset_h, 1.0f);

		GLfloat* value = mMatrix4Helper->getDataCopy(mModelMatrixMove);
		glUniformMatrix4fv(mModelMatrixHandle, 1, false, value);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void SpaceHolder::drawText(std::string& text, float left, float top, float z,
			float textSize, float width, float height) {

		unsigned length = computeUnicodeLengthFromUTF8(text.data(), text.length());
		UNICODE_CHAR * unicode = new UNICODE_CHAR[length];

		if (!UTF8ToUnicode(unicode, length, text.data(), text.length())) {
			return;
		}


		int textureID = this->imagePool->getCharImage(unicode[0]);
		delete[] unicode;

		glBindTexture(GL_TEXTURE_2D, textureID);
		offset_x = this->screen_offset_x + left * this->Pi;
		offset_y = this->screen_offset_y - top * this->Pi;
		offset_z = -10.0f + z / 10.0f;

		offset_w = 2 * width / this->width;
		offset_h = 2 * height / this->width;

		mMatrix4Helper->setRotateM(mModelMatrixMove, angle, 0.0f, 0.0f, 1.0f);
		mMatrix4Helper->translateM(mModelMatrixMove, offset_x, offset_y, offset_z);
		mMatrix4Helper->scaleM(mModelMatrixMove, offset_w, offset_h, 1.0f);

		GLfloat* value = mMatrix4Helper->getDataCopy(mModelMatrixMove);
		glUniformMatrix4fv(mModelMatrixHandle, 1, false, value);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}


	void SpaceHolder::test(){
		LOGI("test SpaceHolder");
	}

}
