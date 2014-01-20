package com.impactjs.ejecta;

import android.content.Context;
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class EjectaGLSurfaceView extends GLSurfaceView {

	
	public EjectaGLSurfaceView(Context context) {
		super(context);
	}
	
	public EjectaGLSurfaceView(Context context, AttributeSet attrs) {
		super(context, attrs);
		mRenderer = new EjectaRenderer(context, 600, 480 );
        setRenderer(mRenderer);
//        setEGLContextClientVersion(2);
	}
	
	EjectaRenderer mRenderer;
	public EjectaGLSurfaceView(Context context, int width, int height) {
		super(context);
		mRenderer = new EjectaRenderer(context, width, height);
        setRenderer(mRenderer);
	}
	
	@Override
	public void onResume() {
		mRenderer.nativeResume();
		super.onResume();
	}
	
	@Override
	public void onPause() {
		super.onPause();
		mRenderer.nativePause();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		System.out.println("nativeTouch onTouchEvent"+(int)event.getX()+"and"+(int)event.getY());
		mRenderer.nativeTouch(event.getAction(), (int)event.getX(), (int)event.getY());
//		int action = event.getAction();
		//return super.onTouchEvent(event);
		return true;
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		mRenderer.nativeOnKeyDown(keyCode);
		return super.onKeyDown(keyCode, event);
	}
	
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		mRenderer.nativeOnKeyUp(keyCode);
		return super.onKeyUp(keyCode, event);
	}
	
	@Override
	protected void onConfigurationChanged(Configuration newConfig) {
		System.out.println("onConfigurationChanged");

		super.onConfigurationChanged(newConfig);
	}
	
	public void onDestroy() {
		mRenderer.nativeFinalize();
	}
	
	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		System.out.println("onSizeChanged");

		super.onSizeChanged(w, h, oldw, oldh);
	}
	
	
	private static native void nativeSetPaths();
}
