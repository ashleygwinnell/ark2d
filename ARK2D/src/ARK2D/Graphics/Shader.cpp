/*
 * Shader.cpp
 *
 *  Created on: 27-Apr-2010
 *      Author: Ashley
 *      		...with much help from http://www.swiftless.com/tutorials/glsl/1_setup.html
 */
#include "Shader.h"
#include "../UI/ErrorDialog.h"
#include "../Util/StringUtil.h"
#include <stdlib.h>

#include "../Includes.h"
#include "../Util/Containers/Vector.h"

namespace ARK {
	namespace Graphics {
 
		Shader::Shader():
			m_programId(0),
			m_vertexShaders(),
			m_fragmentShaders() 
			{
			#ifdef SHADER_SUPPORT 
				m_programId = glCreateProgram();
			#endif
		}

		void Shader::addVertexShader(string file) {
			#ifdef SHADER_SUPPORT
				unsigned int shaderId = addShader(file, GL_VERTEX_SHADER);
				m_vertexShaders.add(shaderId);
			#else
				m_vertexShaders.add(0);
			#endif
		}

		void Shader::addFragmentShader(string file) {
			#ifdef SHADER_SUPPORT
				unsigned int shaderId = addShader(file, GL_FRAGMENT_SHADER);
				m_fragmentShaders.add(shaderId);
			#else
				m_fragmentShaders.add(0);
			#endif
		}


		unsigned int Shader::addShader(string file, GLuint type) {
			#ifdef SHADER_SUPPORT
				unsigned int shaderId = glCreateShader(type);
				String* contents = Resource::get(file)->asString();
				if (contents->length() == 0) {
					ErrorDialog::createAndShow("Shader file not found or it was empty.");
					return (unsigned int) NULL;
				}
				const char* cstr = contents->get().c_str();
				 
				// pass our shader file contents to OpenGL to attach it to our shaders
				glShaderSource(shaderId, 1, &cstr, 0);
				Image::showAnyGlErrorAndExit(); 
				
				// compile our shaders, yes.
				glCompileShader(shaderId);
				Image::showAnyGlErrorAndExit(); 

				// attach it you slut.
				glAttachShader(m_programId, shaderId);
				Image::showAnyGlErrorAndExit(); 

				return shaderId;
			#endif
			return 0;
		}
		unsigned int Shader::getId() {
			return m_programId;
		}

		// get shader vars
		int Shader::getUniformVariable(string var) {
			#ifdef SHADER_SUPPORT
				return glGetUniformLocation(m_programId, var.c_str());
			#endif 
			return 0;
		}

		// set shader vars
		void Shader::setUniformVariableF(int var, float value) {
			#ifdef SHADER_SUPPORT
				glUniform1f(var, value);
			#endif
		}
		void Shader::setUniformVariableI(int var, int value) {
			#ifdef SHADER_SUPPORT
				glUniform1i(var, value);
			#endif
		}
		// set shader vars arrays
		void Shader::setUniformVariableF(int var, int count, float* values) {
			#ifdef SHADER_SUPPORT
				glUniform1fv(var, count, values);
			#endif
		}
		void Shader::setUniformVariableI(int var, int count, int* values) {
			#ifdef SHADER_SUPPORT
				glUniform1iv(var, count, values);	
			#endif
		}


		// ehhh?
		void Shader::setUniformVariable2F(int var, float v1, float v2) {
			#ifdef SHADER_SUPPORT
				glUniform2f(var, v1, v2);
			#endif
		}
		void Shader::setUniformVariable3F(int var, float v1, float v2, float v3) {
			#ifdef SHADER_SUPPORT
				glUniform3f(var, v1, v2, v3);
			#endif
		}



		int Shader::getAttributeVariable(string var) {
			#ifdef SHADER_SUPPORT
				return glGetAttribLocation(m_programId, var.c_str());
			#endif
			return 0;
		}
		void Shader::setAttributeVariableF(int var, float value) {
			#ifdef SHADER_SUPPORT
				glVertexAttrib1f(var, value);
			#endif
		}
		

		void Shader::link() {
			#ifdef SHADER_SUPPORT
				glLinkProgram(m_programId);
			#endif
		}

		

		

		void Shader::bind() {
			#ifdef SHADER_SUPPORT
				glUseProgram(m_programId);
			#endif
		}

		void Shader::unbind() {
			#ifdef SHADER_SUPPORT
				glUseProgram(0);
			#endif
		}

		Shader::~Shader() {
			#ifdef SHADER_SUPPORT
				// detach and delete our vertex shaders from our shader program.
				for(unsigned int i = 0; i < m_vertexShaders.size(); i++) {
					unsigned int item = m_vertexShaders.get(i);
					glDetachShader(m_programId, item);
					glDeleteShader(item);
				}

				// detach and delete fragment shaders from our shader project.
				for(unsigned int i = 0; i < m_fragmentShaders.size(); i++) {
					unsigned int item = m_fragmentShaders.get(i);
					glDetachShader(m_programId, item);
					glDeleteShader(item);
				}

				// delete all of the shaders and then deletes the shader program.
				glDeleteProgram(m_programId);
			#endif
		}
	}
}
