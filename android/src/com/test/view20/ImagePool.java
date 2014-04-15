package com.test.view20;

import java.io.IOException;
import java.io.InputStream;
import java.util.Hashtable;
import java.util.Map;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;

public class ImagePool {

	public class Image {
		public int STATUS_PRELOAD = 0;
		public int STATUS_ONRAM = 1;
		public int STATUS_ONGPU = 2;
		public int STATUS_ONGPUFRAMEBUFER = 3;

		public float width;
		public float height;
		public int textureID;
		public Bitmap bitmap;

		public int status = STATUS_PRELOAD;

		public float u1;
		public float v1;
		public float u2;
		public float v2;
	}

	public int loadingTextureID;
	public Map<String, Image> images = new Hashtable<String, Image>();

	public Context mContext;

	void initialize(Context mContext) {
		this.mContext = mContext;
		Image image = loadImage("loading.png");
		loadingTextureID = image.textureID;
		images.clear();
	}

	public int getImage(String key) {
		// if (key.equals("loading.png")) {
		// return loadingTextureID;
		// }
		int textureID = loadingTextureID;
		Image image = images.get(key);
		if (image == null) {
			textureID = loadingTextureID;
			// TODO load image in other thread.
			image = loadImage(key);
			images.put(key, image);
		} else {
			if (image.status != image.STATUS_ONGPU) {
				textureID = loadingTextureID;
			} else {
				textureID = image.textureID;
			}
		}
		return textureID;
	}

	public Image loadImage(String key) {

		Image image = new Image();
		image.status = image.STATUS_PRELOAD;

		InputStream is = null;
		try {
			is = mContext.getResources().getAssets().open(key);
			image.bitmap = BitmapFactory.decodeStream(is);
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				is.close();
			} catch (IOException e) {
				// Ignore.
			}
		}
		image.height = image.bitmap.getHeight();
		image.width = image.bitmap.getWidth();

		image.status = image.STATUS_ONRAM;

		int[] textures = new int[1];

		GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA);
		GLES20.glEnable(GLES20.GL_BLEND);
		GLES20.glGenTextures(1, textures, 0);

		image.textureID = textures[0];
		GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, image.textureID);

		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
		GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT);
		GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_REPEAT);

		image.status = image.STATUS_ONRAM;

		GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, image.bitmap, 0);

//		GLES20.glDisable(GLES20.GL_BLEND);
		image.bitmap.recycle();
		image.status = image.STATUS_ONGPU;

		return image;
	}

}
