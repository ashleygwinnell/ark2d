
#include "SpineSkeleton.h"

#include "../../Core/ARK2D.h"
#include "../../Util/FileUtil.h"
#include "../../Core/GameContainer.h"
#include "../../Core/GameTimer.h"
#include "../../Core/Graphics/Texture.h"

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 512
#endif

namespace ARK {
	namespace Spine {

		void* Skeleton::s_soundEventCallback = NULL;

		bool SpineUtil::s_usingDummyTexture = false;
		void SpineUtil::setUsingDummyTexture(bool b) {
			s_usingDummyTexture = b;
		}

		void SpineUtil::transformSceneNodeFromBoneName(SceneNode* node, Skeleton* skeleton, string boneName) {
			spBone* bone = skeleton->getBone(boneName);

			float thisX = bone->x;
			float thisY = bone->y*-1.0f;
			float thisRotation = bone->rotation*-1;
			if (bone->parent && bone->data->inheritRotation && bone->parent == skeleton->getRoot()) {
				thisX += skeleton->getX();
				thisY += skeleton->getY();
				thisRotation += 90;
			}
			thisRotation = MathUtil::absangle<double>(thisRotation);
 
			node->transform.position.set(thisX, thisY);
			node->transform.scale.set(bone->scaleY, bone->scaleX);
            node->transform.rotation = Quaternion<float>::angleAxis(thisRotation, 0,0,1);
		}
		void SpineUtil::transformFromBoneName(float* posX, float* posY, float* scaleX, float* scaleY, float* rotation, Skeleton* skeleton, string boneName) {
			spBone* bone = skeleton->getBone(boneName);

			float thisX = bone->x;
			float thisY = bone->y*-1.0f;
			float thisRotation = bone->rotation*-1;
			if (bone->parent && bone->data->inheritRotation && bone->parent == skeleton->getRoot()) {
				thisX += skeleton->getX();
				thisY += skeleton->getY();
				thisRotation += 90;
			}

			(*posX) = thisX;
			(*posY) = thisY;
			(*scaleX) = bone->scaleY;
			(*scaleY) = bone->scaleX;
			(*rotation) = thisRotation;
		}

		void spineCallback(spAnimationState* state, int trackIndex, spEventType type, spEvent* event, int loopCount) 
		{
			spTrackEntry* entry = spAnimationState_getCurrent(state, trackIndex);
			const char* animationName = (entry && entry->animation) ? entry->animation->name : 0;

			switch (type) {
				case SP_ANIMATION_START: {
					Skeleton* sk = (Skeleton*) state->ark2dskeleton;
					sk->doCallback(type, trackIndex, animationName);

					//printf("%d start: %s\n", trackIndex, animationName);
					break;
				}
				case SP_ANIMATION_END: {
					Skeleton* sk2 = (Skeleton*) state->ark2dskeleton;
					sk2->doCallback(type, trackIndex, animationName);

					//printf("%d end: %s\n", trackIndex, animationName);
					break;
				}
				case SP_ANIMATION_COMPLETE: {
					Skeleton* sk3 = (Skeleton*) state->ark2dskeleton;
					sk3->doCallback(type, trackIndex, animationName);

					//printf("%d complete: %s, %d\n", trackIndex, animationName, loopCount);
					break;
				}
				case SP_ANIMATION_EVENT: {
					string s = string(event->data->name);
					if (Skeleton::s_soundEventCallback != NULL && s.substr(0, 6) == "sound_" && s.length() > 6) {
						
						string soundEventName = s.substr(6);
						
						const char* soundEventNameCStr = soundEventName.c_str();
						map<string, float> params;

						if (event->stringValue != NULL) { 
							string soundEventData = string(event->stringValue);
							if (soundEventData.length() > 0) {
								ARK2D::getLog()->e("We have string event data.");

								vector<string> items = StringUtil::split(soundEventData, ",");
								
								if (items.size() > 0) {
									for (unsigned int i = 0; i < items.size(); ++i) {
										vector<string> val = StringUtil::split(items[i], "=");

										if (val[1] == "true") {
											params[val[0]] = 1.0f;
										} else {
											params[val[0]] = Cast::fromString<float>(val[1]);
										}
									}
								} else {
									vector<string> val = StringUtil::split(soundEventData, "=");
									if (val[1] == "true") {
										params[val[0]] = 1.0f;
									} else {
										params[val[0]] = Cast::fromString<float>(val[1]);
									}
								}

								
								
							} else {
								ARK2D::getLog()->e("We had string event data but it was empty.");
							}
						}
						
						void (*pt)(const char*, std::map<string, float>) = (void(*)(const char*, std::map<string, float>)) Skeleton::s_soundEventCallback;
						pt(soundEventNameCStr, params);

					} else { 
						Skeleton* sk4 = (Skeleton*) state->ark2dskeleton;
						sk4->doCallback(type, 0, event->data->name);
						printf("%d event: %s, %s: %d, %f, %s\n", trackIndex, animationName, event->data->name, event->intValue, event->floatValue, event->stringValue);
					}
					break;
				}
			}
			fflush(stdout);
		}


