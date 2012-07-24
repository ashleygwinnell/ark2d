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
	public void maxTextureSizeMustBePO2() {
		try {
			packer.setMaxTextureSize(512);
		} catch (Exception e) {
			Assert.fail();
		}
	}
	
	@Test(expected=Exception.class)
	public void maxTextureSizeMustBePO2_boundary() throws Exception {
		packer.setMaxTextureSize(511);
	}
	
	@Test
	public void canPackMultipleFiles() {
		try {
			packer.setMaxTextureSize(512);
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
			packer.setMaxTextureSize(256);
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
			packer.setMaxTextureSize(256);
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
		packer.setMaxTextureSize(256);
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
