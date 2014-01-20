package com.impactjs.ejecta.sample;

import android.app.Activity;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.widget.LinearLayout;

import com.impactjs.ejecta.CubeRenderer;
import com.impactjs.ejecta.EjectaGLSurfaceView;
import com.impactjs.ejecta.EjectaRenderer;

public class DemoActivity extends Activity {
	private GLSurfaceView mGLView;
	EjectaRenderer mRenderer;
	private GLSurfaceView mGLSurfaceView;
	private LinearLayout linearLayout;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		int width = getWindowManager().getDefaultDisplay().getWidth();
		int height = getWindowManager().getDefaultDisplay().getHeight();
		// mGLView = new EjectaGLSurfaceView(this, width, height);
		setContentView(R.layout.main);
//		mGLView = (GLSurfaceView) findViewById(R.id.vp_content);
//		mRenderer = new EjectaRenderer(600, 480);
//		mGLView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
//		mGLView.setRenderer(mRenderer);
//
//		mGLView.getHolder().setFormat(PixelFormat.TRANSLUCENT);
		
		
        // Create our Preview view and set it as the content of our
        // Activity
        mGLSurfaceView = new GLSurfaceView(this);
        // We want an 8888 pixel format because that's required for
        // a translucent window.
        // And we want a depth buffer.
        mGLSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        // Tell the cube renderer that we want to render a translucent version
        // of the cube:
        mGLSurfaceView.setRenderer(new CubeRenderer(true));
        // Use a surface format with an Alpha channel:
        mGLSurfaceView.getHolder().setFormat(PixelFormat.TRANSLUCENT);

        linearLayout = (LinearLayout) findViewById(R.id.testLayout);   
        linearLayout.addView(mGLSurfaceView);
        
//        setContentView(mGLSurfaceView);
		// setContentView(mGLView);

		int i = 1;
		i++;
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		((EjectaGLSurfaceView) mGLView).onDestroy();
		super.onDestroy();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
	}

	static {
		// System.loadLibrary("corefoundation");
		// System.loadLibrary("JavaScriptCore");
		// System.loadLibrary("ejecta");
	}

}