/*
 * Copy_right 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * y_ou may_ not use this file ex_cept in compliance with the License.
 * You may_ obtain a copy_ of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by_ applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either ex_press or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//--------------------------------------------------------------------------------
// vecmath.cpp
//--------------------------------------------------------------------------------
#include "vecmath.h"
#define  LOG_TAG    "vecmath"
#ifndef LOGI
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#endif /* LOGI */
namespace hsrl
{

	//--------------------------------------------------------------------------------
	// vec3
	//--------------------------------------------------------------------------------
	Vec3::Vec3(const Vec4& vec)
	{
		x_ = vec.x_;
		y_ = vec.y_;
		z_ = vec.z_;
	}

	//--------------------------------------------------------------------------------
	// vec4
	//--------------------------------------------------------------------------------
	Vec4 Vec4::operator*(const Mat4& rhs) const
	{
		Vec4 out;
		out.x_ = x_ * rhs.f_[0] + y_ * rhs.f_[1] + z_ * rhs.f_[2] + w_ * rhs.f_[3];
		out.y_ = x_ * rhs.f_[4] + y_ * rhs.f_[5] + z_ * rhs.f_[6] + w_ * rhs.f_[7];
		out.z_ = x_ * rhs.f_[8] + y_ * rhs.f_[9] + z_ * rhs.f_[10] + w_ * rhs.f_[11];
		out.w_ = x_ * rhs.f_[12] + y_ * rhs.f_[13] + z_ * rhs.f_[14] + w_ * rhs.f_[15];
		return out;
	}

	//--------------------------------------------------------------------------------
	// mat4
	//--------------------------------------------------------------------------------
	Mat4::Mat4()
	{
		this->f_ = new float[16];
		for (int32_t i = 0; i < 16; ++i){
			this->f_[i] = 0.f;
		}
	}

	Mat4::Mat4(const float* mIn)
	{
		this->f_ = new float[16];
		for (int32_t i = 0; i < 16; ++i){
			this->f_[i] = mIn[i];
		}
	}

	Mat4* Mat4::operator*(const Mat4& rhs) const
	{
		Mat4 ret;
		ret.f_[0] = f_[0] * rhs.f_[0] + f_[4] * rhs.f_[1] + f_[8] * rhs.f_[2]
			+ f_[12] * rhs.f_[3];
		ret.f_[1] = f_[1] * rhs.f_[0] + f_[5] * rhs.f_[1] + f_[9] * rhs.f_[2]
			+ f_[13] * rhs.f_[3];
		ret.f_[2] = f_[2] * rhs.f_[0] + f_[6] * rhs.f_[1] + f_[10] * rhs.f_[2]
			+ f_[14] * rhs.f_[3];
		ret.f_[3] = f_[3] * rhs.f_[0] + f_[7] * rhs.f_[1] + f_[11] * rhs.f_[2]
			+ f_[15] * rhs.f_[3];

		ret.f_[4] = f_[0] * rhs.f_[4] + f_[4] * rhs.f_[5] + f_[8] * rhs.f_[6]
			+ f_[12] * rhs.f_[7];
		ret.f_[5] = f_[1] * rhs.f_[4] + f_[5] * rhs.f_[5] + f_[9] * rhs.f_[6]
			+ f_[13] * rhs.f_[7];
		ret.f_[6] = f_[2] * rhs.f_[4] + f_[6] * rhs.f_[5] + f_[10] * rhs.f_[6]
			+ f_[14] * rhs.f_[7];
		ret.f_[7] = f_[3] * rhs.f_[4] + f_[7] * rhs.f_[5] + f_[11] * rhs.f_[6]
			+ f_[15] * rhs.f_[7];

		ret.f_[8] = f_[0] * rhs.f_[8] + f_[4] * rhs.f_[9] + f_[8] * rhs.f_[10]
			+ f_[12] * rhs.f_[11];
		ret.f_[9] = f_[1] * rhs.f_[8] + f_[5] * rhs.f_[9] + f_[9] * rhs.f_[10]
			+ f_[13] * rhs.f_[11];
		ret.f_[10] = f_[2] * rhs.f_[8] + f_[6] * rhs.f_[9] + f_[10] * rhs.f_[10]
			+ f_[14] * rhs.f_[11];
		ret.f_[11] = f_[3] * rhs.f_[8] + f_[7] * rhs.f_[9] + f_[11] * rhs.f_[10]
			+ f_[15] * rhs.f_[11];

		ret.f_[12] = f_[0] * rhs.f_[12] + f_[4] * rhs.f_[13] + f_[8] * rhs.f_[14]
			+ f_[12] * rhs.f_[15];
		ret.f_[13] = f_[1] * rhs.f_[12] + f_[5] * rhs.f_[13] + f_[9] * rhs.f_[14]
			+ f_[13] * rhs.f_[15];
		ret.f_[14] = f_[2] * rhs.f_[12] + f_[6] * rhs.f_[13] + f_[10] * rhs.f_[14]
			+ f_[14] * rhs.f_[15];
		ret.f_[15] = f_[3] * rhs.f_[12] + f_[7] * rhs.f_[13] + f_[11] * rhs.f_[14]
			+ f_[15] * rhs.f_[15];

		Mat4* presult = &ret;
		return presult;
	}

