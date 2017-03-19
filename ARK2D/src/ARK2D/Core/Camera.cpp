
#include "Camera.h"

#include "Vendor/glm/glm/glm.hpp"
#include "Vendor/glm/glm/gtx/transform.hpp"
#include "Vendor/glm/glm/gtc/quaternion.hpp"
#include "Vendor/glm/glm/gtx/quaternion.hpp"
#include "Vendor/glm/glm/gtc/matrix_transform.hpp"
#include "Vendor/glm/glm/gtc/type_ptr.hpp"

#include "ARK2D.h"
#include "GameContainer.h"
#include "Graphics/Renderer.h"
#include "Log.h"

using namespace std;

namespace ARK {
	namespace Core {

		Camera* Camera::current = NULL;

		Camera::Camera():
			ARK::Core::SceneGraph::SceneNode("camera", SceneNode::TYPE_CAMERA),
			type(TYPE_ORTHO_2D),
			viewportX(0),
			viewportY(0),
			viewportWidth(1),
			viewportHeight(1),
			viewportAspectRatio(1.0f),
			viewportAutosize(true),
			viewportOrtho3dScale(1.5f),

			fieldOfView(45.0f),
			nearClip(0.001f),
			farClip(1000.0f),

			camera_heading(0),
			camera_pitch(0),

			clearColor(Color::black),

			lookAt(0.0f, 0.0f, 0.0f),
			direction(0.0f, 0.0f, 0.0f),
			up(0,1,0),
			rotation_quaternion(),

			projection(MatrixStack::TYPE_PROJECTION),
			view(MatrixStack::TYPE_VIEW),
			model(MatrixStack::TYPE_MODEL)
			//projection(MatrixStack::TYPE_PROJECTION),
			//view(MatrixStack::TYPE_VIEW),
			//model(MatrixStack::TYPE_MODEL)
		{
			up = Vector3<float>(0, 1, 0);
			fieldOfView = 45;
			rotation_quaternion = Quaternion<float>();
			// camera_position_delta = glm::vec3(0, 0, 0);
			// camera_scale = .5f;

			viewportOrtho3dScale = 1.5f;
			viewportAutosize = true;

			transform.position.x = 0.0f;
			transform.position.y = 0.0f;
			transform.position.z = 1.0f;
			lookAt.x = 0.0f;
			lookAt.y = 0.0f;
			lookAt.z = 0.0f;

			camera_heading = glm::pi<float>();

			nearClip = 0.001f;
			farClip = 1000.0f;

			if (current == NULL) {
				current = this;
			}


		}
		Camera::~Camera() {

		}

		void Camera::reset() {
			up.set(0, 1, 0);
		}

