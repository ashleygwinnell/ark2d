/*
 * DirectionalLightingShader3d.cpp
 *
 *  Created on: 12 Oct 2014
 *      Author: Ashley
 */

#include "DirectionalLightingShader.h"
#include "../ShaderStore.h"
#include "../Image.h"

namespace ARK { 
	namespace Graphics { 

		// 
		// GEOMETRY
		//
		DirectionalLightingGeometryShader::DirectionalLightingGeometryShader(): 
			ARK::Graphics::Shader(),
			container(NULL),
			_LightColor(0),
			_LightDirection(0),
			_LightAmbientIntensity(0),
			_LightStrength(0)
		{
			
		}
		void DirectionalLightingGeometryShader::load() {
			setName("directional-lighting-geometry");

			bool err1 = false;
			bool err2 = false;
			if (ARK2D::getPlatform() == ARK2D::PLATFORM_ANDROID) {
				addVertexShader("ark2d/shaders/directional-lighting/geometry/glsles100-vertex.txt");
				addFragmentShader("ark2d/shaders/directional-lighting/geometry/glsles100-fragment.txt");
			} else {
				// GLSL 330
				//addVertexShader("ark2d/shaders/directional-lighting/geometry/glsl330-vertex.txt");
				//bool err1 = hasError();
				//addFragmentShader("ark2d/shaders/directional-lighting/geometry/glsl330-fragment.txt");
				//bool err2 = hasError();

				//if (err1 || err2) { 
					err1 = false;
					err2 = false;

					addVertexShader("ark2d/shaders/directional-lighting/geometry/glsl150-vertex.txt");
					err1 = hasError();
					addFragmentShader("ark2d/shaders/directional-lighting/geometry/glsl150-fragment.txt");
					err2 = hasError();

					if (err1 || err2) { 
						addVertexShader("ark2d/shaders/directional-lighting/geometry/glsl130-vertex.txt");
						addFragmentShader("ark2d/shaders/directional-lighting/geometry/glsl130-fragment.txt");
					}
				//}
			}

			showAnyGlErrorAndExitMacro();
			
			ShaderStore::getInstance()->addShader(m_programId, this);
			
			bindAttributeLocation(0, "ark_VertexPositionIn");
			bindAttributeLocation(1, "ark_VertexNormalIn");
			bindAttributeLocation(2, "ark_VertexColorIn");
			bindFragmentDataLocation(0, "ark_FragColor");
			link(); 
			linkDX(); 

			RendererState::start(RendererState::SHADER, getId());

			ark_ModelMatrix = getUniformVariable("ark_ModelMatrix");
			ark_ViewMatrix = getUniformVariable("ark_ViewMatrix");
			ark_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
			ark_NormalMatrix = getUniformVariable("ark_NormalMatrix");

			_LightColor = getUniformVariable("u_sunlight.vColor");
			_LightDirection = getUniformVariable("u_sunlight.vDirection");
			_LightAmbientIntensity = getUniformVariable("u_sunlight.fAmbientIntensity");
			_LightStrength = getUniformVariable("u_sunlight.fStrength");
			
			ark_VertexPositionIn = 0;
			ark_VertexNormalIn = 1;
			ark_VertexColorIn = 2;
			
			RendererState::start(RendererState::NONE);
		}

		void DirectionalLightingGeometryShader::bind() {
			Shader::bind();

			glUniform3f(_LightDirection, container->direction.x, container->direction.y, container->direction.z);

			glUniform3f(_LightColor, container->color.x, container->color.y, container->color.z);
			glUniform1f(_LightAmbientIntensity, container->ambientIntensity);
			glUniform1f(_LightStrength, container->strength);
		}

		void DirectionalLightingGeometryShader::unbind() { Shader::unbind(); }
		DirectionalLightingGeometryShader::~DirectionalLightingGeometryShader() { }



		//
		// TEXTURE
		//

