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

	public long eventCount=0;
	public long eventCount_DOWN=0;
	public long eventCount_MOVE=0;

	public long eventStep_DOWN=0;
	public long eventStep_MOVE=0;


	@Override
	public boolean onTouchEvent(MotionEvent event) {
		super.onTouchEvent(event);
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			renderer.x_move = event.getX();
			renderer.y_move = event.getY();
			eventCount_DOWN++;
			eventStep_DOWN=(eventStep_DOWN+1)%60;
			if(eventStep_DOWN==1){
				eventStep_DOWN++;
				eventCount++;
			}
//			System.out.println("x_move:    " + renderer.x_move + "|||||||||||||||||y_move:    " + renderer.y_move);
		} else if (event.getAction() == MotionEvent.ACTION_MOVE) {
			eventCount_MOVE++;
			eventStep_MOVE=(eventStep_MOVE+1)%60;
			if(eventStep_MOVE==1){
				eventStep_MOVE++;
				eventCount++;
			}
			renderer.x_move = event.getX();
			renderer.y_move = event.getY();
		}
		else if(event.getAction() == MotionEvent.ACTION_UP){
			System.out.println("eventCount:    " + eventCount + "     ----renderCount:    " + renderer.renderCount);
			System.out.println("eventCount_MOVE:    " + eventCount_MOVE + "     ----eventCount_DOWN:    " + eventCount_DOWN);
		}
		return true;
	}

	private GLSurfaceView mGLSurfaceView;
}