		void Camera::update() {
			SceneNode::update();

			if (viewportAutosize) {
				setViewport(0, 0, ARK2D::getContainer()->getDynamicWidth(), ARK2D::getContainer()->getDynamicHeight() );
			}

            direction = (lookAt - transform.position);
            direction.normalise();

			// need to set the matrix state. this is only important because lighting doesn't work if this isn't done

		    if (type == TYPE_ORTHO) {
		    	//our projection matrix will be an orthogonal one in this case
				//if the values are not floating point, this command does not work properly
				//need to multiply by aspect!!! (otherise will not scale properly)

				//projection = glm::ortho((scaleMaybe*-1) * float(viewportAspectRatio), scaleMaybe * float(viewportAspectRatio), scaleMaybe*-1, scaleMaybe, nearClip, farClip);
                projection.identity();
                projection.ortho2d((viewportOrtho3dScale*-1) * float(viewportAspectRatio), viewportOrtho3dScale * float(viewportAspectRatio), viewportOrtho3dScale*-1, viewportOrtho3dScale, nearClip, farClip);


					// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
				float verticalAngle = camera_pitch;
				float horizontalAngle = camera_heading;
				direction.set(
					cos(verticalAngle) * sin(horizontalAngle),
					sin(verticalAngle),
					cos(verticalAngle) * cos(horizontalAngle)
				);

                lookAt = transform.position + direction;

		    } else if (type == TYPE_ORTHO_2D) {

				//projection = glm::ortho(float(viewportX), float(viewportWidth), float(viewportHeight), float(viewportY), float(nearClip), float(farClip));
                projection.identity();
                projection.ortho2d(float(viewportX), float(viewportWidth), float(viewportHeight), float(viewportY), float(nearClip), float(farClip));

                lookAt = transform.position + direction;

			} else if (type == TYPE_PERSPECTIVE) {
				//projection = glm::perspective(fieldOfView, viewportAspectRatio, nearClip, farClip);
                projection.identity();
                projection.perspective(fieldOfView, viewportAspectRatio, nearClip, farClip);
				//detmine axis for pitch rotation
                Vector3<float> axis = direction.cross(up);
				//compute quaternion for pitch based on the camera pitch angle
                Quaternion<float> pitch_quat = Quaternion<float>::angleAxis(MathUtil::toDegrees(camera_pitch), axis);
				//determine heading quaternion from the camera up vector and the heading angle
                Quaternion<float> heading_quat = Quaternion<float>::angleAxis(MathUtil::toDegrees(camera_heading), up);
				//add the two quaternions
				Quaternion<float> temp = pitch_quat.cross(heading_quat);
                temp.normalise();

                // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
				float verticalAngle = camera_pitch;
				float horizontalAngle = camera_heading;
				direction.set(
					cos(verticalAngle) * sin(horizontalAngle),
					sin(verticalAngle),
					cos(verticalAngle) * cos(horizontalAngle)
				);

            	//set the look at to be infront of the camera
                lookAt = transform.position + direction;



			}






			// compute the MVP

			if (type == TYPE_ORTHO_2D) {

                //view.lookAt(transform.position.x, transform.position.y, transform.position.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z);

				//view = glm::translate(view, glm::vec3(container->getTranslateX(), container->getTranslateY(), 0.0f) );
				//view = glm::scale(view, glm::vec3(container->getScaleX(), container->getScaleY(), 0.0f) );
				GameContainer* container = ARK2D::getContainer();
				view.identity();
				view.lookAt(transform.position.x, transform.position.y, transform.position.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z);
                view.translate(container->getTranslateX(), container->getTranslateY(), 0.0f);
                // todo: rotate
                view.scale(container->getScaleX(), container->getScaleY(), 1.0f);

                model.identity(); // = glm::mat4(1.0f);

			} else if (type == TYPE_PERSPECTIVE || type == TYPE_ORTHO) {

				/*float scaleX = 1.0f / float(viewportWidth);
				float scaleY = 1.0f / float(viewportHeight);
				if (scaleX > scaleY) {
					scaleY = scaleX;
				} else {
					scaleX = scaleY;
				}
				view = glm::lookAt(position, lookAt, up);
				view = glm::scale(view, glm::vec3(scaleX, scaleY, 0.0) );
				view = glm::translate(view, glm::vec3(1280.0*-0.5, 720.0*-0.5, 0.0) );*/

//				view = glm::lookAt(position, lookAt, up);
                view.identity();
                view.lookAt(transform.position.x, transform.position.y, transform.position.z, lookAt.x, lookAt.y, lookAt.z, up.x, up.y, up.z);
//				model = glm::mat4(1.0f);
                model.identity();
			}

			//MVP = projection * view * model;

		}

		void Camera::rendererUpdate() {
			//Renderer* r = ARK2D::getRenderer();
			//r->matrixMode(MatrixStack::TYPE_PROJECTION);
			//r->loadIdentity();
			//r->multiplyMatrix(&projection.m_stack[projection.m_stackIndex]);
			//r->multiplyMatrix(&projection);

			//r->matrixMode(MatrixStack::TYPE_VIEW);
			//r->loadIdentity();
			//r->multiplyMatrix(&view.m_stack[view.m_stackIndex]);
			//r->multiplyMatrix(&view);
		}

