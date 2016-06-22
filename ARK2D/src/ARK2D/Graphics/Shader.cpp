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
#include "Image.h"
 #include "ShaderStore.h"

namespace ARK {
	namespace Graphics {

		ShaderInternals::ShaderInternals():
			m_name(""),
			m_programId(0),
			m_vertexShaders(),
			m_fragmentShaders(),
			m_variables(),
			m_error(false),
			m_errorString("")
			{
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					m_programId = glCreateProgram();
				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif
				RendererStats::s_glCalls++;
			#endif

		}

		#if defined(ARK2D_RENDERER_DIRECTX)

			ID3D11Device* ShaderInternals::getD3D11Device()
			{
				return ARK2D::getContainer()->m_platformSpecific.m_device;
			}
			ID3D11DeviceContext* ShaderInternals::getD3D11DeviceContext()
			{
				return ARK2D::getContainer()->m_platformSpecific.m_deviceContext;
			}

		#endif

		void ShaderInternals::addVertexShader(string file) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					ARK2D::getLog()->v("Loading Vertex Shader");
					unsigned int shaderId = addShader(file, GL_VERTEX_SHADER);
					if (hasError()) { return; }
					ARK2D::getLog()->v(StringUtil::append("Shader ID: ", shaderId));
				#elif defined(ARK2D_RENDERER_DIRECTX)
					unsigned int shaderId = 0;
				#endif

				m_vertexShaders.add(shaderId);
			#else
				m_vertexShaders.add(0);
			#endif
		}