		SkeletalAnimationMixer::SkeletalAnimationMixer(Skeleton* sk):
			m_skeleton(sk),
			m_stateData(NULL)
		{
			m_stateData = spAnimationStateData_create(sk->getData());
		}
		void SkeletalAnimationMixer::setMixByName(string from, string to, float mix) {
			spAnimationStateData_setMixByName(m_stateData, from.c_str(), to.c_str(), mix);
		}
		SkeletalAnimationMixer::~SkeletalAnimationMixer() {
			// delete what? 
			//spAnimationStateData_dispose(m_stateData);
			//m_stateData = NULL;
		}


		float Skeleton::s_defaultScale = 1.0f;
		void Skeleton::setDefaultScale(float f) {
			s_defaultScale = f;
		}
		 
		Skeleton::Skeleton(string fname):
			ARK::Core::Resource(),

			m_fname(fname),
			m_fdata_atlas(NULL),
			m_fdatalen_atlas(0),
			m_fdata_json(NULL),
			m_fdatalen_json(0),

			m_atlas(NULL),
			m_skeleton(NULL),
			m_data(NULL),
			m_json(NULL),
			m_root(NULL),
			m_rootName(""),
			m_animationStates(),
			m_delta(0.0f),
			m_scaleX(1.0f),
			m_scaleY(1.0f),
			m_x(0.0f),
			m_y(0.0f),
			m_z(0.0f),
			m_callbacks(),
			m_invokingCallbacks(true),
			m_copy(false)
		{
			//load();
		}

		Skeleton::Skeleton(void* atlasData, unsigned int atlasLength, void* jsonData, unsigned int jsonLength):
			ARK::Core::Resource(),

			m_fname(""),
			m_fdata_atlas(atlasData),
			m_fdatalen_atlas(atlasLength),
			m_fdata_json(jsonData),
			m_fdatalen_json(jsonLength),
			
			m_atlas(NULL),
			m_skeleton(NULL),
			m_data(NULL),
			m_json(NULL),
			m_root(NULL),
			m_rootName(""),
			m_animationStates(),
			m_delta(0.0f),
			m_scaleX(1.0f),
			m_scaleY(1.0f),
			m_x(0.0f),
			m_y(0.0f),
			m_z(0.0f),
			m_callbacks(),
			m_invokingCallbacks(false),
			m_copy(false)
		{ 
			//load();
		}

		Skeleton* Skeleton::copy() {
			Skeleton* sk = new Skeleton(m_fname);
			
			/*int atlasSz = sizeof(m_atlas);
			sk->m_atlas = malloc(atlasSz);
			memcpy(m_atlas, sk->m_atlas, atlasSz);

			int jsonSz = sizeof(m_json);
			sk->m_json = malloc(jsonSz);
			memcpy(m_json, sk->m_json, jsonSz);

			int dataSz = sizeof(m_data);
			sk->m_data = malloc(dataSz);
			memcpy(m_data, sk->m_data, dataSz);*/

			// TODO: deep copy. 
			sk->m_fdata_atlas = m_fdata_atlas;
			sk->m_fdatalen_atlas = m_fdatalen_atlas;
			sk->m_fdata_json = m_fdata_json;
			sk->m_fdatalen_json = m_fdatalen_json;

			sk->m_atlas = m_atlas;
			sk->m_json = m_json;
			sk->m_data = m_data;

			sk->m_skeleton = spSkeleton_create(sk->m_data);
			spSkeleton_setToSetupPose(sk->m_skeleton);

			sk->setRoot(m_rootName);
			sk->m_delta = m_delta;
			sk->m_scaleX = m_scaleX;
			sk->m_scaleY = m_scaleY;
			sk->m_x = m_x;
			sk->m_y = m_y;
			sk->m_z = m_z;
			sk->m_invokingCallbacks = m_invokingCallbacks;
			sk->m_copy = true;

			return sk;
			
		}

