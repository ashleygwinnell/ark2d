/*
 * ShockwaveShader.h
 *
 *  Created on: 11 Nov 2015
 *      Author: Ashley
 */

#ifndef ARK_CORE_GRAPHICS_SHADERS_SHOCKWAVESHADER_H_
#define ARK_CORE_GRAPHICS_SHADERS_SHOCKWAVESHADER_H_

#include "../../ARK2D.h"
#include "../Shader.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            class ShockwaveShader;

            class ARK2D_API ShockwaveTextureShader : public Shader {
                private:
                    ShockwaveShader* _container;
                    unsigned int _center;
                    unsigned int _time;
                    unsigned int _shockParams;
                public:
                    ShockwaveTextureShader(ShockwaveShader* ss);
                    void load();
                    void start();
                    void stop();
                    void startInternal();
                    void stopInternal();

                    virtual void bind();

                    static void startStatic(ShockwaveTextureShader* obj);
                    static void stopStatic(ShockwaveTextureShader* obj);
                    virtual ~ShockwaveTextureShader();
            };
            class ARK2D_API ShockwaveShader {
                public:
                    ShockwaveTextureShader* texture;
                    Vector2<float> center; // center is in texture space [0 - 1]
                    Vector3<float> shockparams; // shockParams[0]: "depth distance" modifier. larger is flatter, ish. / outer edge of thing
                                                // shockParams[1]: "zoom" depth
                                                // shockParams[2]: "size" / timer to check against.
                    float timer;

                public:
                    ShockwaveShader();
                    void load();
                    void start();
                    void stop();
                    virtual ~ShockwaveShader();
            };
        }
    }
}

#endif
