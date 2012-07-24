package org.ashleygwinnell.imagepacker;

public class Util {
	
	public static boolean isPowerOf2(int number) {
	
		if (number < 0) {
			 return false;
		} else {
			 return (number & (number - 1)) == 0;
		}
		
	}
	
}
