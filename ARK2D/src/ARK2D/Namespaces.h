/*
 * Namespaces.h
 *
 *  Created on: Mar 13, 2012
 *      Author: ashleygwinnell
 */

#ifndef ARK2D_NAMESPACES_H_
#define ARK2D_NAMESPACES_H_

/*!
 * \brief The main namespace for the framework, containing all the other namespaces and classes.
 *
 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
 */
namespace ARK {

	//! Contains classes for Audio playback.
	namespace Audio {
		class Sound;
        class SoundStore;
	}

	//! Core framework classes for Windowing, Default Objects, Events, Resources.
	namespace Core {
		class Cast;
		class Constants;
        class Camera;
        class DummyCamera;
		class Event;
		class Exception;
		class Game;
		class GameObject;
		class GameContainer;
		class GameContainerPlatform;
		class GameTimer;
		class KeyPairFile;
		class String;
		class ToString;
		class Resource;
		class Strings;
		class Log;

		//! Contains Container classes such as Vector and Pool.
		//namespace Containers {
			template <class T>
			class Vector;

			template <class T>
			class Pool;
		//}

        //! Contains classes for Input e.g. Keyboard, Mouse, Gamepad and Touch.
        namespace Controls {
            class Input;
            class Gamepad;
            class GamepadListener;
            class KeyListener;

            class ErrorDialog;
			class FileDialog;
        }

		//! Contains classes for Font reading, writing and rendering.
		namespace Font {
			class Font;
			class BMFont;
			class FTFont;
		}

		//! Contains classes for Geometry including collision detection and resolution.
		namespace Geometry {

	        template <class T=int>
	        class CubeTemplate;

	        class Cube;

			template <class T=int>
			class Shape;

			template <class T=int>
			class RectangleTemplate;

			class Rectangle;

			template <class T=int>
			class Circle;

			template <class T=int>
			class Line;

			class AdvancedPolygon;

            template <class T=float>
            class Vector4Template;

            class Transform;

            class Vector4;
		}

		//! Contains classes for Rendering and Animation.
		namespace Graphics {
			class Renderer;
			class Shader;
			class Color;
			class Animation;
			class SpriteSheetDescription;
			class SpriteSheetDescriptionItem;
			class SpriteSheetStore;
			class Image;

			class Texture;
			class TextureStore;

			class Skybox;

	        class MatrixStack;

	        class HSVShader;
	        class ShockwaveShader;

			//! Classes for loading different images types in to pixel buffers.
			namespace ImageIO {
				class BMPImage;
				class PNGImage;
				class TargaImage;
			}

			//! Classes for loading different 3d models as SceneNodes.
			namespace Model {
				class ImageModel;
				class ObjModel;
			}
		}

		//! Contains classes for Maths / Random numbers.
		namespace Math {
			class Random;

            template <class T=float>
            class Matrix44Template;

            class Matrix44;

            template <class T=float>
            class Matrix33Template;

            class Matrix33;

            class MathUtil;
		}

        //! Classes for Scenegraphs
        namespace SceneGraph {
            class Scene;
            class SceneNode;
            class SceneGroup;
            class SceneImage;
        }

		//! Classes for the finite state machine and game state. (e.g. Transitions, Loading)
		namespace State {
			class StateBasedGame;
			class GameState;
			class LoadingState;
			class VideoGameState;

			//! Classes for transitioning between game states.
			namespace Transition {
				class Transition;
				class TranslateOutInTransition;
				class SlideRectanglesAcrossTransition;
				class FadeFromColourTransition;
				class FadeToColourTransition;
				class FadeTransition;
				class EmptyTransition;
			}
		}
		//! Contains classes for Multi-threading.
		namespace Threading {
			class Thread;
			class Mutex;
		}

		//! Easing and time-based events.
		namespace Tween {
			class Easing;
			class Timeline;
			class Timer;
		}

		//! Core utils.
		namespace Utils {
			class Callbacks;
			class Callback;
			class StringUtil;
			class SystemUtil;
		}
	}


	//! Classes for Game Jolt APIs.
	namespace GJ {
		class PropertyItem;
		class DataStore;
		class Highscore;
		class Trophy;
		class User;
		class GameJolt;
	}

	//! Contains classes for Local/Online Networking with a custom UDP protocol.
	namespace Net {
		class Address;
		class Connection;
		class Discovery;
		class FlowControl;
		class PacketQueue;
		class EventQueue;
		class ReliabilitySystem;
		class ReliableConnection;
		class Socket;
	}

	//! Contains classes for Particle Systems.
	namespace Particles {
		class Particle;
		class ParticleSystem;
		class ParticleEmitter;
		class ConfigurableEmitter;
		class ParticlePool;
		class ParticleIO;
	}
	//! Contains classes for Paths including A* pathfinding.
	namespace Path {
		class PathGroup;
		class Path;
		class PathIO;
		class SubPath;
	}
	//! Pathfinding, woo!
	namespace Pathfinding {
		class AStar;
		class AStarNode;
	}


	//! Classes for skeletal animations
	namespace Spine {
		class Skeleton;
	}


	//! Contains classes for reading and rendering TilED maps.
	namespace Tiled {
		class TiledMap;
        class TiledMapObject;
        class TiledMapProperty;
        class TiledMapLayer;
        class TiledMapObjectGroup;
        class TiledMapTile;
        class TiledMapTileset;
		class TiledMapParser;
		class TiledMapParser_TinyXml;
		class TiledMapParser_RapidXml;
		class TiledMapParser_JSON;
	}

	//! Contains classes for building and showing User Interfaces.
	namespace UI {
		class AbstractUIComponent;
		class Button;
		class CheckBox;
		class ComboBox;
		class ComboBoxItem;
		class Label;
		class Panel;
		class ScrollPanel;
		class TextField;
		class UIComponent;
		class Slider;
		class SplitPane;
        class TitledPanel;
        class LogViewer;
	}
	//! Contains miscellaneous classes for mathematical functions, logging, etc.
	namespace Util {
		class AngelScriptUtil;
		class AnalyticsUtil;
		class GooglePlayGameServicesUtil;
		class CameraShake;
		class LocalHighscores;
		class RSSL;
		class URLRequest;
		class Vibrator;
		class DisplayUtil;
		class Wobble;




		template <class T>
		class Range;




	}

}
using namespace ARK;

using namespace ARK::Core;
using namespace ARK::Core::Controls;
using namespace ARK::Core::Font;
using namespace ARK::Core::Geometry;
using namespace ARK::Core::Graphics;
using namespace ARK::Core::Graphics::ImageIO;
using namespace ARK::Core::Graphics::Model;
using namespace ARK::Core::Tween;
using namespace ARK::Core::Threading;
using namespace ARK::Core::Math;
using namespace ARK::Core::SceneGraph;
using namespace ARK::Core::State;
using namespace ARK::Core::State::Transition;
using namespace ARK::Core::Utils;

using namespace ARK::Audio;
using namespace ARK::GJ;
using namespace ARK::Tiled;
using namespace ARK::Net;
using namespace ARK::Path;
using namespace ARK::Pathfinding;
using namespace ARK::Particles;

using namespace ARK::Spine;

using namespace ARK::UI;
using namespace ARK::Util;
//using namespace ARK::Util::Containers;

#endif /* ARK2D_NAMESPACES_H_ */