		void Camera::render() {
			//if (!visible) { return; }

			//ARK2D::getLog()->e("camera render");
			Renderer* r = ARK2D::getRenderer();
			r->debugString("Camera::render");
			r->viewport(viewportX, viewportY, viewportWidth, viewportHeight);
			//r->clear(clearColor);

			//updateMatrices()

			/*r->matrixMode(MatrixStack::TYPE_PROJECTION);
			MatrixStack* projectionStack = r->getMatrix(MatrixStack::TYPE_PROJECTION);
				Matrix44<float>* mymat = projectionStack->current();
				mymat->col[0][0] = projection[0][0];
				mymat->col[0][1] = projection[0][1];
				mymat->col[0][2] = projection[0][2];
				mymat->col[0][3] = projection[0][3];
				mymat->col[1][0] = projection[1][0];
				mymat->col[1][1] = projection[1][1];
				mymat->col[1][2] = projection[1][2];
				mymat->col[1][3] = projection[1][3];
				mymat->col[2][0] = projection[2][0];
				mymat->col[2][1] = projection[2][1];
				mymat->col[2][2] = projection[2][2];
				mymat->col[2][3] = projection[2][3];
				mymat->col[3][0] = projection[3][0];
				mymat->col[3][1] = projection[3][1];
				mymat->col[3][2] = projection[3][2];
				mymat->col[3][3] = projection[3][3];

			r->matrixMode(MatrixStack::TYPE_VIEW);
			MatrixStack* viewStack = r->getMatrix(MatrixStack::TYPE_VIEW);
				mymat = viewStack->current();
				mymat->col[0][0] = view[0][0];
				mymat->col[0][1] = view[0][1];
				mymat->col[0][2] = view[0][2];
				mymat->col[0][3] = view[0][3];
				mymat->col[1][0] = view[1][0];
				mymat->col[1][1] = view[1][1];
				mymat->col[1][2] = view[1][2];
				mymat->col[1][3] = view[1][3];
				mymat->col[2][0] = view[2][0];
				mymat->col[2][1] = view[2][1];
				mymat->col[2][2] = view[2][2];
				mymat->col[2][3] = view[2][3];
				mymat->col[3][0] = view[3][0];
				mymat->col[3][1] = view[3][1];
				mymat->col[3][2] = view[3][2];
				mymat->col[3][3] = view[3][3];*/

			//GameContainer* c = ARK2D::getContainer();
			//viewStack->current()->translate(c->getTranslateX(), c->getTranslateY(), 0.0f);

			unsigned int mode = r->matrixMode();

			r->matrixMode(MatrixStack::TYPE_MODEL);
			r->loadIdentity();

			#if defined(ARK2D_RENDERER_OPENGL)
				if (type == TYPE_PERSPECTIVE) {
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LEQUAL);

					glEnable(GL_CULL_FACE); // glDisable(GL_CULL_FACE);
					glCullFace(GL_BACK);
					glFrontFace(GL_CCW);
				}
			#endif
			SceneNode::render();

			r->matrixMode(mode);
		}



		// Setting Functions
		void Camera::setType(unsigned int ty) {
			type = ty;
			up.set(0, 1, 0);
            rotation_quaternion.identity();
		}

		/*void Camera::setPosition(glm::vec3 pos) {
			position = pos;
		}
		void Camera::setPosition(Vector3<float> pos) {
			position.x = pos.getX();
			position.y = pos.getY();
			position.z = pos.getZ();
		}
		void Camera::movePosition(float x, float y, float z) {
			position.x += x;
			position.y += y;
			position.z += z;
		}*/


		void Camera::setLookAt(Vector3<float> pos) {
			lookAt.x = pos.getX();
			lookAt.y = pos.getY();
			lookAt.z = pos.getZ();
		}