	Vec4 Mat4::operator*(const Vec4& rhs) const
	{
		Vec4 ret;
		ret.x_ = rhs.x_ * f_[0] + rhs.y_ * f_[4] + rhs.z_ * f_[8] + rhs.w_ * f_[12];
		ret.y_ = rhs.x_ * f_[1] + rhs.y_ * f_[5] + rhs.z_ * f_[9] + rhs.w_ * f_[13];
		ret.z_ = rhs.x_ * f_[2] + rhs.y_ * f_[6] + rhs.z_ * f_[10] + rhs.w_ * f_[14];
		ret.w_ = rhs.x_ * f_[3] + rhs.y_ * f_[7] + rhs.z_ * f_[11] + rhs.w_ * f_[15];
		return ret;
	}

	Mat4 Mat4::Inverse()
	{
		Mat4 ret;
		float det_1;
		float pos = 0;
		float neg = 0;
		float temp;

		temp = f_[0] * f_[5] * f_[10];
		if (temp >= 0)
			pos += temp;
		else
			neg += temp;
		temp = f_[4] * f_[9] * f_[2];
		if (temp >= 0)
			pos += temp;
		else
			neg += temp;
		temp = f_[8] * f_[1] * f_[6];
		if (temp >= 0)
			pos += temp;
		else
			neg += temp;
		temp = -f_[8] * f_[5] * f_[2];
		if (temp >= 0)
			pos += temp;
		else
			neg += temp;
		temp = -f_[4] * f_[1] * f_[10];
		if (temp >= 0)
			pos += temp;
		else
			neg += temp;
		temp = -f_[0] * f_[9] * f_[6];
		if (temp >= 0)
			pos += temp;
		else
			neg += temp;
		det_1 = pos + neg;

		if (det_1 == 0.0)
		{
			//Error
		}
		else
		{
			det_1 = 1.0f / det_1;
			ret.f_[0] = (f_[5] * f_[10] - f_[9] * f_[6]) * det_1;
			ret.f_[1] = -(f_[1] * f_[10] - f_[9] * f_[2]) * det_1;
			ret.f_[2] = (f_[1] * f_[6] - f_[5] * f_[2]) * det_1;
			ret.f_[4] = -(f_[4] * f_[10] - f_[8] * f_[6]) * det_1;
			ret.f_[5] = (f_[0] * f_[10] - f_[8] * f_[2]) * det_1;
			ret.f_[6] = -(f_[0] * f_[6] - f_[4] * f_[2]) * det_1;
			ret.f_[8] = (f_[4] * f_[9] - f_[8] * f_[5]) * det_1;
			ret.f_[9] = -(f_[0] * f_[9] - f_[8] * f_[1]) * det_1;
			ret.f_[10] = (f_[0] * f_[5] - f_[4] * f_[1]) * det_1;

			/* Calculate -C * inverse(A) */
			ret.f_[12] = -(f_[12] * ret.f_[0] + f_[13] * ret.f_[4] + f_[14] * ret.f_[8]);
			ret.f_[13] = -(f_[12] * ret.f_[1] + f_[13] * ret.f_[5] + f_[14] * ret.f_[9]);
			ret.f_[14] = -(f_[12] * ret.f_[2] + f_[13] * ret.f_[6] + f_[14] * ret.f_[10]);

			ret.f_[3] = 0.0f;
			ret.f_[7] = 0.0f;
			ret.f_[11] = 0.0f;
			ret.f_[15] = 1.0f;
		}

		*this = ret;
		return *this;
	}

