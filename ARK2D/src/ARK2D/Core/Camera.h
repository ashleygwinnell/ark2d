#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtx/transform.hpp"
#include "../vendor/glm/gtc/quaternion.hpp"
#include "../vendor/glm/gtx/quaternion.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"

#include "../Geometry/Vector3.h"
#include "../Graphics/Color.h"
#include "../SceneGraph/Scene.h"

#include "../Namespaces.h"

#ifndef ARK2D_CORE_CAMERA_H_
#define ARK2D_CORE_CAMERA_H_

namespace ARK {
	namespace Core {

		enum CameraDirection {
			UP, DOWN, LEFT, RIGHT, FORWARD, BACK
		};

		class ARK2D_API Camera : public SceneNode {
			public:
				static Camera* current;

			public:
				static const unsigned int TYPE_ORTHO = 0;
				static const unsigned int TYPE_ORTHO_2D = 1;
				static const unsigned int TYPE_PERSPECTIVE = 2;

			public:
				unsigned int type;

				int viewportX;
				int viewportY;
				int viewportWidth;
				int viewportHeight;
				float viewportAspectRatio;
				bool viewportAutosize;

				float fieldOfView;
				float nearClip;
				float farClip;

				float camera_heading;
				float camera_pitch;

				Color clearColor;

				Vector3<float> lookAt;
				Vector3<float> direction;
				
				Vector3<float> up;
				Quaternion<float> rotation_quaternion;
				
				Matrix44<float> projection;
				Matrix44<float> view;
				Matrix44<float> model;
				Matrix44<float> MVP;

			public:
				Camera();
				
				// Given a specific moving direction, the camera will be moved in the appropriate direction
				// For a spherical camera this will be around the look_at point
				// For a free camera a delta will be computed for the direction of movement.
				// void Move(CameraDirection dir);

				// Change the pitch (up, down) for the free camera
				void setPitch(float degrees);
				void changePitch(float degrees);
				void clampPitch();

				// Change heading (left, right) for the free camera
				void setHeading(float degrees);
				void changeHeading(float degrees);
				void clampHeading();

				// Change the heading and pitch of the camera based on the 2d movement of the mouse
				//void Move2D(int x, int y);

				// Changes the camera mode, only three valid modes, Ortho, Free, and Spherical
				void setType(unsigned int ty);
				unsigned int getType();
				
				// Set the position of the camera
				/*void setPosition(glm::vec3 pos);
				void setPosition(Vector3<float> pos);
				void movePosition(float x, float y, float z);*/

				// Set's the look at point for the camera
				//void setLookAt(glm::vec3 pos);
				void setLookAt(Vector3<float> pos);

				// Changes the Field of View (FOV) for the camera
				void setFOV(float fov);
				
				// Change the viewport location and size
				void setViewport(int loc_x, int loc_y, int width, int height);
				
				// Change the clipping distance for the camera
				void setClipping(float near_clip_distance, float far_clip_distance);

				// todo 
				void setDistance(float cam_dist);
				

				Vector3<float> worldToScreenPoint(const Vector3<float>& p);
				Vector3<float> screenToWorldPoint(const Vector3<float>& p);
				
				//This function updates the camera
				//Depending on the current camera mode, the projection and viewport matricies are computed
				//Then the position and location of the camera is updated
				virtual void update();

				virtual void render();
				
				void reset();
				string toString();

				virtual ~Camera();

			private:
				void rotate(Vector3<float>* axis, float angle);

		};

		class ARK2D_API CameraMovement {
			public:
				Camera* camera;
				float moveSpeed;
				float rotateSpeed;
			public:
				CameraMovement();
				CameraMovement(Camera* c);
				void update();
		};

	}
}
#endif