package org.%COMPANY_NAME%.%GAME_SHORT_NAME%;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
 
import android.app.Activity;
import android.content.Context; 
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.opengl.GLSurfaceView; 
import android.opengl.GLU;
import android.os.Bundle; 
import android.os.Environment;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.inputmethod.InputMethodManager;
import android.widget.Toast;

public class %GAME_CLASS_NAME%Activity extends Activity {
    
	public static %GAME_CLASS_NAME%Activity s_activity;
    /** Called when the activity is first created. */
    @Override 
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState); 
         
        String gameOrientation = "%GAME_ORIENTATION%";
        if (gameOrientation.equals("landscape")) {
        	setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        } else if (gameOrientation.equals("portrait")) {
        	setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        } else {
        	
        }
        
        mGLView = new %GAME_CLASS_NAME%View(this);
        setContentView(mGLView);
        
        s_activity = this;
    } 
    
    @Override
    public void onBackPressed() {
    	//super.onBackPressed();
    	%GAME_CLASS_NAME%Renderer.nativeBackPressed();
    }
    
    @Override
    protected void onPause() {
    	Log.i("game", "Activity Pause");
    	super.onPause();
    	mGLView.onPause();
    }  
    
    @Override
    protected void onStart() {
    	Log.i("game", "Activity Resume");
    	super.onStart();    
    	//mGLView.onStart();  
    }
    
    @Override   
    protected void onResume() {
    	Log.i("game", "Activity Resume");
    	super.onResume();    
    	mGLView.onResume();  
    } 

   	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
	    Log.i("game", "Key Down: " + keyCode);
	    /*switch (keyCode) {
	    	case KeyEvent.KEYCODE_BACK:
				// Sample for handling the Menu button globally
				return true;
	    }*/
	    %GAME_CLASS_NAME%Renderer.nativeKeyDown(keyCode, new String(Character.toString((char)event.getUnicodeChar())));
	    return false;
	}
	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
 		Log.i("game", "Key Up: " + keyCode);
 		%GAME_CLASS_NAME%Renderer.nativeKeyUp(keyCode, new String(Character.toString((char)event.getUnicodeChar())));
 		return false;
	}

	public static void openSoftwareKeyboard() {
		Log.i("game", "keyboard open java");
		InputMethodManager imm = (InputMethodManager) s_activity.getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.toggleSoftInputFromWindow(s_activity.mGLView.getApplicationWindowToken(), InputMethodManager.SHOW_FORCED, 0);
	}
	public static void closeSoftwareKeyboard() {
		Log.i("game", "keyboard close java");
		InputMethodManager imm = (InputMethodManager) s_activity.getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.hideSoftInputFromWindow(s_activity.mGLView.getApplicationWindowToken(), 0);
	}
	public static String urlRequest(String urlString) {
		Log.i("game", "url request java: " + urlString);
		String response = new String("");
		try {
			URL url = new URL(urlString);
			HttpURLConnection connection = (HttpURLConnection) url.openConnection();
			connection.connect();
			InputStream stream = connection.getInputStream();
			BufferedInputStream buff = new BufferedInputStream(stream);
			int character = -1;
			while ((character = buff.read()) != -1) {
				response += (char) character;
			}
			stream.close();
			return response;
		} catch (IOException e) {
			Log.i("game", "IO Exception: " + e.getMessage());
			Log.i("game", "Have you added \"INTERNET\" to {'android':{'permissions':[]}} to the ARK2D configuration?");
			
			s_activity.runOnUiThread(new Runnable() {
				public void run() {
					Toast.makeText(s_activity, "No Internet Connection", Toast.LENGTH_SHORT).show();
				}
			});
		}
		return response;
	}
    private %GAME_CLASS_NAME%View mGLView;
    
    static {
    	try {
    		//System.load("data/data/org.%COMPANY_NAME%.%GAME_SHORT_NAME%/lib/libark2d.so");
    		System.loadLibrary("openal");
    		System.loadLibrary("zip");
    		System.loadLibrary("ark2d");
    		System.loadLibrary("%GAME_SHORT_NAME%");
    		Log.i("game", "Loading libraries");
    	} catch(UnsatisfiedLinkError e) {
    		throw new RuntimeException("Could not load native libraries");
    	}
    } 
    
    
}
 
class %GAME_CLASS_NAME%View extends GLSurfaceView {  
	public %GAME_CLASS_NAME%View(Context context) {
		super(context);
		mRenderer = new %GAME_CLASS_NAME%Renderer(context);
        setRenderer(mRenderer); 
	}
	