	//--------------------------------------------------------------------------------
	// Misc
	//--------------------------------------------------------------------------------
	Mat4 Mat4::RotationX(const float fAngle)
	{
		Mat4 ret;
		float fCosine, fSine;

		fCosine = cosf(fAngle);
		fSine = sinf(fAngle);

		ret.f_[0] = 1.0f;
		ret.f_[4] = 0.0f;
		ret.f_[8] = 0.0f;
		ret.f_[12] = 0.0f;
		ret.f_[1] = 0.0f;
		ret.f_[5] = fCosine;
		ret.f_[9] = fSine;
		ret.f_[13] = 0.0f;
		ret.f_[2] = 0.0f;
		ret.f_[6] = -fSine;
		ret.f_[10] = fCosine;
		ret.f_[14] = 0.0f;
		ret.f_[3] = 0.0f;
		ret.f_[7] = 0.0f;
		ret.f_[11] = 0.0f;
		ret.f_[15] = 1.0f;
		return ret;
	}

	Mat4 Mat4::RotationY(const float fAngle)
	{
		Mat4 ret;
		float fCosine, fSine;

		fCosine = cosf(fAngle);
		fSine = sinf(fAngle);

		ret.f_[0] = fCosine;
		ret.f_[4] = 0.0f;
		ret.f_[8] = -fSine;
		ret.f_[12] = 0.0f;
		ret.f_[1] = 0.0f;
		ret.f_[5] = 1.0f;
		ret.f_[9] = 0.0f;
		ret.f_[13] = 0.0f;
		ret.f_[2] = fSine;
		ret.f_[6] = 0.0f;
		ret.f_[10] = fCosine;
		ret.f_[14] = 0.0f;
		ret.f_[3] = 0.0f;
		ret.f_[7] = 0.0f;
		ret.f_[11] = 0.0f;
		ret.f_[15] = 1.0f;
		return ret;

	}

	Mat4 Mat4::RotationZ(const float fAngle)
	{
		Mat4 ret;
		float fCosine, fSine;

		fCosine = cosf(fAngle);
		fSine = sinf(fAngle);

		ret.f_[0] = fCosine;
		ret.f_[4] = fSine;
		ret.f_[8] = 0.0f;
		ret.f_[12] = 0.0f;
		ret.f_[1] = -fSine;
		ret.f_[5] = fCosine;
		ret.f_[9] = 0.0f;
		ret.f_[13] = 0.0f;
		ret.f_[2] = 0.0f;
		ret.f_[6] = 0.0f;
		ret.f_[10] = 1.0f;
		ret.f_[14] = 0.0f;
		ret.f_[3] = 0.0f;
		ret.f_[7] = 0.0f;
		ret.f_[11] = 0.0f;
		ret.f_[15] = 1.0f;
		return ret;
	}

	Mat4 Mat4::Translation(const float fX, const float fY, const float fZ)
	{
		Mat4 ret;
		ret.f_[0] = 1.0f;
		ret.f_[4] = 0.0f;
		ret.f_[8] = 0.0f;
		ret.f_[12] = fX;
		ret.f_[1] = 0.0f;
		ret.f_[5] = 1.0f;
		ret.f_[9] = 0.0f;
		ret.f_[13] = fY;
		ret.f_[2] = 0.0f;
		ret.f_[6] = 0.0f;
		ret.f_[10] = 1.0f;
		ret.f_[14] = fZ;
		ret.f_[3] = 0.0f;
		ret.f_[7] = 0.0f;
		ret.f_[11] = 0.0f;
		ret.f_[15] = 1.0f;
		return ret;
	}

	Mat4 Mat4::Translation(const Vec3 vec)
	{
		Mat4 ret;
		ret.f_[0] = 1.0f;
		ret.f_[4] = 0.0f;
		ret.f_[8] = 0.0f;
		ret.f_[12] = vec.x_;
		ret.f_[1] = 0.0f;
		ret.f_[5] = 1.0f;
		ret.f_[9] = 0.0f;
		ret.f_[13] = vec.y_;
		ret.f_[2] = 0.0f;
		ret.f_[6] = 0.0f;
		ret.f_[10] = 1.0f;
		ret.f_[14] = vec.z_;
		ret.f_[3] = 0.0f;
		ret.f_[7] = 0.0f;
		ret.f_[11] = 0.0f;
		ret.f_[15] = 1.0f;
		return ret;
	}