		void ShaderInternals::addFragmentShader(string file) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					ARK2D::getLog()->v("Loading Fragment Shader");
					unsigned int shaderId = addShader(file, GL_FRAGMENT_SHADER);
					if (hasError()) { return; }
					ARK2D::getLog()->v(StringUtil::append("Shader ID: ", shaderId));
				#elif defined(ARK2D_RENDERER_DIRECTX)
					unsigned int shaderId = 0;
				#endif
				m_fragmentShaders.add(shaderId);
			#else
				m_fragmentShaders.add(0);
			#endif
		}

		void ShaderInternals::addVertexShaderFromString(string contents) {
			addVertexShaderFromData((void*) contents.c_str(), contents.length());
		}
		void ShaderInternals::addFragmentShaderFromString(string contents) {
			addFragmentShaderFromData((void*) contents.c_str(), contents.length());
		}

		void ShaderInternals::addVertexShaderFromData(void* contents, unsigned int datalength) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					ARK2D::getLog()->v("Loading Vertex Shader from Data");
					string contentsStr = string((const char*) contents);
					unsigned int shaderId = addShaderFromString(contentsStr, GL_VERTEX_SHADER);
					if (hasError()) { return; }
					ARK2D::getLog()->v(StringUtil::append("Shader ID: ", shaderId));
				#elif defined(ARK2D_RENDERER_DIRECTX)
					unsigned int shaderId = 0;

					ID3D11Device* dxdevice = getD3D11Device();
					HRESULT rs = dxdevice->CreateVertexShader(
						contents,
						datalength,
						nullptr,
						&m_d3d_vertexShader
					);
					if (FAILED(rs)) {
						ErrorDialog::createAndShow(StringUtil::append("Failed to create DirectX 11 Vertex Shader.", DX_GetError(rs)));
						exit(0);
					}
					ARK2D::getLog()->v("Created DirectX 11 Vertex Shader");

					m_d3d_vertexCSO = contents;
					m_d3d_vertexCSOLength = datalength;

				#endif
				m_vertexShaders.add(shaderId);
			#else
				m_vertexShaders.add(0);
			#endif
		}
		void ShaderInternals::addFragmentShaderFromData(void* contents, unsigned int datalength) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					ARK2D::getLog()->v("Loading Fragment Shader from Data");
					string contentsStr = string((const char*) contents);
					unsigned int shaderId = addShaderFromString(contentsStr, GL_FRAGMENT_SHADER);
					if (hasError()) { return; }
					ARK2D::getLog()->v(StringUtil::append("Shader ID: ", shaderId));
				#elif defined(ARK2D_RENDERER_DIRECTX)
					unsigned int shaderId = 0;

					ID3D11Device* dxdevice = getD3D11Device();
					HRESULT rs = dxdevice->CreatePixelShader(
						contents,
						datalength,
						nullptr,
						&m_d3d_pixelShader
					);
					if (FAILED(rs)) {
						ErrorDialog::createAndShow(StringUtil::append("Failed to create DirectX 11 Pixel Shader.", DX_GetError(rs)));
						exit(0);
					}
					ARK2D::getLog()->v("Created DirectX 11 Pixel Shader");

					m_d3d_pixelCSO = contents;
					m_d3d_pixelCSOLength = datalength;


				#endif
				m_fragmentShaders.add(shaderId);
			#else
				m_fragmentShaders.add(0);
			#endif
		}


		unsigned int ShaderInternals::addShader(string file, GLuint type) {
			#ifdef SHADER_SUPPORT

				#if defined(ARK2D_RENDERER_OPENGL)
					String* contents = Resource::get(file)->asString();
					if (contents->length() == 0) {
						ErrorDialog::createAndShow("Shader file not found or it was empty.");
						return (unsigned int) NULL;
					}
					string ss = contents->getc();
					int shaderId = addShaderFromString(ss, type);
					//delete contents;

					/*
					ARK2D::getLog()->v("Creating Shader");
					unsigned int shaderId = glCreateShader(type);
					if (shaderId == 0) {
						ErrorDialog::createAndShow("Could not create shader. glCreateShader returned 0.");
						return (unsigned int) NULL;
					}

					String* contents = Resource::get(file)->asString();
					if (contents->length() == 0) {
						ErrorDialog::createAndShow("Shader file not found or it was empty.");
						return (unsigned int) NULL;
					}
					string ss = contents->getc();

					// loop through lines and look for includes.
					// ... replace inline.


					const char* cstr = ss.c_str();


					//ARK2D::getLog()->v("Shader Source: ");
					//ARK2D::getLog()->v(cstr);

					// pass our shader file contents to OpenGL to attach it to our shaders
					ARK2D::getLog()->v("Setting Shader source");
					glShaderSource(shaderId, 1, &cstr, NULL);
					showAnyGlErrorAndExitMacro();

					// compile our shaders, yes.
					ARK2D::getLog()->v("Compiling");
					glCompileShader(shaderId);
					bool b = checkShaderCompiled(shaderId);
					if (!b) { return 0; }
					showAnyGlErrorAndExitMacro();

					// attach it you slut.
					ARK2D::getLog()->v("Attaching");
					glAttachShader(m_programId, shaderId);
					showAnyGlErrorAndExitMacro();

					RendererStats::s_glCalls += 4;*/

				#elif defined(ARK2D_RENDERER_DIRECTX)
					unsigned int shaderId = 0;
				#endif

				return shaderId;
			#endif
			return 0;
		}
		unsigned int ShaderInternals::addShaderFromString(string contents, GLuint type) {
			#ifdef SHADER_SUPPORT

				#if defined(ARK2D_RENDERER_OPENGL)

					ARK2D::getLog()->v("Creating Shader");
					unsigned int shaderId = glCreateShader(type);
					if (contents.length() == 0) {
						ErrorDialog::createAndShow("Shader file not found or it was empty.");
						return (unsigned int) NULL;
					}
					contents += string("// End shader.\0");


					string processedString = processGLSLForIncludes(contents);

					const char* cstr = processedString.c_str();

					ARK2D::getLog()->v("Shader Source: ");
					ARK2D::getLog()->v(cstr);

					// pass our shader file contents to OpenGL to attach it to our shaders
					ARK2D::getLog()->v("Setting Shader source");
					glShaderSource(shaderId, 1, &cstr, NULL);
					//Image::showAnyGlErrorAndExit();
					showAnyGlErrorAndExitMacro();

					// compile our shaders, yes.
					ARK2D::getLog()->v("Compiling");
					glCompileShader(shaderId);
					bool b = checkShaderCompiled(shaderId);
					if (!b) { return 0; }
					//Image::showAnyGlErrorAndExit();
					showAnyGlErrorAndExitMacro();

					// attach it you slut.
					ARK2D::getLog()->v("Attaching");
					glAttachShader(m_programId, shaderId);
					//Image::showAnyGlErrorAndExit();
					showAnyGlErrorAndExitMacro();

					RendererStats::s_glCalls += 4;

				#elif defined(ARK2D_RENDERER_DIRECTX)
					unsigned int shaderId = 0;
				#endif

				return shaderId;
			#endif
			return 0;
		}
		signed int ShaderInternals::getId() {
			return m_programId;
		}

		bool ShaderInternals::checkShaderCompiled(unsigned int shaderId) {
			#if defined(ARK2D_RENDERER_OPENGL)

				m_error = false;
				m_errorString = "";

				ARK2D::getLog()->v("Check Shader Compiled");
				GLint shaderCompiled;
				glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompiled);
				RendererStats::s_glCalls++;

				if (shaderCompiled == GL_FALSE) {

					char* logmsg;
					GLint length;

					/* get the shader info log */
					glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
					logmsg = (char*) malloc(length);
					glGetShaderInfoLog(shaderId, length, &shaderCompiled, logmsg);

					m_error = true;
					m_errorString += string(logmsg);

					/* print an error message and the info log */
					ARK2D::getLog()->e(StringUtil::append("Unable to compile shader: ", logmsg));
					//fprintf(stderr, "Unable to compile Shader: %s\n", logmsg);
					free(logmsg);

					glDeleteShader(shaderId);
					RendererStats::s_glCalls += 3;
					return false;
				}
				return true;
			#elif defined(ARK2D_RENDERER_DIRECTX)
				return true;
			#endif
			return false;
		}

		// get shader vars
		int ShaderInternals::getUniformVariable(string var) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls++;
					int loc = glGetUniformLocation(m_programId, var.c_str());
					ARK2D::getLog()->v(StringUtil::append(var + " ", loc));
					return loc;
				#elif defined(ARK2D_RENDERER_DIRECTX)
					//ARK2D::getLog()->w("getUniformVariable not implemented");
					return 0;
				#endif
			#endif
			return 0;
		}

		// set shader vars
		void ShaderInternals::setUniformVariableF(int var, float value) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls++;
					glUniform1f(var, value);
				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif
			#endif
		}
		void ShaderInternals::setUniformVariableI(int var, int value) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					glUniform1i(var, value);
				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif
			#endif
		}
		// set shader vars arrays
		void ShaderInternals::setUniformVariableF(int var, int count, float* values) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					glUniform1fv(var, count, values);
				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif
			#endif
		}
		void ShaderInternals::setUniformVariableI(int var, int count, int* values) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls++;
					glUniform1iv(var, count, values);
				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif
			#endif
		}


		// ehhh?
		void ShaderInternals::setUniformVariable2I(int var, int v1, int v2) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls++;
					glUniform2i(var, v1, v2);
				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif
			#endif
		}
		// ehhh?
		void ShaderInternals::setUniformVariable2F(int var, float v1, float v2) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls++;
					glUniform2f(var, v1, v2);
				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif
			#endif
		}
		void ShaderInternals::setUniformVariable3F(int var, float v1, float v2, float v3) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls++;
					glUniform3f(var, v1, v2, v3);
				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif
			#endif
		}
		void ShaderInternals::setUniformVariable4FV(int var, int count, float* data) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls++;
					//glUniform3f(var, v1, v2, v3);
					glUniform4fv(var, count, data);
				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif
			#endif
		}
		void ShaderInternals::setUniformVariableMat4f(int var, float* mat) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls++;
					glUniformMatrix4fv(var, 1, GL_FALSE, mat);
				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif
			#endif
		}



		int ShaderInternals::getAttributeVariable(string var) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls++;
					int loc = glGetAttribLocation(m_programId, var.c_str());
					ARK2D::getLog()->v(StringUtil::append(var + " ", loc));
					return loc;
				#elif defined(ARK2D_RENDERER_DIRECTX)
					ARK2D::getLog()->w("getAttributeVariable not implemented for DX");
					return 0;
				#endif
			#endif
			return 0;
		}
		int ShaderInternals::getAttributeVariableVertexArray(string var) {
			#ifdef SHADER_SUPPORT
				#if defined(ARK2D_RENDERER_OPENGL)
					RendererStats::s_glCalls += 2;
					int ret = glGetAttribLocation(m_programId, var.c_str());
					ARK2D::getLog()->v(StringUtil::append(var + " ", ret));
					enableVertexAttribArray(ret);
					return ret;
				#elif defined(ARK2D_RENDERER_DIRECTX)
					ARK2D::getLog()->w("getAttributeVariableVertexArray not implemented for DX");
					return 0;
				#endif
			#endif
			return 0;
		}
		void ShaderInternals::enableVertexAttribArray(int var) {
			#if defined(ARK2D_RENDERER_OPENGL)
				RendererStats::s_glCalls++;

				if ( var < 0 ) {
					// See: http://stackoverflow.com/questions/30164391/glgeterror-returned-the-following-error-codes-after-a-call-to-glenablevertexat
					//ARK2D::getLog()->w(StringUtil::append("Could not enableVertexAttribArray. It was likely never used in the shader and so was stripped out. ", var) );
					return;
				}

				glEnableVertexAttribArray(var);
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
		}
		void ShaderInternals::setAttributeVariableF(int var, float value) {
			#if defined(ARK2D_RENDERER_OPENGL)
				#ifdef SHADER_SUPPORT
					RendererStats::s_glCalls++;
					glVertexAttrib1f(var, value);
				#endif
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
		}
		void ShaderInternals::setAttributeVariableVertexPointerFloat(int var, int sz, bool normalise, float* data) {
			#if defined(ARK2D_RENDERER_OPENGL)
				#ifdef SHADER_SUPPORT
					RendererStats::s_glCalls++;
					glVertexAttribPointer(var, sz, GL_FLOAT, (normalise)?GL_TRUE:GL_FALSE, 0, (void*) data);
				#endif
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
		}
		void ShaderInternals::setAttributeVariableVertexPointerFloatStride(int var, int sz, bool normalise, float* data, unsigned int stride) {
			#if defined(ARK2D_RENDERER_OPENGL)
				#ifdef SHADER_SUPPORT
					RendererStats::s_glCalls++;
					glVertexAttribPointer(var, sz, GL_FLOAT, (normalise)?GL_TRUE:GL_FALSE, stride, (void*) data);
				#endif
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
		}

		void ShaderInternals::setAttributeVariableVertexPointerStride(int var, int sz, bool normalise, unsigned int stride, void* data) {
			#if defined(ARK2D_RENDERER_OPENGL)
				#ifdef SHADER_SUPPORT
					RendererStats::s_glCalls++;
					glVertexAttribPointer(var, sz, GL_FLOAT, (normalise)?GL_TRUE:GL_FALSE, stride, data);
				#endif
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
		}

		void ShaderInternals::setAttributeVariableVertexPointerUnsignedChar(int var, int sz, void* data) {
			#if defined(ARK2D_RENDERER_OPENGL)
				#ifdef SHADER_SUPPORT
					RendererStats::s_glCalls++;
					glVertexAttribPointer(var, sz, GL_UNSIGNED_BYTE, GL_TRUE, 0, data);
				#endif
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
		}


		void ShaderInternals::link() {
			#if defined(ARK2D_RENDERER_OPENGL)
				#ifdef SHADER_SUPPORT
					ARK2D::getLog()->i("Linking...");
					glLinkProgram(m_programId);

					ARK2D::getLog()->i("Linking done. Checking for errors...");
					GLint linkSuccess;
				    glGetProgramiv(m_programId, GL_LINK_STATUS, &linkSuccess);

				    RendererStats::s_glCalls += 2;

				    if (linkSuccess == GL_FALSE) {
				        GLchar messages[256];
				        glGetProgramInfoLog(m_programId, sizeof(messages), 0, &messages[0]);
				        RendererStats::s_glCalls++;

				       	string errorStr(messages);
				       	ErrorDialog::createAndShow(errorStr);
				        exit(1);
				    }
				#endif
			#elif defined(ARK2D_RENDERER_DIRECTX)

				//linkDX();

			#endif
		}

		void ShaderInternals::linkDX() {
			#if defined(ARK2D_RENDERER_DIRECTX)

			ID3D11Device* dxdevice = getD3D11Device();
			ID3D11DeviceContext* dxcontext = getD3D11DeviceContext();
			if (m_name == "geometry-dx11")
			{
				const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 } // 16 // D3D11_APPEND_ALIGNED_ELEMENT
				};

				HRESULT rs = dxdevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), m_d3d_vertexCSO, m_d3d_vertexCSOLength, &m_d3d_inputLayout);
				if (FAILED(rs)) {
					ErrorDialog::createAndShow(StringUtil::append("Failed to create DirectX 11 Input Layout.", DX_GetError(rs)));
					exit(0);
				}
				dxcontext->IASetInputLayout(m_d3d_inputLayout);
			}
			else if (m_name == "texture-dx11")
			{
				const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	 	 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 16
					{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 } // 24
				};

				HRESULT rs = dxdevice->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), m_d3d_vertexCSO, m_d3d_vertexCSOLength, &m_d3d_inputLayout);
				if (FAILED(rs)) {
					ErrorDialog::createAndShow(StringUtil::append("Failed to create DirectX 11 Input Layout.", DX_GetError(rs)));
					exit(0);
				}
				dxcontext->IASetInputLayout(m_d3d_inputLayout);
			}

			//linkDXCallback();

			#endif
		}


		void ShaderInternals::initVariables() {

			/*int ref_ProjectionMatrix = getUniformVariable("ark_ProjectionMatrix");
			int ref_ViewMatrix = getUniformVariable("ark_ViewMatrix");
			int ref_ModelMatrix = getUniformVariable("ark_ModelMatrix");
			int ref_VertexPosition = getAttributeVariable("ark_VertexPositionIn");
			int ref_VertexColorIn = getAttributeVariable("ark_VertexColorIn");

 			#if defined(ARK2D_RENDERER_OPENGL)
				enableVertexAttribArray(ref_VertexPosition);
				enableVertexAttribArray(ref_VertexColorIn);
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif

			m_variables["ark_ProjectionMatrix"] = ref_ProjectionMatrix;
			m_variables["ark_ViewMatrix"] = ref_ViewMatrix;
			m_variables["ark_ModelMatrix"] = ref_ModelMatrix;
			m_variables["ark_VertexPositionIn"] = ref_VertexPosition;
			m_variables["ark_VertexColorIn"] = ref_VertexColorIn;

			RendererStats::s_glCalls += 6; */
			ARK2D::getLog()->e("Shader::initVariables deprecated!");
		}
		int ShaderInternals::getInittedVariable(string s) {
			return m_variables[s.c_str()];
		}





		void ShaderInternals::bindAttributeLocation(unsigned int loc, string var) {
			#if defined(ARK2D_RENDERER_OPENGL)
				glBindAttribLocation(m_programId, loc, var.c_str());
				RendererStats::s_glCalls++;
			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
		}
