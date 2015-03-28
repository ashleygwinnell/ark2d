package org.ashleygwinnell.imagepacker;

public enum ExportFormat  {
	XML,
	JSON,
	TEXTUREPACKERJSONV1;
	
	public static boolean isValid(String s) {
		try {
			valueOf(s);
			return true;
		} catch (IllegalArgumentException e) {
			return false;
		}
	}
}
