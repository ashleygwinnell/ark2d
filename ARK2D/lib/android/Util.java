package org.ashleygwinnell.omnivium;

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

public class Util {
	public static void fillRect(GL10 gl, int x, int y, int w, int h) {
		gl.glDisable(GL10.GL_TEXTURE_2D); 
		gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
		 
		//int m_raw_vertices[] = {    
		float m_raw_vertices[] = {
			0,	0,	0,  // tl
			w,	0,	0,	// tr
			0,	h,	0,  // bl
			w,	h,	0	// br
	    };
		
		ByteBuffer vbb = ByteBuffer.allocateDirect(m_raw_vertices.length * 4);
	    vbb.order(ByteOrder.nativeOrder());
	   // IntBuffer vertexBuffer = vbb.asIntBuffer();
	    FloatBuffer vertexBuffer = vbb.asFloatBuffer();
	    vertexBuffer.put(m_raw_vertices); 
	    vertexBuffer.position(0);
		
		gl.glTranslatef(x, y, 0); 
		
		//gl.glVertexPointer(3, GL10.GL_FIXED, 0, vertexBuffer);
		gl.glVertexPointer(3, GL10.GL_FLOAT, 0, vertexBuffer);
		gl.glDrawArrays(GL10.GL_TRIANGLE_STRIP, 0, 4);
		
		gl.glTranslatef(-x, -y, 0);
		
		gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
		gl.glEnable(GL10.GL_TEXTURE_2D);
		
		vbb = null;
		vertexBuffer = null;
	}
}