//		void Shader::bindAttributeLocationVertexArray(unsigned int loc, string var) {
//			glBindAttribLocation(m_programId, loc, var.c_str());
//			glEnableVertexAttribArray(loc);
//		}
		void ShaderInternals::bindFragmentDataLocation(unsigned int loc, string var) {
			#if defined(ARK2D_OPENGL_3_2)
				glBindFragDataLocation(m_programId, loc, var.c_str());
				RendererStats::s_glCalls++;
			#endif
		}

		string ShaderInternals::processGLSLForIncludes(string ss) {

			vector<string> lines = StringUtil::split(ss, "\n");
			for(unsigned int i = 0; i < lines.size(); ++i) {
				string line = lines[i];
				int pos;
				if ((pos = line.find("#include ")) != string::npos) {
					string filetoinclude = line.substr(pos+9);
					string filetoinclude2 = filetoinclude.substr(1, filetoinclude.length() - 2);
					//lines[i] = StringUtil::append("// INCLUDE A THING HERE ", filetoinclude2);
					String* contents = Resource::get(filetoinclude2)->asString();
					string thisLine = contents->getc();
					lines[i] = thisLine;
					delete contents;


				}
			}

			string result;
			for(unsigned int i = 0; i < lines.size(); ++i) {
				result += lines[i] + "\n";
			}
			return result;
		}

		ShaderInternals::~ShaderInternals() {

			#ifdef SHADER_SUPPORT

				#if defined(ARK2D_RENDERER_OPENGL)
					// detach and delete our vertex shaders from our shader program.
					for(unsigned int i = 0; i < m_vertexShaders.size(); i++) {
						unsigned int item = m_vertexShaders.get(i);
						glDetachShader(m_programId, item);
						glDeleteShader(item);
						RendererStats::s_glCalls += 2;
					}

					// detach and delete fragment shaders from our shader project.
					for(unsigned int i = 0; i < m_fragmentShaders.size(); i++) {
						unsigned int item = m_fragmentShaders.get(i);
						glDetachShader(m_programId, item);
						glDeleteShader(item);
						RendererStats::s_glCalls += 2;
					}

					// delete all of the shaders and then deletes the shader program.
					glDeleteProgram(m_programId);
					RendererStats::s_glCalls++;

				#elif defined(ARK2D_RENDERER_DIRECTX)

				#endif

			#endif
		}




		Shader::Shader():
			ShaderInternals(),
			ark_ModelMatrix(-1),
			ark_ViewMatrix(-1),
			ark_ProjectionMatrix(-1),
			ark_NormalMatrix(-1),

			ark_TextureId(-1),

			ark_VertexPositionIn(-1),
			ark_VertexNormalIn(-1),
			ark_VertexTexCoordIn(-1),
			ark_VertexColorIn(-1)
				{

		}

		void Shader::load() {
			// Add shaders / load from file.
			// Bind attribute locations.
			// Bind fragment data location.
			// Link
			// Link DX
			// Assign shader properties to locations.
			// Add to shader store.
			// ShaderStore::getInstance()->addShader(m_programId, this);
		}

		void Shader::bind() {
			#if defined(ARK2D_RENDERER_OPENGL)

				glUseProgram(m_programId);

			#elif defined(ARK2D_RENDERER_DIRECTX)

				ID3D11DeviceContext* dxcontext = getD3D11DeviceContext();
				dxcontext->VSSetShader(m_d3d_vertexShader, NULL, 0);
				dxcontext->PSSetShader(m_d3d_pixelShader, NULL, 0);
				dxcontext->IASetInputLayout( m_d3d_inputLayout );

			#endif
			RendererStats::s_glCalls++;
		}
		void Shader::unbind() {
			#if defined(ARK2D_RENDERER_OPENGL)

				glUseProgram(0);
				RendererStats::s_glCalls++;

			#elif defined(ARK2D_RENDERER_DIRECTX)

			#endif
		}

		void Shader::setData(float* rawVertices, float* rawNormals, float* rawTexCoords, unsigned char* rawColors, unsigned int length)
		{
			this->length = length;

			#if defined(ARK2D_RENDERER_OPENGL)

				#if defined(ARK2D_OPENGL_3_2)

					Renderer::s_vboQuadVerts->setWidth(3);
		            Renderer::s_vboQuadNormals->setWidth(3);
		            Renderer::s_vboQuadColors->setWidth(4);
		            if (rawTexCoords != NULL) { Renderer::s_vboQuadTexCoords->setWidth(2); }

					Renderer::s_vboQuadVerts->setHeight(length);
		            Renderer::s_vboQuadNormals->setHeight(length);
		            Renderer::s_vboQuadColors->setHeight(length);
		            if (rawTexCoords != NULL) { Renderer::s_vboQuadTexCoords->setHeight(length); }

					Renderer::s_vaoQuad->bind();
					enableVertexAttribArray(ark_VertexPositionIn);
	                enableVertexAttribArray(ark_VertexNormalIn);
	                enableVertexAttribArray(ark_VertexColorIn);
	                if (rawTexCoords != NULL) { enableVertexAttribArray(ark_VertexTexCoordIn); }

					Renderer::s_vboQuadVerts->bind();
					Renderer::s_vboQuadVerts->setData(&rawVertices[0]);
					glVertexAttribPointer(ark_VertexPositionIn, 3, GL_FLOAT, GL_FALSE, 0, 0);

	                Renderer::s_vboQuadNormals->bind();
	                Renderer::s_vboQuadNormals->setData(&rawNormals[0]);
	                if (ark_VertexNormalIn >= 0) { glVertexAttribPointer(ark_VertexNormalIn, 3, GL_FLOAT, GL_FALSE, 0, 0); }

	                Renderer::s_vboQuadColors->bind();
					Renderer::s_vboQuadColors->setData(&rawColors[0]);
					glVertexAttribPointer(ark_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);

					if (rawTexCoords != NULL) {
	                	Renderer::s_vboQuadTexCoords->bind();
	            	    Renderer::s_vboQuadTexCoords->setData(&rawTexCoords[0]);
	            	    glVertexAttribPointer(ark_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, 0);
	                }

					glUniformMatrix4fv(ark_ModelMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODEL)->pointer());
					glUniformMatrix4fv(ark_ViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_VIEW)->pointer());
					glUniformMatrix4fv(ark_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());
	                if (ark_NormalMatrix >= 0) { glUniformMatrix3fv(ark_NormalMatrix, 1, GL_FALSE, (float*) Renderer::getNormalMatrix()->pointer()); }

					RendererStats::s_glCalls += 10;

					if (rawTexCoords != NULL) {
						//glActiveTexture(GL_TEXTURE0);
						//glBindTexture(GL_TEXTURE_2D, dg->m_clouds->getTexture()->getId());
						glUniform1i(ark_TextureId, 0);
					}

				#elif defined(ARK2D_OPENGL_ES_2_0)

					//ARK2D::getLog()->e("this is rendering");

					enableVertexAttribArray(ark_VertexPositionIn);
					enableVertexAttribArray(ark_VertexNormalIn);
					enableVertexAttribArray(ark_VertexColorIn);
					if (rawTexCoords != NULL) { enableVertexAttribArray(ark_VertexTexCoordIn); }

					glVertexAttribPointer(ark_VertexPositionIn, 3, GL_FLOAT, GL_FALSE, 0, rawVertices);
	                if (ark_VertexNormalIn >= 0) { glVertexAttribPointer(ark_VertexNormalIn, 3, GL_FLOAT, GL_FALSE, 0, rawNormals); }
	                glVertexAttribPointer(ark_VertexColorIn, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, rawColors);
	                if (rawTexCoords != NULL) {
	                	glVertexAttribPointer(ark_VertexTexCoordIn, 2, GL_FLOAT, GL_FALSE, 0, rawTexCoords);
	                }

					glUniformMatrix4fv(ark_ModelMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_MODEL)->pointer());
					glUniformMatrix4fv(ark_ViewMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_VIEW)->pointer());
					glUniformMatrix4fv(ark_ProjectionMatrix, 1, GL_FALSE, (float*) Renderer::getMatrix(MatrixStack::TYPE_PROJECTION)->pointer());
	                if (ark_NormalMatrix >= 0) { glUniformMatrix3fv(ark_NormalMatrix, 1, GL_FALSE, (float*) Renderer::getNormalMatrix()->pointer()); }

	                if (rawTexCoords != NULL) {
						glUniform1i(ark_TextureId, 0);
					}

	                RendererStats::s_glCalls += 7;

            	#else

	                glVertexPointer(3, GL_FLOAT, 0, rawVertices);
	                glNormalPointer(3, GL_FLOAT, 0, rawNormals);
	                glColorPointer(4, GL_UNSIGNED_BYTE, 0, rawColors);
	                if (rawTexCoords != NULL) {
	                	glTexCoordPointer(2, GL_FLOAT, 0, rawTexCoords);
	                }

	                if (rawTexCoords != NULL) {
						glUniform1i(ark_TextureId, 0);
					}

					RendererStats::s_glCalls += 3;
				#endif


			#elif defined(ARK2D_RENDERER_DIRECTX)
				// ...
			#endif
		}
		void Shader::drawTriangleStrip() {
			#if defined(ARK2D_RENDERER_OPENGL)

				glDrawArrays(GL_TRIANGLE_STRIP, 0, length);
				RendererStats::s_tris += (length-2);
				RendererStats::s_glCalls += 1;

			#endif
		}
		void Shader::drawTriangleFan() {
			#if defined(ARK2D_RENDERER_OPENGL)

				glDrawArrays(GL_TRIANGLE_FAN, 0, length);
				RendererStats::s_tris += length;
				RendererStats::s_glCalls += 1;

			#endif
		}
		void Shader::drawTriangles() {
			#if defined(ARK2D_RENDERER_OPENGL)

				glDrawArrays(GL_TRIANGLES, 0, length);
				RendererStats::s_tris += length/3;
				RendererStats::s_glCalls += 1;

			#endif
		}
		Shader::~Shader() {

		}

	}
}
