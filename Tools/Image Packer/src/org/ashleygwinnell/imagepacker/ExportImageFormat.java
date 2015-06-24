package org.ashleygwinnell.imagepacker;

public enum ExportImageFormat  {
	PNG,
	JPG;
	
	public static boolean isValid(String s) {
		try {
			valueOf(s);
			return true;
		} catch (IllegalArgumentException e) {
			return false;
		}
	}
}
