/*
 * Shader.cpp
 *
 *  Created on: 27-Apr-2010
 *      Author: Ashley
 *      		...with much help from http://www.swiftless.com/tutorials/glsl/1_setup.html
 */
#include "Shader.h"
#include "ErrorDialog.h"
#include "StringUtil.h"
#include <stdlib.h>

#include "ARK2D_GL.h"

Shader::Shader() {
}

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) {
    init(vertexShaderFile, fragmentShaderFile);
}

void Shader::init(const char* vertexShaderFile, const char* fragmentShaderFile) {
	// read in our vertex shader and fragment shader files
	// These are just standard text documents, with whatever extension you wish to add
	m_shader_vertex_program = glCreateShader(GL_VERTEX_SHADER);
	m_shader_fragment_program = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexShaderText = StringUtil::file_get_contents(vertexShaderFile);
    const char* fragmentShaderText = StringUtil::file_get_contents(fragmentShaderFile);

    // do a little check to see if we were able to load both the files,
    if (vertexShaderText == NULL || fragmentShaderText == NULL) {
    	ErrorDialog::createAndShow("Either vertex shader or fragment shader file not found.");
		return;
	}

    // pass our shader file contents to OpenGL to attach it to our shaders
    glShaderSource(m_shader_vertex_program, 1, &vertexShaderText, 0);
    glShaderSource(m_shader_fragment_program, 1, &fragmentShaderText, 0);

    // compile our shaders, yes.
    glCompileShader(m_shader_vertex_program);
    glCompileShader(m_shader_fragment_program);

    // we create our shader_id as a shader program.
    // And then attach the vertex shader and the fragment shader to the shader program,
    // finally, link the program.
    m_shader_id = glCreateProgram();
    glAttachShader(m_shader_id, m_shader_fragment_program);
    glAttachShader(m_shader_id, m_shader_vertex_program);
    glLinkProgram(m_shader_id);

}

unsigned int Shader::getId() {
    return m_shader_id;
}

void Shader::bind() {
    glUseProgram(m_shader_id);
}

void Shader::unbind() {
    glUseProgram(0);
}

Shader::~Shader() {
	// detach our vertex shader and fragment shader from our shader project
    glDetachShader(m_shader_id, m_shader_fragment_program);
    glDetachShader(m_shader_id, m_shader_vertex_program);

    // delete all of the shaders and then deletes the shader program.
    glDeleteShader(m_shader_fragment_program);
    glDeleteShader(m_shader_vertex_program);
    glDeleteProgram(m_shader_id);
}

