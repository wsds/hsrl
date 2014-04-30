package com.open.hsrl;

public class MyNativeActivity extends android.app.NativeActivity {

    static {
		System.loadLibrary("sooo1");
        System.loadLibrary("hsrl-frame");
    }
 }