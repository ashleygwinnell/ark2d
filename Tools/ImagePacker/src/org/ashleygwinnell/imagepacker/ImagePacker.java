package org.ashleygwinnell.imagepacker;

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import javax.imageio.ImageIO;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class ImagePacker {
	
	private ArrayList<Image> m_images;
	private int m_maxTextureSize;
	private int m_spacing;
	private String m_exportName;
	private ExportFormat m_exportFormat;
	
	public ImagePacker() {
		m_images = new ArrayList<Image>();
		m_maxTextureSize = 512;
		m_spacing = 1;
		m_exportName = "";
		m_exportFormat = ExportFormat.JSON;
	}
	
	public void addImage(Image img) {
		m_images.add(img);
	}
	
	public void setMaxTextureSize(int size) throws Exception {
		if (!Util.isPowerOf2(size)) {
			throw new Exception();
		}
		m_maxTextureSize = size;
	}
	
	public void setExportFormat(ExportFormat e) {
		m_exportFormat = e;
	}
	public void setExportName(String s) {
		m_exportName = s;
	}
	
	public void setSpacing(int s) {
		m_spacing = s;
	}
	public int getSpacing() {
		return m_spacing;
	}
	
	public void pack() throws Exception {
		
		layoutImagesImpl();
		
		// write image.
		BufferedImage bi = new BufferedImage(m_maxTextureSize, m_maxTextureSize, BufferedImage.TYPE_4BYTE_ABGR);
		Graphics2D g2d = bi.createGraphics();
		for (int i = 0; i < m_images.size(); i++) {
			Image c = m_images.get(i);
			g2d.drawImage(c.getAWTImage(), c.x, c.y, null);
		}
	//	WritableRaster out = bi.copyData(null);
		try {
			ImageIO.write(bi, "png", new File(m_exportName + ".png"));
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		
		// write xml file?
		final String CRLF = "\r\n";
		if (m_exportFormat == ExportFormat.XML) 
		{
			String xml = new String();
			xml += "<?xml version=\"1.0\" ?>" + CRLF;
			xml += "<sheet size=\"" + m_maxTextureSize + "\" spacing=\"" + m_spacing + "\">" + CRLF;
			for (int i = 0; i < m_images.size(); i++) {
				Image c = m_images.get(i);
				xml += "<item x=\"" + c.x + "\" y=\"" + c.y + "\" width=\"" + c.getWidth() + "\" height=\"" + c.getHeight() + "\" name=\"" + c.getName() + "\" path=\"" + c.getPath() + "\" />" + CRLF;
			}
			xml += "</sheet>" + CRLF;
			try {
				FileOutputStream s = new FileOutputStream(new File(m_exportName + ".xml"));
				s.write(xml.getBytes());
				s.close();
				//s_rfs.add(pathfile + ".xml");
			} catch (FileNotFoundException e1) {
				e1.printStackTrace();
			} catch (IOException e2) {
				e2.printStackTrace();
			}
		} 
		else if (m_exportFormat == ExportFormat.JSON) 
		{
			String json = new String();
			json += "{";
				json += "\"sheet\": {";
					json += "\"size\": " + m_maxTextureSize + ",";
					json += "\"spacing\": " + m_spacing + ",";
					json += "\"items\": [";
						for (int i = 0; i < m_images.size(); i++) {
							Image c = m_images.get(i);
							json += "{";
								json += "\"x\":" + c.x + ",";
								json += "\"y\":" + c.y + ",";
								json += "\"width\":" + c.getWidth() + ",";
								json += "\"height\":" + c.getHeight() + ",";
								json += "\"name\": \"" + c.getName() + "\",";
								json += "\"path\": \"" + c.getPath().replace("\\", "\\\\") + "\"";
							json += "}";
							if (i != m_images.size() - 1) {
								json += ",";
							}
						}
					json += "]";
				json += "}";
			json += "}";
			
			try {
				FileOutputStream s = new FileOutputStream(new File(m_exportName + ".json"));
				s.write(json.getBytes());
				s.close();
				//s_rfs.add(m_exportName + "_json.json");
			} catch (FileNotFoundException e1) {
				e1.printStackTrace();
			} catch (IOException e2) {
				e2.printStackTrace();
			}
		}
		
	}
	
	private void layoutImagesImpl() throws Exception {
		flowStrategy();
	}
	
	private void flowStrategy() throws Exception {
		
		// Sort the images from largest to smallest.
		int maxpixels = 1;
		ArrayList<Image> sorted = new ArrayList<Image>();
		for (int i = 0; i < m_images.size(); i++) {
			Image c = m_images.get(i);
			
			if (c.getWidth() > m_maxTextureSize || c.getHeight() > m_maxTextureSize) {
				throw new Exception(c.getName() + " is too large for the texture.");
			}
			
			int size = c.getWidth() * c.getHeight();
			if (size >= maxpixels) {
				sorted.add(0, c);
				maxpixels = size;
			} else {
				sorted.add(c);
			}
		}
		
		// Lay them out.
		int curx = 0;
		int cury = 0;
		int tempheight = 0;
		for (int i = 0; i < sorted.size(); i++) {
			Image c = sorted.get(i);
			
			// Does it exceed the width? Start a new row.
			if (curx + c.getWidth() + m_spacing > m_maxTextureSize ) {
				cury += tempheight;
				curx = 0;
			}
			
			// Does the current image exceeded the texture height? This is bad.
			if (cury + c.getHeight() + m_spacing > m_maxTextureSize) {
				throw new Exception("The images do not fit in to this texture.");
			}
			
			if (c.getHeight() + m_spacing > tempheight) {
				tempheight = c.getHeight() + m_spacing;
			}
			
			c.x = curx;
			c.y = cury;
			
			curx += c.getWidth() + m_spacing;
			
			
			
		}
		
		m_images.clear();
		for (int i = 0; i < sorted.size(); i++) {
			m_images.add(sorted.get(i));
		}
	}
	
	public static void init(String jsonString) throws JSONException, IllegalArgumentException, Exception {
		//System.out.println(jsonString);
		JSONObject o = new JSONObject(jsonString);
		
		ImagePacker packer = new ImagePacker();
		
		int texSize = o.getInt("max_texture_size");
		packer.setMaxTextureSize(texSize);
		
		int spacing = o.getInt("spacing");
		packer.setSpacing(spacing);
		
		JSONArray files = o.getJSONArray("files");
		for(int i = 0; i < files.length(); i++) {
			String file = files.getString(i);
			Image img = new Image(new File(file));
			packer.addImage(img);
		}
		
		String output = o.getString("output");
		packer.setExportName(output);

		ExportFormat f = ExportFormat.valueOf(o.getString("output_format"));
		packer.setExportFormat(f);
		
		packer.pack();
	}
	
	/**
	 * Arg passed in is JSON.
	 * @param args
	 */
	public static void main (String[] args)  {
		try {
			String jsonString = args[0];
			init(jsonString);
		} catch (ArrayIndexOutOfBoundsException e) { 
			System.err.println("Missing command line JSON argument.");
		} catch (JSONException e) { 
			System.err.println("Invalid JSON passed.");
		} catch (IllegalArgumentException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	
	
}


