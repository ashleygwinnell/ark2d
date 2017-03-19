#include "../../Namespaces.h"
#include "../../Common/DLL.h"
#include "../../Common/OpenGL.h"
#include "../Math/Matrix44.h"
#include <vector>

#ifndef ARK_CORE_GRAPHICS_MATRIXSTACK_H_
#define ARK_CORE_GRAPHICS_MATRIXSTACK_H_


namespace ARK {
    namespace Core {
        namespace Graphics {

            class ARK2D_API MatrixStack {
                public:
                    unsigned int m_type;

                    vector<Matrix44> m_stack;
                    unsigned int m_stackIndex;
                    unsigned int m_root; // the lowest stack index after ark2d does its bits. e.g. letterboxing. 
                                         // we reset to this instead of the identity matrix when starting new renderbatches.

                    bool m_dirty;

                    static const unsigned int TYPE_NULL = 0;
                    static const unsigned int TYPE_PROJECTION = 1; 
                    static const unsigned int TYPE_VIEW = 2;
                    static const unsigned int TYPE_MODEL = 3;
         
                    MatrixStack(); 
                    MatrixStack(unsigned int type); 
                    MatrixStack(unsigned int type, Matrix44 basecopy);
                    void translate(float x, float y, float z = 0.0f); 			// glTranslatef(x,y,0);
                    void rotate(float angle, float x, float y, float z = 0.0f); // glRotatef(angle, 0, 0, 1);
                    void scale(float x, float y, float z = 0.0f); 				// glScalef(x, y, 0.0f);
                    void ortho2d(float l, float r, float b, float t, float n, float f);
                    void frustum(float l, float r, float b, float t, float n, float f);
                    void perspective(float fov, float aspect, float n, float f);
                    void lookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ);
                    void identity();
                    float* data(float* mx);
                    void* pointer();  
                    #ifdef ARK2D_RENDERER_DIRECTX
                        DirectX::XMMATRIX d3dpointer();  
                    #endif
                    const Matrix44* current() const;
                    Matrix44* current();
                    Matrix44* at(unsigned int index);
                    void pushMatrix(bool setroot = false);
                    void popMatrix();
                    void toStringDialog();
                    bool isDirty();
                    void setDirty(bool b);
                    unsigned int height();
                    unsigned int root();
                    MatrixStack operator*(const MatrixStack& m);


                    virtual ~MatrixStack();
            };
        }
	}
}

#endif