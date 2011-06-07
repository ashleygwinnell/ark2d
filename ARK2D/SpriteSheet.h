/*
 * SpriteSheet.h
 *
 *  Created on: 11-Jan-2010
 *      Author: Ashley
 */

#ifndef SPRITESHEET_H_
#define SPRITESHEET_H_

class SpriteSheet : Image {
private:
	int tw;
	int th;
	int margin;
	int spacing;
	Image target;
	Image subimages[][];

public:
	SpriteSheet(Image image, int tw, int th);
	void getSubImage(int x, int y);
};

#endif /* SPRITESHEET_H_ */