		void Camera::setFOV(float fov) {
			fieldOfView = fov;
		}
		void Camera::setViewport(int loc_x, int loc_y, int width, int height) {
			viewportX = loc_x;
			viewportY = loc_y;
			viewportWidth = width;
			viewportHeight = height;
			//need to use doubles division here, it will not work otherwise and it is possible to get a zero aspect ratio with integer rounding
			viewportAspectRatio = double(width) / double(height);

		}
		void Camera::setClipping(float near_clip_distance, float far_clip_distance) {
			nearClip = near_clip_distance;
			farClip = far_clip_distance;
		}

		//void Camera::Move(CameraDirection dir) {
			/*if (type == TYPE_PERSPECTIVE) {
				switch (dir) {
					case UP:
						camera_position_delta += camera_up * camera_scale;
						break;
					case DOWN:
						camera_position_delta -= camera_up * camera_scale;
						break;
					case LEFT:
						camera_position_delta -= glm::cross(camera_direction, camera_up) * camera_scale;
						break;
					case RIGHT:
						camera_position_delta += glm::cross(camera_direction, camera_up) * camera_scale;
						break;
					case FORWARD:
						camera_position_delta += camera_direction * camera_scale;
						break;
					case BACK:
						camera_position_delta -= camera_direction * camera_scale;
						break;
				}
			}*/
		//}
		void Camera::setPitch(float degrees) {
			camera_pitch = degrees;
			clampPitch();
		}
		void Camera::changePitch(float degrees) {
			// Check bounds with the max pitch rate so that we aren't moving too fast
			camera_pitch += degrees;
			clampPitch();
		}
		void Camera::clampPitch() {
			// Check bounds for the camera pitch
			/*if (camera_pitch > 360.0f) {
				camera_pitch -= 360.0f;
			} else if (camera_pitch < -360.0f) {
				camera_pitch += 360.0f;
			}*/
			//camera_pitch = fmod(camera_pitch, 360.0f);
            float pid2 = (glm::pi<float>()/2) - 0.001f;;
            if (camera_pitch >= pid2) {
                camera_pitch = pid2;
            } else if ( camera_pitch <= pid2*-1){
                camera_pitch = pid2*-1;
            }
		}
		void Camera::setHeading(float degrees) {
			camera_heading = degrees;
			clampHeading();
		}
		void Camera::changeHeading(float degrees) {
			//This controls how the heading is changed if the camera is pointed straight up or down
			//The heading delta direction changes
			if ((camera_pitch > 90 && camera_pitch < 270) || (camera_pitch < -90 && camera_pitch > -270)) {
				camera_heading -= degrees;
			} else {
				camera_heading += degrees;
			}
			clampHeading();
		}
		void Camera::clampHeading() {
			//Check bounds for the camera heading
			/*if (camera_heading > 360.0f) {
				camera_heading -= 360.0f;
			} else if (camera_heading < -360.0f) {
				camera_heading += 360.0f;
			}*/
			camera_heading = fmod(camera_heading, 360.0f);
		}
		//void Camera::Move2D(int x, int y) {
			//compute the mouse delta from the previous mouse position
			/*glm::vec3 mouse_delta = mouse_position - glm::vec3(x, y, 0);
			//if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
			if (move_camera) {
				ChangeHeading(.08f * mouse_delta.x);
				ChangePitch(.08f * mouse_delta.y);
			}
			mouse_position = glm::vec3(x, y, 0);*/
		//}

		//void Camera::SetPos(int button, int state, int x, int y) {
			/*if (button == 3 && state == GLUT_DOWN) {
				camera_position_delta += camera_up * .05f;
			} else if (button == 4 && state == GLUT_DOWN) {
				camera_position_delta -= camera_up * .05f;
			} else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
				move_camera = true;
			} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
				move_camera = false;
			}
			mouse_position = glm::vec3(x, y, 0);*/
		//}

