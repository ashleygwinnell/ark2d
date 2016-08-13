
#ifndef ARK_SPINE_SKELETON_H_
#define ARK_SPINE_SKELETON_H_


//#include "../../Includes.h"
#include "../../Common/DLL.h"
#include "../../Core/Resource.h"
#include "includes/spine/spine.h"

/**
 * ARK2D Spine API Modifications.
 *
 *  - Add "void* ark2dskeleton" to spAnimationState in AnimationState.h
 *
 */

namespace ARK {
	namespace Spine {

		class SkeletalAnimationMixer;
		class Skeleton;

		class SpineCallback {
			public:
				unsigned int type;
				void* eventObject;
				void* eventFunction;
				const char* animationName;
				unsigned int trackIndex;
		};

		class ARK2D_API SpineUtil {
			public:
				static bool s_usingDummyTexture;
			public:
				static void transformSceneNodeFromBoneName(SceneNode* node, Skeleton* skeleton, string boneName);
				static void transformFromBoneName(float* posX, float* posY, float* scaleX, float* scaleY, float* rotation, Skeleton* skeleton, string boneName);
				static void setUsingDummyTexture(bool b);
		};

		class ARK2D_API Skeleton : public ARK::Core::Resource 
		{
			public:
				static float s_defaultScale;
				static void setDefaultScale(float f);

			public:
				string m_fname;
				void* m_fdata_atlas;
				unsigned int m_fdatalen_atlas;
				void* m_fdata_json;
				unsigned int m_fdatalen_json;

				spAtlas* m_atlas;
				spSkeleton* m_skeleton;
				spSkeletonData* m_data;
				spSkeletonJson* m_json;
				spBone* m_root;
				string m_rootName;
				map<string, spAnimationState*> m_animationStates;
				string m_animationStateCurrent;
				float m_delta;
				float m_scaleX;
				float m_scaleY;

				float m_x;
				float m_y;
				float m_z;

				vector<SpineCallback*> m_callbacks;
				bool m_invokingCallbacks;
				static const unsigned int CALLBACK_ANIMATION_START = 0;
				static const unsigned int CALLBACK_ANIMATION_END = 1;
				static const unsigned int CALLBACK_ANIMATION_COMPLETE = 2;
				static const unsigned int CALLBACK_ANIMATION_EVENT = 3;
				static const unsigned int CALLBACK_PARAMETERISED_EVENT = 4;

				// callback pointer used for sound events. not sure why this is going here.
				// void soundEventCallbackTestFunction(const char* name, std::map<string, float> params);
				static void* s_soundEventCallback;

				bool m_copy;

			public:
				Skeleton(string fname);
				Skeleton(void* atlasData, unsigned int atlasLength, void* jsonData, unsigned int jsonLength);
				
				void load();
				void reset();
				void clearCallbacks();
				void clearAnimationMixers();
				
				SkeletalAnimationMixer* newAnimation();
				void addAnimation(string key, SkeletalAnimationMixer* data);
				void mixAnimation(string key, string subkey, bool loop);
				void playAnimation(string key, string subkey, bool loop);
				void queueAnimation(string key, string subkey, bool loop, float delay=0.0f);
				void clearAnimationQueue(string key);
				string getCurrentAnimationName(string key);
				bool hasAnimation(string key);

				spAnimation* findAnimation(string key);
				spSkeleton* getSkeleton();
				spSkeletonData* getData();

				bool hasSlot(string name);
				bool setAttachment(string slot, string attachment);
				bool removeAttachment(string slot);

				void setSkin(string name);
				
				void setInvokingCallbacks(bool b);
				
				spBone* getRoot();
				void setRoot(string name);

				spBone* getBone(string name);
				spSlot* getSlot(string name);
				void getSlotCenter(string name, float& x, float& y);

				float getX();
				float getY();
				float getZ();
				
				void setY(float y);
				void setLocation(float x, float y);
				void setLocation(float x, float y, float z);
				//void setLocation(float x, float y, string bone);
				void setScale(float x, float y);

				void update();
				void update(float multiplier);
				void render();

				void addCallback(unsigned int type, void* obj, void* function);
				void doCallback(spEventType type, int trackIndex, const char* animationName);
				void doCallbackInternal(SpineCallback* cb);

				Skeleton* copy();

				virtual ~Skeleton();
		};

		class ARK2D_API SkeletalAnimationMixer {
			public:
				Skeleton* m_skeleton;
				spAnimationStateData* m_stateData;

			public:
				SkeletalAnimationMixer(Skeleton* sk);
				void setMixByName(string from, string to, float mix);
				~SkeletalAnimationMixer();
		};


	}
}

#endif /* ARK_SPINE_SKELETON_H_ */
