/*
 * Shader.h
 *
 *  Created on: 27-Apr-2010
 *      Author: Ashley
 *      		...with much help from
 */

#ifndef ARK_CORE_GRAPHICS_SHADER_H_
#define ARK_CORE_GRAPHICS_SHADER_H_

#include "../../Common/OpenGL.h"
#include "../Vector.h"

namespace ARK {
    namespace Core {
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
            class ARK2D_API ShaderInternals {
                public:
                    ShaderInternals();
                    void addVertexShader(string file);
                    void addFragmentShader(string file);
                    void addVertexShaderFromString(string file);
                    void addFragmentShaderFromString(string file);
                    void addVertexShaderFromData(void* data, unsigned int datalength);
                    void addFragmentShaderFromData(void* data, unsigned int datalength);
                    signed int getId();

                    void setName(string s) { m_name = s; }
                    string getName() { return m_name; }

                    bool hasError() { return m_error; }
                    string getErrorString() { return m_errorString; }

                    // getting/setting uniform shader vars -- these don't (can't) change inbetween begin/end calls within the shader.
                    int getUniformVariable(string var);
                    void setUniformVariableF(int var, float value);
                    void setUniformVariableF(int var, int count, float* values);
                    void setUniformVariableI(int var, int value);
                    void setUniformVariableI(int var, int count, int* values);

                    void setUniformVariable2I(int var, int v1, int v2);
                    void setUniformVariable2F(int var, float v1, float v2);
                    void setUniformVariable3F(int var, float v1, float v2, float v3);
                    void setUniformVariable4FV(int var, int count, float* data);
                    void setUniformVariableMat4f(int var, float* mat);

                    // getting/setting attrib shader vars -- these are per-vertex variables. they cannot be changed in vertex shader.
                    int getAttributeVariable(string var);
                    int getAttributeVariableVertexArray(string var);
                    void enableVertexAttribArray(int var);
                    void setAttributeVariableF(int var, float value);
                    void setAttributeVariableVertexPointerFloat(int var, int sz, bool normalise, float* data);
                    void setAttributeVariableVertexPointerFloatStride(int var, int sz, bool normalise, float* data, unsigned int stride);
                    void setAttributeVariableVertexPointerStride(int var, int sz, bool normalise, unsigned int stride, void* data);
                    void setAttributeVariableVertexPointerUnsignedChar(int var, int sz, void* data);

                    // bind things
                    void bindAttributeLocation(unsigned int loc, string var);
                    void bindFragmentDataLocation(unsigned int loc, string var);
                    //void bindAttributeLocationVertexArray(unsigned int loc, string var);

                    void link();
                    virtual void linkDX();

                    void initVariables(); // do MANUALLY after link to get ARK2D variable references.
                    int getInittedVariable(string s);

                    virtual ~ShaderInternals();

                    static string processGLSLForIncludes(string ss);

                private:
                    unsigned int addShader(string file, GLuint type);
                    unsigned int addShaderFromString(string contents, GLuint type);
                    bool checkShaderCompiled(unsigned int shaderId);

                protected:
                    string m_name;
                    signed int m_programId;
                    Vector<int> m_vertexShaders;
                    Vector<int> m_fragmentShaders;
                    //unsigned int m_shader_vertex_program;
                    //unsigned int m_shader_fragment_program;

                    map<string, int> m_variables;

                    bool m_error;
                    string m_errorString;

                    #if defined(ARK2D_RENDERER_DIRECTX)
                        public:
                            ID3D11VertexShader* m_d3d_vertexShader;
                            ID3D11PixelShader* m_d3d_pixelShader;
                            ID3D11InputLayout* m_d3d_inputLayout;
                            ID3D11Device3* getD3D11Device();
                            ID3D11DeviceContext3* getD3D11DeviceContext();

                            void* m_d3d_vertexCSO;
                            unsigned int m_d3d_vertexCSOLength;

                            void* m_d3d_pixelCSO;
                            unsigned m_d3d_pixelCSOLength;

                            static unsigned int s_d3d_arkShaderIncrement;

                            ID3D11InputLayout* getD3DInputLayout() { return m_d3d_inputLayout; }

                    #endif
            };

            class ARK2D_API Shader : public ShaderInternals {
                public:

                    // Uniforms
                    signed int ark_ModelMatrix;
                    signed int ark_ViewMatrix;
                    signed int ark_ProjectionMatrix;
                    signed int ark_NormalMatrix;

                    // Textures? Uhhhhhh....
                    signed int ark_TextureId;

                    // Attributes/varyings (per-vertex properties)
                    signed int ark_VertexPositionIn;
                    signed int ark_VertexNormalIn;
                    signed int ark_VertexTexCoordIn;
                    signed int ark_VertexColorIn;

                    // Length of data being used.
                    signed int length;

                public:
                    Shader();
                    virtual void load();
                    virtual void bind();
                    virtual void unbind();
                    virtual void setData(float* verts, float* normals, float* texcoords, unsigned char* colors, unsigned int length);
                    virtual void drawTriangleStrip();
                    virtual void drawTriangleFan();
                    virtual void drawTriangles();
                    virtual ~Shader();
            };
        }
	}
}

#endif /* ARK_CORE_GRAPHICS_SHADER_H_ */
