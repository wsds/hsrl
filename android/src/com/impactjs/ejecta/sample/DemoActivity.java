package com.impactjs.ejecta.sample;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;

import com.impactjs.ejecta.EjectaGLSurfaceView;
import com.impactjs.ejecta.EjectaRenderer;
import com.open.hsrl.WorldHelper;

public class DemoActivity extends Activity {
	private GLSurfaceView mGLView;
	EjectaRenderer mRenderer;
	private LinearLayout linearLayout;
	private Context mContext;
	WorldHelper worldHelper = WorldHelper.getInstance();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		mContext = this;
		worldHelper.context = mContext;
		int width = getWindowManager().getDefaultDisplay().getWidth();
		int height = getWindowManager().getDefaultDisplay().getHeight();

		setContentView(R.layout.main);
//		mGLView = new EjectaGLSurfaceView(this, width, height);
//		mRenderer = new EjectaRenderer(600, 480);
//		mGLView.setRenderer(mRenderer);
//
//		linearLayout = (LinearLayout) findViewById(R.id.testLayout);
//		linearLayout.addView(mGLView);

		// setContentView(mGLView);
		//
		// int i = 1;
		// i++;

		Button button = (Button) findViewById(R.id.buttonTest);
		button.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent entry = new Intent(Intent.ACTION_MAIN);
				entry.addCategory(Intent.CATEGORY_SAMPLE_CODE);
				// entry.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
				entry.setClassName("com.impactjs.ejecta.sample", "com.test.view20.GLES20Activity");
				mContext.startActivity(entry);

			}
		});

		Button button2 = (Button) findViewById(R.id.buttonTest2);
		button2.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				worldHelper.initializeWorld();
			}
		});

	}

	@Override
	protected void onDestroy() {
		((EjectaGLSurfaceView) mGLView).onDestroy();
		super.onDestroy();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
	}

	static {
		// System.loadLibrary("corefoundation");
		// System.loadLibrary("JavaScriptCore");
		// System.loadLibrary("ejecta");
	}

}