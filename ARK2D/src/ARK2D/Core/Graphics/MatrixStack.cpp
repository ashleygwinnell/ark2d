
#include "MatrixStack.h"
#include "../Controls/ErrorDialog.h"

#include "../../Util/Log.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            MatrixStack::MatrixStack():
                m_type(TYPE_NULL),
                m_stack(),  
                m_stackIndex(0),
                m_root(0),
                m_dirty(true)
            {
                Matrix44 defaultMatrix;
                defaultMatrix.identity();
                m_stack.push_back(defaultMatrix);
            }
            MatrixStack::MatrixStack(unsigned int type):
                m_type(type),
                m_stack(),  
                m_stackIndex(0),
                m_root(0),
                m_dirty(true)
            {
                Matrix44 defaultMatrix;
                defaultMatrix.identity();
                m_stack.push_back(defaultMatrix);
            }
            MatrixStack::MatrixStack(unsigned int type, Matrix44 basecopy):
                m_type(type),
                m_stack(),  
                m_stackIndex(0),
                m_root(0),
                m_dirty(true) {
                Matrix44 defaultMatrix = basecopy.copy();
                m_stack.push_back(defaultMatrix);
            }
            void MatrixStack::translate(float x, float y, float z) {
                // glTranslatef(x,y,0);
                m_stack.at(m_stackIndex).translate(x, y, z); 
                m_dirty = true;
            }
            void MatrixStack::rotate(float angle, float x, float y, float z) {
                // glRotatef(angle, 0, 0, 1);
                m_stack.at(m_stackIndex).rotate(angle, x, y, z);
                m_dirty = true;
            }
            void MatrixStack::scale(float x, float y, float z) {
                // glScalef(x, y, 0.0f);
                m_stack.at(m_stackIndex).scale(x, y, z);
                m_dirty = true;
            }
            void MatrixStack::ortho2d(float l, float r, float b, float t, float n, float f) {
                m_stack.at(m_stackIndex).ortho2d(l, r, b, t, n, f);
                m_dirty = true; 
            }
            void MatrixStack::frustum(float l, float r, float b, float t, float n, float f) {
                m_stack.at(m_stackIndex).frustum(l, r, b, t, n, r);
                m_dirty = true;
            }
            void MatrixStack::perspective(float fov, float aspect, float n, float f) {
                m_stack.at(m_stackIndex).perspective(fov, aspect, n, f);
                m_dirty = true;
            }
            void MatrixStack::lookAt(float eyeX, float eyeY, float eyeZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ) {
                m_stack.at(m_stackIndex).lookAt(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, upX, upY, upZ);
                m_dirty = true;
            }
            void MatrixStack::identity() {
                m_stack.at(m_stackIndex).identity();
                m_dirty = true;
            }
            float* MatrixStack::data(float* mx) {
                const Matrix44& thismx = m_stack.at(m_stackIndex);
                mx[0] = thismx[0][0];
                mx[1] = thismx[0][1];
                mx[2] = thismx[0][2];
                mx[3] = thismx[0][3];
                mx[4] = thismx[1][0];
                mx[5] = thismx[1][1]; 
                mx[6] = thismx[1][2]; 
                mx[7] = thismx[1][3];
                mx[8] = thismx[2][0];
                mx[9] = thismx[2][1];
                mx[10] = thismx[2][2];
                mx[11] = thismx[2][3];
                mx[12] = thismx[3][0];
                mx[13] = thismx[3][1];
                mx[14] = thismx[3][2];
                mx[15] = thismx[3][3]; 
                return &mx[0];
            }
            void* MatrixStack::pointer() {
                return m_stack.at(m_stackIndex).pointer();
            }

            void MatrixStack::setDirty(bool b) {
                m_dirty = b;
            }
            bool MatrixStack::isDirty() {
                return m_dirty;
            }
            unsigned int MatrixStack::height() {
                return m_stackIndex; // m_stack.size();
            }
            unsigned int MatrixStack::root() {
                return m_root;
            }

            const Matrix44* MatrixStack::current() const {
                return &m_stack[m_stackIndex];
            }
            Matrix44* MatrixStack::current() {
                return &m_stack[m_stackIndex];
            }
            Matrix44* MatrixStack::at(unsigned int index) {
                return &m_stack[index];
            }

            

            
            #ifdef ARK2D_RENDERER_DIRECTX
                DirectX::XMMATRIX MatrixStack::d3dpointer() {
                    //ARK2D::getLog()->v("Updating Matrix Buffer...");

                    /*float* d = (float*) m_stack.at(m_stackIndex).pointer();

                    ID3D11DeviceContext* deviceContext = ARK2D::getContainer()->m_platformSpecific.m_deviceContext;
                    D3D11_MAPPED_SUBRESOURCE mappedResource;
                    HRESULT result = deviceContext->Map(m_d3d_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
                    if (FAILED(result)) {
                        ARK2D::getLog()->e(StringUtil::append("Could not Update (Map) Matrix Buffer (d3dpointer): ", m_type));
                        return m_d3d_matrix;
                    }
                    void* dataPtr = (void*) mappedResource.pData;
                    memcpy(dataPtr, d, sizeof(float) * 16);
                    deviceContext->Unmap(m_d3d_buffer, 0);*/

                    const Matrix44<float>& thismx = m_stack.at(m_stackIndex); 
                    //ARK2D::getLog()->v("1");
                    
                    using namespace DirectX;
                    
                    

                    // switched row-major for column-major!
                    /*
                    XMMATRIX m_d3d_matrix;
                    XMVectorSetX(m_d3d_matrix.r[0], thismx[0][0]);
                    XMVectorSetY(m_d3d_matrix.r[0], thismx[1][0]);
                    XMVectorSetW(m_d3d_matrix.r[0], thismx[2][0]);
                    XMVectorSetZ(m_d3d_matrix.r[0], thismx[3][0]);

                    XMVectorSetX(m_d3d_matrix.r[1], thismx[0][1]); 
                    XMVectorSetY(m_d3d_matrix.r[1], thismx[1][1]);
                    XMVectorSetW(m_d3d_matrix.r[1], thismx[2][1]);
                    XMVectorSetZ(m_d3d_matrix.r[1], thismx[3][1]);
     
                    XMVectorSetX(m_d3d_matrix.r[2], thismx[0][2]);
                    XMVectorSetY(m_d3d_matrix.r[2], thismx[1][2]);
                    XMVectorSetW(m_d3d_matrix.r[2], thismx[2][2]);
                    XMVectorSetZ(m_d3d_matrix.r[2], thismx[3][2]);

                    XMVectorSetX(m_d3d_matrix.r[3], thismx[0][3]);
                    XMVectorSetY(m_d3d_matrix.r[3], thismx[1][3]);
                    XMVectorSetW(m_d3d_matrix.r[3], thismx[2][3]);
                    XMVectorSetZ(m_d3d_matrix.r[3], thismx[3][3]);*/

                    /*XMMATRIX m_d3d_matrix;
                    XMVectorSetX(m_d3d_matrix.r[0], thismx[0][0]);
                    XMVectorSetY(m_d3d_matrix.r[0], thismx[0][1]);
                    XMVectorSetW(m_d3d_matrix.r[0], thismx[0][2]);
                    XMVectorSetZ(m_d3d_matrix.r[0], thismx[0][3]);

                    XMVectorSetX(m_d3d_matrix.r[1], thismx[1][0]); 
                    XMVectorSetY(m_d3d_matrix.r[1], thismx[1][1]);
                    XMVectorSetW(m_d3d_matrix.r[1], thismx[1][2]);
                    XMVectorSetZ(m_d3d_matrix.r[1], thismx[1][3]);
     
                    XMVectorSetX(m_d3d_matrix.r[2], thismx[2][0]);
                    XMVectorSetY(m_d3d_matrix.r[2], thismx[2][1]);
                    XMVectorSetW(m_d3d_matrix.r[2], thismx[2][2]);
                    XMVectorSetZ(m_d3d_matrix.r[2], thismx[2][3]);

                    XMVectorSetX(m_d3d_matrix.r[3], thismx[3][0]);
                    XMVectorSetY(m_d3d_matrix.r[3], thismx[3][1]);
                    XMVectorSetW(m_d3d_matrix.r[3], thismx[3][2]);
                    XMVectorSetZ(m_d3d_matrix.r[3], thismx[3][3]);*/

                    XMMATRIX m_d3d_matrix = XMMATRIX(
                        thismx[0][0], thismx[0][1], thismx[0][2], thismx[0][3],
                        thismx[1][0], thismx[1][1], thismx[1][2], thismx[1][3],
                        thismx[2][0], thismx[2][1], thismx[2][2], thismx[2][3],
                        thismx[3][0], thismx[3][1], thismx[3][2], thismx[3][3]
                    );
                     
                    //ARK2D::getLog()->v("4");
                    return m_d3d_matrix;
                    //return NULL;

                }
            #endif

            void MatrixStack::toStringDialog() {
                float mx[16]; 
                data(&mx[0]);
     
                float* mx2;
                mx2 = (float*) pointer(); 

                string compareStr = "";
                for(unsigned int i = 0; i < 16; i++) {
                    compareStr += "{";
                    compareStr += Cast::toString<float>(mx[i]);
                    compareStr += ",";
                    compareStr += Cast::toString<float>(mx2[i]);
                    compareStr += "}, ";
                }
                ErrorDialog::createAndShow(compareStr);
            }
            void MatrixStack::pushMatrix(bool setRoot) {
                //Matrix44<float> copy = m_stack.at(m_stackIndex).copy();
                //m_stack.push_back(copy);
                m_stackIndex++; 
                if (setRoot) { m_root = m_stackIndex; }

                if (m_stackIndex >= m_stack.size()) {
                    Matrix44 copy = m_stack.at(m_stackIndex-1).copy();
                    m_stack.push_back(copy);
                } else { 
                    m_stack.at(m_stackIndex).set(m_stack.at(m_stackIndex-1));
                }

                m_dirty = true;
            }
            void MatrixStack::popMatrix() {
     
                if (m_stackIndex == 0) {
                    ARK2D::getLog()->e("Cannot pop Matrix Stack below 0.");
                    ARK2D::getLog()->e(StringUtil::append("MatrixStack type: ", m_type));
                    ARK2D::getLog()->backtrace();
                    return;
                }
                //m_stack.pop_back();
                m_stackIndex--;
                 
                m_dirty = true;
            }
            MatrixStack MatrixStack::operator*(const MatrixStack& m) {
                MatrixStack ret(m_type, m_stack[m_stackIndex]);
                ret.m_stack[ret.m_stackIndex] *= m.m_stack[m.m_stackIndex];
                return ret;
            }
            MatrixStack::~MatrixStack() {
     
            }
        }
	}
}