		unsigned int Camera::getType() {
			return type;
		}

		string Camera::toString() {
			string nl = "\r\n";
			string s = "{"; s += nl;
				s += "\"type\":"; s += Cast::toString<unsigned int>(type); s += ","; s += nl;
				s += "\"viewportX\":"; s += Cast::toString<int>(viewportX); s += ","; s += nl;
				s += "\"viewportY\":"; s += Cast::toString<int>(viewportY); s += ","; s += nl;
				s += "\"viewportWidth\":"; s += Cast::toString<int>(viewportWidth); s += ","; s += nl;
				s += "\"viewportHeight\":"; s += Cast::toString<int>(viewportHeight); s += ","; s += nl;
				s += "\"viewportAspectRatio\":"; s += Cast::toString<float>(viewportAspectRatio); s += ","; s += nl;
				s += "\"viewportAutosize\":"; s += Cast::toString<int>(viewportAutosize); s += ","; s += nl;
				s += "\"fieldOfView\":"; s += Cast::toString<float>(fieldOfView); s += ","; s += nl;
				s += "\"nearClip\":"; s += Cast::toString<float>(nearClip); s += ","; s += nl;
				s += "\"farClip\":"; s += Cast::toString<float>(farClip); s += ","; s += nl;
				s += "\"camera_heading\":"; s += Cast::toString<float>(camera_heading); s += ","; s += nl;
				s += "\"camera_pitch\":"; s += Cast::toString<float>(camera_pitch); s += ","; s += nl;

			s += "}"; s += nl;
			return s;
		}

		/*void Camera::GetViewport(int &loc_x, int &loc_y, int &width, int &height) {
			loc_x = viewportX;
			loc_y = viewportY;
			width = viewportWidth;
			height = viewportHeight;
		}

		void Camera::GetMatrices(glm::mat4 &P, glm::mat4 &V, glm::mat4 &M) {
			P = projection;
			V = view;
			M = model;
		}*/

		Vector3<float> Camera::worldToScreenPoint(const Vector3<float>& p) {

			// http://webglfactory.blogspot.co.uk/2011/05/how-to-convert-world-to-screen.html
			Vector4 point3d = Vector4(p.getX(), p.getY(), p.getZ(), 0);
			if (type == TYPE_ORTHO_2D) {
				point3d.x -= viewportWidth * 0.5f;
				point3d.y -= viewportHeight * 0.5f;
			}
			Matrix44 viewProjectionMatrix = projection.m_stack[projection.m_stackIndex] * view.m_stack[view.m_stackIndex];
			//Matrix44<float> viewProjectionMatrix = *projection.current() * *view.current();

			// transform world to clipping coordinates
			point3d = viewProjectionMatrix * point3d;

			// we calculate -point3D.getY() because the screen Y axis is oriented top->down
			float newX = round( (( point3d.x + 1 ) / 2.0) * viewportWidth );
			float newY = round( (( 1 - point3d.y ) / 2.0) * viewportHeight );

			return Vector3<float>(newX, newY, 0.0f);
		//	return Vector3<float>(0,0,0);
		}
		Vector3<float> Camera::screenToWorldPoint(const Vector3<float>& p) {

			double x = p.x;
			double y = p.y;
			//if (type == TYPE_ORTHO || type == TYPE_PERSPECTIVE) {
				x = 2.0 * p.getX() / viewportWidth - 1;
				y = -2.0 * p.getY() / viewportHeight + 1;
			//} else if (type == TYPE_ORTHO_2D) {
			//	x = p.getX() / viewportWidth;
			//	y = -p.getY() / viewportHeight;
			//}
			//Matrix44<float> viewProjection = *projection.current() * *view.current();
			Matrix44 viewProjection = projection.m_stack[projection.m_stackIndex] * view.m_stack[view.m_stackIndex];
            viewProjection.inverse();

            Vector4 point4d(x, y, 0, 0);
            point4d = viewProjection * point4d;
			Vector3<float> ret(point4d.x, point4d.y, 0.0f);
			if (type == TYPE_ORTHO_2D) {
				ret.x += viewportWidth * 0.5f;
				ret.y += viewportHeight * 0.5f;
			}
			return ret;
			//return Vector3<float>(0,0,0);
		}

