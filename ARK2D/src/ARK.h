/*
 * ARK.h
 *
 *  Created on: 19-Jun-2010
 *      Author: Ashley
 */

//const bool ARK2D_VERBOSE = true;
// OH U

#include "ARK2D/ARK2D.h"
#include "ARK2D/ARKException.h"


#include "ARK2D/Shader.h"
#include "ARK2D/GameTimer.h"

#include "ARK2D/GameContainer.h"

// Geometry
#include "ARK2D/Geometry/Vector2.h"
#include "ARK2D/Geometry/Vector4.h"
#include "ARK2D/Geometry/Matrix44.h"
#include "ARK2D/Geometry/GigaRectangle.h"

#include "ARK2D/Geometry/Shape.h"
#include "ARK2D/Geometry/Polygon.h"
#include "ARK2D/Geometry/Rectangle.h"
#include "ARK2D/Geometry/Circle.h"
#include "ARK2D/Geometry/Line.h"


#include "ARK2D/Game.h"

#include "ARK2D/Constants.h"

#include "ARK2D/State/StateBasedGame.h"
#include "ARK2D/State/GameState.h"
#include "ARK2D/State/IntelligentGameState.h"
#include "ARK2D/State/LoadingState.h"
#include "ARK2D/State/Transition/Transition.h"
#include "ARK2D/State/Transition/EmptyTransition.h"
#include "ARK2D/State/Transition/TranslateOutInTransition.h"
#include "ARK2D/State/Transition/SlideRectanglesAcrossTransition.h"
#include "ARK2D/State/Transition/FadeTransition.h"
#include "ARK2D/State/Transition/FadeToColourTransition.h"
#include "ARK2D/State/Transition/FadeFromColourTransition.h"

#include "ARK2D/Constants.h"

#include "ARK2D/Color.h"
#include "ARK2D/Graphics.h"
#include "ARK2D/Image/Image.h"
#include "ARK2D/Animation.h"

// Audio
#include "ARK2D/Audio/Sound.h"

// Fonts
#include "ARK2D/Font/Font.h"
#include "ARK2D/Font/BMFont.h"
#include "ARK2D/Font/FTFont.h"

// Resources
#include "ARK2D/Resource.h"

// Tiled
#include "ARK2D/Tiled/TiledMap.h"

// Input
#include "ARK2D/Input.h"

// Utils
#include "ARK2D/Util/FileUtil.h"
#include "ARK2D/Util/MathUtil.h"
#include "ARK2D/Util/StringUtil.h"
#include "ARK2D/Util/ARKLog.h"
#include "ARK2D/Util/Cast.h"
#include "ARK2D/Util/VerticalMenu.h" // can probably be removed?
#include "ARK2D/Util/VerticalMenuItem.h" // can probably be removed?
#include "ARK2D/Util/CameraShake.h" // can probably be removed?
#include "ARK2D/Util/LocalHighscores.h"
#include "ARK2D/Util/RSSL.h" // basic scripting.

#include "ARK2D/Timeline.h"
#include "ARK2D/SpriteSheetDescription.h"
#include "ARK2D/Event.h"

#include "ARK2D/StringStore.h"

// JSON Implementation
#include "ARK2D/ARK2D_JSON.h"

// UI
#include "ARK2D/UI/Label.h"
#include "ARK2D/UI/TextField.h"
#include "ARK2D/UI/Button.h"
#include "ARK2D/UI/Panel.h"
#include "ARK2D/UI/ComboBox.h"
#include "ARK2D/UI/ComboBoxItem.h"
#include "ARK2D/UI/ScrollPanel.h"
#include "ARK2D/UI/Checkbox.h"
#include "ARK2D/UI/FileDialog.h"
#include "ARK2D/UI/ErrorDialog.h"

// Particle Engine
#include "ARK2D/Particles/Particle.h"
#include "ARK2D/Particles/ParticleEmitter.h"
#include "ARK2D/Particles/ParticleIO.h"
#include "ARK2D/Particles/ParticlePool.h"
#include "ARK2D/Particles/ParticleSystem.h"
#include "ARK2D/Particles/Range.h"
#include "ARK2D/Particles/ConfigurableEmitter.h"

// Containers
#include "ARK2D/ARKGameObject.h"
#include "ARK2D/ARKVector.h"
#include "ARK2D/ARKPool.h"

// Paths
#include "ARK2D/Path/SubPath.h"
#include "ARK2D/Path/Path.h"
#include "ARK2D/Path/PathGroup.h"
#include "ARK2D/Path/PathIO.h"

// Tools
#include "ARK2D/Tools/Packer.h"

// Misc
#include "ARK2D/ToString.h"
