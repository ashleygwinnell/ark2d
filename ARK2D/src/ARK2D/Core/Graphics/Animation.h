/*
 * Animation.h
 *
 *  Created on: 29 Jan 2011
 *      Author: Ashley
 */

#ifndef ARK_CORE_GRAPHICS_ANIMATION_H_
#define ARK_CORE_GRAPHICS_ANIMATION_H_

#include "../../Namespaces.h"
#include "../../Common/DLL.h"
#include "../Vector.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            /*!
             * \brief A container for animation base images. Quick creation into Animations.
             *
             * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
             */
            class ARK2D_API AnimationFrameList {
                public:
                    /*!
                    * Quick creation.
                    * @param endNumInclusive of -1 means iterator until an image is not found in the Description.
                    * @param sheet of NULL means use SpriteSheetStore
                    * @param desc of NULL means use SpriteSheetStore
                    *
                    * All frames must be in the same texture sheet.
                    */
                    static AnimationFrameList* create(string ref);
                    static AnimationFrameList* create(string ref, Image* sheet = NULL, SpriteSheetDescription* desc = NULL);
                    static AnimationFrameList* create(string ref, int startNum = 0, int endNumInclusive = -1, Image* sheet = NULL, SpriteSheetDescription* desc = NULL);

                    vector<Image*> frames;

                    AnimationFrameList();
                    vector<Image*>* getFrames();
                    virtual ~AnimationFrameList();
            };

            /*!
             * \brief A bunch of Image frames thrown together to produce an animation.
             *
             * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
             */
            class ARK2D_API Animation {
                public:
                    Animation();

                    void addImage(Image* image);
                    void addFrame(Image* image);
                    void addFrames(AnimationFrameList* frameList);
                    void setFrames(AnimationFrameList* frameList);
                    Image* getCurrentFrame();
                    inline unsigned int getCurrentFrameIndex() { return m_currentFrameIndex; }
                    inline void setCurrentFrameIndex(unsigned int i) { m_currentFrameIndex = i; }
                    Image* getNextFrame(unsigned int i = 1);
                    Image* getFrame(unsigned int i);

                    void setNextFrameModifier(signed int mod);
                    signed int getNextFrameModifier();

                    void update(GameTimer* timer);
                    void update(float delta);

                    void setTime(float f);
                    float getTime();

                    void setFrameTime(unsigned int ft);
                    void setFrameTime(float ft);
                    float getFrameTime();
                    float getDuration() { return getFrameTime(); }

                    unsigned int size();

                    void setPingPong(bool b);
                    bool isPingPong();

                    void setAlpha(float f);

                    void draw(int x, int y);
                    void drawCentered(int x, int y);
                    void drawFlipped(int x, int y, bool fx, bool fy);
                    void drawCenteredFlipped(int x, int y, bool fx, bool fy);

                    void clear();
                    void reset();

                    virtual ~Animation();

                private:
                    Vector<Image*> m_frames;
                    signed int m_currentFrameIndex;
                    signed int m_nextFrameModifier;

                    float m_timer;
                    float m_frameTime;

                    bool m_pingPong;
            };
        }
	}
}

#endif /* ANIMATION_H_ */
