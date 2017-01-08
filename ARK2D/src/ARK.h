/*
 * ARK.h
 *
 *  Created on: 19-Jun-2010
 *      Author: Ashley
 */

// Main
#include "ARK2D/Core/ARK2D.h"

// Core
#include "ARK2D/Core/Constants.h"
#include "ARK2D/Core/Exception.h"
#include "ARK2D/Core/Game.h"
#include "ARK2D/Core/GameContainer.h"
#include "ARK2D/Core/GameObject.h"
#include "ARK2D/Core/GameTimer.h"
#include "ARK2D/Core/Event.h"
#include "ARK2D/Core/Resource.h"
#include "ARK2D/Core/ToString.h"
#include "ARK2D/Core/Camera.h"
#include "ARK2D/Core/Log.h"
#include "ARK2D/Core/Strings.h"
#include "ARK2D/Core/Math/SAT.h"
#include "ARK2D/Core/Cast.h"
#include "ARK2D/Core/KeyPairFile.h"

// Geometry
#include "ARK2D/Core/Geometry/Vector2.h"
#include "ARK2D/Core/Geometry/Vector3.h"
#include "ARK2D/Core/Geometry/GigaRectangle.h"
#include "ARK2D/Core/Geometry/Shape.h"
#include "ARK2D/Core/Geometry/Polygon.h"
#include "ARK2D/Core/Geometry/Rectangle.h"
#include "ARK2D/Core/Geometry/Cube.h"
#include "ARK2D/Core/Geometry/Circle.h"
#include "ARK2D/Core/Geometry/Line.h"
#include "ARK2D/Core/Geometry/Transform.h"
#include "ARK2D/Core/Geometry/AdvancedPolygon.h"
#include "ARK2D/Core/Geometry/Plane.h"

// State Based Game / Finite State Machine
#include "ARK2D/Core/State/StateBasedGame.h"
#include "ARK2D/Core/State/GameState.h"
#include "ARK2D/Core/State/IntelligentGameState.h"
#include "ARK2D/Core/State/VideoGameState.h"
#include "ARK2D/Core/State/LoadingState.h"
#include "ARK2D/Core/State/Transition/Transition.h"
#include "ARK2D/Core/State/Transition/EmptyTransition.h"
#include "ARK2D/Core/State/Transition/TranslateOutInTransition.h"
#include "ARK2D/Core/State/Transition/SlideRectanglesAcrossTransition.h"
#include "ARK2D/Core/State/Transition/SquaresOutTransition.h"
#include "ARK2D/Core/State/Transition/SquaresInTransition.h"
#include "ARK2D/Core/State/Transition/FadeTransition.h"
#include "ARK2D/Core/State/Transition/FadeToColourTransition.h"
#include "ARK2D/Core/State/Transition/FadeFromColourTransition.h"


// Graphics
#include "ARK2D/Core/Graphics/Renderer.h"
#include "ARK2D/Core/Graphics/Color.h"
#include "ARK2D/Core/Graphics/Animation.h"
#include "ARK2D/Core/Graphics/SpriteSheetDescription.h"
#include "ARK2D/Core/Graphics/SpriteSheetStore.h"
#include "ARK2D/Core/Graphics/Shader.h"
#include "ARK2D/Core/Graphics/FBO.h"
#include "ARK2D/Core/Graphics/Image.h"
#include "ARK2D/Core/Graphics/Texture.h"
#include "ARK2D/Core/Graphics/TextureStore.h"
#include "ARK2D/Core/Graphics/ShaderStore.h"
#include "ARK2D/Core/Graphics/Shaders/BasicShader.h"
#include "ARK2D/Core/Graphics/Shaders/AlphaMaskShader.h"
#include "ARK2D/Core/Graphics/Shaders/HSVShader.h"
#include "ARK2D/Core/Graphics/Shaders/StencilShader.h"
#include "ARK2D/Core/Graphics/Shaders/DirectionalLightingShader.h"
#include "ARK2D/Core/Graphics/Shaders/PointLightingShader.h"
#include "ARK2D/Core/Graphics/Shaders/ShockwaveShader.h"
#include "ARK2D/Core/Graphics/Skybox.h"
#include "ARK2D/Core/Graphics/Model/ObjModel.h"
#include "ARK2D/Core/Graphics/Model/ImageModel.h"
#include "ARK2D/Core/Graphics/MatrixStack.h"
#include "ARK2D/Core/Graphics/ImageIO/PNGImage.h"

 // Fonts
#include "ARK2D/Core/Font/Font.h"
#include "ARK2D/Core/Font/BMFont.h"
#include "ARK2D/Core/Font/FTFont.h"

// Math
#include "ARK2D/Core/Math/Random.h"
#include "ARK2D/Core/Math/MathUtil.h"
#include "ARK2D/Core/Geometry/Vector4.h"
#include "ARK2D/Core/Math/Matrix44.h"

 // Tweening
#include "ARK2D/Core/Tween/Timeline.h"
#include "ARK2D/Core/Tween/Easing.h"
#include "ARK2D/Core/Tween/Timer.h"

