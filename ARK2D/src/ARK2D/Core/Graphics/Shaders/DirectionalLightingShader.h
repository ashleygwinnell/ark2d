/*
 * DirectionalLightingShader3d.h
 *
 *  Created on: 12 Oct 2014
 *      Author: Ashley
 */

#ifndef ARK_CORE_GRAPHICS_SHADERS_DIRECTIONALLIGHTINGSHADER_H_
#define ARK_CORE_GRAPHICS_SHADERS_DIRECTIONALLIGHTINGSHADER_H_

#include "../Shader.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            class DirectionalLightingShader;
            class ARK2D_API DirectionalLightingGeometryShader : public ARK::Core::Graphics::Shader
            { 
                public:
                    DirectionalLightingShader* container;

                    unsigned int _LightColor;
                    unsigned int _LightDirection;
                    unsigned int _LightAmbientIntensity;
                    unsigned int _LightStrength;

                public:
                    DirectionalLightingGeometryShader();  
                    void load();
                    virtual void bind();
                    virtual void unbind();
                    virtual ~DirectionalLightingGeometryShader();
            };
            class ARK2D_API DirectionalLightingTextureShader : public ARK::Core::Graphics::Shader 
            { 
                public:
                    DirectionalLightingShader* container;

                    unsigned int _LightColor;
                    unsigned int _LightDirection;
                    unsigned int _LightAmbientIntensity;
                    unsigned int _LightStrength;

                public:
                    DirectionalLightingTextureShader();  
                    void load();
                    virtual void bind();
                    virtual void unbind();
                    virtual ~DirectionalLightingTextureShader();
            };

            class ARK2D_API DirectionalLightingShader {
                public:
                    DirectionalLightingGeometryShader* geometry;
                    DirectionalLightingTextureShader* texture;

                    Vector3<float> direction;
                    Vector3<float> color;
                    float strength;
                    float ambientIntensity;
                    
                public:
                    DirectionalLightingShader();
                    void load();

                    void setLighting(float dx, float dy, float dz, float r, float g, float b, float stength, float ambientIntensity);
                    void setLightDirection(const Vector3<float>& newDirection);
                    void setLightColor(const Vector3<float>& newColor);
                    void setLightStrength(float str);
                    void setLightAmbientIntensity(float str);

                    virtual ~DirectionalLightingShader();
            };
        }
	}
}

#endif /* ARK_CORE_GRAPHICS_DIRECTIONALLIGHTINGSHADER_H_ */
