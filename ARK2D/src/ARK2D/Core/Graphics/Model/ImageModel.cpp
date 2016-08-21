/*
 * ImageModel.cpp
 *
 *  Created on: 7 Oct 2014
 *      Author: Ashley
 */

#include "ImageModel.h"
#include "../../Core/GameContainer.h"
#include "../ImageIO/PNGImage.h"
#include "../Renderer.h"
#include "../Shader.h"
#include "../ShaderStore.h"

namespace ARK {
	namespace Core {
		namespace Graphics {
			namespace Model {

				ImageModel::ImageModel(string fn):
					SceneNode("imagemodel"),
					filename(""),
					vertices(),
					bounds()
				{
					transform.position += Vector3<float>(-2.0f, 6.0f, 0.0f);
					transform.scale *= Vector3<float>(0.5f, 0.5f, 0.5f);

					filename = ARK2D::getContainer()->getResourcePath() + fn;
					load();
				}

				void ImageModel::load() {
					GameContainer* container = ARK2D::getContainer();
					PNGImage* png = new PNGImage(filename.c_str());
					png->load();
					if (!png->isRGBA()) {
						ErrorDialog::createAndShow("png must be rgba.");
						exit(0);
					}

					bounds.set(0,0,0, png->getWidth(), png->getHeight(), 1);
					pivot.set(0.5f, -1.0f, 0.5f);

					unsigned char* pngdata = (unsigned char*) png->getImageData();
					for(unsigned int x = 0; x < png->getWidth(); x++) {
						for(unsigned int y = 0; y < png->getHeight(); y++) {
							unsigned char* root = pngdata + (((y * png->getWidth()) + x) * 4);
							unsigned char red = *root;
							unsigned char green = *(root+1);
							unsigned char blue = *(root+2);
							unsigned char alpha = *(root+3);

							unsigned int red_i = int(red);
							unsigned int green_i = int(green);
							unsigned int blue_i = int(blue);
							unsigned int alpha_i = int(alpha);

							String s;
							s += "{ x: ";
							s += x;
							s += ", y: ";
							s += y;
							s += ", r: ";
							s += red_i;
							s += ", g: ";
							s += green_i;
							s += ", b: ";
							s += blue_i;
							s += ", a: ";
							s += alpha_i;
							s += " }";
							//ARK2D::getLog()->w(s.get());



							if (alpha_i > 0) {

								// color data
								//colors.push_back(Vector4<float>(red_i / 255.0f, green_i / 255.0f, blue_i / 255.0f, alpha_i / 255.0f));
								colors.push_back(Vector4<unsigned char>(red_i, green_i , blue_i, alpha_i));
								int thisColor = colors.size() - 1;

								// vertex data
								// front side.
								{
									unsigned int tl = addVertex(x,   y,   0);
									unsigned int tr = addVertex(x+1, y,   0);
									unsigned int bl = addVertex(x,   y+1, 0);
									unsigned int br = addVertex(x+1, y+1, 0);

									ImageModelTriangle* tri = new ImageModelTriangle();
									tri->vert1 = tl;
									tri->vert2 = bl;
									tri->vert3 = tr;
									tri->color1 = thisColor;
									tri->color2 = thisColor;
									tri->color3 = thisColor;
									tri->normal = addNormal(0, 0, 1);

									ImageModelTriangle* tri2 = new ImageModelTriangle();
									tri2->vert1 = tr;
									tri2->vert2 = bl;
									tri2->vert3 = br;
									tri2->color1 = thisColor;
									tri2->color2 = thisColor;
									tri2->color3 = thisColor;
									tri2->normal = addNormal(0, 0, 1);

									indices.push_back(tri);
									indices.push_back(tri2);
								}

								// back side
								{
									unsigned int tl = addVertex(x,   y,   -1);
									unsigned int tr = addVertex(x+1, y,   -1);
									unsigned int bl = addVertex(x,   y+1, -1);
									unsigned int br = addVertex(x+1, y+1, -1);

									ImageModelTriangle* tri = new ImageModelTriangle();
									tri->vert1 = tr;
									tri->vert2 = br;
									tri->vert3 = tl;
									tri->color1 = thisColor;
									tri->color2 = thisColor;
									tri->color3 = thisColor;
									tri->normal = addNormal(0, 0, -1);

									ImageModelTriangle* tri2 = new ImageModelTriangle();
									tri2->vert1 = tl;
									tri2->vert2 = br;
									tri2->vert3 = bl;
									tri2->color1 = thisColor;
									tri2->color2 = thisColor;
									tri2->color3 = thisColor;
									tri2->normal = addNormal(0, 0, -1);

									indices.push_back(tri);
									indices.push_back(tri2);
								}

								// left side
								{
									if (!isFilledAt(png, x-1, y)) {
										unsigned int tl = addVertex(x,   y,   -1);
										unsigned int tr = addVertex(x, 	 y,   0);
										unsigned int bl = addVertex(x,   y+1, -1);
										unsigned int br = addVertex(x,   y+1, 0);

										ImageModelTriangle* tri = new ImageModelTriangle();
										tri->vert1 = tl;
										tri->vert2 = bl;
										tri->vert3 = tr;
										tri->color1 = thisColor;
										tri->color2 = thisColor;
										tri->color3 = thisColor;
										tri->normal = addNormal(-1, 0, 0);

										ImageModelTriangle* tri2 = new ImageModelTriangle();
										tri2->vert1 = tr;
										tri2->vert2 = bl;
										tri2->vert3 = br;
										tri2->color1 = thisColor;
										tri2->color2 = thisColor;
										tri2->color3 = thisColor;
										tri2->normal = addNormal(-1, 0, 0);

										indices.push_back(tri);
	                                    indices.push_back(tri2);
									}
								}

								// right side
								{
									if (!isFilledAt(png, x+1, y)) {
										unsigned int tl = addVertex(x+1,   y,   -1);
										unsigned int tr = addVertex(x+1,   y,   0);
										unsigned int bl = addVertex(x+1,   y+1, -1);
										unsigned int br = addVertex(x+1,   y+1, 0);

										ImageModelTriangle* tri = new ImageModelTriangle();
										tri->vert1 = tr;
										tri->vert2 = br;
										tri->vert3 = tl;
										tri->color1 = thisColor;
										tri->color2 = thisColor;
										tri->color3 = thisColor;
										tri->normal = addNormal(1, 0, 0);

										ImageModelTriangle* tri2 = new ImageModelTriangle();
										tri2->vert1 = tl;
										tri2->vert2 = br;
										tri2->vert3 = bl;
										tri2->color1 = thisColor;
										tri2->color2 = thisColor;
										tri2->color3 = thisColor;
										tri2->normal = addNormal(1, 0, 0);

										indices.push_back(tri);
										indices.push_back(tri2);
									}
								}

								// bottom side
								{
									if (!isFilledAt(png, x, y+1)) {
										unsigned int tl = addVertex(x,     y+1, 0);
										unsigned int tr = addVertex(x+1,   y+1, 0);
										unsigned int bl = addVertex(x,     y+1, -1);
										unsigned int br = addVertex(x+1,   y+1, -1);

										ImageModelTriangle* tri = new ImageModelTriangle();
										tri->vert1 = tl;
										tri->vert2 = bl;
										tri->vert3 = tr;
										tri->color1 = thisColor;
										tri->color2 = thisColor;
										tri->color3 = thisColor;
										tri->normal = addNormal(0, -1, 0);

										ImageModelTriangle* tri2 = new ImageModelTriangle();
										tri2->vert1 = tr;
										tri2->vert2 = bl;
										tri2->vert3 = br;
										tri2->color1 = thisColor;
										tri2->color2 = thisColor;
										tri2->color3 = thisColor;
										tri2->normal = addNormal(0, -1, 0);

										indices.push_back(tri);
										indices.push_back(tri2);
									}
								}

								// top side
								{
									if (!isFilledAt(png, x, y-1)) {
										unsigned int tl = addVertex(x,     y, -1);
										unsigned int tr = addVertex(x+1,   y, -1);
										unsigned int bl = addVertex(x,     y, 0);
										unsigned int br = addVertex(x+1,   y, 0);

										ImageModelTriangle* tri = new ImageModelTriangle();
										tri->vert1 = tl;
										tri->vert2 = bl;
										tri->vert3 = tr;
										tri->color1 = thisColor;
										tri->color2 = thisColor;
										tri->color3 = thisColor;
										tri->normal = addNormal(0, 1, 0);

										ImageModelTriangle* tri2 = new ImageModelTriangle();
										tri2->vert1 = tr;
										tri2->vert2 = bl;
										tri2->vert3 = br;
										tri2->color1 = thisColor;
										tri2->color2 = thisColor;
										tri2->color3 = thisColor;
										tri2->normal = addNormal(0, 1, 0);

										indices.push_back(tri);
										indices.push_back(tri2);
									}
								}

								//colors.push_back(Vector4<float>(red_i/255.0f, green_i/255.0f, blue_i/255.0f, alpha_a/255.0f));
								//colors.push_back(Vector4<float>(red_i/255.0f, green_i/255.0f, blue_i/255.0f, alpha_a/255.0f));
								//colors.push_back(Vector4<float>(red_i/255.0f, green_i/255.0f, blue_i/255.0f, alpha_a/255.0f));
								//colors.push_back(Vector4<float>(red_i/255.0f, green_i/255.0f, blue_i/255.0f, alpha_a/255.0f));

							}
						}
					}

					// remove duplicate verts
					/*for(signed int i = 0; i < (signed int) vertices.size(); i++) {
						for(signed int j = 0; j < (signed int) vertices.size(); j++) {
							if (i == j) { continue; }
							Vector3<float>* one = &vertices.at(i);
							Vector3<float>* two = &vertices.at(j);
							if (one->m_x == two->m_x && one->m_y == two->m_y && one->m_z == two->m_z ) {
								vertices.erase(vertices.begin() + j);
								i = -1;
								j = -1; // I really should learn these stl iterators at some point.
							}
						}
					}*/
				}