		void Skeleton::load() {
			ARK2D::getLog()->v("Skeleton::load()"); 
			spBone_setYDown(1);

			string genericName = m_fname.substr(0, m_fname.find_last_of("."));

			string atlasFile = genericName + string(".atlas");
			//ARK2D::getLog()->v(StringUtil::append("atlas name was: ", atlasFile.c_str()));

			string skeletonFile = genericName + string(".json");
			//ARK2D::getLog()->v(StringUtil::append("skeleton name was: ", skeletonFile.c_str()));
 
			// read from files.
			if (m_fdata_atlas == NULL || m_fdata_json == NULL) {

				ARK2D::getLog()->v("Creating Skeleton from file...");
				m_atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
				if (m_atlas == NULL) { 
					ARK2D::getLog()->w("spAtlas was null. maybe it's not meant to be?");
					m_json = spSkeletonJson_create(m_atlas);
				} else { 
					m_json = spSkeletonJson_create(m_atlas);
					m_json->scale = s_defaultScale;
				}

				m_data = spSkeletonJson_readSkeletonDataFile(m_json, skeletonFile.c_str());
				if (!m_data) { printf("Error: %s\n", m_json->error); exit(0); }
			} 
			// read from pointers / preloaded files.
			else {
				ARK2D::getLog()->i("Creating Skeleton from pointer...");
				ARK2D::getLog()->i("NEW NEW NEW NEW NEW");
				string prependHere = atlasFile.substr(0, atlasFile.find_last_of("/"));

				m_atlas = spAtlas_create((const char*) m_fdata_atlas, m_fdatalen_atlas, prependHere.c_str(), 0);
				if (m_atlas == NULL) { printf("spAtlas was null"); exit(0); }

				m_json = spSkeletonJson_create(m_atlas);
				m_json->scale = s_defaultScale;

				m_data = spSkeletonJson_readSkeletonData(m_json, (const char*) m_fdata_json);
				if (!m_data) { printf("Error: %s\n", m_json->error); exit(0); }
			}

			//printf("First region name: %s, x: %d, y: %d\n", m_skeletonAtlas->regions->name, m_skeletonAtlas->regions->x, m_skeletonAtlas->regions->y);
			//printf("First page name: %s, size: %d, %d\n", m_skeletonAtlas->pages->name, m_skeletonAtlas->pages->width, m_skeletonAtlas->pages->height);

			//printf("Default skin name: %s\n", m_data->defaultSkin->name);

			m_skeleton = spSkeleton_create(m_data);
			spSkeleton_setToSetupPose(m_skeleton);



			
			
		}
		void Skeleton::clearCallbacks() {
			m_callbacks.clear();
		}
		void Skeleton::clearAnimationMixers() {
			m_animationStates.clear();
		}

		void Skeleton::reset() {
			m_skeleton->time = 0.0f;
			spSkeleton_setToSetupPose(m_skeleton);

		}

		void Skeleton::addCallback(unsigned int type, void* obj, void* function) {
			SpineCallback* cb = new SpineCallback();
			cb->type = type;
			cb->eventObject = obj;
			cb->eventFunction = function;
			m_callbacks.push_back(cb);
		}
		void Skeleton::doCallback(spEventType type, int trackIndex, const char* animationName) {
			
			for(unsigned int i = 0; i < m_callbacks.size(); ++i) 
			{
				SpineCallback* cb = m_callbacks[i];
				if (
					(type == SP_ANIMATION_START && cb->type == CALLBACK_ANIMATION_START) || 
					(type == SP_ANIMATION_END && cb->type == CALLBACK_ANIMATION_END) || 
					(type == SP_ANIMATION_COMPLETE && cb->type == CALLBACK_ANIMATION_COMPLETE) || 
					(type == SP_ANIMATION_EVENT && cb->type == CALLBACK_ANIMATION_EVENT)
				) {
					cb->animationName = animationName;
					cb->trackIndex = trackIndex;
					doCallbackInternal(cb);
				}
			}

		}
		void Skeleton::doCallbackInternal(SpineCallback* cb) {
			if (cb->eventFunction != NULL) {
				if (cb->eventObject == NULL) {
					
					void (*pt)(SpineCallback*) = (void(*)(SpineCallback*)) cb->eventFunction;
					//typedef void fnct();
					//fnct* pt = (fnct*) m_event;
					pt(cb);
				} else {
					void (*pt)(SpineCallback*, void*) = (void(*)(SpineCallback*, void*)) cb->eventFunction;
					pt(cb, cb->eventObject);
				}
			}
		}

