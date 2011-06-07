/*
 * Shader.h
 *
 *  Created on: 27-Apr-2010
 *      Author: Ashley
 *      		...with much help from http://www.swiftless.com/tutorials/glsl/1_setup.html
 */

#ifndef SHADER_H_
#define SHADER_H_


#include "ARK2D_GL.h"

#include <string>

class Shader {
	public:
		Shader();
		Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
		void init(const char* vertexShaderFile, const char* fragmentShaderFile);
		unsigned int getId();
		void bind();
		void unbind();
		~Shader();

	private:
		unsigned int m_shader_id;
		unsigned int m_shader_vertex_program;
		unsigned int m_shader_fragment_program;
};
#endif/* SHADER_H_ */
