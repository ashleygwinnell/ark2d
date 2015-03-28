package org.ashleygwinnell.imagepacker;

import java.io.File;
 
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class ImagePackerTest {

	ImagePacker packer; 
	
	@Before
	public void init() {
		packer = new ImagePacker();
	}
	
	@Test
	public void canLoadFile() {
		try {
			Image img = new Image(new File("data/tests/60x60.png"));
		} catch (Exception e) {
			Assert.fail();
		}
	}
	
	@Test
	public void maxTextureWidthMustBePO2() {
		try {
			packer.setMaxTextureWidth(512);
		} catch (Exception e) {
			Assert.fail();
		}
	}
	
	@Test
	public void maxTextureHeightMustBePO2() {
		try {
			packer.setMaxTextureHeight(512);
		} catch (Exception e) {
			Assert.fail();
		}
	}
	
	/*@Test(expected=Exception.class)
	public void maxTextureWidthMustBePO2_boundary() throws Exception {
		packer.setMaxTextureWidth(511);
	}
	
	@Test(expected=Exception.class)
	public void maxTextureHeightMustBePO2_boundary() throws Exception {
		packer.setMaxTextureHeight(511);
	}*/
	
	@Test
	public void canPackMultipleFiles() {
		try {
			packer.setMaxTextureWidth(512);
			packer.setMaxTextureHeight(512);
			packer.setExportFormat(ExportFormat.JSON);
			packer.setExportName("data/tests/atlas-1");
			packer.addImage(new Image(new File("data/tests/60x60.png")));
			packer.addImage(new Image(new File("data/tests/60x60.png")));
			packer.pack();
		} catch (Exception e) {
			Assert.fail();	
		}
	}
	
	@Test
	public void canExportJson() {
		try {
			packer.setMaxTextureWidth(256);
			packer.setMaxTextureHeight(256);
			packer.setExportFormat(ExportFormat.JSON);
			packer.setExportName("data/tests/atlas-2");
			packer.addImage(new Image(new File("data/tests/60x60.png")));
			packer.addImage(new Image(new File("data/tests/60x60.png")));
			packer.pack();
		} catch (Exception e) {
			Assert.fail();	
		}
	}
	
	@Test
	public void canExportXml() {
		try {
			packer.setMaxTextureWidth(256);
			packer.setMaxTextureHeight(256);
			packer.setExportFormat(ExportFormat.XML);
			packer.setExportName("data/tests/atlas-3");
			packer.addImage(new Image(new File("data/tests/60x60.png")));
			packer.addImage(new Image(new File("data/tests/60x60.png")));
			packer.pack();
		} catch (Exception e) {
			Assert.fail();	
		}
	}
	
	@Test(expected=Exception.class) 
	public void cannotExceedMaxTextureSize() throws Exception {
		packer.setMaxTextureWidth(256);
		packer.setMaxTextureHeight(256);
		packer.setExportFormat(ExportFormat.JSON);
		packer.setExportName("data/tests/atlas-4");
		packer.addImage(new Image(new File("data/tests/120x120.png")));
		packer.addImage(new Image(new File("data/tests/120x120.png")));
		packer.addImage(new Image(new File("data/tests/120x120.png")));
		packer.addImage(new Image(new File("data/tests/120x120.png")));
		packer.addImage(new Image(new File("data/tests/120x120.png"))); // this cannot fit into 256x256
		packer.pack();
	}
	
}
