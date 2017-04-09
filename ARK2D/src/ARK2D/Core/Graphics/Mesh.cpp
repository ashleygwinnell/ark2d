
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"
#include "ImageIO/PNGImage.h"
#include "../Log.h"

namespace ARK {
    namespace Core {
        namespace Graphics {

			Mesh* Mesh::createFromPNGFile(string ref, bool appendPath) {

				string oldref = ref;
				ref = Resource::fixpath(ref, appendPath);

                Mesh* mesh = new Mesh();

				auto addVertex = [mesh](float x, float y, float z, bool shared = false) -> unsigned int {
					//y *= -1.0f;
					if (shared) {
						for(unsigned int i = 0; i < mesh->vertices.size(); ++i) {
							Vector3<float>* one = &mesh->vertices.at(i);
							if (one->x == x && one->y == y && one->z == z ) {
								return i;
							}
						}
					}
					mesh->vertices.push_back(Vector3<float>(x, y, z));
					return mesh->vertices.size() - 1;
                };

				auto addNormal = [mesh](float x, float y, float z, bool shared = false) -> unsigned int  {
					if (shared) {
						for(unsigned int i = 0; i < mesh->normals.size(); ++i) {
							Vector3<float>* one = &mesh->normals.at(i);
							if (one->x == x && one->y == y && one->z == z ) {
								return i;
							}
						}
					}
					mesh->normals.push_back(Vector3<float>(x, y, z));
					return mesh->normals.size() - 1;
                };

                auto addColor = [mesh](unsigned int r, unsigned int g, unsigned int b, unsigned int a, bool shared = false) -> unsigned int {
                	if (shared) {
						for(unsigned int i = 0; i < mesh->colors.size(); ++i) {
							uint32_t one = mesh->colors.at(i);
                            if (Color::unpackRed(one) == r && Color::unpackGreen(one) == g && Color::unpackBlue(one) == b && Color::unpackAlpha(one) == a) {
								return i;
							}
						}
					}
                    mesh->colors.push_back(Color::pack(r, g, b, a));
					return mesh->colors.size() - 1;
                };

				PNGImage* png = new PNGImage(ref.c_str());
				png->load();
				if (!png->isRGBA()) {
					ARK2D::getLog()->e("png must be rgba.");
					exit(0);
				}

				//bounds.set(0,0,0, png->getWidth(), png->getHeight(), 1);
				//pivot.set(0.5f, -1.0f, 0.5f);

				unsigned char* pngdata = (unsigned char*) png->getImageData();

				for(unsigned int x = 0; x < png->getWidth(); x++)
				{
					for(unsigned int y = 0; y < png->getHeight(); y++)
					{
						unsigned char* root = pngdata + (((y * png->getWidth()) + x) * 4);

						unsigned char red = *root;
						unsigned char green = *(root+1);
						unsigned char blue = *(root+2);
						unsigned char alpha = *(root+3);

						unsigned int red_i = int(red);
						unsigned int green_i = int(green);
						unsigned int blue_i = int(blue);
						unsigned int alpha_i = int(alpha);

//						String s;
//						s += "{ x: ";
//						s += x;
//						s += ", y: ";
//						s += y;
//						s += ", r: ";
//						s += red_i;
//						s += ", g: ";
//						s += green_i;
//						s += ", b: ";
//						s += blue_i;
//						s += ", a: ";
//						s += alpha_i;
//						s += " }";
						//ARK2D::getLog()->w(s.get());



						if (alpha_i > 0) {

							// color data
							//colors.push_back(Vector4<float>(red_i / 255.0f, green_i / 255.0f, blue_i / 255.0f, alpha_i / 255.0f));
							//mesh->colors.push_back(Vector4(red_i, green_i , blue_i, alpha_i));
							//int thisColor = addColor(red_i, green_i, blue_i, alpha_i); //mesh->colors.size() - 1;

							// vertex data
							// front side.
							{
								unsigned short tl = addVertex(x,   y,   0);
								unsigned short tr = addVertex(x+1, y,   0);
								unsigned short bl = addVertex(x,   y+1, 0);
								unsigned short br = addVertex(x+1, y+1, 0);

								addNormal(0, 0, 1);
								addNormal(0, 0, 1);
								addNormal(0, 0, 1);
								addNormal(0, 0, 1);

								addColor(red_i, green_i, blue_i, alpha_i);
								addColor(red_i, green_i, blue_i, alpha_i);
								addColor(red_i, green_i, blue_i, alpha_i);
								addColor(red_i, green_i, blue_i, alpha_i);


								mesh->indices.push_back(tl);
								mesh->indices.push_back(bl);
								mesh->indices.push_back(tr);

								mesh->indices.push_back(tr);
								mesh->indices.push_back(bl);
								mesh->indices.push_back(br);

								// ImageModelTriangle* tri = new ImageModelTriangle();
								// tri->vert1 = tl;
								// tri->vert2 = bl;
								// tri->vert3 = tr;
								// tri->color1 = thisColor;
								// tri->color2 = thisColor;
								// tri->color3 = thisColor;
								// tri->normal = addNormal(0, 0, 1);

								// ImageModelTriangle* tri2 = new ImageModelTriangle();
								// tri2->vert1 = tr;
								// tri2->vert2 = bl;
								// tri2->vert3 = br;
								// tri2->color1 = thisColor;
								// tri2->color2 = thisColor;
								// tri2->color3 = thisColor;
								// tri2->normal = addNormal(0, 0, 1);

								// mesh->indices.push_back(tri);
								// mesh->indices.push_back(tri2);
							}

							// back side
							{
								unsigned short tl = addVertex(x,   y,   -1);
								unsigned short tr = addVertex(x+1, y,   -1);
								unsigned short bl = addVertex(x,   y+1, -1);
								unsigned short br = addVertex(x+1, y+1, -1);

								addNormal(0, 0, -1);
								addNormal(0, 0, -1);
								addNormal(0, 0, -1);
								addNormal(0, 0, -1);

								addColor(red_i, green_i, blue_i, alpha_i);
								addColor(red_i, green_i, blue_i, alpha_i);
								addColor(red_i, green_i, blue_i, alpha_i);
								addColor(red_i, green_i, blue_i, alpha_i);

								mesh->indices.push_back(tr);
								mesh->indices.push_back(br);
								mesh->indices.push_back(tl);

								mesh->indices.push_back(tl);
								mesh->indices.push_back(br);
								mesh->indices.push_back(bl);

								// ImageModelTriangle* tri = new ImageModelTriangle();
								// tri->vert1 = tr;
								// tri->vert2 = br;
								// tri->vert3 = tl;
								// tri->color1 = thisColor;
								// tri->color2 = thisColor;
								// tri->color3 = thisColor;
								// tri->normal = addNormal(0, 0, -1);

								// ImageModelTriangle* tri2 = new ImageModelTriangle();
								// tri2->vert1 = tl;
								// tri2->vert2 = br;
								// tri2->vert3 = bl;
								// tri2->color1 = thisColor;
								// tri2->color2 = thisColor;
								// tri2->color3 = thisColor;
								// tri2->normal = addNormal(0, 0, -1);

								// indices.push_back(tri);
								// indices.push_back(tri2);
							}

							// left side
							{
								if (!png->isPixelFilled(x-1, y)) {
									unsigned short tl = addVertex(x,   y,   -1);
									unsigned short tr = addVertex(x, 	 y,    0);
									unsigned short bl = addVertex(x,   y+1, -1);
									unsigned short br = addVertex(x,   y+1,  0);

									addNormal(-1, 0, 0);
									addNormal(-1, 0, 0);
									addNormal(-1, 0, 0);
									addNormal(-1, 0, 0);

									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);

									mesh->indices.push_back(tl);
									mesh->indices.push_back(bl);
									mesh->indices.push_back(tr);

									mesh->indices.push_back(tr);
									mesh->indices.push_back(bl);
									mesh->indices.push_back(br);

									/*ImageModelTriangle* tri = new ImageModelTriangle();
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
                                    indices.push_back(tri2);*/
								}
							}

							// right side
							{
								if (!png->isPixelFilled(x+1, y)) {
									unsigned short tl = addVertex(x+1,   y,   -1);
									unsigned short tr = addVertex(x+1,   y,    0);
									unsigned short bl = addVertex(x+1,   y+1, -1);
									unsigned short br = addVertex(x+1,   y+1,  0);

									addNormal(1, 0, 0);
									addNormal(1, 0, 0);
									addNormal(1, 0, 0);
									addNormal(1, 0, 0);

									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);

									mesh->indices.push_back(tr);
									mesh->indices.push_back(br);
									mesh->indices.push_back(tl);

									mesh->indices.push_back(tl);
									mesh->indices.push_back(br);
									mesh->indices.push_back(bl);

									/*ImageModelTriangle* tri = new ImageModelTriangle();
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
									indices.push_back(tri2);*/
								}
							}

							// bottom side
							{
								if (!png->isPixelFilled(x, y+1)) {
									unsigned short tl = addVertex(x,     y+1, 0);
									unsigned short tr = addVertex(x+1,   y+1, 0);
									unsigned short bl = addVertex(x,     y+1, -1);
									unsigned short br = addVertex(x+1,   y+1, -1);

									addNormal(0, -1, 0);
									addNormal(0, -1, 0);
									addNormal(0, -1, 0);
									addNormal(0, -1, 0);

									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);

									mesh->indices.push_back(tl);
									mesh->indices.push_back(bl);
									mesh->indices.push_back(tr);

									mesh->indices.push_back(tr);
									mesh->indices.push_back(bl);
									mesh->indices.push_back(br);

									/*ImageModelTriangle* tri = new ImageModelTriangle();
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
									indices.push_back(tri2);*/
								}
							}

							// top side
							{
								if (!png->isPixelFilled(x, y-1)) {
									unsigned short tl = addVertex(x,     y, -1);
									unsigned short tr = addVertex(x+1,   y, -1);
									unsigned short bl = addVertex(x,     y,  0);
									unsigned short br = addVertex(x+1,   y,  0);

									addNormal(0, 1, 0);
									addNormal(0, 1, 0);
									addNormal(0, 1, 0);
									addNormal(0, 1, 0);

									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);
									addColor(red_i, green_i, blue_i, alpha_i);

									mesh->indices.push_back(tl);
									mesh->indices.push_back(bl);
									mesh->indices.push_back(tr);

									mesh->indices.push_back(tr);
									mesh->indices.push_back(bl);
									mesh->indices.push_back(br);

									/*ImageModelTriangle* tri = new ImageModelTriangle();
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
									indices.push_back(tri2);*/
								}
							}

