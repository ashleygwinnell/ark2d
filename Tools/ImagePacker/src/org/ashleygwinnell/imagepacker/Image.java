package org.ashleygwinnell.imagepacker;

import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class Image {
	private String m_name;
	private String m_path;
	private java.awt.Image m_image;
	
	int index;
	int x;
	int y;
	
	private int m_width;
	private int m_height;
	
	public Image(File f) throws IOException {
		java.awt.Image img = ImageIO.read(f);
		
		index = 0;
		x = 0;
		y = 0;
		
		m_image = img;
		m_name = f.getName();
		m_path = f.getAbsolutePath();
		m_width = img.getWidth(null);
		m_height = img.getHeight(null);
	}
	
	public void setName(String name) {
		m_name = name;
	}
	public String getName() {
		return m_name;
	}
	
	public void setPath(String path) {
		m_path = path;
	}
	public String getPath() {
		return m_path;
	}
	
	public void setAWTImage(java.awt.Image i) {
		m_image = i;
	}
	java.awt.Image getAWTImage() {
		return m_image;
	}
	
	public void setLocation(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	public void setSize(int width, int height) {
		m_width = width;
		m_height = height;
	}
	public int getWidth() {
		return m_width;
	}
	public int getHeight() {
		return m_height;
	}
}
