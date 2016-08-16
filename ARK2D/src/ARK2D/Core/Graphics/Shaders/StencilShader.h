/*
 * StencilShader.h
 *
 *  Created on: 5 Jun 2014
 *      Author: Ashley
 */

#ifndef ARK_CORE_GRAPHICS_STENCILSHADER_H_
#define ARK_CORE_GRAPHICS_STENCILSHADER_H_

#include "../../ARK2D.h"

namespace ARK {
    namespace Core {
        namespace Graphics {
        
            class ARK2D_API StencilShader : public ARK::Core::Graphics::Shader
            { 
                public:
                    StencilShader();  
                    void load();
                    void start();
                    void stop();
                    void startInternal();
                    void stopInternal();
                    
                    static void startStatic(StencilShader* obj);
                    static void stopStatic(StencilShader* obj);

                    virtual ~StencilShader();
            };
        }
	}
}

#endif /* ARK_GRAPHICS_STENCILSHADER_H_ */
