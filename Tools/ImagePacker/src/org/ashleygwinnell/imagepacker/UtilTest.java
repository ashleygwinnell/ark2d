package org.ashleygwinnell.imagepacker;

import org.junit.Assert;
import org.junit.Test;

public class UtilTest {

	@Test
	public void testIsPowerOf2() {
		Assert.assertTrue(Util.isPowerOf2(1));
		Assert.assertTrue(Util.isPowerOf2(2));
		Assert.assertTrue(Util.isPowerOf2(16));
		Assert.assertTrue(Util.isPowerOf2(256));
		
		Assert.assertFalse(Util.isPowerOf2(15));
		Assert.assertFalse(Util.isPowerOf2(255));
	}
	
}
