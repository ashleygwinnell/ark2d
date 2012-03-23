/*
 * SpriteSheetDescriptionItem.h
 *
 *  Created on: 31 Jul 2010
 *      Author: Ashley
 */

#ifndef SPRITESHEETDESCRIPTIONITEM_H_
#define SPRITESHEETDESCRIPTIONITEM_H_

/*!
 * \brief An item in a SpriteSheetDescription.
 *
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */
class SpriteSheetDescriptionItem {
	private:
		const char* m_name;
		unsigned int m_x;
		unsigned int m_y;
		unsigned int m_width;
		unsigned int m_height;

	public:
		SpriteSheetDescriptionItem();
		const char* getName() const;
		unsigned int getX() const;
		unsigned int getY() const;
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		void setName(const char* name);
		void setX(unsigned int x);
		void setY(unsigned int y);
		void setWidth(unsigned int width);
		void setHeight(unsigned int height);
		~SpriteSheetDescriptionItem();
};

#endif /* SPRITESHEETDESCRIPTIONITEM_H_ */