		void Camera::rotate(Vector3<float>* axis, float angle) {
			/*if (axis->m_x != lookAt._x || axis->m_y != lookAt.m_y || axis->m_z != lookAt.m_z) {
				MathUtil::rotateVectorAroundAxis(&lookAt, axis, angle);
			}
			if (axis->m_x != right.m_x || axis->m_y != right.m_y || axis->m_z != right.m_z) {
				MathUtil::rotateVectorAroundAxis(&right, axis, angle);
			}
			if (axis->m_x != up.m_x || axis->m_y != up.m_y || axis->m_z != up.m_z) {
				MathUtil::rotateVectorAroundAxis(&up, axis, angle);
			}*/
		}



		// CAMERA MOVEMENT
		CameraMovement::CameraMovement():
			camera(NULL),
			moveSpeed(4.0f),
			rotateSpeed(2.5f)
			{

		}
		CameraMovement::CameraMovement(Camera* c):
			camera(c),
			moveSpeed(4.0f),
			rotateSpeed(2.5f)
			{

		}
		void CameraMovement::update() {
			GameTimer* timer = ARK2D::getTimer();
            Input* in = ARK2D::getInput();

			// movement on wasd
			float d = moveSpeed * timer->getDelta();
		 	if (in->isKeyDown(Input::KEY_W)) {
				camera->transform.position += camera->direction * d;
		    }
			if (in->isKeyDown(Input::KEY_S)) {
				camera->transform.position -= camera->direction * d;
			}
			if (in->isKeyDown(Input::KEY_A)) {
				Vector3<float> right = camera->direction.cross(camera->up).normalise();
				camera->transform.position -= right * d;
				camera->lookAt -= right * d;
			}
			if (in->isKeyDown(Input::KEY_D)) {
				Vector3<float> right = camera->direction.cross(camera->up).normalise();
				camera->transform.position += right * d;
				camera->lookAt += right * d;
			}


			// up down
			if (in->isKeyDown(Input::KEY_SPACE)) {
				camera->transform.position += camera->up * d;
				camera->lookAt += camera->up * d;

			} else if (in->isKeyDown(Input::KEY_V)) {
				camera->transform.position -= camera->up * d;
		        camera->lookAt -= camera->up * d;
			}

			// rotation on arrow keys
			float r = rotateSpeed * timer->getDelta();
			if (in->isKeyDown(Input::KEY_LEFT)) {
				camera->setHeading(camera->camera_heading + r);
			} else if (in->isKeyDown(Input::KEY_RIGHT)) {
				camera->setHeading(camera->camera_heading - r);
			}
			if (in->isKeyDown(Input::KEY_UP)) {
				camera->setPitch(camera->camera_pitch + r);
			} else if (in->isKeyDown(Input::KEY_DOWN)) {
				camera->setPitch(camera->camera_pitch - r);
			}

			// zoom
			if (in->isKeyDown(Input::KEY_SHIFT) && in->isKeyDown(Input::KEY_EQUALS)) {
				camera->fieldOfView -= 1.0f * timer->getDelta();
			} else if (in->isKeyDown(Input::KEY_SHIFT) && in->isKeyDown(Input::KEY_HYPHEN)) {
				camera->fieldOfView += 1.0f * timer->getDelta();
			}
		}

		// DUMMY CAMERA THAT DOES NOTHING
		DummyCamera::DummyCamera(): Camera() {

		}
		void DummyCamera::update() {

		}
		void DummyCamera::render() {

		}

	}

}



