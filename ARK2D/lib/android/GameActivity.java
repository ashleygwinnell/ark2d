package org.%COMPANY_NAME%.%GAME_SHORT_NAME%;

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
import android.util.Log;
import android.view.MotionEvent;

public class %GAME_CLASS_NAME%Activity extends Activity {
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
        
        
    } 
    
    @Override
    protected void onPause() {
    	Log.i("game", "Activity Pause");
    	super.onPause();
    	mGLView.onPause();
    }  
    
    @Override   
    protected void onResume() {
    	Log.i("game", "Activity Resume");
    	super.onResume();     
    	mGLView.onResume();  
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
	public Context context;
	public %GAME_CLASS_NAME%Renderer(Context context) {
		this.context = context;
	}
	@Override  
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	//	nativeInit();   
		Log.i("game", "Surface Created");
	
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = context.getPackageManager();  
		try {  
            appInfo = packMgmr.getApplicationInfo("%PACKAGE_DOT_NOTATION%", 0);
        } catch (NameNotFoundException e) {
            e.printStackTrace();
            throw new RuntimeException("Unable to locate assets, aborting...");
        }
		apkFilePath = appInfo.sourceDir;
		Log.i("game", "APK File Path:" + apkFilePath);
		nativeInit(apkFilePath);
		 
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
	  
	public static native void nativeInit(String apkPath); 
	public static native void nativeResize(int width, int height);
	public static native void nativeRender();
	public static native void nativeDone();
	
	public static native void nativeTouchDown(int x, int y); 
	public static native void nativeTouchMove(int x, int y);
	public static native void nativeTouchUp(int x, int y);
	
	public static native void nativePause();
	public static native void nativeResume();
	
	
}