	@Override
	public void onPause() {
		Log.i("game", "View Pause");
		super.onPause();
		%GAME_CLASS_NAME%Renderer.nativePause();
	}
	/*@Override
	public void onStart() {
		Log.i("game", "View Start");
		super.onStart();
		%GAME_CLASS_NAME%Renderer.nativeStart();
	}*/
	@Override
	public void onResume() {
		Log.i("game", "View Resume");
		super.onResume();
		%GAME_CLASS_NAME%Renderer.nativeResume();
	}
	
	
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		Log.i("game", "View Touch Event");
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			//int pointerIndex = event.getPointerId(0);
			int thisx = (int) event.getX(); //pointerIndex); 
			int thisy = (int) event.getY(); //pointerIndex); 
			
			//Log.e("jni", "touch-down: " + thisx + "," + thisy);
			%GAME_CLASS_NAME%Renderer.nativeTouchDown(thisx, thisy);
		} else if (event.getAction() == MotionEvent.ACTION_MOVE) {
			int thisx = (int) event.getX(); //pointerIndex); 
			int thisy = (int) event.getY(); //pointerIndex);
			//Log.e("jni", "touch-move: " + thisx + "," + thisy);
			%GAME_CLASS_NAME%Renderer.nativeTouchMove(thisx, thisy);
		} else if (event.getAction() == MotionEvent.ACTION_UP) {
			int thisx = (int) event.getX(); //pointerIndex); 
			int thisy = (int) event.getY(); //pointerIndex);
			//Log.e("jni", "touch-up: " + thisx + "," + thisy);
			%GAME_CLASS_NAME%Renderer.nativeTouchUp(thisx, thisy);
		}
		return true; 
		//return super.onTouchEvent(event); 
	}
	 
	%GAME_CLASS_NAME%Renderer mRenderer;
	 
	
}
 
class %GAME_CLASS_NAME%Renderer implements GLSurfaceView.Renderer {
	//public static boolean s_initted = false;
	public Context context;
	public %GAME_CLASS_NAME%Renderer(Context context) {
		this.context = context;
	}
	
	public static void createDir(String path) {
		Log.i("game", "making path; " + path);
		File extf = new File(path);
		if (!extf.exists()) {
	        if (!extf.mkdirs()) {
	            Log.e("game", "Problem creating folder: " + path);
	        }
	    }
	}
	
	@Override  
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	//	nativeInit();   
		//if (s_initted) {
		//	Log.i("game", "Surface Already Created");
		//} else {
			Log.i("game", "Surface Created");
			//s_initted = true;
		 
			String apkFilePath = null; 
			ApplicationInfo appInfo = null;
			String externalDataPath = null; 
			PackageManager packMgmr = context.getPackageManager();  
			try {  
	            appInfo = packMgmr.getApplicationInfo("%PACKAGE_DOT_NOTATION%", 0);
	            externalDataPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/Android/data/%PACKAGE_DOT_NOTATION%/files/"; // does not include trailing slash.
	        } catch (NameNotFoundException e) {
	            e.printStackTrace();
	            throw new RuntimeException("Unable to locate assets, aborting...");
	        }
			apkFilePath = appInfo.sourceDir;
			Log.i("game", "APK File Path:" + apkFilePath);
			Log.i("game", "Save File Path:" + externalDataPath);
		
			createDir(externalDataPath);
			createDir(externalDataPath+"assets/"); 
			
			
			nativeInit(apkFilePath, externalDataPath);
		//}
	}
	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		Log.i("game", "Surface Changed");
		/*int w = width;//480;
		int h = height;//320;
		gl.glViewport(0, 0, w, h);
		
		gl.glMatrixMode(GL10.GL_PROJECTION); 
		gl.glLoadIdentity(); 

		//gl.glOrthox(0, w, h, 0,  -1, 1);//(0, 1,  1, 0, -1, 1);
		GLU.gluOrtho2D(gl, 0.0f, w, h, 0.0f);
  
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();
		*/
		nativeResize(width, height);
	} 
	@Override
	public void onDrawFrame(GL10 gl) {
		//Log.i("game", "Render");
		nativeRender();
	}  
	  
	public static native void nativeInit(String apkPath, String externalDataPath); 
	public static native void nativeResize(int width, int height);
	public static native void nativeRender();
	public static native void nativeDone();
	
	public static native void nativeTouchDown(int x, int y); 
	public static native void nativeTouchMove(int x, int y);
	public static native void nativeTouchUp(int x, int y);

	public static native void nativeKeyDown(int key, String keyChar);
	public static native void nativeKeyUp(int key, String keyChar);
	
	public static native void nativeStart();
	public static native void nativePause();
	public static native void nativeResume();
	public static native void nativeBackPressed();
	
	
}