		void Skeleton::update() {
			update(1.0f);
		}
		void Skeleton::update(float multiplier) {
			m_root->scaleX = m_scaleX;
			m_root->scaleY = m_scaleY;

			float thisDelta = ARK2D::getContainer()->getTimer()->getDelta() * multiplier;
			spSkeleton_update(m_skeleton, thisDelta);
			spSkeleton_updateWorldTransform(m_skeleton);

			// update current animation
			//spAnimationState_update
			spAnimationState* as = m_animationStates[m_animationStateCurrent];
			if (as != NULL) {
				spAnimationState_update(as, thisDelta);
				spAnimationState_apply(as, m_skeleton);
			}
			//spAnimation_apply(m_animation, m_skeleton, m_delta, m_delta + thisDelta, loop, NULL, 0);

			m_delta += thisDelta;
		}
		
		SkeletalAnimationMixer* Skeleton::newAnimation() {
			return new SkeletalAnimationMixer(this);
		}

		void Skeleton::addAnimation(string key, SkeletalAnimationMixer* stateData) {
			spAnimationState* as = spAnimationState_create(stateData->m_stateData);
			if (as == NULL) { ErrorDialog::createAndShow("Could not create Spine Skeleton (addAnimation, spAnimationState)."); exit(0); }
			if (m_invokingCallbacks) {
				as->ark2dskeleton = this;
				as->listener = spineCallback;
			}
			
			m_animationStates[key] = as;
			delete stateData;
		}
		void Skeleton::playAnimation(string key, string subkey, bool loop) {
			spAnimationState* as = NULL;
			map<string, spAnimationState*>::iterator it = m_animationStates.find(key);
			if (it != m_animationStates.end()) { as = it->second; }
			if (as == NULL) { ErrorDialog::createAndShow("Could not create Spine Skeleton (playAnimation, spAnimationState)."); exit(0); }

			int loop2 = (loop)?1:0;
			m_animationStateCurrent = key;
			spAnimationState_setAnimationByName(as, 0, subkey.c_str(), loop2);
		}
		void Skeleton::queueAnimation(string key, string subkey, bool loop, float delay) {
			spAnimationState* as = NULL;
			map<string, spAnimationState*>::iterator it = m_animationStates.find(key); 
			if (it != m_animationStates.end()) { as = it->second; }
			if (as == NULL) { ErrorDialog::createAndShow("Could not create Spine Skeleton (queueAnimation, spAnimationState)."); exit(0); }
			
			spAnimationState_addAnimationByName(as, 0, subkey.c_str(), loop, delay);
		}
		void Skeleton::clearAnimationQueue(string key) {
			spAnimationState* as = NULL;
			map<string, spAnimationState*>::iterator it = m_animationStates.find(key); 
			if (it != m_animationStates.end()) { as = it->second; }
			if (as == NULL) { ErrorDialog::createAndShow("Could not clear Spine Skeleton animation queue Spine Skeleton."); exit(0); }

			//spAnimationState_clearTrack(as, 0);
			spAnimationState_clearTracks(as);
		}
		string Skeleton::getCurrentAnimationName(string key) {
			spAnimationState* as = NULL;
			map<string, spAnimationState*>::iterator it = m_animationStates.find(key);
			if (it != m_animationStates.end()) { as = it->second; }
			if (as == NULL) { ErrorDialog::createAndShow("Could not create Spine Skeleton (playAnimation, spAnimationState)."); exit(0); }

			spTrackEntry* entry = spAnimationState_getCurrent(as, 0);
			if (entry != NULL) { 
				return entry->animation->name;
			}
			return "";
		}
		
		bool Skeleton::hasAnimation(string key) {
			return (m_animationStates.find(key) != m_animationStates.end());
		}

