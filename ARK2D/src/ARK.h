/*
 * ARK.h
 *
 *  Created on: 19-Jun-2010
 *      Author: Ashley
 */

// Main
#include "ARK2D/ARK2D.h"

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

// Geometry
#include "ARK2D/Geometry/Vector2.h"
#include "ARK2D/Geometry/Vector3.h"
#include "ARK2D/Geometry/GigaRectangle.h"
#include "ARK2D/Geometry/Shape.h"
#include "ARK2D/Geometry/Polygon.h"
#include "ARK2D/Geometry/Rectangle.h"
#include "ARK2D/Geometry/Cube.h"
#include "ARK2D/Geometry/Circle.h"
#include "ARK2D/Geometry/Line.h" 
#include "ARK2D/Geometry/Transform.h" 

// Game Jolt
#include "ARK2D/GJ/GameJolt.h"
#include "ARK2D/GJ/PropertyItem.h"
#include "ARK2D/GJ/User.h"
#include "ARK2D/GJ/DataStore.h"
#include "ARK2D/GJ/Highscore.h"
#include "ARK2D/GJ/Trophy.h"

// Game Jolt Next
#include "ARK2D/GJ/Next/Overlay.h"

// State Based Game / Finite State Machine
#include "ARK2D/State/StateBasedGame.h"
#include "ARK2D/State/GameState.h"
#include "ARK2D/State/IntelligentGameState.h"
#include "ARK2D/State/VideoGameState.h"
#include "ARK2D/State/LoadingState.h"
#include "ARK2D/State/Transition/Transition.h"
#include "ARK2D/State/Transition/EmptyTransition.h"
#include "ARK2D/State/Transition/TranslateOutInTransition.h"
#include "ARK2D/State/Transition/SlideRectanglesAcrossTransition.h"
#include "ARK2D/State/Transition/SquaresOutTransition.h"
#include "ARK2D/State/Transition/SquaresInTransition.h"
#include "ARK2D/State/Transition/FadeTransition.h"
#include "ARK2D/State/Transition/FadeToColourTransition.h"
#include "ARK2D/State/Transition/FadeFromColourTransition.h"


// Graphics
#include "ARK2D/Graphics/Renderer.h"
#include "ARK2D/Graphics/Color.h"
#include "ARK2D/Graphics/Animation.h"
#include "ARK2D/Graphics/SpriteSheetDescription.h"
#include "ARK2D/Graphics/SpriteSheetStore.h"
#include "ARK2D/Graphics/Shader.h"
#include "ARK2D/Graphics/FBO.h"
#include "ARK2D/Graphics/Image.h"
#include "ARK2D/Graphics/Texture.h"
#include "ARK2D/Graphics/TextureStore.h"
#include "ARK2D/Graphics/Shaders/HSVShader.h"
#include "ARK2D/Graphics/Shaders/HSVGeometryShader.h"
#include "ARK2D/Graphics/Shaders/HSVTextureShader.h"
#include "ARK2D/Graphics/Shaders/StencilShader.h"


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

// Fonts
#include "ARK2D/Font/Font.h"
#include "ARK2D/Font/BMFont.h"
#include "ARK2D/Font/FTFont.h"

// Tiled
#include "ARK2D/Tiled/TiledMap.h"

// Tweening
#include "ARK2D/Tween/Timeline.h"
#include "ARK2D/Tween/Easing.h"
#include "ARK2D/Tween/Timer.h"

// Input / Controls
#include "ARK2D/Controls/Input.h"
#include "ARK2D/Controls/Gamepad.h"

// Utils
#include "ARK2D/Util/AnalyticsUtil.h"
#include "ARK2D/Util/GameCenterUtil.h"
#include "ARK2D/Util/ICloudUtil.h"
#include "ARK2D/Util/GooglePlayGameServicesUtil.h"
#include "ARK2D/Util/GameCircleUtil.h"
#include "ARK2D/Util/ArcadeHighscoreInput.h"
#include "ARK2D/Util/Callbacks.h"
#include "ARK2D/Util/FileUtil.h"
#include "ARK2D/Util/MathUtil.h"
#include "ARK2D/Util/StringUtil.h"
#include "ARK2D/Util/DisplayUtil.h"
#include "ARK2D/Util/Log.h"
#include "ARK2D/Util/Cast.h"
#include "ARK2D/Util/VerticalMenu.h" // can probably be removed?
#include "ARK2D/Util/VerticalMenuItem.h" // can probably be removed?
#include "ARK2D/Util/CameraShake.h" // can probably be removed?
#include "ARK2D/Util/LocalHighscores.h"
#include "ARK2D/Util/KeyPairFile.h"
#include "ARK2D/Util/RSSL.h" // basic scripting.
#include "ARK2D/Util/Vector4.h"
#include "ARK2D/Util/Matrix44.h"
#include "ARK2D/Util/Range.h"
#include "ARK2D/Util/StringStore.h"
#include "ARK2D/Util/SocialUtil.h"
#include "ARK2D/Util/SAT.h"
#include "ARK2D/Util/URLRequest.h"
#include "ARK2D/Util/Vibrator.h"

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
#include "ARK2D/UI/FileDialog.h"
#include "ARK2D/UI/FileDialog.h"
#include "ARK2D/UI/ErrorDialog.h"
#include "ARK2D/UI/SimpleTextField.h"
#include "ARK2D/UI/Slider.h"

// Particle Engine
#include "ARK2D/Particles/Particle.h"
#include "ARK2D/Particles/ParticleEmitter.h"
#include "ARK2D/Particles/ParticleIO.h"
#include "ARK2D/Particles/ParticlePool.h"
#include "ARK2D/Particles/ParticleSystem.h"
#include "ARK2D/Particles/ConfigurableEmitter.h"

// Containers
#include "ARK2D/Util/Containers/Vector.h"
#include "ARK2D/Util/Containers/Pool.h"

// Multithreading
#include "ARK2D/Threading/Thread.h"
#include "ARK2D/Threading/Mutex.h"

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

// Spine
#include "ARK2D/vendor/spine/SpineSkeleton.h"

// Namespaces
#include "ARK2D/Namespaces.h"




