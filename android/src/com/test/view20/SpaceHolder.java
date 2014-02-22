package com.test.view20;

import com.open.hsrl.World;

import android.opengl.GLES20;
import android.opengl.Matrix;

public class SpaceHolder {

	ImagePool imagePool;

	public float width;
	public float height;
	public float ratio;
	public float Pi;

	public float screen_offset_x;

	public float screen_offset_y;

	public SpaceHolder(float ratio, float width, float height, int mModelMatrixHandle, ImagePool imagePool) {
		this.width = width;
		this.height = height;
		this.ratio = ratio;
		this.Pi = 2.0f / width;

		this.screen_offset_x = -1.0f;
		this.screen_offset_y = 1.0f / ratio;

		this.mModelMatrixHandle = mModelMatrixHandle;
		this.imagePool = imagePool;
	}

	void drawImage(String key, float left, float top, float width, float height) {
		drawImage(key, left, top, width, height, null);
	}

	private float[] mModelMatrixpreMove = new float[16];

	private float[] mModelMatrix = new float[16];
	private int mModelMatrixHandle;
	
	float angle = 0;
	float offset_x = 0;
	float offset_y = 0;
	float offset_w = 0;
	float offset_h = 0;
	float[] modelMatrix = null;
	void drawImage(String key, float left, float top, float width, float height, float[] modelMatrixMove) {
		int textureID = imagePool.getImage(key);
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureID);

		offset_x = this.screen_offset_x + left * this.Pi;
		offset_y = this.screen_offset_y - top * this.Pi;

		offset_w = 2 * width / this.width;
		offset_h = 2 * height / this.width;

		Matrix.setRotateM(mModelMatrixpreMove, 0, angle, 0, 0, 1.0f);
		if (modelMatrixMove != null) {
			Matrix.multiplyMM(mModelMatrix, 0, mModelMatrixpreMove, 0, modelMatrixMove, 0);
			modelMatrix = mModelMatrix;
		} else {
			Matrix.translateM(mModelMatrixpreMove, 0, offset_x, offset_y, 0.0f);
			modelMatrix = mModelMatrixpreMove;
		}

		Matrix.scaleM(modelMatrix, 0, offset_w, offset_h, 1.0f);

		GLES20.glUniformMatrix4fv(mModelMatrixHandle, 1, false, modelMatrix, 0);
		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
	}
	
	
	World world=World.getInstance();
	
	
	
}
