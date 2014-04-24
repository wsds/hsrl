/*
 * ImagePool.h
 *
 *  Created on: 2014-4-16
 *      Author: wsds
 */

#ifndef IMAGEPOOL_H_
#define IMAGEPOOL_H_

#define NULL 0

//#include <vector>
//#include <map>
//#include <string>

namespace hsrl {

class ImagePool {
public:
	ImagePool();
	virtual ~ImagePool();

	static ImagePool imagePool;

	static ImagePool* getInstance();
};

}

#endif /* IMAGEPOOL_H_ */
