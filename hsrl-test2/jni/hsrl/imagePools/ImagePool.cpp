/*
 * ImagePool.cpp
 *
 *  Created on: 2014-4-16
 *      Author: wsds
 */

#include "ImagePool.hpp"
namespace hsrl {

ImagePool::ImagePool() {
	// TODO Auto-generated constructor stub

}

ImagePool::~ImagePool() {
	// TODO Auto-generated destructor stub
}

static ImagePool* ImagePool::getInstance() {
	if (imagePool == NULL) {
		imagePool = new ImagePool();
	}
	return imagePool;
}

}
