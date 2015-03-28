/*
 * SpriteSheetDescriptionItem.h
 *
 *  Created on: 31 Jul 2010
 *      Author: Ashley
 */

#ifndef SPRITESHEETDESCRIPTIONITEM_H_
#define SPRITESHEETDESCRIPTIONITEM_H_

#include "../Includes.h"

namespace ARK {
	namespace Graphics {

		/*!
		 * \brief An item in a SpriteSheetDescription.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class ARK2D_API SpriteSheetDescriptionItem {
			private:
				const char* m_name;
				unsigned int m_x;
				unsigned int m_y;
				unsigned int m_width;
				unsigned int m_height;
				bool m_rotated;

			public:
				SpriteSheetDescriptionItem();
				const char* getName() const;
				unsigned int getX() const;
				unsigned int getY() const;
				unsigned int getWidth() const;
				unsigned int getHeight() const;
				bool isRotated() const;
				void setName(const char* name);
				void setX(unsigned int x);
				void setY(unsigned int y);
				void setWidth(unsigned int width);
				void setHeight(unsigned int height);
				void setRotated(bool b);
				~SpriteSheetDescriptionItem();
		};
	}
}

#endif /* SPRITESHEETDESCRIPTIONITEM_H_ */