				unsigned int ImageModel::addVertex(float x, float y, float z) {
					y *= -1.0f;
					for(unsigned int i = 0; i < vertices.size(); ++i) {
						Vector3<float>* one = &vertices.at(i);
						if (one->x == x && one->y == y && one->z == z ) {
							return i;
						}
					}
					vertices.push_back(Vector3<float>(x, y, z));
					return vertices.size() - 1;
				}

				unsigned int ImageModel::addNormal(float x, float y, float z) {
					for(unsigned int i = 0; i < normals.size(); ++i) {
						Vector3<float>* one = &normals.at(i);
						if (one->x == x && one->y == y && one->z == z ) {
							return i;
						}
					}
					normals.push_back(Vector3<float>(x, y, z));
					return normals.size() - 1;
				}

				bool ImageModel::isFilledAt(PNGImage* png, unsigned int x, unsigned int y)
				{
					if (x >= png->getWidth()) { return false; }
					if (y >= png->getHeight()) { return false; }

					unsigned char* pngdata = (unsigned char*) png->getImageData();
					unsigned char* root = pngdata + (((y * png->getWidth()) + x) * 4);
					unsigned char alpha = *(root+3);
					unsigned int alpha_i = int(alpha);
					return (alpha_i > 0);
				}

				void ImageModel::render() {
					SceneNode::render();

					Renderer* r = ARK2D::getRenderer();
					Matrix44<float> m = transform.toMatrix();

				    r->matrixMode(MatrixStack::TYPE_MODEL); //MODELVIEW);
					r->pushMatrix();
				    r->multiplyMatrix(&m);

				    SceneNode::preRenderFromPivot();

					float* rawVertices = (float*) malloc(sizeof(float) * indices.size() * 3 * 3);
					for(unsigned int i = 0; i < indices.size(); i++) {
						rawVertices[i*9] = vertices.at(indices[i]->vert1).getX();
						rawVertices[(i*9)+1] = vertices.at(indices[i]->vert1).getY();
						rawVertices[(i*9)+2] = vertices.at(indices[i]->vert1).getZ();
						rawVertices[(i*9)+3] = vertices.at(indices[i]->vert2).getX();
						rawVertices[(i*9)+4] = vertices.at(indices[i]->vert2).getY();
						rawVertices[(i*9)+5] = vertices.at(indices[i]->vert2).getZ();
						rawVertices[(i*9)+6] = vertices.at(indices[i]->vert3).getX();
						rawVertices[(i*9)+7] = vertices.at(indices[i]->vert3).getY();
						rawVertices[(i*9)+8] = vertices.at(indices[i]->vert3).getZ();
					}

					float* rawNormals = (float*) malloc(sizeof(float) * indices.size() * 3 * 3);
					for(unsigned int i = 0; i < indices.size(); i++) {
						rawNormals[i*9]     = normals.at(indices[i]->normal).getX();
						rawNormals[(i*9)+1] = normals.at(indices[i]->normal).getY();
						rawNormals[(i*9)+2] = normals.at(indices[i]->normal).getZ();
						rawNormals[(i*9)+3] = normals.at(indices[i]->normal).getX();
						rawNormals[(i*9)+4] = normals.at(indices[i]->normal).getY();
						rawNormals[(i*9)+5] = normals.at(indices[i]->normal).getZ();
						rawNormals[(i*9)+6] = normals.at(indices[i]->normal).getX();
						rawNormals[(i*9)+7] = normals.at(indices[i]->normal).getY();
						rawNormals[(i*9)+8] = normals.at(indices[i]->normal).getZ();
					}

					float default_r = r->getDrawColor().getRedf();
					float default_g = r->getDrawColor().getGreenf();
					float default_b = r->getDrawColor().getBluef();
					unsigned char* rawColors = (unsigned char*) malloc(indices.size() * 3 * 4);
					for(unsigned int i = 0; i < indices.size(); i++) {
						rawColors[(i*12)+0] = default_r * colors.at(indices[i]->color1)[0];
						rawColors[(i*12)+1] = default_g * colors.at(indices[i]->color1)[1];
						rawColors[(i*12)+2] = default_b * colors.at(indices[i]->color1)[2];
				        rawColors[(i*12)+3] = 255;

						rawColors[(i*12)+4] = default_r * colors.at(indices[i]->color2)[0];
						rawColors[(i*12)+5] = default_g * colors.at(indices[i]->color2)[1];
						rawColors[(i*12)+6] = default_b * colors.at(indices[i]->color2)[2];
				        rawColors[(i*12)+7] = 255;

						rawColors[(i*12)+8] = default_r * colors.at(indices[i]->color3)[0];
						rawColors[(i*12)+9] = default_g * colors.at(indices[i]->color3)[1];
						rawColors[(i*12)+10] = default_b * colors.at(indices[i]->color3)[2];
				        rawColors[(i*12)+11] = 255;
					}

					Shader* shader = RendererState::start(RendererState::GEOMETRY);
					shader->setData(rawVertices, rawNormals, NULL, rawColors, indices.size() * 3);
					shader->drawTriangles();

	                SceneNode::postRenderFromPivot();

					r->popMatrix();

				 	free(rawVertices);
				 	free(rawColors);


				}


				ImageModel::~ImageModel() {

				}
			}

		}
	}
}




