/*
 * Shader.h
 *
 *  Created on: 27-Apr-2010
 *      Author: Ashley
 *      		...with much help from
 */

#ifndef SHADER_H_
#define SHADER_H_


#include "../Includes.h"
#include "../Util/Containers/Vector.h"

namespace ARK {
	namespace Graphics {

		/*!
		 * \brief OpenGL Shader support.
		 *
		 * @todo Read Shader from memory buffer, as well as file.
		 * @todo LOGGING: http://www.lighthouse3d.com/tutorials/glsl-tutorial/troubleshooting-the-infolog/
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 * @author unascribed http://www.swiftless.com/tutorials/glsl/1_setup.html
		 * 
		 * REFERENCE:
		 * http://www.lighthouse3d.com/tutorials/glsl-tutorial/uniform-variables/
		 *
		 * DATA TYPES:
		 * http://www.lighthouse3d.com/tutorials/glsl-tutorial/data-types-and-variables/
		 *
		 * STATEMENTS, FUNCTIONS:
		 * http://www.lighthouse3d.com/tutorials/glsl-tutorial/statements-and-functions/
		 *
		 * RADIAL BLUR:
		 * http://stackoverflow.com/questions/4579020/how-do-i-use-a-glsl-shader-to-apply-a-radial-blur-to-an-entire-scene
		 * 
		 * OTHER USEFUL?!
		 * http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
		 * http://www.lighthouse3d.com
		 *
		 */
		class Shader {
			public:
				Shader();
				void addVertexShader(string file);
				void addFragmentShader(string file);
				unsigned int getId();

				// getting/setting uniform shader vars -- these don't (can't) change inbetween begin/end calls within the shader.
				int getUniformVariable(string var); 
				void setUniformVariableF(int var, float value); 
				void setUniformVariableF(int var, int count, float* values); 
				void setUniformVariableI(int var, int value); 
				void setUniformVariableI(int var, int count, int* values); 

				void setUniformVariable2F(int var, float v1, float v2); 
				void setUniformVariable3F(int var, float v1, float v2, float v3); 

				// getting/setting attrib shader vars -- these are per-vertex variables. they cannot be changed in vertex shader.
				int getAttributeVariable(string var); 
				void setAttributeVariableF(int var, float value); 

				void link();
				void bind();
				void unbind();
				virtual ~Shader();

			private:
				unsigned int addShader(string file, GLuint type);

			private:
				unsigned int m_programId;
				Vector<int> m_vertexShaders;
				Vector<int> m_fragmentShaders;
				//unsigned int m_shader_vertex_program;
				//unsigned int m_shader_fragment_program;
		};

	}
}

#endif/* SHADER_H_ */