		DirectionalLightingTextureShader::DirectionalLightingTextureShader(): 
			ARK::Graphics::Shader(),
			container(NULL),
			_LightColor(0),
			_LightDirection(0),
			_LightAmbientIntensity(0),
			_LightStrength(0)
		{
			
		}
		void DirectionalLightingTextureShader::load() {
			setName("directional-lighting-texture");

			bool err1 = false;
			bool err2 = false;
			if (ARK2D::getPlatform() == ARK2D::PLATFORM_ANDROID) {
				addVertexShader("ark2d/shaders/directional-lighting/texture/glsles100-vertex.txt");
				addFragmentShader("ark2d/shaders/directional-lighting/texture/glsles100-fragment.txt");
			} else {
				// GLSL 330
				//addVertexShader("ark2d/shaders/directional-lighting/texture/glsl330-vertex.txt");
				//bool err1 = hasError();
				//addFragmentShader("ark2d/shaders/directional-lighting/texture/glsl330-fragment.txt");
				//bool err2 = hasError();

				//if (err1 || err2) { 
					err1 = false;
					err2 = false;

					addVertexShader("ark2d/shaders/directional-lighting/texture/glsl150-vertex.txt");
					err1 = hasError();
					addFragmentShader("ark2d/shaders/directional-lighting/texture/glsl150-fragment.txt");
					err2 = hasError();

					if (err1 || err2) { 
						addVertexShader("ark2d/shaders/directional-lighting/texture/glsl130-vertex.txt");
						addFragmentShader("ark2d/shaders/directional-lighting/texture/glsl130-fragment.txt");
					}
				//}
			}
			showAnyGlErrorAndExitMacro();
			
			ShaderStore::getInstance()->addShader(m_programId, this);
			
			bindAttributeLocation(1, "ark_VertexPositionIn");
			bindAttributeLocation(2, "ark_VertexNormalIn");
			bindAttributeLocation(3, "ark_VertexTexCoordIn");
			bindAttributeLocation(4, "ark_VertexColorIn");
			bindFragmentDataLocation(0, "ark_FragColor");
			link(); 
			linkDX(); 

			RendererState::start(RendererState::SHADER, getId());

			ark_ModelMatrix = getUniformVariable("ark_ModelMatrix");
			ark_ViewMatrix = getUniformVariable("ark_ViewMatrix");
			ark_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
			ark_NormalMatrix = getUniformVariable("ark_NormalMatrix");

			ark_TextureId = getUniformVariable("ark_TextureId");

			_LightColor = getUniformVariable("u_sunlight.vColor");
			_LightDirection = getUniformVariable("u_sunlight.vDirection");
			_LightAmbientIntensity = getUniformVariable("u_sunlight.fAmbientIntensity");
			_LightStrength = getUniformVariable("u_sunlight.fStrength");
			
			ark_VertexPositionIn = 1;
			ark_VertexNormalIn = 2;
			ark_VertexTexCoordIn = 3;
			ark_VertexColorIn = 4;
			
			RendererState::start(RendererState::NONE);
		}

		void DirectionalLightingTextureShader::bind() {
			Shader::bind();

			glUniform3f(_LightDirection, container->direction.x, container->direction.y, container->direction.z);

			glUniform3f(_LightColor, container->color.x, container->color.y, container->color.z);
			glUniform1f(_LightAmbientIntensity, container->ambientIntensity);
			glUniform1f(_LightStrength, container->strength);
		}

		void DirectionalLightingTextureShader::unbind() { Shader::unbind(); }
		DirectionalLightingTextureShader::~DirectionalLightingTextureShader() { }



		//
		// CONTAINER
		//
		DirectionalLightingShader::DirectionalLightingShader():
			geometry(NULL),
			texture(NULL),
			direction(0,0,1),
			color(1.0f, 1.0f, 1.0f),
			strength(1.0f),
			ambientIntensity(0.4f)
		{
			geometry = new DirectionalLightingGeometryShader();
			geometry->container = this;

			texture = new DirectionalLightingTextureShader();
			texture->container = this;
		}
		void DirectionalLightingShader::load() {
			geometry->load();
			texture->load();
			setLighting(1,1,1, 1.0f, 1.0f, 1.0f, 1.0f, 0.4f);
		}
		void DirectionalLightingShader::setLighting(float dx, float dy, float dz, float r, float g, float b, float stre, float ambience) {
			setLightDirection(Vector3<float>(dx, dy, dz));
			setLightColor(Vector3<float>(r, g, b));
			setLightStrength(stre);
			setLightAmbientIntensity(ambience);
		}

		void DirectionalLightingShader::setLightDirection(const Vector3<float>& newDirection) {
			direction.set(newDirection);
			direction.normalise();
		}
		void DirectionalLightingShader::setLightColor(const Vector3<float>& newColor) {
			color.set(newColor);		
		}
		void DirectionalLightingShader::setLightStrength(float str) {
			strength = str;
		}
		void DirectionalLightingShader::setLightAmbientIntensity(float inten) {
			ambientIntensity = inten;
		}
		DirectionalLightingShader::~DirectionalLightingShader() {
			delete geometry;
			delete texture;
		}
	}
}
