/*
 * AdvancedPolygon.h
 *
 *  Created on: 28th August 2015
 *      Author: ashleygwinnell
 */
#ifndef ARK_CORE_GEOMETRY_ADVANCEDPOLYGON_H_
#define ARK_CORE_GEOMETRY_ADVANCEDPOLYGON_H_

#include "Polygon.h"

namespace ARK {
    namespace Core {
        namespace Geometry {

            class ARK2D_API AdvancedPolygon {
                public:
                    vector<ARK::Core::Geometry::Polygon<float> > outers;
                    vector<ARK::Core::Geometry::Polygon<float> > holes;
                    vector<ARK::Core::Geometry::Line<float> > lines;
                    void updateLines();
                    void render();
                    void renderInner(ARK::Core::Geometry::Polygon<float>* inner, bool hole);
            };
        }
	}
}

#endif /* ARK_GEOMETRY_ADVANCEDPOLYGON_H_ */