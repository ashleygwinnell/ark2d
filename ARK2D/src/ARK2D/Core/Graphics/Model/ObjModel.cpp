/*
 * ObjModel.cpp
 *
 *  Created on: 5 Oct 2014
 *      Author: Ashley
 */

#include "ObjModel.h"
#include "../../Core/GameContainer.h"
#include "../Renderer.h"
#include "../Shader.h"
#include "../ShaderStore.h"
#include "../../Util/MathUtil.h"
 
namespace ARK { 
	namespace Graphics { 
		namespace Model {

			ObjModel::ObjModel(string fn):
				SceneNode("objmodel"),
				filename(""),
				vertices(),
				uvs(),
				normals(),
				colors()
			{
				filename = ARK2D::getContainer()->getResourcePath() + fn;
				loadObj();
			}

			void ObjModel::loadObj() {
				std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
				std::vector<Vector3<float> > temp_vertices; 
				std::vector<Vector2<float> > temp_uvs;
				std::vector<Vector3<float> > temp_normals;
				std::vector<Vector4<unsigned char> > temp_colors;

				FILE* file = fopen(filename.c_str(), "r");
				if (file == NULL) {
					ErrorDialog::createAndShow(StringUtil::append("file doesn't exist: ", filename));
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

					    Vector4<unsigned char> vertexcolor(255, 255, 255, 255);
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
				                    ErrorDialog::createAndShow(".obj file can't be read by our simple parser : ( Try exporting with other options.");
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
					Vector4<unsigned char> color = temp_colors[ vertexIndex-1 ];

					// Put the attributes in buffers
					vertices.push_back(vertex);
					uvs     .push_back(uv);
					normals .push_back(normal);
					colors .push_back(color);
			    }

				fclose(file);
			}

			void ObjModel::render() {
				SceneNode::render();

				Renderer* r = ARK2D::getRenderer();
				
				Matrix44<float> m = transform.toMatrix();
			    r->matrixMode(MatrixStack::TYPE_MODEL);
				r->pushMatrix();
			    r->multiplyMatrix(&m);

				preRenderFromPivot();

			    Shader* shader = RendererState::start(RendererState::GEOMETRY);
			    shader->setData((float*) &vertices[0], (float*)&normals[0], (float*)&uvs[0], (unsigned char*)&colors[0], vertices.size());
			    shader->drawTriangles();

                postRenderFromPivot();

				r->popMatrix();
			}
			  
			

			ObjModel::~ObjModel() { 
				 
			}
		}
	}
}