// Input / Controls
#include "ARK2D/Core/Controls/Input.h"
#include "ARK2D/Core/Controls/Gamepad.h"
#include "ARK2D/Core/Controls/ErrorDialog.h"
#include "ARK2D/Core/Controls/FileDialog.h"

// Multithreading
#include "ARK2D/Core/Threading/Thread.h"
#include "ARK2D/Core/Threading/Mutex.h"

// Containers
#include "ARK2D/Core/Vector.h"
#include "ARK2D/Core/Pool.h"

// Scenegraph
#include "ARK2D/Core/SceneGraph/Scene.h"

// Core Utils
#include "ARK2D/Core/Util/StringUtil.h"
#include "ARK2D/Core/Util/SystemUtil.h"
#include "ARK2D/Core/Util/Callbacks.h"


// Game Jolt
#include "ARK2D/GJ/GameJolt.h"
#include "ARK2D/GJ/PropertyItem.h"
#include "ARK2D/GJ/User.h"
#include "ARK2D/GJ/DataStore.h"
#include "ARK2D/GJ/Highscore.h"
#include "ARK2D/GJ/Trophy.h"

// Game Jolt Next
#include "ARK2D/GJ/Next/Overlay.h"


// Net
#include "ARK2D/Net/Address.h"
#include "ARK2D/Net/Connection.h"
#include "ARK2D/Net/Discovery.h"
#include "ARK2D/Net/EventQueue.h"
#include "ARK2D/Net/FlowControl.h"
#include "ARK2D/Net/PacketQueue.h"
#include "ARK2D/Net/ReliabilitySystem.h"
#include "ARK2D/Net/ReliableConnection.h"
#include "ARK2D/Net/Socket.h"

// Audio
#include "ARK2D/Audio/Sound.h"
#include "ARK2D/Audio/SoundStore.h"

// Tiled
#include "ARK2D/Tiled/TiledMap.h"

// Utils
#include "ARK2D/Util/AnalyticsUtil.h"
#include "ARK2D/Util/AngelScriptUtil.h"
#include "ARK2D/Util/GameCenterUtil.h"
#include "ARK2D/Util/ICloudUtil.h"
#include "ARK2D/Util/GooglePlayGameServicesUtil.h"
#include "ARK2D/Util/GameCircleUtil.h"
#include "ARK2D/Util/ArcadeHighscoreInput.h"
#include "ARK2D/Util/DisplayUtil.h"
#include "ARK2D/Util/VerticalMenu.h" // can probably be removed?
#include "ARK2D/Util/VerticalMenuItem.h" // can probably be removed?
#include "ARK2D/Util/CameraShake.h" // can probably be removed?
#include "ARK2D/Util/LocalHighscores.h"
#include "ARK2D/Util/RSSL.h" // basic scripting.
#include "ARK2D/Util/Range.h"
#include "ARK2D/Util/StringStore.h"
#include "ARK2D/Util/SocialUtil.h"
#include "ARK2D/Util/URLRequest.h"
#include "ARK2D/Util/Vibrator.h"
#include "ARK2D/Util/Wobble.h"
#include "ARK2D/Util/ZXRenderer.h"


// JSON Implementation
#include "ARK2D/Includes.h"

// User Interface
#include "ARK2D/UI/AbstractUIComponent.h"
#include "ARK2D/UI/Label.h"
#include "ARK2D/UI/TextField.h"
#include "ARK2D/UI/Button.h"
#include "ARK2D/UI/Panel.h"
#include "ARK2D/UI/ComboBox.h"
#include "ARK2D/UI/ComboBoxItem.h"
#include "ARK2D/UI/ScrollPanel.h"
#include "ARK2D/UI/CheckBox.h"
#include "ARK2D/UI/Dialog.h"
#include "ARK2D/UI/SimpleTextField.h"
#include "ARK2D/UI/Slider.h"
#include "ARK2D/UI/SplitPane.h"
#include "ARK2D/UI/LogViewer.h"

// Particle Engine
#include "ARK2D/Particles/Particle.h"
#include "ARK2D/Particles/ParticleEmitter.h"
#include "ARK2D/Particles/ParticleIO.h"
#include "ARK2D/Particles/ParticlePool.h"
#include "ARK2D/Particles/ParticleSystem.h"
#include "ARK2D/Particles/ConfigurableEmitter.h"



// Paths
#include "ARK2D/Path/Path.h"
#include "ARK2D/Path/PathGroup.h"
#include "ARK2D/Path/PathIO.h"

// Pathfinding
#include "ARK2D/Pathfinding/AStar.h"



// Tools
#include "ARK2D/Tools/Packer.h"


// Tests
#include "ARK2D/Tests/TransitionTest.h"
#include "ARK2D/Tests/CollisionTest.h"
#include "ARK2D/Tests/GamepadsTest.h"
#include "ARK2D/Tests/NetTest.h"
#include "ARK2D/Tests/ParticlesTest.h"
#include "ARK2D/Tests/UITest.h"
#include "ARK2D/Tests/ShaderTest.h"
#include "ARK2D/Tests/PathfindingTest.h"

// Spine
#include "ARK2D/vendor/spine/SpineSkeleton.h"

// Namespaces
#include "ARK2D/Namespaces.h"




