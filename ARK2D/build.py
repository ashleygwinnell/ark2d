import sys
import os
import subprocess
import json 
import platform;
import base64
import binascii

#
# Windows requires MinGW with includes:
# 	AL/ 
#		al.h, alc.h, alext.h, alut.h
#	GL/
#		gl.h, glew.h, glext.h, glu.h, glxew.h, glxext.h, wglew.h, wglext.h
#
#	... and probably some others which I don't remember right now.
#
# Mac requires you do not install OpenAL from their website 
#  - i.e. you should not have Library/Frameworks/OpenAL.framework
#

class ARK2DBuildSystem:

	def __init__(self):
		
		self.building_library = False;
		self.building_game = True;
		
		self.ark2d_dir = "";
		self.game_dir = "";
		self.game_name = "";
		self.game_short_name = "";
		
		self.build_folder = "build_release";
		self.arch = platform.machine();
		
		if (sys.platform == "win32"):
			self.ds = "\\";
			self.mingw_dir = "C:\\MinGW";
			self.mingw_link = "-L" + self.mingw_dir + self.ds + "lib"
			self.gccCompiler = "gcc";
			self.gppCompiler = "g++";
			self.build_artifact = self.build_folder + self.ds + self.arch + self.ds + "libARK2D.dll";
			
		elif(sys.platform == "darwin"):
			self.ds = "/";
			self.mingw_dir = ""; #/usr";
			self.mingw_link = ""; #-L" + self.mingw_dir + self.ds + "lib"
			self.gccCompiler = "i686-apple-darwin11-llvm-gcc-4.2";
			self.gppCompiler = "i686-apple-darwin11-llvm-g++-4.2";
			#self.gccCompiler = "llvm-gcc-4.2";
			#self.gppCompiler = "llvm-g++-4.2";
			#self.gccCompiler = "gcc";
			#self.gppCompiler = "g++";
			self.build_artifact = self.build_folder + self.ds + self.arch + self.ds + "libARK2D.dylib";
		
		self.windresources = [];
		
		self.mkdirs = [];
		self.src_files = [];
		self.dll_files = [];
		self.static_libraries = [];
		self.linkingFlags = "";
		
	def dllInit(self):
		
		self.building_library = True;
		self.building_game = False;
	
		self.mkdirs.extend([
			self.build_folder,
			self.build_folder + self.ds + "build-cache", # cache folder
			self.build_folder + self.ds + self.arch,
			self.build_folder + self.ds + self.arch + self.ds + "src",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D", 
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "ARK2D_Path",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "ARK2D_State",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "ARK2D_Util",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "particles",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Font",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tiled",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "UI",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "libJSON",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "libJSON",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "lpng151",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "ogg",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "tinyxml",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "vorbis",
			self.build_folder + self.ds + self.arch + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "zlib123"
		]);
		
		self.src_files.extend([
			'src' + self.ds + 'main.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Animation.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARKException.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARKGameObject.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARKMutex.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARKString.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARKThread.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'BMPImage.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'CameraShake.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Circle.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Color.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Easing.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ErrorDialog.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Event.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'FileUtil.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Game.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'GameContainer.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'GameContainerLinux.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'GameContainerWindows.cpp', 
			'src' + self.ds + 'ARK2D' + self.ds + 'GameContainerMac.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Gamepad.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'GameTimer.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'GigaRectangle.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'GigaRectangleF.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Graphics.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Image.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Input.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'LocalHighscores.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'MathUtil.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'OutputWrapper.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'PNGImage.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Shader.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Sort.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Sound.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'SoundStore.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'SpriteSheet.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'SpriteSheetDescription.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'SpriteSheetDescriptionItem.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'StringStore.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'StringUtil.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'TargaImage.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Texture.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Timeline.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Font' + self.ds + 'FTFont.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Font' + self.ds + 'BMFont.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Tiled' + self.ds + 'TiledMap.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Tiled' + self.ds + 'TiledMapLayer.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Tiled' + self.ds + 'TiledMapObject.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Tiled' + self.ds + 'TiledMapObjectGroup.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Tiled' + self.ds + 'TiledMapProperty.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Tiled' + self.ds + 'TiledMapTile.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'Tiled' + self.ds + 'TiledMapTileset.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'UIComponent.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'AbstractUIComponent.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'Panel.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'ScrollPanel.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'Label.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'TextField.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'Button.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'CheckBox.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'FileDialog.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'ComboBox.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'UI' + self.ds + 'ComboBoxItem.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_Path' + self.ds + 'PathGroup.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_Path' + self.ds + 'Path.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_Path' + self.ds + 'SubPath.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_Path' + self.ds + 'PathIO.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_State' + self.ds + 'EmptyTransition.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_State' + self.ds + 'FadeTransition.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_State' + self.ds + 'GameState.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_State' + self.ds + 'LoadingState.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_State' + self.ds + 'SlideRectanglesAcrossTransition.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_State' + self.ds + 'StateBasedGame.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_State' + self.ds + 'Transition.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_State' + self.ds + 'TranslateOutInTransition.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_Util' + self.ds + 'VerticalMenu.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_Util' + self.ds + 'VerticalMenuItem.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'particles' + self.ds + 'ConfigurableEmitter.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'particles' + self.ds + 'Particle.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'particles' + self.ds + 'ParticleIO.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'particles' + self.ds + 'ParticlePool.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'particles' + self.ds + 'ParticleSystem.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'libJSON' + self.ds + 'JSON_Worker.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'libJSON' + self.ds + 'jsonmain.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'libJSON' + self.ds + 'JSONNode.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'png.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngerror.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngget.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngmem.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngpread.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngread.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngrio.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngrtran.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngrutil.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngset.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngtrans.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngwio.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngwrite.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngwtran.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'lpng151' + self.ds + 'pngwutil.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'ogg' + self.ds + 'bitwise.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'ogg' + self.ds + 'framing.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'tinyxml' + self.ds + 'tinystr.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'tinyxml' + self.ds + 'tinyxml.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'tinyxml' + self.ds + 'tinyxmlerror.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'tinyxml' + self.ds + 'tinyxmlparser.cpp',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'analysis.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'barkmel.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'bitrate.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'block.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'codebook.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'envelope.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'floor0.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'floor1.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'info.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'lookup.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'lpc.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'lsp.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'mapping0.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'mdct.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'psy.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'registry.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'res0.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'sharedbook.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'smallft.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'synthesis.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'tone.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'vorbisfile.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'vorbis' + self.ds + 'window.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'adler32.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'compress.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'crc32.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'deflate.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'gzio.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'infback.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'inffast.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'inflate.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'inftrees.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'trees.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'uncompr.c',
			'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'zlib123' + self.ds + 'zutil.c'
		]);
			
		if (sys.platform == "win32"):
			
			self.src_files.extend([
				'src' + self.ds + 'ARK2D' + self.ds + 'glew.c',
			]);
			
			self.dll_files.extend([
				'lib' + self.ds + 'kernel32.dll', 
				'lib' + self.ds + 'glew32.dll', 
				'lib' + self.ds + 'OpenAL32.dll', 
				'lib' + self.ds + 'alut.dll', 
				'lib' + self.ds + 'winmm.dll',
				'lib' + self.ds + 'freetype6.dll'
			]);
			self.static_libraries.extend([
				'glu32', 
				'winmm', 
				'opengl32', 
				'mingw32',
				'freetype'
			]);
		
			self.linkingFlags = " -mwindows -shared ";		

			self.src_files.extend([
				'src' + self.ds + 'ARK2D' + self.ds + 'ARK2D_windres.rc'
			]);		
			
		elif (sys.platform == "darwin"):
			
			self.src_files.extend([
				#'src' + self.ds + 'ARK2D' + self.ds + 'src-AL' + self.ds + 'glew.c',
			]);
			
			self.dll_files.extend([
			#	'lib' + self.ds + 'glew32.dll', 
			#	'lib' + self.ds + 'OpenAL32.dll', 
			#	'lib' + self.ds + 'alut.dll', 
			#	'lib' + self.ds + 'winmm.dll',
			#	'lib' + self.ds + 'freetype6.dll'
				"/usr/X11/lib/libfreetype.6.dylib "
			#	"lib/osx/openal.dylib "
			]);
			
			self.static_libraries.extend([
				#'glu32', 
				#'opengl32', 
				#'freetype2'
			]);
			
			self.build_artifact = self.build_folder + self.ds + self.arch + self.ds + "libARK2D.dylib";
			
			self.linkingFlags = "";
			
					
		
		
	def gamePreInit(self):
	
		self.building_library = False;
		self.building_game = True;
	
		#self.src_files[:] = []; # clear the lists.
		self.dll_files[:] = [];
		self.static_libraries[:] = [];
		
		self.mkdirs.extend([
			self.build_folder,
			self.build_folder + self.ds + self.arch,
			self.build_folder + self.ds + self.arch + self.ds + "src",
			self.build_folder + self.ds + "build-cache" # cache folder
			
		]);
		
	def gamePostInit(self):
	
		if (sys.platform == "win32"):
			self.build_artifact = self.build_folder + self.ds + self.game_name.replace(" ", "_") + ".exe";
		elif(sys.platform == "darwin"):
			self.build_artifact = self.build_folder + self.ds + self.game_name.replace(" ", "_");
			
			
	def createCacheFile(self, path):
		try:
			f = open(path, "r");
			f.close();
		except IOError as e:
			f = open(path, "w");
			f.write("{}");
			f.close();
		
	def startWindows(self):
		print("Hurray for windows");
		
		#prepare dirs
		for h in self.mkdirs:
			print("mkdir " + h);
			subprocess.call(["mkdir " + h], shell=True);
		
		# make sure cache file exists
		
		# compile cache thing
		cachefilename = "";
		if (self.building_game):
			cachefilename += self.game_dir + self.ds;
			
		cachefilename += self.build_folder + self.ds + "build-cache" + self.ds + "compiled.json";
		self.createCacheFile(cachefilename);
		f = open(cachefilename, "r")
		fcontents = f.read();
		f.close();
		fjson = json.loads(fcontents);
		fchanged = False;
		
		print("loaded build cache file: ");
		print(cachefilename);
			
		print("compiling");
		#print(self.src_files);
			
		#compile
		for h in self.src_files:
			compileStr = "";
			
			findex = h.rfind('.');
			h_ext = h[findex+1:len(h)];
			newf = h[0:findex] + ".o";
			
			if h_ext == 'c':
				compileStr += self.gccCompiler;
			elif h_ext == 'cpp':
				compileStr += self.gppCompiler;
			elif h_ext == 'rc':
				compileStr += "windres ";
				
			if (not h in fjson or fjson[h]['date_modified'] < os.stat(h).st_mtime):
				
				if (h_ext == 'c' or h_ext == 'cpp'):
					compileStr += " -O3 -Wall -c -fmessage-length=0 ";
					if (sys.platform == "darwin"): #compiling on mac
						compileStr += "-I /usr/X11/include "; 
						compileStr += " -march=i386 ";
						#-march=i386 "; # i386
						#-arch i386
					compileStr += " -o";
					compileStr += self.build_folder + self.ds + self.arch + self.ds + newf + " " + h + " ";
				elif h_ext == 'rc':
					compileStr += h + " " + self.build_folder + self.ds + self.arch + self.ds + newf + " ";
			
				fjson[h] = {"date_modified": os.stat(h).st_mtime };
			
				print(compileStr);
				subprocess.call([compileStr], shell=True);	
				fchanged = True;
					
					
	
		# update compile cache thing
		if (fchanged == True):
			f = open(self.build_folder + self.ds + "build-cache" + self.ds + "compiled.json", "w")
			f.write(json.dumps(fjson, sort_keys=True, indent=4));
			f.close();
	
		#link
		self.doLink();
		
		
	def doLink(self):
		
		print("linking binary");
		if (sys.platform=="win32"):
		 
			linkingStr = "";
			linkingStr += self.gppCompiler + " " + self.mingw_link + " " + self.linkingFlags + " -o" + self.build_artifact + "";
		
			for h in self.src_files:
				findex = h.rfind('.');
				newf = h[0:findex] + ".o";
				#print(newf);
				linkingStr += " " + self.build_folder + self.ds + self.arch + self.ds + newf;
			
			for f in self.dll_files:
				linkingStr += " " + f;
				
			for f in self.static_libraries:
				linkingStr += " -l" + f;
				
			print(linkingStr);
			
			subprocess.call([linkingStr], shell=True);	
			
		elif(sys.platform=="darwin"):
			
			if (self.building_library):
				linkingStr = "";
				linkingStr += self.gppCompiler + " -framework OpenGL -framework OpenAL -install_name @executable_path/../Frameworks/libARK2D.dylib" + self.linkingFlags + " -march=i386 -dynamiclib -o " + self.build_artifact;
			
				for h in self.src_files:
					findex = h.rfind('.');
					newf = h[0:findex] + ".o";
					linkingStr += " " + self.build_folder + self.ds + self.arch + self.ds + newf;
				
				for f in self.dll_files:
					linkingStr += " " + f;
					
				for f in self.static_libraries:
					linkingStr += " -l" + f;
					
				print(linkingStr);
				
				subprocess.call([linkingStr], shell=True);	
					
			elif(self.building_game):
				
				# we need to make the dirs for the .app file.
				gn = self.game_name.replace(' ', '\ ');
				app_folder = self.build_folder + self.ds + gn + ".app";
				contents_folder = app_folder + self.ds + "Contents";
				frameworks_folder = app_folder + self.ds + "Contents" + self.ds + "Frameworks";
				resources_folder = app_folder + self.ds + "Contents" + self.ds + "Resources";
				subprocess.call(['mkdir ' + app_folder], shell=True);
				subprocess.call(['mkdir ' + contents_folder ], shell=True);
				subprocess.call(['mkdir ' + contents_folder + self.ds + "MacOS"], shell=True);
				subprocess.call(['mkdir ' + resources_folder], shell=True);
				subprocess.call(['mkdir ' + frameworks_folder], shell=True);
				
				dylibsrc = self.ark2d_dir + self.ds + self.build_folder + self.ds + self.arch + self.ds + 'libARK2D.dylib'
				subprocess.call(['cp ' + dylibsrc + ' ' + frameworks_folder + self.ds + 'libARK2D.dylib'], shell=True);
				
				#copy icns in to .app folder
				subprocess.call(['cp ' + self.mac_game_icns + ' ' + resources_folder + self.ds + gn +'.icns'], shell=True);
				
				cr = "\r";
				infoplistcontents  = "";
				infoplistcontents += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + cr;
				infoplistcontents += "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" + cr;
				infoplistcontents += "<plist version=\"1.0\">" + cr;
				infoplistcontents += "	<dict>" + cr;
				infoplistcontents += "		<key>CFBundleDevelopmentRegion</key>" + cr;
				infoplistcontents += "		<string>English</string>" + cr;
				infoplistcontents += "		<key>CFBundleExecutable</key>" + cr;
				infoplistcontents += "		<string>" + self.game_name + "</string>" + cr;
				infoplistcontents += "		<key>CFBundleIdentifier</key>" + cr;
				infoplistcontents += "		<string>com.ark2d." + self.game_short_name + "</string>" + cr;
				infoplistcontents += "		<key>CFBundleIconFile</key>" + cr;
				infoplistcontents += "		<string>" + self.game_short_name + "</string>" + cr;
				infoplistcontents += "	</dict>" + cr;
				infoplistcontents += "</plist>" + cr;
				
				f = open(contents_folder.replace('\ ', ' ') + self.ds + "Info.plist", "w");
				f.write(infoplistcontents);
				f.close();
				
				self.build_artifact = app_folder + self.ds + "Contents" + self.ds + "MacOS" + self.ds + gn;
				
				linkingStr = "";
				linkingStr += self.gppCompiler + " -o " + self.build_artifact + " "; 
				for h in self.src_files:
					findex = h.rfind('.');
					newf = h[0:findex] + ".o";
					linkingStr += " " + self.build_folder + self.ds + self.arch + self.ds + newf;
				
				for f in self.dll_files:
					linkingStr += " " + f;
					
				for f in self.static_libraries:
					linkingStr += " -l" + f;
					
				print(linkingStr);
				subprocess.call([linkingStr], shell=True);	
				
				# update library search path for executable.
				#updatePathStr = "install_name_tool -change libARK2D.dylib @executable_path/../Frameworks/libARK2D.dylib " + self.game_name;
				#subprocess.call([updatePathStr], shell=True);	 
				
				#-march=i386 
				#-framework OpenAL
			
		
		
	def startMac(self):
		print("Hurray for Mac");
		self.startWindows();
		
	def clean(self):
		subprocess.call(["rm -r -d " + self.build_folder], shell=True);
			
	def start(self):
	
		if (sys.platform == "win32"):
			self.startWindows();
		elif(sys.platform == "darwin"):
			self.startMac();
		else:
			print(sys.platform); 
			print("platform " + sys.platform + " is not supported yet");

	
	pass;
	
		
