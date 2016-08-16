/*
 * PointLightingShader3d.h
 *
 *  Created on: 12 Oct 2014
 *      Author: Ashley
 */

#ifndef ARK_CORE_GRAPHICS_POINTLIGHTINGSHADER3D_H_
#define ARK_CORE_GRAPHICS_POINTLIGHTINGSHADER3D_H_

#include "../Shader.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            class PointLightingShader;
            class ARK2D_API PointLightingGeometryShader : public ARK::Core::Graphics::Shader
            { 
                public:
                    PointLightingShader* container;
                    unsigned int _LightColor;
                    unsigned int _LightDirection;
                    unsigned int _LightAmbientIntensity;
                    unsigned int _LightStrength;

                    unsigned int _PointLightColor;
                    unsigned int _PointLightPosition;
                    unsigned int _PointLightAmbient; 
                    unsigned int _PointLightConstantAtt; 
                    unsigned int _PointLightLinearAtt; 
                    unsigned int _PointLightExpAtt; 

                public:
                    PointLightingGeometryShader();
                    void load();
                    virtual void bind();
                    virtual void unbind();
                    virtual ~PointLightingGeometryShader();
            };
            class ARK2D_API PointLightingTextureShader : public ARK::Core::Graphics::Shader
            { 
                public:
                    PointLightingShader* container; 
                    unsigned int _LightColor;
                    unsigned int _LightDirection;
                    unsigned int _LightAmbientIntensity;
                    unsigned int _LightStrength;

                    unsigned int _PointLightColor;
                    unsigned int _PointLightPosition;
                    unsigned int _PointLightAmbient; 
                    unsigned int _PointLightConstantAtt; 
                    unsigned int _PointLightLinearAtt; 
                    unsigned int _PointLightExpAtt; 

                public:
                    PointLightingTextureShader();  
                    void load();
                    virtual void bind();
                    virtual void unbind();
                    virtual ~PointLightingTextureShader();
            };

            class ARK2D_API PointLightingShader {
                public:
                    // Directional lighting vars
                    Vector3<float> direction;
                    Vector3<float> directionalColor;
                    float directionalAmbientIntensity;
                    float directionalStrength;

                    // Point lighting vars
                    Vector3<float> pointPosition;
                    Vector3<float> pointColor;
                    float pointAmbience;
                    float pointConstantAtt;
                    float pointLinearAtt;
                    float pointExpAtt;
                    
                public:
                    PointLightingGeometryShader* geometry;
                    PointLightingTextureShader* texture;
                    PointLightingShader();
                    void load();
                    ~PointLightingShader();
            };
        }
	}
}

#endif /* ARK_GRAPHICS_POINTLIGHTINGSHADER_H_ */
