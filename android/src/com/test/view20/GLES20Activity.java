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

import com.open.hsrl.Interpolator;
import com.open.hsrl.Node;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;

/**
 * This sample shows how to check for OpenGL ES 2.0 support at runtime, and then
 * use either OpenGL ES 1.0 or OpenGL ES 2.0, as appropriate.
 */
public class GLES20Activity extends Activity {
	public GLES20TriangleRenderer renderer;
	private Context mContext;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mContext = this;
		mGLSurfaceView = new GLSurfaceView(this);
		if (detectOpenGLES20()) {
			// Tell the surface view we want to create an OpenGL ES
			// 2.0-compatible
			// context, and set an OpenGL ES 2.0-compatible renderer.
			mGLSurfaceView.setEGLContextClientVersion(2);
			renderer = new GLES20TriangleRenderer(this);
			mGLSurfaceView.setRenderer(renderer);
		} else {
			// Set an OpenGL ES 1.x-compatible renderer. In a real application
			// this renderer might approximate the same output as the 2.0
			// renderer.
			// mGLSurfaceView.setRenderer(new TriangleRenderer(this));
		}
		setContentView(mGLSurfaceView);
	}

	private boolean detectOpenGLES20() {
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo info = am.getDeviceConfigurationInfo();
		return (info.reqGlEsVersion >= 0x20000);
	}

	@Override
	protected void onResume() {
		// Ideally a game should implement onResume() and onPause()
		// to take appropriate action when the activity looses focus
		super.onResume();
		mGLSurfaceView.onResume();
	}

	@Override
	protected void onPause() {
		// Ideally a game should implement onResume() and onPause()
		// to take appropriate action when the activity looses focus
		super.onPause();
		mGLSurfaceView.onPause();
		renderer.imagePool.initialize(mContext);
	}

	public long eventCount = 0;

	public int preTouchTimes = 5;
	public float pre_x = 0;
	public float pre_y = 0;
	long lastMillis = 0;

	public float pre_pre_x = 0;
	public float pre_pre_y = 0;
	long pre_lastMillis = 0;

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		eventCount++;
		float x = event.getX();
		float y = event.getY();
		super.onTouchEvent(event);

		long currentMillis = System.currentTimeMillis();

		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			renderer.x_move = x;
			renderer.y_move = y;

			renderer.spaceHolder.x = x;
			renderer.spaceHolder.x0 = x;
			renderer.spaceHolder.y = y;
			renderer.spaceHolder.y0 = y;

			pre_x = x;
			pre_y = y;

			Node node = renderer.spaceHolder.w.$("A c c1");
			Interpolator interpolator = node.interpolators.get("deceleration");

			interpolator.active = false;

			// System.out.println("x_move:    " + renderer.x_move +
			// "|||||||||||||||||y_move:    " + renderer.y_move);
		} else if (event.getAction() == MotionEvent.ACTION_MOVE) {
			if (lastMillis == 0) {
				lastMillis = currentMillis;
				return true;
			}

			renderer.x_move = x;
			renderer.y_move = y;
			if (preTouchTimes < 0) {
				preTouchTimes = 2;
				pre_pre_x = pre_x;
				pre_pre_y = pre_y;
				pre_lastMillis = lastMillis;

				pre_x = x;
				pre_y = y;

				lastMillis = currentMillis;
			}
			preTouchTimes--;

			renderer.spaceHolder.x = x;
			renderer.spaceHolder.y = y;
		} else if (event.getAction() == MotionEvent.ACTION_UP) {
			long delta = currentMillis - lastMillis;

			if (delta == 0 || x == pre_x || y == -pre_y) {
				delta = currentMillis - pre_lastMillis;
				pre_x = pre_pre_x;
				pre_y = pre_pre_y;
			}

			renderer.spaceHolder.x1 = x;
			renderer.spaceHolder.y1 = y;

			renderer.spaceHolder.ax = (x - pre_x) / delta;
			renderer.spaceHolder.ay = (y - pre_y) / delta;

			renderer.spaceHolder.resolveWorld();

			Node node = renderer.spaceHolder.w.$("A c c1");
			Interpolator interpolator = node.interpolators.get("deceleration");

			interpolator.active = true;
			interpolator.dxSpeed = renderer.spaceHolder.ax;
			interpolator.dySpeed = renderer.spaceHolder.ay;

			System.out.println("ax:    " + renderer.spaceHolder.ax + "     ----ay:    " + renderer.spaceHolder.ay);
			// System.out.println("eventCount:    " + eventCount +
			// "     ----renderCount:    " + renderer.renderCount);

		}
		return true;
	}

	private GLSurfaceView mGLSurfaceView;
}