if __name__ == "__main__":
	
	try:
		#print(sys.argv[1]);
		j = sys.argv[1];
		j = str(base64.b64decode(j.encode('latin-1')));
		j = str(j[2:len(j)-1]);
		#print(j);
		j = json.loads(j);#.replace("-", " "));
			
		a = ARK2DBuildSystem();
		a.ark2d_dir = j["ark2d_dir"]
		a.game_name = j["game_name"];
		a.game_short_name = j['game_short_name'];
		a.game_dir  = j["game_dir"];
		a.src_files.extend(j["game_src_files"]);
		a.mkdirs.extend(j['game_mkdirs']);
		a.build_artifact = "";
		
		a.clean();
		a.gamePreInit();
		
		a.mingw_link = "";
		
		if(sys.platform=="win32"):
			a.dll_files.append(a.ark2d_dir + a.ds + a.build_folder + a.ds + 'libARK2D.dll');
			a.linkingFlags += " -mwindows "; 
			a.linkingFlags += " -enable-auto-import ";
		elif(sys.platform=="darwin"):
			a.dll_files.append(a.ark2d_dir + a.ds + a.build_folder + a.ds + a.arch + a.ds + 'libARK2D.dylib');
			a.mac_game_icns = j['mac_game_icns'];
		
		a.gamePostInit();
		a.start();
		
		#copyCommand = "copy /y " + a.ark2d_dir + a.ds + a.build_folder + a.ds + 'libARK2D.dll \"' + a.game_dir + a.ds + a.build_folder + a.ds + 'libARK2D.dll\"';
		#print(copyCommand);
		#subprocess.call([copyCommand], shell=True);
		
		## 
		# TODO:
		##
		# 1) 
		# 	copy build_release\libARK2D.dll
		# 	copy lib\alut.dll
		# 	copy lib\glew32.dll
		# 	copy/merge data directory
		# 2)
		# 	when compiling games, check to see if the library needs updating and compile that too (before the game).
		##
		
	except IndexError as ex:
		a = ARK2DBuildSystem();
		a.dllInit();
		a.start();	
