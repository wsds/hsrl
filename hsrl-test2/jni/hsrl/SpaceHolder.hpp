/*
 * SpaceHolder.h
 *
 *  Created on: 2014-4-16
 *      Author: wsds
 */

#ifndef SPACEHOLDER_H_
#define SPACEHOLDER_H_

#ifndef NULL
#define NULL 0;
#endif /* NULL */

#include "imagePools/ImagePool.hpp"
#include "./../prelibs/math/vecmath.h"
#include <android/log.h>

namespace hsrl {

	class SpaceHolder {
	public:
		SpaceHolder();
		virtual ~SpaceHolder();

		static SpaceHolder *instance;

		static SpaceHolder * getInstance()
		{
			if (instance == NULL) { //判断是否第一次调用  
				instance = new SpaceHolder();
			}
			return instance;
		}

		void initialize(float width, float height, int mModelMatrixHandle);

		void drawImage(std::string key, float left, float top, float z, float width, float height);
		void test();

		ImagePool* imagePool;

		float width;
		float height;
		float ratio;
		float Pi;

		float screen_offset_x;
		float screen_offset_y;

		float x0;// touch_down
		float y0;

		float x;// touch_move
		float y;

		float x1;// touch_up
		float y1;

		float ax;// Acceleration
		float ay;

		float dx;
		float dy;

		int mModelMatrixHandle;
		Mat4* mModelMatrixMove;

		float angle;
		float offset_x;
		float offset_y;
		float offset_z;
		float offset_w;
		float offset_h;
	};

}
#endif /* SPACEHOLDER_H_ */

