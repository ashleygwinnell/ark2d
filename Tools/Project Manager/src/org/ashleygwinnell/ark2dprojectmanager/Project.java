package org.ashleygwinnell.ark2dprojectmanager;

import java.awt.Color;

class MultiplatformString { 
	public String windows;
	public String osx;
}

class Developer {
	public String name;
	public String nameSafe;
	public String website; 
}

public class Project {

	public String name;
	public String nameSafe;
	public String description;
	
	public String className;
	
	public enum Orientation { PORTRAIT, LANDSCAPE };
	public Orientation orientation;
	
	public Color clearColor;
	
	
	
}
