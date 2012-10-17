package org.ashleygwinnell.imageresizer;

import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.Transparency;
import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class ImageResizer {

	/**
     * Convenience method that returns a scaled instance of the
     * provided {@code BufferedImage}.
     *
     * @param img the original image to be scaled
     * @param targetWidth the desired width of the scaled instance,
     *    in pixels
     * @param targetHeight the desired height of the scaled instance,
     *    in pixels
     * @param hint one of the rendering hints that corresponds to
     *    {@code RenderingHints.KEY_INTERPOLATION} (e.g.
     *    {@code RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR},
     *    {@code RenderingHints.VALUE_INTERPOLATION_BILINEAR},
     *    {@code RenderingHints.VALUE_INTERPOLATION_BICUBIC})
     * @param higherQuality if true, this method will use a multi-step
     *    scaling technique that provides higher quality than the usual
     *    one-step technique (only useful in downscaling cases, where
     *    {@code targetWidth} or {@code targetHeight} is
     *    smaller than the original dimensions, and generally only when
     *    the {@code BILINEAR} hint is specified)
     * @return a scaled version of the original {@code BufferedImage}
     */
    public static BufferedImage getScaledInstance(BufferedImage img,
                                           int targetWidth,
                                           int targetHeight,
                                           Object hint,
                                           boolean higherQuality)
    {
        int type = (img.getTransparency() == Transparency.OPAQUE) ?
            BufferedImage.TYPE_INT_RGB : BufferedImage.TYPE_INT_ARGB;
        BufferedImage ret = (BufferedImage)img;
        int w, h;
        if (higherQuality) {
            // Use multi-step technique: start with original size, then
            // scale down in multiple passes with drawImage()
            // until the target size is reached
            w = img.getWidth();
            h = img.getHeight();
        } else {
            // Use one-step technique: scale directly from original
            // size to target size with a single drawImage() call
            w = targetWidth;
            h = targetHeight;
        }
        
        do {
            if (higherQuality && w > targetWidth) {
                w /= 2;
                if (w < targetWidth) {
                    w = targetWidth;
                }
            }

            if (higherQuality && h > targetHeight) {
                h /= 2;
                if (h < targetHeight) {
                    h = targetHeight;
                }
            }

            BufferedImage tmp = new BufferedImage(w, h, type);
            Graphics2D g2 = tmp.createGraphics();
            g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION, hint);
            g2.drawImage(ret, 0, 0, w, h, null);
            g2.dispose();

            ret = tmp;
        } while (w != targetWidth || h != targetHeight);

        return ret;
    }
	
    
    /*
	
	[
		{
			"from": "filename.png",
			"to": [
				{
					"filename": "new1.png",
					"width": 128,
					"height":128,
					"interpolation": "VALUE_INTERPOLATION_BICUBIC" 			nearest_neighbour, bilinear, bicubic (optional)
				}
			]
		}
	]
	
	
	 */
	public static void main(String[] args) {
		try {
			String jsonString = args[0];
			
			JSONArray o = new JSONArray(jsonString);
			for(int i = 0; i < o.length(); i++) {
				JSONObject item = o.getJSONObject(i);
				String from = item.getString("from");
				JSONArray to = item.getJSONArray("to");
				for(int j = 0; j < to.length(); j++) {
					JSONObject itemTo = to.getJSONObject(j);
					String tofilename = itemTo.getString("filename");
					int width = itemTo.getInt("width");
					int height = itemTo.getInt("height");
					//String interpolation = itemTo.get(key)
					
					java.awt.Image img = ImageIO.read(new File(from));
					
					if (width > img.getWidth(null) && height > img.getHeight(null)) {
						continue;
					}
					
					BufferedImage bi = new BufferedImage(img.getWidth(null), img.getHeight(null), BufferedImage.TYPE_4BYTE_ABGR);
					Graphics2D g2d = bi.createGraphics();
					g2d.drawImage(img, 0, 0, null);
					
					String renderingHint = itemTo.getString("interpolation");
					Object actualRenderingHint = RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR;
					if (renderingHint.equals("bicubic")) {
						actualRenderingHint = RenderingHints.VALUE_INTERPOLATION_BICUBIC;
					} else if (renderingHint.equals("bilinear")) {
						actualRenderingHint = RenderingHints.VALUE_INTERPOLATION_BILINEAR;
					}
					
					BufferedImage toImage = ImageResizer.getScaledInstance(bi, width, height, actualRenderingHint, true);
					ImageIO.write(toImage, "png", new File(tofilename));
				}
			}
			
			
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
