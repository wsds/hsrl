/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef VECMATH_H_
#define VECMATH_H_

#include <android/log.h>

#include <math.h>

#define  LOG_TAG    "Matrix4"
#ifndef LOGI
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#endif /* LOGI */

#ifndef NULL
#define NULL 0;
#endif /* NULL */

namespace hsrl
{

	class Matrix4{
	public:
		float* data;
		//public:
		Matrix4();

	};

	class Matrix4Helper{
	public:
		static Matrix4Helper *instance;

		static Matrix4Helper * getInstance()
		{
			if (instance == NULL) {  
				instance = new Matrix4Helper();
			}
			return instance;
		}

		float* getDataCopy(Matrix4* matrix);


		Matrix4* Identity();
		Matrix4* OrthoM(float left, float right, float bottom, float top, float near, float far);
		Matrix4* setLookAtM(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);

		Matrix4* multiplyMM(Matrix4* matrixL, Matrix4* matrixR);

		void translateM(Matrix4* result, float x, float y, float z);

	};
} //namespace ndk_helper
#endif /* VECMATH_H_ */
