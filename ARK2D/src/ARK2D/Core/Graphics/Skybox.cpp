/*
 * Skybox.cpp
 *
 *  Created on: 1 Dec 2015
 *      Author: Ashley
 */

#include "Skybox.h"
#include "Renderer.h"
#include "Image.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

            Skybox::Skybox(string boxImageFilename):
                SceneNode("skybox"),
                sz(0.2f)
            {
                Renderer::setInterpolation(Renderer::INTERPOLATION_LINEAR);
                Image* box = Resource::get(boxImageFilename)->asImage();
                set(box);
            }
            Skybox::Skybox(Image* box):
                SceneNode("skybox"),
                sz(0.2f)
            {
                set(box);
            }
            Skybox::Skybox(Image* new_front, Image* new_back, Image* new_left, Image* new_right, Image* new_top, Image* new_bottom):
                SceneNode("skybox"), 
                sz(0.2f)
            {
                set(new_front, new_back, new_left, new_right, new_top, new_bottom);
            }

            void Skybox::set(Image* boxImage) {
                set(
                    boxImage->getSubImage(128,256,128,-128), // front
                    boxImage->getSubImage(384,256,128,-128), // back
                    boxImage->getSubImage(0,256,128,-128), 	// left
                    boxImage->getSubImage(256,256,128,-128), // right
                    boxImage->getSubImage(128,128,128,-128), // top
                    boxImage->getSubImage(128,384,128,-128) 	// bottom
                );
            }
            void Skybox::set(Image* new_front, Image* new_back, Image* new_left, Image* new_right, Image* new_top, Image* new_bottom) {
                front = new_front;
                back = new_back;
                left = new_left;
                right = new_right;
                top = new_top;
                bottom = new_bottom;

                front->transform.rotation = Quaternion<float>::angleAxis(0, 0, 1, 0);
                left->transform.rotation = Quaternion<float>::angleAxis(90, 0, 1, 0);
                back->transform.rotation = Quaternion<float>::angleAxis(180, 0, 1, 0);
                right->transform.rotation = Quaternion<float>::angleAxis(270, 0, 1, 0);
                top->transform.rotation = Quaternion<float>::angleAxis(90, 1, 0, 0);
                bottom->transform.rotation = Quaternion<float>::angleAxis(270, 1, 0, 0);
            }

            void Skybox::setSize(float s) {
                sz = s;
                front->setSize(sz, sz);
                left->setSize(sz, sz);
                back->setSize(sz, sz);
                right->setSize(sz, sz);
                top->setSize(sz, sz);
                bottom->setSize(sz, sz);
            }

            void Skybox::render() {

                float tx = 0.0f; 
                float ty = 0.0f;
                float tz = 0.0f;

                float hz = sz*0.5f;
                
                float zf = tz + hz;
                float zb = tz - hz; 

                float tl_f_x = tx - hz;
                float tl_f_y = ty + hz;
                float tl_f_z = zf;

                float tr_f_x = tx + hz;
                float tr_f_y = ty + hz;
                float tr_f_z = zf;

                float bl_f_x = tx - hz;
                float bl_f_y = ty - hz;
                float bl_f_z = zf;

                float br_f_x = tx + hz;
                float br_f_y = ty - hz;
                float br_f_z = zf;

                Renderer* r = ARK2D::getRenderer();
                
                r->getMatrix(MatrixStack::TYPE_MODEL)->pushMatrix();
                r->translate(transform.position.x, transform.position.y, transform.position.z);
             
                front->draw(tl_f_x, bl_f_y, zb);
                left->draw(tl_f_x, bl_f_y, zf);
                back->draw(tr_f_x, bl_f_y, zf);
                right->draw(tr_f_x, bl_f_y, zb);
                top->draw(tl_f_x, tl_f_y, zb);
                bottom->draw(tl_f_x, bl_f_y, zf);
                
                r->getMatrix(MatrixStack::TYPE_MODEL)->popMatrix();
            }
              
            Skybox::~Skybox() { 
                 
            }
        }

	}
}



