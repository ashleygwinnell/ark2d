/*
 * GrainShader.h
 *
 *  Created on: 06 Oct 2017
 *      Author: Ashley
 * 
 * Author:              https://medium.com/@mattdesl/filmic-effects-for-webgl-9dab4bc899dc
 * CC:                  https://github.com/mattdesl/filmic-gl
 * Shadertoy version:   https://www.shadertoy.com/view/Mly3Rw
 */

#ifndef ARK_CORE_GRAPHICS_SHADERS_GRAINSHADER_H_
#define ARK_CORE_GRAPHICS_SHADERS_GRAINSHADER_H_

#include "../../ARK2D.h"
#include "../Shader.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            class GrainShader;
            
            class ARK2D_API GrainTextureShader : public Shader {
                public:
                	GrainShader* _container;
                	unsigned int _timer;
                	unsigned int _resolution;
                	unsigned int _strength;

                    GrainTextureShader(GrainShader* container);  
                    void load();
                    void start();
                    void stop();
                    void startInternal();
                    void stopInternal();
                    virtual void bind();
                    static void startStatic(GrainTextureShader* obj);
            		static void stopStatic(GrainTextureShader* obj);
                    virtual ~GrainTextureShader();
            };
            class GrainShader {
                public:
                    GrainTextureShader* texture;
                    Vector2<float> resolution;
                    float timer;
                    float strength;

                    
                public:
                    GrainShader();
                    void load();
                    void start();
            		void stop();
                    virtual ~GrainShader();
            };
        }
    }
}

#endif