	Mat4 Mat4::Perspective(float width, float height, float nearPlane, float farPlane)
	{
		float n2 = 2.0f * nearPlane;
		float rcpnmf = 1.f / (nearPlane - farPlane);

		Mat4 result;
		result.f_[0] = n2 / width;
		result.f_[4] = 0;
		result.f_[8] = 0;
		result.f_[12] = 0;
		result.f_[1] = 0;
		result.f_[5] = n2 / height;
		result.f_[9] = 0;
		result.f_[13] = 0;
		result.f_[2] = 0;
		result.f_[6] = 0;
		result.f_[10] = (farPlane + nearPlane) * rcpnmf;
		result.f_[14] = farPlane * rcpnmf * n2;
		result.f_[3] = 0;
		result.f_[7] = 0;
		result.f_[11] = -1.0;
		result.f_[15] = 0;

		return result;
	}

	Mat4* Mat4::LookAt(const Vec3& vec_eye, const Vec3& vec_at, const Vec3& vec_up)
	{
		Vec3 vec_forward, vec_up_norm, vec_side;
		Mat4 result;

		vec_forward.x_ = vec_eye.x_ - vec_at.x_;
		vec_forward.y_ = vec_eye.y_ - vec_at.y_;
		vec_forward.z_ = vec_eye.z_ - vec_at.z_;

		vec_forward.Normalize();
		vec_up_norm = vec_up;
		vec_up_norm.Normalize();
		vec_side = vec_up_norm.Cross(vec_forward);
		vec_up_norm = vec_forward.Cross(vec_side);

		result.f_[0] = vec_side.x_;
		result.f_[4] = vec_side.y_;
		result.f_[8] = vec_side.z_;
		result.f_[12] = 0;
		result.f_[1] = vec_up_norm.x_;
		result.f_[5] = vec_up_norm.y_;
		result.f_[9] = vec_up_norm.z_;
		result.f_[13] = 0;
		result.f_[2] = vec_forward.x_;
		result.f_[6] = vec_forward.y_;
		result.f_[10] = vec_forward.z_;
		result.f_[14] = 0;
		result.f_[3] = 0;
		result.f_[7] = 0;
		result.f_[11] = 0;
		result.f_[15] = 1.0;

		result.PostTranslate(-vec_eye.x_, -vec_eye.y_, -vec_eye.z_);
		Mat4* presult = &result;
		return presult;
	}


	Mat4* Mat4::OrthoM(float left, float right, float bottom, float top, float near, float far)
	{
		Mat4 result;
		Mat4* presult = &result;

		int mOffset = 0;

		float* m = result.Ptr();
		if (left == right) {
			return presult;
		}
		if (bottom == top) {
			return presult;
		}
		if (near == far) {
			return presult;
		}

		float r_width = 1.0f / (right - left);
		float r_height = 1.0f / (top - bottom);
		float r_depth = 1.0f / (far - near);
		float x = 2.0f * (r_width);
		float y = 2.0f * (r_height);
		float z = -2.0f * (r_depth);
		float tx = -(right + left) * r_width;
		float ty = -(top + bottom) * r_height;
		float tz = -(far + near) * r_depth;
		m[mOffset + 0] = x;
		m[mOffset + 5] = y;
		m[mOffset + 10] = z;
		m[mOffset + 12] = tx;
		m[mOffset + 13] = ty;
		m[mOffset + 14] = tz;
		m[mOffset + 15] = 1.0f;
		m[mOffset + 1] = 0.0f;
		m[mOffset + 2] = 0.0f;
		m[mOffset + 3] = 0.0f;
		m[mOffset + 4] = 0.0f;
		m[mOffset + 6] = 0.0f;
		m[mOffset + 7] = 0.0f;
		m[mOffset + 8] = 0.0f;
		m[mOffset + 9] = 0.0f;
		m[mOffset + 11] = 0.0f;

		return presult;
	}


	void Mat4::scaleM(float x, float y, float z) {
		float* m = f_;
		for (int i = 0; i < 4; i++) {
			int mi = 0 + i;
			m[mi] *= x;
			m[4 + mi] *= y;
			m[8 + mi] *= z;
		}
	}

	void Mat4::translateM(float x, float y, float z) {
		float* m = f_;
		for (int i = 0; i < 4; i++) {
			int mi = 0 + i;
			m[12 + mi] += m[mi] * x + m[4 + mi] * y + m[8 + mi] * z;
		}
	}