		spAnimation* Skeleton::findAnimation(string key) {
			return spSkeletonData_findAnimation(m_data, key.c_str());
		}
		spSkeleton* Skeleton::getSkeleton() {
			return m_skeleton;
		}
		spSkeletonData* Skeleton::getData() {
			return m_data;
		}
		bool Skeleton::hasSlot(string name) {
			return (spSkeleton_findSlot(m_skeleton, name.c_str()) == 0)?false:true;
		}
		bool Skeleton::setAttachment(string slot, string attachment) {
			int success = spSkeleton_setAttachment (m_skeleton, slot.c_str(), attachment.c_str());
			return (success == 0)?false:true;
		}
		bool Skeleton::removeAttachment(string slot) {
			int success = spSkeleton_setAttachment (m_skeleton, slot.c_str(), 0);
			return (success == 0)?false:true;
		}
		spBone* Skeleton::getRoot() {
			return m_root;
		}
		void Skeleton::setRoot(string name) {
			m_root = spSkeleton_findBone(m_skeleton, name.c_str());
			if (m_root == NULL) { 
				ErrorDialog::createAndShow("Could not set Spine Skeleton root."); 
				exit(0); 
			}
			m_rootName = name;
		}

		spBone* Skeleton::getBone(string name) {
			return spSkeleton_findBone(m_skeleton, name.c_str());
		}

		spSlot* Skeleton::getSlot(string name) {
			return spSkeleton_findSlot(m_skeleton, name.c_str());
		}
		void Skeleton::getSlotCenter(string name, float& x, float& y) {
			spSlot* slot = spSkeleton_findSlot(m_skeleton, name.c_str());
			float cx = slot->bone->x;
			float cy = slot->bone->y * -1;

			// get center y of this object.
			float worldVertices[512];
			spAttachment* attachment = slot->attachment;
			if (!attachment) { ARK2D::getLog()->w("Attachment was null."); return; } 

			if (attachment->type == SP_ATTACHMENT_REGION) {
				spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
				spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

				cx = ((worldVertices[SP_VERTEX_X1] + worldVertices[SP_VERTEX_X2] + worldVertices[SP_VERTEX_X3] + worldVertices[SP_VERTEX_X4]) / 4);
				cy = ((worldVertices[SP_VERTEX_Y1] + worldVertices[SP_VERTEX_Y2] + worldVertices[SP_VERTEX_Y3] + worldVertices[SP_VERTEX_Y4]) / 4);
		 
			} /*else if (attachment->type == SP_ATTACHMENT_MESH) {
				spMeshAttachment* mesh = (spMeshAttachment*) attachment;
				if (mesh->uvsCount > SPINE_MESH_VERTEX_COUNT_MAX) { continue; }
				spMeshAttachment_computeWorldVertices(mesh, slot->skeleton->x, slot->skeleton->y, slot, worldVertices);
			} else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH) {
				spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*) attachment;
				if (mesh->uvsCount > SPINE_MESH_VERTEX_COUNT_MAX) continue;
				spSkinnedMeshAttachment_computeWorldVertices(mesh, slot->skeleton->x, slot->skeleton->y, slot, worldVertices);
			}*/
			x = cx;
			y = cy;
		}


		void Skeleton::setSkin(string name) {
			

			int ret = spSkeleton_setSkinByName(m_skeleton, name.c_str());
			if (ret == 0) {
				string errstr1 = StringUtil::append(m_fname, " , ");
				string errStr = StringUtil::append(errstr1, name);
				ErrorDialog::createAndShow(StringUtil::append("Could not set Spine Skeleton skin: ", errStr)); 
				//exit(0); 
			}

			//spSkeleton_setToSetupPose(m_skeleton);
			spSkeleton_setSlotsToSetupPose(m_skeleton);
			setLocation(m_x, m_y);
			
		}
		void Skeleton::setInvokingCallbacks(bool b) { 
			m_invokingCallbacks = b; 
		}

		/*void Skeleton::setLocation(float x, float y, string bone) {
			m_x = x;
			m_y = y;

			spBone* bo = spSkeleton_findBone(m_skeleton, bone.c_str());
			bo->x = x;
			bo->y = y * -1;

			spSkeleton_updateWorldTransform(m_skeleton);
		}*/

		void Skeleton::setY(float y) {
			m_y = y;
			m_skeleton->y = y;
			spSkeleton_updateWorldTransform(m_skeleton);
		}

