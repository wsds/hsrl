package com.impactjs.ejecta;

import java.nio.IntBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLES20;
import android.opengl.GLES11Ext;
import android.opengl.GLSurfaceView.Renderer;

public class EjectaRenderer implements Renderer {

	public static String mainBundle;
	private int screen_width;
	private int screen_height;
	private Cube mCube;
	private float mAngle = 0;

	public EjectaRenderer(int width, int height) {
		// mainBundle = "/data/data/" + ctx.getPackageName();
		// System.out.println(mainBundle);
		// Utils.copyDatFiles(ctx, mainBundle+"/files/build/", "build");
		screen_width = width;
		screen_height = height;
		mCube = new Cube();
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		
		
		IntBuffer coords = null;
		// GLES20.glViewport(0, 0, 50, 50);
		// GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
//		GLES11Ext.glDrawTexivOES(coords);
		// nativeRender();



		/*
		 * Now we're ready to draw some 3D objects
		 */

		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();
		gl.glTranslatef(0, 0, -3.0f);
		gl.glRotatef(mAngle, 0, 1, 0);
		gl.glRotatef(mAngle * 0.25f, 1, 0, 0);

		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glEnableClientState(GL10.GL_COLOR_ARRAY);

		mCube.draw(gl);

		mAngle += 1.2f;

	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		System.out.println("onSurfaceChanged");
		GLES20.glViewport(0, 0, width, height);
		// nativeChanged(width, height);

		screen_width = width;
		screen_height = height;
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		// GLES20.glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		// nativeCreated(mainBundle, screen_width, screen_height);

		gl.glDisable(GL10.GL_DITHER);

		/*
		 * Some one-time OpenGL initialization can be made here probably based
		 * on features of this particular context
		 */
		gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT, GL10.GL_FASTEST);

		gl.glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		gl.glEnable(GL10.GL_CULL_FACE);
		gl.glShadeModel(GL10.GL_SMOOTH);
		gl.glEnable(GL10.GL_DEPTH_TEST);
	}

	private native void nativeRender();

	private native void nativeCreated(String mainBundle, int width, int height);

	private native void nativeChanged(int width, int height);

	public native void nativeFinalize();

	public native void nativePause();

	public native void nativeResume();

	public native void nativeTouch(int action, int x, int y);

	public native void nativeOnSensorChanged(float accle_x, float accle_y, float accle_z);

	public native void nativeOnKeyDown(int key_code);

	public native void nativeOnKeyUp(int key_code);
}