							//colors.push_back(Vector4<float>(red_i/255.0f, green_i/255.0f, blue_i/255.0f, alpha_a/255.0f));
							//colors.push_back(Vector4<float>(red_i/255.0f, green_i/255.0f, blue_i/255.0f, alpha_a/255.0f));
							//colors.push_back(Vector4<float>(red_i/255.0f, green_i/255.0f, blue_i/255.0f, alpha_a/255.0f));
							//colors.push_back(Vector4<float>(red_i/255.0f, green_i/255.0f, blue_i/255.0f, alpha_a/255.0f));

						}
					}
				}
				return mesh;

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

			Mesh* Mesh::createFromObjFile(string ref, bool appendPath) {

				string oldref = ref;
				ref = Resource::fixpath(ref, appendPath);

				// ARK::Tiled::TiledMap* map = NULL;

				// #if defined(ARK2D_ANDROID)
				// 	RawDataReturns* rt = ARK::Core::Resource::getRawData(ref);
				// 	map = new ARK::Tiled::TiledMap(ref, rt->data);
				// 	map->load();
				// #else
				// 	map = new ARK::Tiled::TiledMap(ref);
				// 	map->load();
				// #endif
				// return map;

				//filename = ARK2D::getContainer()->getResourcePath() + fn;

				Mesh* mesh = new Mesh();

				std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
				std::vector<Vector3<float> > temp_vertices;
				std::vector<Vector2<float> > temp_uvs;
				std::vector<Vector3<float> > temp_normals;
				std::vector<Vector4> temp_colors;


				FILE* file = fopen(ref.c_str(), "r");
				if (file == NULL) {
                    ARK2D::getLog()->e(StringUtil::append("file doesn't exist: ", ref));
					exit(0);
				}

				while (true) {
					char lineHeader[128];

					// read the first word of the line
					int res = fscanf(file, "%s", lineHeader);
					if (res == EOF) {
						break; // EOF = End Of File. Quit the loop.
					}
					int spos = ftell(file);

					// else : parse lineHeader
					if (strcmp( lineHeader, "v" ) == 0)
					{
					    Vector3<float> vertex;
					    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
					    temp_vertices.push_back(vertex);

					    Vector4 vertexcolor(255, 255, 255, 255);
					    temp_colors.push_back(vertexcolor);
					}
					else if (strcmp( lineHeader, "vt" ) == 0)
					{
						Vector2<float> uv;
					    fscanf(file, "%f %f\n", &uv.m_x, &uv.m_y );
					    temp_uvs.push_back(uv);
					}
					else if (strcmp( lineHeader, "vn" ) == 0)
					{
						Vector3<float> normal;
						fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
						temp_normals.push_back(normal);
					}
					else if ( strcmp( lineHeader, "f" ) == 0)
					{
						// std::string vertex1, vertex2, vertex3;
						unsigned int vertexIndex[4], uvIndex[3], normalIndex[3];
						fseek(file, spos, SEEK_SET);
						int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
						if (matches == 9) {

						    vertexIndices.push_back(vertexIndex[0]);
							vertexIndices.push_back(vertexIndex[1]);
							vertexIndices.push_back(vertexIndex[2]);
							uvIndices    .push_back(uvIndex[0]);
							uvIndices    .push_back(uvIndex[1]);
							uvIndices    .push_back(uvIndex[2]);
							normalIndices.push_back(normalIndex[0]);
							normalIndices.push_back(normalIndex[1]);
							normalIndices.push_back(normalIndex[2]);
			            } else {
			            	vertexIndex[0] = 0;
			            	vertexIndex[1] = 0;
			            	vertexIndex[2] = 0;
			            	vertexIndex[3] = 0;
			            	fseek(file, spos, SEEK_SET);
			                matches = fscanf(file, "%d %d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2], &vertexIndex[3] );
			                if (matches == 4) {
			                	vertexIndices.push_back(vertexIndex[0]);
				                vertexIndices.push_back(vertexIndex[1]);
				                vertexIndices.push_back(vertexIndex[2]);

				                vertexIndices.push_back(vertexIndex[0]);
				                vertexIndices.push_back(vertexIndex[2]);
				                vertexIndices.push_back(vertexIndex[3]);

			                    Vector3<float> normal = MathUtil::calculateFaceNormal(
			                   		&temp_vertices[vertexIndex[0]-1],
			                    	&temp_vertices[vertexIndex[1]-1],
			                    	&temp_vertices[vertexIndex[2]-1]
			                   	);
				                temp_normals.push_back(normal);
				                normalIndices.push_back(temp_normals.size());
				                normalIndices.push_back(temp_normals.size());
				                normalIndices.push_back(temp_normals.size());

			                	Vector3<float> normal2 = MathUtil::calculateFaceNormal(
			                   		&temp_vertices[vertexIndex[0]-1],
			                    	&temp_vertices[vertexIndex[2]-1],
			                    	&temp_vertices[vertexIndex[3]-1]
			                   	);
				                temp_normals.push_back(normal2);
				                normalIndices.push_back(temp_normals.size());
				                normalIndices.push_back(temp_normals.size());
				                normalIndices.push_back(temp_normals.size());

				                Vector2<float> uv(0.0f, 0.0f);
								temp_uvs.push_back(uv);
								uvIndices.push_back(temp_uvs.size());
								uvIndices.push_back(temp_uvs.size());
								uvIndices.push_back(temp_uvs.size());
								uvIndices.push_back(temp_uvs.size());
								uvIndices.push_back(temp_uvs.size());
								uvIndices.push_back(temp_uvs.size());

			                } else {
			                	vertexIndex[0] = 0;
				            	vertexIndex[1] = 0;
				            	vertexIndex[2] = 0;
				            	vertexIndex[3] = 0;
				            	fseek(file, spos, SEEK_SET);
				                matches = fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2] );
				                if (matches == 3) {
				                    vertexIndices.push_back(vertexIndex[0]);
				                    vertexIndices.push_back(vertexIndex[1]);
				                    vertexIndices.push_back(vertexIndex[2]);

			                        Vector3<float> normal = MathUtil::calculateFaceNormal(
				                   		&temp_vertices[vertexIndex[0]-1],
				                    	&temp_vertices[vertexIndex[1]-1],
				                    	&temp_vertices[vertexIndex[2]-1]
				                   	);
					                temp_normals.push_back(normal);
									normalIndices.push_back(temp_normals.size());
									normalIndices.push_back(temp_normals.size());
									normalIndices.push_back(temp_normals.size());

									Vector2<float> uv(0.0f, 0.0f);
									temp_uvs.push_back(uv);
									uvIndices.push_back(temp_uvs.size());
									uvIndices.push_back(temp_uvs.size());
									uvIndices.push_back(temp_uvs.size());

				                } else {
                                    ARK2D::getLog()->e(".obj file can't be read by our simple parser : ( Try exporting with other options.");
				                    exit(0);
				                }
				            }
			            }

					} else {
						// Probably a comment, eat up the rest of the line
						char stupidBuffer[1000];
						fgets(stupidBuffer, 1000, file);
					}
				}

				// For each vertex of each triangle
				for (unsigned int i = 0; i < vertexIndices.size(); ++i)
				{
					// Get the indices of its attributes
					unsigned int vertexIndex = vertexIndices[i];
					unsigned int uvIndex     = uvIndices[i];
					unsigned int normalIndex = normalIndices[i];

					// Get the attributes thanks to the index
					Vector3<float> vertex = temp_vertices[ vertexIndex-1 ];
					Vector2<float> uv 	  = temp_uvs[ uvIndex-1 ];
					Vector3<float> normal = temp_normals[ normalIndex-1 ];
					Vector4 color = temp_colors[ vertexIndex-1 ];

					// Put the attributes in buffers
					mesh->vertices.push_back(vertex);
					mesh->uvs     .push_back(uv);
					mesh->normals .push_back(normal);
					mesh->colors  .push_back( Color::pack(&color) );
			    }

				fclose(file);

				return mesh;
			}


			MeshRenderer::MeshRenderer(Mesh* m):
				GameComponent(),
				mesh(m) {

			}
			void MeshRenderer::render() {
				Shader* shader = RendererState::start(RendererState::GEOMETRY);
			    shader->setData((float*) &mesh->vertices[0], (float*)&mesh->normals[0], (float*)&mesh->uvs[0], (unsigned char*)&mesh->colors[0], mesh->vertices.size());
			    if (mesh->indices.size() > 0) {
			    	shader->setIndices( &mesh->indices[0], mesh->indices.size() );
					shader->drawTrianglesIndexed( );
				}
				else {
					shader->drawTriangles();
				}


			}

		}
	}
}
