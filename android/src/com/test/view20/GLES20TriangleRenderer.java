/*
 * Copyright (C) 2009 The Android Open Source Project
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

package com.test.view20;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.util.Log;

class GLES20TriangleRenderer implements GLSurfaceView.Renderer {

	public GLES20TriangleRenderer(Context context) {
		mContext = context;
		mTriangleVertices = ByteBuffer.allocateDirect(mTriangleVerticesData.length * FLOAT_SIZE_BYTES).order(ByteOrder.nativeOrder()).asFloatBuffer();
		mTriangleVertices.put(mTriangleVerticesData).position(0);
	}

	public SpaceHolder spaceHolder;

	public class SpaceHolder {

		public float width;
		public float height;
		public float ratio;
		public float Pi;

		public float screen_offset_x;

		public float screen_offset_y;

		public SpaceHolder(float ratio, float width, float height) {
			this.width = width;
			this.height = height;
			this.ratio = ratio;
			this.Pi = 2.0f / width;

			this.screen_offset_x = -1.0f;
			this.screen_offset_y = 1.0f / ratio;
		}

		void drawImage(String key, float left, float top, float width, float height) {
			drawImage(key, left, top, width, height, null);
		}

		void drawImage(String key, float left, float top, float width, float height, float[] mModelMatrixMove) {
			int textureID = imagePool.getImage(key);
			GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureID);

			float angle = 0;

			float offset_x = 0;
			float offset_y = 0;

			offset_x = this.screen_offset_x + left * this.Pi;
			offset_y = this.screen_offset_y - top * this.Pi;

			float offset_w = 0;
			float offset_h = 0;

			offset_w = 2 * width / this.width;
			offset_h = 2 * height / this.width;

			float[] modelMatrix = null;
			Matrix.setRotateM(mModelMatrixpreMove, 0, angle, 0, 0, 1.0f);
			if (mModelMatrixMove != null) {
				Matrix.multiplyMM(mModelMatrix, 0, mModelMatrixpreMove, 0, mModelMatrixMove, 0);
				modelMatrix = mModelMatrix;
			} else {
				Matrix.translateM(mModelMatrixpreMove, 0, offset_x, offset_y, 0.0f);
				modelMatrix = mModelMatrixpreMove;
			}

			Matrix.scaleM(modelMatrix, 0, offset_w, offset_h, 1.0f);

			GLES20.glUniformMatrix4fv(mModelMatrixHandle, 1, false, modelMatrix, 0);
			GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
		}
	}
	public long renderCount=0;
	public long renderStep=0;

	public void onDrawFrame(GL10 glUnused) {
		renderStep=(renderStep+1)%60;
		if(renderStep==1){
			renderCount++;
		}
		GLES20.glClearColor(0.0f, 0.6f, 0.804f, 1.0f);
		GLES20.glClear(GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
		GLES20.glUseProgram(mProgram);

		mTriangleVertices.position(TRIANGLE_VERTICES_DATA_POS_OFFSET);
		GLES20.glVertexAttribPointer(maPositionHandle, 3, GLES20.GL_FLOAT, false, TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
		mTriangleVertices.position(TRIANGLE_VERTICES_DATA_UV_OFFSET);
		GLES20.glEnableVertexAttribArray(maPositionHandle);

		GLES20.glVertexAttribPointer(maTextureHandle, 2, GLES20.GL_FLOAT, false, TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
		GLES20.glEnableVertexAttribArray(maTextureHandle);

		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glUniformMatrix4fv(mProjectionViewMatrixHandle, 1, false, mProjectionViewMatrix, 0);
		spaceHolder.drawImage("snow_d_blur.jpg", 0, 0, spaceHolder.width, spaceHolder.height);

		spaceHolder.drawImage("test2.png", 0, 0, 50, 50);
		spaceHolder.drawImage("face_man.png", 100, 200, 250, 250);
		for (int i = 1; i < 20; i++) {
			spaceHolder.drawImage("test2.png", 0, i * 60, 50, 50);
		}

		Matrix.setIdentityM(mModelMatrixMove, 0);

		float offset_x_move = 0;
		float offset_y_move = 0;

		offset_x_move = spaceHolder.screen_offset_x + (x_move - 48) * spaceHolder.Pi;
		offset_y_move = spaceHolder.screen_offset_y - (y_move - 48)* spaceHolder.Pi;

		// offset_x_move = x_move * spaceHolder.Pi;
		//
		// offset_y_move = -y_move * spaceHolder.Pi;

		Matrix.translateM(mModelMatrixMove, 0, offset_x_move, offset_y_move, 0.0f);

		spaceHolder.drawImage("emoji_normal.png", 100, 600, 96, 96, mModelMatrixMove);
	}

	public float x_move = 230;
	public float y_move = 200;

	public void onSurfaceChanged(GL10 glUnused, int width, int height) {
		GLES20.glViewport(0, 0, width, height);
		float ratio = (float) width / height;
		System.out.println("width:" + width + "height:" + height + "ratio:" + ratio);
		Matrix.orthoM(mProjectionMatrix, 0, -1, 1, -1 / ratio, 1 / ratio, 3, 7);// 正交投影
		spaceHolder = new SpaceHolder(ratio, width, height);
		setupProjectionView();
	}

	ImagePool imagePool = new ImagePool();

	public void onSurfaceCreated(GL10 glUnused, EGLConfig config) {
		mVertexShader = getFromAssets("VertexShader.js");

		mFragmentShader = getFromAssets("FragmentShader.js");

		mProgram = createProgram(mVertexShader, mFragmentShader);
		if (mProgram == 0) {
			return;
		}
		maPositionHandle = GLES20.glGetAttribLocation(mProgram, "aPosition");
		checkGlError("glGetAttribLocation aPosition");
		if (maPositionHandle == -1) {
			throw new RuntimeException("Could not get attrib location for aPosition");
		}
		maTextureHandle = GLES20.glGetAttribLocation(mProgram, "aTextureCoord");
		checkGlError("glGetAttribLocation aTextureCoord");
		if (maTextureHandle == -1) {
			throw new RuntimeException("Could not get attrib location for aTextureCoord");
		}

		mProjectionViewMatrixHandle = GLES20.glGetUniformLocation(mProgram, "projectionView");
		checkGlError("glGetUniformLocation projectionView");
		if (mProjectionViewMatrixHandle == -1) {
			throw new RuntimeException("Could not get attrib location for projectionView");
		}

		mModelMatrixHandle = GLES20.glGetUniformLocation(mProgram, "model");
		checkGlError("glGetUniformLocation model");
		if (mProjectionViewMatrixHandle == -1) {
			throw new RuntimeException("Could not get attrib location for model");
		}

		imagePool.initialize(mContext);

		// ioadTexture
		Matrix.setLookAtM(mViewMatrix, 0, 0, 0, 5, 0f, 0f, 0f, 0f, 1.0f, 0.0f);
	}

	public void setupProjectionView() {
		Matrix.multiplyMM(mProjectionViewMatrix, 0, mProjectionMatrix, 0, mViewMatrix, 0);// mModelMatrix模型矩阵；mViewMatrix视图矩阵；mProjectionMatrix透视矩阵；
		GLES20.glUniformMatrix4fv(mProjectionViewMatrixHandle, 1, false, mProjectionViewMatrix, 0);
	}

	private int loadShader(int shaderType, String source) {
		int shader = GLES20.glCreateShader(shaderType);
		if (shader != 0) {
			GLES20.glShaderSource(shader, source);
			GLES20.glCompileShader(shader);
			int[] compiled = new int[1];
			GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
			if (compiled[0] == 0) {
				Log.e(TAG, "Could not compile shader " + shaderType + ":");
				Log.e(TAG, GLES20.glGetShaderInfoLog(shader));
				GLES20.glDeleteShader(shader);
				shader = 0;
			}
		}
		return shader;
	}

	private int createProgram(String vertexSource, String fragmentSource) {
		int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
		if (vertexShader == 0) {
			return 0;
		}

		int pixelShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
		if (pixelShader == 0) {
			return 0;
		}

		int program = GLES20.glCreateProgram();
		if (program != 0) {
			GLES20.glAttachShader(program, vertexShader);
			checkGlError("glAttachShader");
			GLES20.glAttachShader(program, pixelShader);
			checkGlError("glAttachShader");
			GLES20.glLinkProgram(program);
			int[] linkStatus = new int[1];
			GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
			if (linkStatus[0] != GLES20.GL_TRUE) {
				Log.e(TAG, "Could not link program: ");
				Log.e(TAG, GLES20.glGetProgramInfoLog(program));
				GLES20.glDeleteProgram(program);
				program = 0;
			}
		}
		return program;
	}

	private void checkGlError(String op) {
		int error;
		while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
			Log.e(TAG, op + ": glError " + error);
			throw new RuntimeException(op + ": glError " + error);
		}
	}

	private static final int FLOAT_SIZE_BYTES = 4;
	private static final int TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
	private static final int TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
	private static final int TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;
	private final float[] mTriangleVerticesData = {
			// X, Y, Z, U, V
			// 1
			0.0f, -1.0f, 0, 0.0f, 1.0f,
			// 2
			1.0f, -1.0f, 0, 1.0f, 1.0f,
			// 3
			0.0f, 0.0f, 0, 0.0f, 0.0f,
			// 4
			1.0f, 0.0f, 0, 1.0f, 0.0f

	};

	private final float[] mTriangleVerticesDataCenter = {
			// X, Y, Z, U, V
			// 1
			-0.5f, -0.5f, 0, 0.0f, 1.0f,
			// 2
			0.5f, -0.5f, 0, 1.0f, 1.0f,
			// 3
			-0.5f, 0.5f, 0, 0.0f, 0.0f,
			// 4
			0.5f, 0.5f, 0, 1.0f, 0.0f

	};

	private FloatBuffer mTriangleVertices;

	private String mVertexShader;

	private String mFragmentShader;

	public String getFromAssets(String fileName) {
		String result = null;
		try {
			InputStreamReader inputReader = new InputStreamReader(mContext.getResources().getAssets().open(fileName));
			BufferedReader bufReader = new BufferedReader(inputReader);
			String line = "";
			result = "";
			while ((line = bufReader.readLine()) != null) {
				result += line;
			}
			return result;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return result;
	}

	private float[] mModelMatrixMove = new float[16];
	private float[] mModelMatrixpreMove = new float[16];

	private float[] mProjectionViewMatrix = new float[16];

	private float[] mProjectionMatrix = new float[16];
	private float[] mModelMatrix = new float[16];
	private float[] mViewMatrix = new float[16];

	private int mProgram;
	private int mProjectionViewMatrixHandle;
	private int mModelMatrixHandle;
	private int maPositionHandle;
	private int maTextureHandle;

	private Context mContext;
	private static String TAG = "GLES20TriangleRenderer";
}