	void Mat4::setRotateM(float a, float x, float y, float z) {

		LOGI("debugging the meid puzzle 0, y=%f", y);
		//float x1 = 155.5f;
		//float y1 = 65429.8f;
		//float z1 = 3213.0f;
		//float a1 = 1.8f;
		//LOGI("debugging the meid puzzle 1, y=%f", y);
		//x1 = x + 10.0f;
		//y1 = y + 120.0f;
		//z1 = z + 1210.0f;
		//a1 = a + 0.0f;
		//LOGI("debugging the meid puzzle 2, y=%f", y);
		//float* rm = this->f_;
		//LOGI("debugging the meid puzzle 2.0, *rm=%d,  y=%f, y1=%f",  rm,y, y1);
		//*(rm + 7) = 1668.0f;
		//LOGI("debugging the meid puzzle 2.1, *rm=%d,  y=%f, y1=%f", rm, y, y1);
		//*(rm + 10) = 888848.0f;
		//LOGI("debugging the meid puzzle 2.2, *rm=%d,  y=%f, y1=%f", rm, y, y1);
		//rm[7] = 145236.0f;
		//LOGI("debugging the meid puzzle 2.3, *rm=%d,  y=%f, y1=%f", rm, y, y1);
		//LOGI("debugging the meid puzzle 3, y=%f", y);
		//int rmOffset = 0;
		//rm[rmOffset + 3] = 0.0f;
		//rm[rmOffset + 7] = 0.0f;
		//rm[rmOffset + 11] = 0.0f;
		//rm[rmOffset + 12] = 0.0f;
		//rm[rmOffset + 13] = 0.0f;
		//rm[rmOffset + 14] = 0.0f;
		//rm[rmOffset + 15] = 1.0f;
		//a1 *= (float)(M_PI / 180.0f);
		//float s = (float)sin(a1);
		//float c = (float)cos(a1);
		//LOGI("debugging the meid puzzle 4, y=%f", y);
		//if (1.0f == x && 0.0f == y && 0.0f == z) {
		//	rm[rmOffset + 5] = c;   rm[rmOffset + 10] = c;
		//	rm[rmOffset + 6] = s;   rm[rmOffset + 9] = -s;
		//	rm[rmOffset + 1] = 0;   rm[rmOffset + 2] = 0;
		//	rm[rmOffset + 4] = 0;   rm[rmOffset + 8] = 0;
		//	rm[rmOffset + 0] = 1;
		//}
		//else if (0.0f == x && 1.0f == y && 0.0f == z) {
		//	rm[rmOffset + 0] = c;   rm[rmOffset + 10] = c;
		//	rm[rmOffset + 8] = s;   rm[rmOffset + 2] = -s;
		//	rm[rmOffset + 1] = 0;   rm[rmOffset + 4] = 0;
		//	rm[rmOffset + 6] = 0;   rm[rmOffset + 9] = 0;
		//	rm[rmOffset + 5] = 1;
		//}
		//else if (0.0f == x && 0.0f == y && 1.0f == z) {
		//	rm[rmOffset + 0] = c;   rm[rmOffset + 5] = c;
		//	rm[rmOffset + 1] = s;   rm[rmOffset + 4] = -s;
		//	rm[rmOffset + 2] = 0;   rm[rmOffset + 6] = 0;
		//	rm[rmOffset + 8] = 0;   rm[rmOffset + 9] = 0;
		//	rm[rmOffset + 10] = 1;
		//}
		//else {
		//	LOGI("debugging the meid puzzle");



		//	/*int testa = 10;
		//	testa = testa + 121234;
		//	LOGI("debugging the meid puzzle, %d", testa);*/

		//	//float len = (float)sqrt(x1 * x1 + y1 * y1 + z1 * z1);
		//	//if (1.0f != len) {
		//	//float recipLen = 1.0f / len;
		//	//x1 *= recipLen;
		//	//y1 *= recipLen;
		//	//z1 *= recipLen;
		//	//}

		//	//float nc = 1.0f - c;
		//	//float xy = x1 * y1;
		//	//float yz = y1 * z1;
		//	//float zx = z1 * x1;
		//	//float xs = x1 * s;
		//	//float ys = y1 * s;
		//	//float zs = z1 * s;
		//	//rm[rmOffset + 0] = x1*x1*nc + c;
		//	//rm[rmOffset + 4] = xy*nc - zs;
		//	//rm[rmOffset + 8] = zx*nc + ys;
		//	//rm[rmOffset + 1] = xy*nc + zs;
		//	//rm[rmOffset + 5] = y1*y1*nc + c;
		//	//rm[rmOffset + 9] = yz*nc - xs;
		//	//rm[rmOffset + 2] = zx*nc - ys;
		//	//rm[rmOffset + 6] = yz*nc + xs;
		//	//rm[rmOffset + 10] = z1*z1*nc + c;

		//}
		LOGI("setRotateM completed");
	}




} //namespace ndkHelper

