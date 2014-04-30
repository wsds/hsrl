/*
 * SpaceHolder.cpp
 *
 *  Created on: 2014-4-16
 *      Author: wsds
 */

#include "SpaceHolder.hpp"
namespace hsrl {

#define  LOG_TAG    "SpaceHolder"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

SpaceHolder::SpaceHolder() {
//	this->imagePool = ImagePool::getInstance();
}

SpaceHolder::~SpaceHolder() {
	// TODO Auto-generated destructor stub
}

void SpaceHolder::test(){
	LOGI("test SpaceHolder");
}

}