		void Skeleton::setLocation(float x, float y) { 
			m_x = x;
			m_y = y;

			/*m_root->x = x; 
			m_root->y = y * -1;*/
			m_skeleton->x = x;
			m_skeleton->y = y;

			spSkeleton_updateWorldTransform(m_skeleton);

			//m_skeleton->x = x;
			//m_skeleton->y = y * -1;
		}
		void Skeleton::setLocation(float x, float y, float z) { 
			m_x = x;
			m_y = y;
			m_z = z;

			/*m_root->x = x; 
			m_root->y = y * -1;*/
			m_skeleton->x = x;
			m_skeleton->y = y;
//			m_skeleton->z = z;

			spSkeleton_updateWorldTransform(m_skeleton);

			//m_skeleton->x = x;
			//m_skeleton->y = y * -1;
		}
		void Skeleton::setScale(float x, float y) { 
			m_scaleX = x;
			m_scaleY = y;
		}

		float Skeleton::getX() {
			if (m_skeleton->flipX) { return m_x * -1; }
			return m_x;
		}
		float Skeleton::getY() {
			return m_y;
		}
		float Skeleton::getZ() {
			return m_z;
		}

		void Skeleton::render() {

			unsigned int BLEND_ADDITIVE = Renderer::BLEND_ADDITIVE;
			unsigned int BLEND_SPINE = Renderer::BLEND_NORMAL;

			Renderer* r = ARK2D::getRenderer();
			float rr_r = r->getDrawColor().getRedf();
			float rr_g = r->getDrawColor().getGreenf();
			float rr_b = r->getDrawColor().getBluef();
			float rr_a = r->getDrawColor().getAlphaf();

			float rawNormals[] = {
				0,0,1,
				0,0,1,
				0,0,1,
				0,0,1,
				0,0,1,
				0,0,1
			};

			//r->setDrawColor(Color::white); 
			float worldVertices[SPINE_MESH_VERTEX_COUNT_MAX];
			for (int i = 0; i < m_skeleton->slotsCount; ++i)
			{
				spSlot* slot = m_skeleton->drawOrder[i];
				spSlotData* slotdata = slot->data;
				
				spAttachment* attachment = slot->attachment;
				if (!attachment) { continue; } 

				unsigned int blend = slot->data->additiveBlending ? BLEND_ADDITIVE  : BLEND_SPINE;
				r->setBlendMode(blend);

				if (attachment->type == SP_ATTACHMENT_REGION) {

					spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
					spRegionAttachment_computeWorldVertices(regionAttachment, slot->bone, worldVertices);

					spAtlasRegion* spatreg = (spAtlasRegion*)regionAttachment->rendererObject;
					Image* img = (Image*) spatreg->page->rendererObject;
					unsigned int texId = img->getTexture()->getId();
					float sx = img->getTextureW();
					float sy = img->getTextureH();
					
					float rawVertices[] = {
						worldVertices[SP_VERTEX_X1], worldVertices[SP_VERTEX_Y1], m_z,
						worldVertices[SP_VERTEX_X2], worldVertices[SP_VERTEX_Y2], m_z,
						worldVertices[SP_VERTEX_X4], worldVertices[SP_VERTEX_Y4], m_z,
						worldVertices[SP_VERTEX_X4], worldVertices[SP_VERTEX_Y4], m_z,
						worldVertices[SP_VERTEX_X2], worldVertices[SP_VERTEX_Y2], m_z,
						worldVertices[SP_VERTEX_X3], worldVertices[SP_VERTEX_Y3], m_z
					};

					float rawTexCoords[] = {
						regionAttachment->uvs[SP_VERTEX_X1] * sx, regionAttachment->uvs[SP_VERTEX_Y1] * sy,
						regionAttachment->uvs[SP_VERTEX_X2] * sx, regionAttachment->uvs[SP_VERTEX_Y2] * sy,
						regionAttachment->uvs[SP_VERTEX_X4] * sx, regionAttachment->uvs[SP_VERTEX_Y4] * sy,
						regionAttachment->uvs[SP_VERTEX_X4] * sx, regionAttachment->uvs[SP_VERTEX_Y4] * sy,
						regionAttachment->uvs[SP_VERTEX_X2] * sx, regionAttachment->uvs[SP_VERTEX_Y2] * sy,
						regionAttachment->uvs[SP_VERTEX_X3] * sx, regionAttachment->uvs[SP_VERTEX_Y3] * sy
					};

					unsigned char re = (rr_r * m_skeleton->r * slot->r * regionAttachment->r) * 255;
					unsigned char g =  (rr_g * m_skeleton->g * slot->g * regionAttachment->g) * 255;
					unsigned char b =  (rr_b * m_skeleton->b * slot->b * regionAttachment->b) * 255;
					unsigned char a =  (rr_a * m_skeleton->a * slot->a * regionAttachment->a) * 255;
					unsigned char rawColors[] = {
						re, g, b, a,
						re, g, b, a,
						re, g, b, a,
						re, g, b, a,
						re, g, b, a,
						re, g, b, a
					};

					r->texturedTriangles(texId, &rawVertices[0], &rawNormals[0], &rawTexCoords[0], &rawColors[0], 2);
					
					
				} else if (attachment->type == SP_ATTACHMENT_MESH) {
					//spRegionAttachment* regionAttachment = (spRegionAttachment*)attachment;
					spMeshAttachment* mesh = (spMeshAttachment*) attachment;
					if (mesh->verticesCount > SPINE_MESH_VERTEX_COUNT_MAX) { continue; }

					spAtlasRegion* spatreg = (spAtlasRegion*) mesh->rendererObject;
					Image* img = (Image*) spatreg->page->rendererObject;
					unsigned int texId = img->getTexture()->getId();
					float sx = img->getTextureW();
					float sy = img->getTextureH();

					spMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

					float re_f = m_skeleton->r * slot->r * mesh->r;
					float g_f  = m_skeleton->g * slot->g * mesh->g;
					float b_f  = m_skeleton->b * slot->b * mesh->b;
					float a_f  = m_skeleton->a * slot->a * mesh->a;

					unsigned char re = (unsigned char) (rr_r * re_f * 255.0f);
					unsigned char g =  (unsigned char) (rr_g * g_f  * 255.0f);
					unsigned char b =  (unsigned char) (rr_b * b_f  * 255.0f);
					unsigned char a =  (unsigned char) (rr_a * a_f  * 255.0f);
					
					#ifdef ARK2D_WINDOWS_VS
						float* rawVertices = (float*)malloc((mesh->trianglesCount * 3) * sizeof(float));
						float* rawNormalsLocal = (float*)malloc((mesh->trianglesCount * 3) * sizeof(float));
						float* rawTexCoords = (float*)malloc((mesh->trianglesCount * 2) * sizeof(float));
						unsigned char* rawColors = (unsigned char*)malloc((mesh->trianglesCount * 4) * sizeof(unsigned char));
					#else
						float rawVertices[mesh->trianglesCount*3];
						float rawNormalsLocal[mesh->trianglesCount*3];
						float rawTexCoords[mesh->trianglesCount*2];
						unsigned char rawColors[mesh->trianglesCount*4];
					#endif

					/*string thisc = string("r: ");
					thisc += Cast::toString<float>(re_f); 
					thisc += string(", g: ");
					thisc += Cast::toString<float>(g_f);
					thisc += string(", b: ");
					thisc += Cast::toString<float>(b_f);
					thisc += string(", a: ");
					thisc += Cast::toString<float>(a_f);
					ARK2D::getLog()->w(thisc); */

					int k = 0;
					int l = 0;
					int m = 0;
					for (int j = 0; j < mesh->trianglesCount; ++j) {
						int index = mesh->triangles[j] << 1;
						rawVertices[k] = worldVertices[index];
						rawVertices[k+1] = worldVertices[index+1];
						rawVertices[k+2] = m_z;

						rawNormalsLocal[k] = 0;
						rawNormalsLocal[k+1] = 0;
						rawNormalsLocal[k+2] = 1;

						rawTexCoords[m] = mesh->uvs[index] * sx;
						rawTexCoords[m+1] = mesh->uvs[index + 1] * sy;
						
						rawColors[l] = re;
						rawColors[l+1] = g;
						rawColors[l+2] = b;
						rawColors[l+3] = a;
						k += 3;
						m += 2;
						l += 4;
					}
					r->texturedTriangles(texId, &rawVertices[0], &rawTexCoords[0], &rawNormalsLocal[0], &rawColors[0], mesh->trianglesCount/3);
 
					#ifdef ARK2D_WINDOWS_VS
						free(rawVertices);
						free(rawNormalsLocal);
						free(rawTexCoords);
						free(rawColors);
					#endif

				} else if (attachment->type == SP_ATTACHMENT_SKINNED_MESH) {
					spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*) attachment;
					if (mesh->uvsCount > SPINE_MESH_VERTEX_COUNT_MAX) { continue; }  // uvsCount

					spAtlasRegion* spatreg = (spAtlasRegion*)mesh->rendererObject;
					Image* img = (Image*) spatreg->page->rendererObject;
					unsigned int texId = img->getTexture()->getId();
					float sx = img->getTextureW();
					float sy = img->getTextureH();
					spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, worldVertices);

					float re_f = m_skeleton->r * slot->r * mesh->r;
					float g_f  = m_skeleton->g * slot->g * mesh->g;
					float b_f  = m_skeleton->b * slot->b * mesh->b;
					float a_f  = m_skeleton->a * slot->a * mesh->a;

					unsigned char re = (unsigned char) (rr_r * re_f * 255.0f);
					unsigned char g =  (unsigned char) (rr_g * g_f  * 255.0f);
					unsigned char b =  (unsigned char) (rr_b * b_f  * 255.0f);
					unsigned char a =  (unsigned char) (rr_a * a_f  * 255.0f);

					#ifdef ARK2D_WINDOWS_VS
						float* rawVertices = (float*)malloc((mesh->trianglesCount * 3) * sizeof(float));
						float* rawNormalsLocal = (float*)malloc((mesh->trianglesCount * 3) * sizeof(float));
						float* rawTexCoords = (float*)malloc((mesh->trianglesCount * 2) * sizeof(float));
						unsigned char* rawColors = (unsigned char*)malloc((mesh->trianglesCount * 4) * sizeof(unsigned char));
					#else
						float rawVertices[mesh->trianglesCount * 3];
						float rawNormalsLocal[mesh->trianglesCount * 3];
						float rawTexCoords[mesh->trianglesCount * 2];
						unsigned char rawColors[mesh->trianglesCount * 4];
					#endif

					int k = 0;
					int m = 0;
					int l = 0;
					for (int j = 0; j < mesh->trianglesCount; ++j) {
						int index = mesh->triangles[j] << 1;
						
						rawVertices[k] = worldVertices[index];
						rawVertices[k+1] = worldVertices[index+1];
						rawVertices[k+2] = m_z;
						rawNormalsLocal[k] = 0;
						rawNormalsLocal[k+1] = 0;
						rawNormalsLocal[k+2] = 1;
						rawTexCoords[m] = mesh->uvs[index] * sx;
						rawTexCoords[m+1] = mesh->uvs[index + 1] * sy;
						rawColors[l] = re;
						rawColors[l+1] = g;
						rawColors[l+2] = b;
						rawColors[l+3] = a;
						k += 3;
						m += 2;
						l += 4;

					}
					
					r->texturedTriangles(texId, &rawVertices[0], &rawNormalsLocal[0], &rawTexCoords[0], &rawColors[0], mesh->trianglesCount/3);

					#ifdef ARK2D_WINDOWS_VS
						free(rawVertices);
						free(rawNormalsLocal);
						free(rawTexCoords);
						free(rawColors);
					#endif

				} /*else if (attachment->type == SP_ATTACHMENT_BOUNDING_BOX) {

					
					spBoundingBoxAttachment* boxAttachment = (spBoundingBoxAttachment*) attachment;
					spBoundingBoxAttachment_computeWorldVertices (boxAttachment, slot->skeleton->x, slot->skeleton->y, slot->bone, worldVertices);

					r->setDrawColor(Color::white);
					r->drawLine(worldVertices[SP_VERTEX_X1], worldVertices[SP_VERTEX_Y1], worldVertices[SP_VERTEX_X2], worldVertices[SP_VERTEX_Y2]);
					r->drawLine(worldVertices[SP_VERTEX_X2], worldVertices[SP_VERTEX_Y2], worldVertices[SP_VERTEX_X3], worldVertices[SP_VERTEX_Y3]);
					r->drawLine(worldVertices[SP_VERTEX_X3], worldVertices[SP_VERTEX_Y3], worldVertices[SP_VERTEX_X4], worldVertices[SP_VERTEX_Y4]);
					r->drawLine(worldVertices[SP_VERTEX_X4], worldVertices[SP_VERTEX_Y4], worldVertices[SP_VERTEX_X1], worldVertices[SP_VERTEX_Y1]);
					continue;
					
				}*/

			}
			
			r->setBlendMode(Renderer::BLEND_NORMAL);
		}

		Skeleton::~Skeleton() {		
			if (!m_copy) { 
				spSkeleton_dispose(m_skeleton);
				spSkeletonData_dispose(m_data);
				spSkeletonJson_dispose(m_json);
				spAtlas_dispose(m_atlas);
			}
		}

	}
}
