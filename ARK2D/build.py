import sys
import os
import errno
import subprocess
import json 
import platform
import zlib
import base64
import binascii
import shutil
 
#
# Windows requires MinGW 3.4.5 with includes:
# 	AL/ 
#		al.h, alc.h, alext.h, alut.h
#	GL/
#		gl.h, glew.h, glext.h, glu.h, glxew.h, glxext.h, wglew.h, wglext.h
# 
#	... and probably some others which I don't remember right now.
# 
# 
# Python 2.7?
#	
#
# Mac requires you do not install OpenAL from their website 
#  - i.e. you should not have Library/Frameworks/OpenAL.framework
#  - Add Tools/ to path
# 		http://architectryan.com/2012/10/02/add-to-the-path-on-mac-os-x-mountain-lion/#.Uvn-EEJ_vPY
#

#
# Windows Phone builds...
# http://software.intel.com/en-us/articles/using-winrt-apis-from-desktop-applications
# 	-	Add /ZW
# 	-	Remove /Gm
#	
#	Add xaudio2.lib to library dependencies.
#	Add vendor/wp8/gl2dx to compile files.
#
# Windows 8 (vs) builds...
# 	Add /LIBPATH:folder (linker)
#		%(AdditionalLibraryDirectories); $(VCInstallDir)\lib\store; $(VCInstallDir)\lib
# 	Add libs to linker
#		d2d1.lib
#		d3d11.lib
#		dxgi.lib
#		dwrite.lib
#		advapi32.lib - for controllers
# 		user32.lib
#		gdi32.lib
#		shell32.lib
#

#
# Linux bits
# 	X11. 			sudo apt-get install libx11-dev
#	X11 Xinerama	sudo apt-get install libxinerama-dev
#	GL. 			sudo apt-get install mesa-common-dev
#	OpenAL. 		sudo apt-get install libalut-dev
#	cURL. 			sudo apt-get install libcurl4-openssl-dev
#	SDL2?			sudo apt-get install libsdl2-dev
#

# HTML5 / Emscripten bits. 
#	Tutorial: https://developer.mozilla.org/en-US/docs/Mozilla/Projects/Emscripten/Introducing/Emscripten_beginners_tutorial
#	Download & install: https://developer.mozilla.org/en-US/docs/Mozilla/Projects/Emscripten/Download_and_install
#	Run ./emsdk update
#	Run ./emsdk install latest
#	Run ./emsdk activate latest

# Build Line examples
# python /Users/ashleygwinnell/Dropbox/ark2d/ARK2D/build.py type=game dir=/Users/ashleygwinnell/Dropbox/Projects/C++/ToastTime/ spritesheets=true target=xcode

class ARK2DGame:
	def __init__(self):
		self.name = "";   
		self.name_safe = "";
		self.name_class = "";
		self.dir = "";
		self.dir_resources = "";
		# TODO:


class ARK2DBuildSystem:

	def __init__(self):
		
		self.building_library = False;
		self.building_game = True;
		
		self.ark2d_dir = "";

		#self.game = ARK2DGame();
		self.game_dir = "";
		self.game_name = "";
		self.game_short_name = "";
		self.game_resources_dir = "";
		self.libs = [];
		self.include_dirs = [];
		self.preprocessor_definitions = [];
		#self.mac_game_icns = "";

		self.build_folder = "build";
		self.arch = platform.machine();

		self.ouya = False;
		self.firetv = False;
		
		if ((len(sys.argv)==3 and sys.argv[2] == "android") or (len(sys.argv)==2 and sys.argv[1] == "android")):
			self.platform = "android";
			if (sys.platform == "win32"):
				self.platformOn = "windows";
				self.ds = "\\";
			elif(sys.platform == "darwin"):
				self.platformOn = "osx";
				self.ds = "/";
			pass;

		elif ((len(sys.argv)==3 and sys.argv[2] == "iphone") or (len(sys.argv)==2 and sys.argv[1] == "iphone")):
			self.platform = "ios";
			self.ds = "/";

			self.varPLATFORM = "iPhoneSimulator"; #"iPhoneSimulator"; #iPhoneOS
			self.varPLATFORMsml = "iphoneos"; #"iphonesimulator"; #iphoneos

			self.gccCompiler = "/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc-4.2"; #i686-apple-darwin11-llvm-gcc-4.2 ";
			self.gppCompiler = "/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/g++-4.2"; #i686-apple-darwin11-llvm-g++-4.2 ";
			self.objcCompiler = "/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/g++-4.2";# i686-apple-darwin11-llvm-g++-4.2 ";
			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + "libARK2D.dylib";

			self.platformOn = "osx";

			pass;
		elif (sys.platform == "win32"):
			self.ds = "\\";
			self.platform = "windows";
			self.mingw_dir = "C:\\MinGW";
			self.mingw_link = "-L" + self.mingw_dir + self.ds + "lib"
			self.gccCompiler = "gcc";
			self.gppCompiler = "g++";
			self.objcCompiler = "g++";
			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + "libARK2D.dll";
			
			self.platformOn = "windows";

		elif (sys.platform == "linux2"):
			self.ds = "/";
			self.platform = "linux";
			self.gccCompiler = "gcc";
			self.gppCompiler = "g++";
			self.platformOn = "linux";
			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + "libark2d.so";

		elif(sys.platform == "darwin"):
			self.ds = "/";
			self.platform = "osx";
			self.mingw_dir = ""; #/usr";
			self.mingw_link = ""; #-L" + self.mingw_dir + self.ds + "lib"
			
			# lion
			self.gccCompiler = "i686-apple-darwin11-llvm-gcc-4.2 ";
			self.gppCompiler = "i686-apple-darwin11-llvm-g++-4.2 ";
			self.objcCompiler = "i686-apple-darwin11-llvm-g++-4.2 ";

			# mountain lion
			#self.gccCompiler = "/Developer/usr/llvm-gcc-4.2/bin/i686-apple-darwin11-llvm-gcc-4.2 ";
			#self.gppCompiler = "/Developer/usr/llvm-gcc-4.2/bin/i686-apple-darwin11-llvm-g++-4.2 ";
			#self.objcCompiler = "/Developer/usr/llvm-gcc-4.2/bin/i686-apple-darwin11-llvm-g++-4.2 ";
			
			#self.gccCompiler = "/Developer/usr/bin/i686-apple-darwin11-gcc-4.2.1 ";
			#self.gppCompiler = "/Developer/usr/bin/i686-apple-darwin11-g++-4.2.1 ";
			#self.objcCompiler = "/Developer/usr/bin/i686-apple-darwin11-g++-4.2.1 ";

			# apple break things every release. run this if 
			# sudo ln -s /Developer/SDKs/MacOSX10.6.sdk/usr/lib/crt1.10.6.o /Developer/usr/llvm-gcc-4.2/lib
			

			#self.gccCompiler = "llvm-gcc-4.2";
			#self.gppCompiler = "llvm-g++-4.2";
			#self.gccCompiler = "gcc";
			#self.gppCompiler = "g++";

			self.platformOn = "osx";

			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + "libARK2D.dylib";
			self.mac_game_icns = '';
		
		self.windresources = [];
		
		self.mkdirs = [];
		self.game_mkdirs = [];
		self.src_files = [];
		self.dll_files = [];
		self.static_libraries = [];
		self.linkingFlags = "";
	
	def getTargetPlatform(self):
		if ((len(sys.argv)==3 and sys.argv[2] == "android") or (len(sys.argv)==2 and sys.argv[1] == "android")):
			return "android";
		elif ((len(sys.argv)==3 and sys.argv[2] == "iphone") or (len(sys.argv)==2 and sys.argv[1] == "iphone")):
			return "ios";
		elif (sys.platform == "win32"):
			return "windows";
		elif(sys.platform == "darwin"):
			return "osx";
		return "error";

	def dllInit(self):
		
		config = self.config;
		
		self.building_library = True;
		self.building_game = False;
		
		""" TODO: put this in config.""" 
		self.mkdirs.extend([
			self.build_folder,
			self.build_folder + self.ds + self.output,
			self.build_folder + self.ds + self.output + self.ds + "build-cache", # cache folder
			self.build_folder + self.ds + self.output + self.ds + "src",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D", 
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Audio",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Controls",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Core",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Core" + self.ds + "Platform",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Font",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Geometry",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "GJ",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "GJ" + self.ds + "Next",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Graphics",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Graphics" + self.ds + "HLSL",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Graphics" + self.ds + "ImageIO",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Graphics" + self.ds + "Shaders",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Net",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Particles",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Path",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Pathfinding",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "SceneGraph",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "State",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "State" + self.ds + "Transition",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tests",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Threading",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tiled",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tools",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tween",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "UI",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Util",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Util" + self.ds + "Containers",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "angelscript",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "angelscript" + self.ds + "add_on",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "angelscript" + self.ds + "add_on" + self.ds + "scriptarray",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "angelscript" + self.ds + "add_on" + self.ds + "scriptbuilder",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "angelscript" + self.ds + "add_on" + self.ds + "scriptstdstring",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "libJSON",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "lpng151",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "ogg130",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "tinyxml",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "spine",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "spine" + self.ds + "src",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "vorbis132",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "vorbis132" + self.ds + "modes",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "utf8",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "utf8" + self.ds + "utf8",
			self.build_folder + self.ds + self.output + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "zlib123"
		]);
		
		
		#self.src_files.extend(config['src_files']['all']);
		
		if (self.platform == "flascc"):
			self.src_files.extend(config['src_files']['flascc']);
			self.ark2d_dir = config[self.platformOn]['ark2d_dir'];

		elif (self.platform == "android"):
			self.src_files.extend(config['src_files']['android']);
			self.ark2d_dir = config[self.platformOn]['ark2d_dir'];

		elif (self.platform == "linux"):
			self.src_files.extend(config['src_files']['linux']);
			self.ark2d_dir = config[self.platformOn]['ark2d_dir'];
			self.config = config;

		elif (self.platform == "html5"):
			self.src_files.extend(config['src_files']['html5']);
			self.ark2d_dir = config[self.platformOn]['ark2d_dir'];
			self.config = config;

		elif (self.platform == "ios"):
			self.src_files.extend(config['src_files']['ios']);
			self.ark2d_dir = config["osx"]['ark2d_dir'];

			self.static_libraries.extend(config['static_libraries']['ios']);
		elif (self.platform == "windows-old"):
		
			self.src_files.extend(config['src_files']['windows']);
			self.dll_files.extend(config['dynamic_libraries']['windows']);
			self.static_libraries.extend(config['static_libraries']['windows']);
			self.linkingFlags = " -mwindows -shared ";
			
			self.ark2d_dir = config["windows"]['ark2d_dir'];
			self.ark2d_tmpdir = "C:\\a2d\\l";

		elif (self.platform=="windows"):
			self.src_files.extend(config['src_files']['windows']);
			self.dll_files.extend(config['dynamic_libraries']['windows']);
			self.static_libraries.extend(config['static_libraries']['windows']);
			
			#self.ark2d_dir = ""; #config["windows"]['ark2d_dir']; 
			self.ark2d_tmpdir = "C:\\a2dvs\\l";

		elif (self.platform=="windows-phone"):
			self.src_files.extend(config['src_files']['windows-phone']);
			#self.dll_files.extend(config['dynamic_libraries']['windows']);
			#self.static_libraries.extend(config['static_libraries']['windows']);
			
			self.ark2d_dir = config["windows"]['ark2d_dir']; 
			self.ark2d_tmpdir = "C:\\a2dwpvs\\l";
				
		elif (self.platform == "osx"):
			
			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + "libARK2D.dylib";
			
			self.src_files.extend(config['src_files']['osx']);
			self.dll_files.extend(config['dynamic_libraries']['osx']);
			self.static_libraries.extend(config['static_libraries']['osx']);
			self.linkingFlags = "";

			self.ark2d_dir = config["osx"]['ark2d_dir'];
			self.ark2d_tmpdir = "/ark2d/lib";

		elif (self.platform == "xcode"):
			
			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + "libARK2D.dylib";
			
			self.src_files.extend(config['src_files']['osx']);
			self.dll_files.extend(config['dynamic_libraries']['osx']);
			self.static_libraries.extend(config['static_libraries']['osx']);
			self.linkingFlags = "";

			self.ark2d_dir = config["osx"]['ark2d_dir'];
			self.ark2d_tmpdir = "/ark2d/lib";

		self.game_dir = self.ark2d_dir;
		self.src_files.extend(config['src_files']['all']);
		
	
		
	def gamePreInit(self):
	
		self.building_library = False;
		self.building_game = True;
	
		#self.src_files[:] = []; # clear the lists.
		self.dll_files[:] = [];
		self.static_libraries[:] = [];
		
		#self.build_folder = self.game_dir + "/" + self.build_folder;

		self.mkdirs.extend([
			self.build_folder,
			self.build_folder + self.ds + self.output,
			self.build_folder + self.ds + self.output + self.ds + "src",
			self.build_folder + self.ds + self.output + self.ds + "build-cache" # cache folder
			
		]);
		self.mkdirs.extend(self.game_mkdirs);
		
		if (self.platform == "windows-old"):
			self.ark2d_tmpdir = "C:\\a2d\\gm";
		else:
			self.ark2d_tmpdir = "/ark2d/gm";
		
	def gamePostInit(self):
	
		if (sys.platform == "win32"):
			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + self.game_name.replace(" ", "_") + ".exe";
		elif(sys.platform == "darwin"):
			self.build_artifact = self.build_folder + self.ds + self.game_name.replace(" ", "_");

		"""
		# open config
		f = open(self.game_dir + "/config.json", "r");
		fcontents = f.read();
		f.close();
		config = json.loads(fcontents);

		#create ARK.h file
		self.generateARKH(self.game_dir + self.ds + "src" + self.ds + "ARK.h");

		self.config = config;
		"""
			


	def generateARKH(self, floc):
		"""if (self.newconfig == True):
			# rewrite this for compatibility with new config layout.
			# must take into account all config files somehow...
			# do something like...
			# 	#include ARK_INCLUDE_H
			# and set this variable here in the build script depending on the platform!?
			print("Generating ARK.h file at " + floc);
			nl = " \r\n"; 
			if (sys.platform == "win32"):
				nl = "\n"; 

			arkh = " #include <ARK.h> " + nl;
			f = open(floc, "w");
			f.write(arkh);
			f.close();

		else:
		"""

		print("Generating ARK.h file at " + floc);
		nl = " \r\n"; 
		if (sys.platform == "win32"):
			nl = "\n"; 

		arkh  = ""; 
		arkh += "#if defined(ARK2D_WINDOWS) || defined(ARK2D_ANDROID_ON_WINDOWS) || defined(ARK2D_WINDOWS_PHONE_8) " + nl;
		arkh += "	#include \"" + self.str_replace(self.ark_config['windows']['ark2d_dir'], [("\\", "\\\\")]) + "\\\\src\\\\ARK.h\"" + nl;
		arkh += "#elif defined(ARK2D_MACINTOSH) || defined(ARK2D_ANDROID_ON_MACINTOSH) || defined(ARK2D_EMSCRIPTEN_JS_ON_MACINTOSH) || defined(ARK2D_IPHONE) " + nl;
		arkh += "	#include \"" + self.ark_config['osx']['ark2d_dir'] + "/src/ARK.h\"" + nl;
		arkh += "#elif defined(ARK2D_UBUNTU_LINUX) " + nl;
		#if "linux" in config:
		arkh += "	#include \"" + self.ark_config['linux']['ark2d_dir'] + "/src/ARK.h\"" + nl;
		arkh += "#endif" + nl;
		f = open(floc, "w");
		f.write(arkh);
		f.close();

		pass;
			
			
	def createCacheFile(self, path):
		try:
			f = open(path, "r");
			f.close();
		except IOError as e:
			f = open(path, "w");
			f.write("{}");
			f.close();
			
	def openCacheFile(self, file):
		self.createCacheFile(file);
		f = open(file, "r")
		fcontents = f.read();
		f.close();
		fjson = json.loads(fcontents);
		return fjson;
			
	def rmdir_recursive(self, dir):
	    """Remove a directory, and all its contents if it is not already empty."""
	    for name in os.listdir(dir):
	        full_name = os.path.join(dir, name);
	        # on Windows, if we don't have write permission we can't remove
	        # the file/directory either, so turn that on
	        if not os.access(full_name, os.W_OK):
	           # os.chmod(full_name, 0600);
	           pass;
	        if os.path.isdir(full_name):
	            self.rmdir_recursive(full_name);
	        else:
	            os.remove(full_name);
	    os.rmdir(dir);
	    
	def get_str_extension(self, str):
		findex = str.rfind('.');
		h_ext = str[findex+1:len(str)];
		return h_ext;

	def get_str_filename(self, str):
		findex = str.rfind(self.ds);
		filename = str[findex+1:len(str)];
		return filename;

	def get_str_filename2(self, str):
		findex = str.rfind("/");
		filename = str[findex+1:len(str)];
		return filename;

	def is_image_extension(self, ext):
		if (ext == "png" or ext == "tga" or ext == "bmp" or ext == "gif" or ext == "jpg" or ext == "pkm_png"):
			return True;
		return False;

	def is_audio_extension(self, ext):
		if (ext == "ogg" or ext == "mp3" or ext == "wav"):
			return True;
		return False;
	
	def startWindowsPhone(self):
		# open config
		f = open(self.ark2d_dir + "/config.json", "r");
		fcontents = f.read();
		f.close(); 
		self.config = json.loads(fcontents);

		if (self.building_library):
			print("Building Windows Phone dll.");

			# output directory
			output_folder = self.ark2d_dir + "/build/windows-phone";

			# make directories
			mkdirs = [];
			mkdirs.extend(self.mkdirs);
			self.makeDirectories(mkdirs);

			sln_contents = ""; ####
			vcxproj_contents = "";

			# dll sln and vcxproj files
			f1 = open(self.ark2d_dir + "/lib/windows-phone/dll-template/dll-template.sln", "r");
			f2 = open(self.ark2d_dir + "/lib/windows-phone/dll-template/dll-template.vcxproj", "r");
			sln_contents = f1.read();
			vcxproj_contents = f2.read();
			f1.close(); 
			f2.close(); 

			# modify sln/vcxproj files
			vcxproj_headerfiles = "";
			vcxproj_sourcefiles = "";
			for srcfile in self.src_files:  

				if "lpng151" in srcfile: 
					continue; 

				#check if src file has a corresponding .h file. add that to the project...
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newfh = srcfile[0:findex] + ".h";
				newfhfull = self.ark2d_dir + self.ds + newfh; 
				if (os.path.exists(newfhfull)): 
					vcxproj_headerfiles += "<ClInclude Include=\"../../"+newfh+"\" /> \n";

				if "lpng1251" in srcfile: 
					continue; 
				if "zlib123" in srcfile: 
					continue; 
				
				if h_ext == "c":
					vcxproj_sourcefiles += "<ClCompile Include=\"../../"+srcfile+"\" > \n";
					vcxproj_sourcefiles += "<CompileAsWinRT Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</CompileAsWinRT>\n";
					vcxproj_sourcefiles += "<CompileAsWinRT Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">false</CompileAsWinRT>\n";
					vcxproj_sourcefiles += "<CompileAsWinRT Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">false</CompileAsWinRT>\n";
					vcxproj_sourcefiles += "<CompileAsWinRT Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">false</CompileAsWinRT>\n";
 
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_WINDOWS_DLL;ARK2D_DEBUG;PSAPI_VERSION=2;WINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP;_UITHREADCTXT_SUPPORT=0;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_WINDOWS_DLL;PSAPI_VERSION=2;WINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP;_UITHREADCTXT_SUPPORT=0;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_WINDOWS_DLL;ARK2D_DEBUG;PSAPI_VERSION=2;WINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP;_UITHREADCTXT_SUPPORT=0;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_WINDOWS_DLL;PSAPI_VERSION=2;WINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP;_UITHREADCTXT_SUPPORT=0;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";

					#if "ogg130" in srcfile: 
					#	continue; 
					#if "vorbis132" in srcfile: 
					#	continue; 

					vcxproj_sourcefiles += "</ClCompile> \n";
				elif h_ext == "hlsl":
					hlsltype = "";
					if "pixel" in srcfile:
						hlsltype = "Pixel";
					elif "vertex" in srcfile:
						hlsltype = "Vertex";

					vcxproj_sourcefiles += "	<FxCompile Include=\"../../"+srcfile+"\"> \n ";
					vcxproj_sourcefiles += "		<FileType>Document</FileType> \n";
					vcxproj_sourcefiles += "		<DisableOptimizations Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</DisableOptimizations>\n"; # was true...
					vcxproj_sourcefiles += "		<DisableOptimizations Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">false</DisableOptimizations>\n";
					vcxproj_sourcefiles += "		<EnableDebuggingInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</EnableDebuggingInformation>\n";
					vcxproj_sourcefiles += "		<EnableDebuggingInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">false</EnableDebuggingInformation>\n";
					vcxproj_sourcefiles += "		<ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">" + hlsltype + "</ShaderType>\n";
					vcxproj_sourcefiles += "		<ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">" + hlsltype + "</ShaderType>\n";
					vcxproj_sourcefiles += "		<ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">" + hlsltype + "</ShaderType>\n";
					vcxproj_sourcefiles += "		<ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">" + hlsltype + "</ShaderType>\n";
					vcxproj_sourcefiles += "	</FxCompile> \n";
				
				elif h_ext == "cpp":
					vcxproj_sourcefiles += "<ClCompile Include=\"../../"+srcfile+"\" > \n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_WINDOWS_DLL;ARK2D_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_WINDOWS_DLL;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_WINDOWS_DLL;ARK2D_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_WINDOWS_DLL;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";

					vcxproj_sourcefiles += "</ClCompile> \n";

				
				else:
					vcxproj_sourcefiles += "<ClCompile Include=\"../../"+srcfile+"\" /> \n";

				

			vcxproj_contents = self.str_replace(vcxproj_contents, [("%COMPILE_HEADER_FILES%", vcxproj_headerfiles)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%COMPILE_SOURCE_FILES%", vcxproj_sourcefiles)]);

			# write sln file
			print("Write sln file...");
			f1 = open(output_folder + "/ARK2D.sln", "w");
			f1.write(sln_contents);
			f1.close();

			# write vcxproj file
			print("Write vcxproj file...");
			f1 = open(output_folder + "/ARK2D.vcxproj", "w");
			f1.write(vcxproj_contents);
			f1.close();

			# copy pch.h file/s
			print("Write pch.cpp file...");
			f1 = open(self.ark2d_dir + "/lib/windows-phone/dll-template/pch.cpp", "r");
			pch_cpp_contents = f1.read();
			f1.close(); 
			f1 = open(output_folder + "/pch.cpp", "w");
			f1.write(pch_cpp_contents);
			f1.close();

			# copy pch.h file/s
			print("Write pch.h file...");
			f1 = open(self.ark2d_dir + "/lib/windows-phone/dll-template/pch.h", "r");
			pch_h_contents = f1.read();
			f1.close(); 
			f1 = open(output_folder + "/pch.h", "w");
			f1.write(pch_h_contents);
			f1.close();

			# copy target versionh file/s
			print("Write targetver.h file...");
			f1 = open(self.ark2d_dir + "/lib/windows-phone/dll-template/targetver.h", "r");
			targetver_h_contents = f1.read();
			f1.close(); 
			f1 = open(output_folder + "/targetver.h", "w");
			f1.write(targetver_h_contents);
			f1.close(); 
			
			

		else:
			print("Building Windows Phone game.");

			# output directory
			output_folder = self.game_dir + "/build/" + self.output; 
			game_name = self.game_name; #['game_name'];
			game_name_safe = self.game_name_safe; #config['game_name_safe'];
			game_short_name = self.game_class_name; #config['game_short_name'];
			game_description = self.game_description; #config['game_description'];
			game_resources_dir = self.game_resources_dir; #config['windows']['game_resources_dir'];


			# make directories
			print("Making directories...");
			mkdirs = [];
			mkdirs.extend(self.mkdirs);
			self.makeDirectories(mkdirs);

			sln_contents = "";
			vcxproj_contents = "";

			windowsPhone81 = True;
			projectTemplateFolder = "project-template";
			projectTemplateClassName = "WindowsPhone8Game";
			projectTemplateManifest = "WMAppManifest.xml";
			projectTemplateShortFolder = "wp8";
			if windowsPhone81 == True:
				projectTemplateFolder = "project-template-wp81";
				projectTemplateClassName = "WindowsPhone81Game";
				projectTemplateManifest = "Package.appxmanifest";
				projectTemplateShortFolder = "wp81";


			# dll sln and vcxproj files 
			print("Making sln and vcxproj...");
			f1 = open(self.ark2d_dir + "/lib/windows-phone/" + projectTemplateFolder + "/" + projectTemplateFolder + ".sln", "r");
			f2 = open(self.ark2d_dir + "/lib/windows-phone/" + projectTemplateFolder + "/" + projectTemplateFolder + ".vcxproj", "r");
			sln_contents = f1.read();#.encode('ascii');
			vcxproj_contents = f2.read();
			f1.close(); 
			f2.close(); 

			# modify sln strings
			print("Configuring sln file...");
			sln_contents = self.str_replace(sln_contents, [("%GAME_SHORT_NAME%", game_short_name)]);
			sln_contents = self.str_replace(sln_contents, [("%GAME_NAME_SAFE%", game_name_safe)]);



			# resources to copy to game project. gotta do this early to generate the VS project
			print("sort game resources"); 
			game_resources_list = [];
			image_resources = [];
			audio_resources = [];
			document_resources = [];
			filesToCopy = self.listFiles(game_resources_dir, False);
			#print(filesToCopy);
			for file in filesToCopy:
				fromfile = game_resources_dir + self.ds + file;
				tofile = output_folder + "/data/" + file;

				game_resources_list.extend(["data\\" + file]);

				file_ext = self.get_str_extension(file);
				if (self.is_image_extension(file_ext)):
					image_resources.extend(["data\\" + file]);
				elif (self.is_audio_extension(file_ext)):
					# what do we do when it's an audio file idon't know?!
					pass;
				else:
					document_resources.extend(["data\\" + file]);


			# make list of dlls/libs
			extra_dlls_arm = [];
			extra_libs_arm = [];
			extra_dlls_x86 = [];
			extra_libs_x86 = [];

			if "native_libraries" in self.wp8_config:
				for native_lib in self.wp8_config['native_libraries']:
					print("Adding native library: " + native_lib['name']);
					if "arm" in native_lib:
						for native_lib_arm in native_lib['arm']:
							if (self.get_str_extension(native_lib_arm) == "dll"):
								extra_dlls_arm.extend([native_lib_arm]);
							elif (self.get_str_extension(native_lib_arm) == "lib"):
								extra_libs_arm.extend([native_lib_arm]);

					if "x86" in native_lib:
						for native_lib_x86 in native_lib['x86']:
							if (self.get_str_extension(native_lib_x86) == "dll"):
								extra_dlls_x86.extend([native_lib_x86]);
							elif (self.get_str_extension(native_lib_x86) == "lib"):
								extra_libs_x86.extend([native_lib_x86]);

			# print(extra_dlls_arm);
			# print(extra_libs_arm);

			# list of game resources in .data dir 
			game_resources_str = "";
			game_image_resources_str = "";
			if len(game_resources_list) > 0:
				print("Generating resource list...");
				for item in game_resources_list:

					file_ext = self.get_str_extension(item);
					if (self.is_image_extension(file_ext)):
						game_image_resources_str += "<Image Include=\"" + item + "\" /> \n";
					else:
						game_resources_str += "<None Include=\"" + item + "\"> \n";
						game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">true</DeploymentContent> \n";
						game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">true</DeploymentContent> \n";
						game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">true</DeploymentContent> \n";
						game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">true</DeploymentContent> \n";
						game_resources_str += "</None> \n";

			# need to put dlls in here too. 
			for nativelib in extra_dlls_arm:
				nativelib_name = self.get_str_filename2(nativelib);
				game_resources_str += "<None Include=\"" + nativelib_name + "\"> \n";
				game_resources_str += "	<FileType>Document</FileType> \n";
				game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">true</DeploymentContent> \n";
				game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">true</DeploymentContent> \n";
				game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</DeploymentContent> \n";
				game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">false</DeploymentContent> \n";
				game_resources_str += "</None> \n";

			for nativelib in extra_dlls_x86:
				nativelib_name = self.get_str_filename2(nativelib);
				game_resources_str += "<None Include=\"" + nativelib_name + "\"> \n";
				game_resources_str += "	<FileType>Document</FileType> \n";
				game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">false</DeploymentContent> \n";
				game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">false</DeploymentContent> \n";
				game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">true</DeploymentContent> \n";
				game_resources_str += "	<DeploymentContent Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">true</DeploymentContent> \n";
				game_resources_str += "</None> \n";


			# list of sourcefiles
			print("Creating list of Source Files...");
			vcxproj_headerfiles = "";
			vcxproj_sourcefiles = "";
			for srcfile in self.src_files:  

				#check if src file has a corresponding .h file. add that to the project...
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newfh = srcfile[0:findex] + ".h";
				newfhfull = self.ark2d_dir + self.ds + newfh; 
				if (os.path.exists(newfhfull)): 
					vcxproj_headerfiles += "<ClInclude Include=\"../../"+newfh+"\" /> \n";

				if h_ext == "c":
					vcxproj_sourcefiles += "<ClCompile Include=\"../../"+srcfile+"\" > \n";
					vcxproj_sourcefiles += "<CompileAsWinRT Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</CompileAsWinRT>\n";
					vcxproj_sourcefiles += "<CompileAsWinRT Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">false</CompileAsWinRT>\n";
					vcxproj_sourcefiles += "<CompileAsWinRT Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">false</CompileAsWinRT>\n";
					vcxproj_sourcefiles += "<CompileAsWinRT Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">false</CompileAsWinRT>\n";
 
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_DEBUG;PSAPI_VERSION=2;WINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP;_UITHREADCTXT_SUPPORT=0;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;PSAPI_VERSION=2;WINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP;_UITHREADCTXT_SUPPORT=0;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_DEBUG;PSAPI_VERSION=2;WINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP;_UITHREADCTXT_SUPPORT=0;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;PSAPI_VERSION=2;WINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP;_UITHREADCTXT_SUPPORT=0;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";

					vcxproj_sourcefiles += "</ClCompile> \n";
				elif h_ext == "hlsl":
					hlsltype = "";
					if "pixel" in srcfile:
						hlsltype = "Pixel";
					elif "vertex" in srcfile:
						hlsltype = "Vertex";

					vcxproj_sourcefiles += "	<FxCompile Include=\"../../"+srcfile+"\"> \n ";
					vcxproj_sourcefiles += "		<FileType>Document</FileType> \n";
					vcxproj_sourcefiles += "		<DisableOptimizations Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</DisableOptimizations>\n"; # was true...
					vcxproj_sourcefiles += "		<DisableOptimizations Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">false</DisableOptimizations>\n";
					vcxproj_sourcefiles += "		<EnableDebuggingInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</EnableDebuggingInformation>\n";
					vcxproj_sourcefiles += "		<EnableDebuggingInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">false</EnableDebuggingInformation>\n";
					vcxproj_sourcefiles += "		<ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">" + hlsltype + "</ShaderType>\n";
					vcxproj_sourcefiles += "		<ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">" + hlsltype + "</ShaderType>\n";
					vcxproj_sourcefiles += "		<ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">" + hlsltype + "</ShaderType>\n";
					vcxproj_sourcefiles += "		<ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">" + hlsltype + "</ShaderType>\n";
					vcxproj_sourcefiles += "	</FxCompile> \n";
				
				elif h_ext == "cpp":
					vcxproj_sourcefiles += "<ClCompile Include=\"../../"+srcfile+"\" > \n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Debug|ARM'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;ARK2D_DEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";
					vcxproj_sourcefiles += "<PreprocessorDefinitions Condition=\"'$(Configuration)|$(Platform)'=='Release|ARM'\">_CRT_SECURE_NO_WARNINGS;ARK2D_WINDOWS_PHONE_8;%(PreprocessorDefinitions)</PreprocessorDefinitions>\n";

					vcxproj_sourcefiles += "</ClCompile> \n";

					
				else:
					vcxproj_sourcefiles += "<ClCompile Include=\"../../"+srcfile+"\" /> \n";


			print("add libs")

			# extra include dirs
			vcxproj_AdditionalIncludeDirs = ""; 
			for includedir in self.include_dirs: 
				includedir_actual = self.str_replace(includedir, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
				vcxproj_AdditionalIncludeDirs += includedir_actual + ";";

			# extra defines
			vcxproj_AdditionalPreprocessorDefinitions = "";
			for ppdefinition in self.preprocessor_definitions:
				vcxproj_AdditionalPreprocessorDefinitions += ppdefinition + ";";

			# additional lib files
			vcxproj_AdditionalX86DotLibs = "";
			for extra_lib in extra_libs_x86:
				extra_lib2 = self.str_replace(extra_lib, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%GAME_DIR%", self.game_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
				extra_lib2 = self.fixLocalPath(extra_lib2);
				vcxproj_AdditionalX86DotLibs += ";" + extra_lib2;

			vcxproj_AdditionalARMDotLibs = "";
			for extra_lib in extra_libs_arm:
				extra_lib2 = self.str_replace(extra_lib, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%GAME_DIR%", self.game_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
				extra_lib2 = self.fixLocalPath(extra_lib2);

				vcxproj_AdditionalARMDotLibs += ";" + extra_lib2;

			
			# modify vcxproj strings
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%GAME_SHORT_NAME%", game_short_name)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%GAME_NAME_SAFE%", game_name_safe)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%GAME_RESOURCES%", game_resources_str)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%GAME_IMAGE_RESOURCES%", game_image_resources_str)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%COMPILE_HEADER_FILES%", vcxproj_headerfiles)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%COMPILE_SOURCE_FILES%", vcxproj_sourcefiles)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%ADDITIONAL_INCLUDE_DIRECTORIES%", vcxproj_AdditionalIncludeDirs)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%ADDITIONAL_PREPROCESSOR_DEFINITIONS%", vcxproj_AdditionalPreprocessorDefinitions)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%ADDITIONAL_DOTLIB_FILES_X86%", vcxproj_AdditionalX86DotLibs)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%ADDITIONAL_DOTLIB_FILES_ARM%", vcxproj_AdditionalARMDotLibs)]);

			# write sln file
			print("Write sln file...");
			f1 = open(output_folder + "/" + game_name_safe + ".sln", "w");
			f1.write(sln_contents);
			f1.close();

			# write vcxproj file
			print("Write vcxproj file...");
			f1 = open(output_folder + "/" + game_name_safe + ".vcxproj", "w");
			f1.write(vcxproj_contents);
			f1.close();

			# copy pch.h file/s
			print("Write pch.cpp file...");
			f1 = open(self.ark2d_dir + "/lib/windows-phone/" + projectTemplateFolder + "/pch.cpp", "r");
			pch_cpp_contents = f1.read();
			f1.close(); 
			f1 = open(output_folder + "/pch.cpp", "w");
			f1.write(pch_cpp_contents);
			f1.close();

			# copy pch.h file/s
			print("Write pch.h file...");
			f1 = open(self.ark2d_dir + "/lib/windows-phone/" + projectTemplateFolder + "/pch.h", "r");
			pch_h_contents = f1.read();
			f1.close(); 
			f1 = open(output_folder + "/pch.h", "w");
			f1.write(pch_h_contents);
			f1.close();

			# icons
			wp8_iconTypes = [
				'square_44x44_scale100',
				'square_44x44_scale140',
				'square_44x44_scale240',
				'square_71x71_scale100',
				'square_71x71_scale140',
				'square_71x71_scale240',
				'square_150x150_scale100',
				'square_150x150_scale140',
				'square_150x150_scale240',
				'wide_310x150_scale100',
				'wide_310x150_scale140',
				'wide_310x150_scale240',
				'store_scale100',
				'store_scale140',
				'store_scale240',
				'badge_scale100',
				'badge_scale140',
				'badge_scale240',
				'splash_scale100',
				'splash_scale140',
				'splash_scale240'
			];
			wp8_iconTypeMap = {
				"square_44x44_scale100" : "SmallLogo.scale-100.png",
				"square_44x44_scale140" : "SmallLogo.scale-140.png",
				"square_44x44_scale240" : "SmallLogo.scale-240.png",
				"square_71x71_scale100" : "Square71x71Logo.scale-100.png",
				"square_71x71_scale140" : "Square71x71Logo.scale-140.png",
				"square_71x71_scale240" : "Square71x71Logo.scale-240.png",
				"square_150x150_scale100" : "Logo.scale-100.png",
				"square_150x150_scale140" : "Logo.scale-140.png",
				"square_150x150_scale240" : "Logo.scale-240.png",
				"wide_310x150_scale100" : "WideLogo.scale-100.png",
				"wide_310x150_scale140" : "WideLogo.scale-140.png",
				"wide_310x150_scale240" : "WideLogo.scale-240.png",
				"store_scale100" : "StoreLogo.scale-100.png",
				"store_scale140" : "StoreLogo.scale-140.png",
				"store_scale240" : "StoreLogo.scale-240.png",
				"badge_scale100" : "BadgeLogo.scale-100.png",
				"badge_scale140" : "BadgeLogo.scale-140.png",
				"badge_scale240" : "BadgeLogo.scale-240.png",
				"splash_scale100" : "SplashScreen.scale-100.png",
				"splash_scale140" : "SplashScreen.scale-140.png",
				"splash_scale240" : "SplashScreen.scale-240.png"
			};

			# manifest file
			print("Create " + projectTemplateManifest + " file...");
			f1 = open(self.ark2d_dir + "/lib/windows-phone/" + projectTemplateFolder + "/" + projectTemplateManifest, "r");
			pch_h_contents = f1.read();
			f1.close(); 
  
			pch_h_contents = self.str_replace(pch_h_contents, [("%GAME_NAME%", game_name)]);
			pch_h_contents = self.str_replace(pch_h_contents, [("%GAME_SHORT_NAME%", game_short_name)]);
			pch_h_contents = self.str_replace(pch_h_contents, [("%GAME_NAME_SAFE%", game_name_safe)]);
			pch_h_contents = self.str_replace(pch_h_contents, [("%GAME_DESCRIPTION%", game_description)]);
			pch_h_contents = self.str_replace(pch_h_contents, [("%COMPANY_NAME%", self.developer_name)]);
			pch_h_contents = self.str_replace(pch_h_contents, [("%COMPANY_NAME_SAFE%", self.developer_name_safe)]);
			pch_h_contents = self.str_replace(pch_h_contents, [("%GAME_ORIENTATION%", self.game_orientation)]);
			pch_h_contents = self.str_replace(pch_h_contents, [("%GAME_CLEAR_COLOR%", self.game_clear_color)]);

		



			f1 = open(output_folder + "/" + projectTemplateManifest, "w"); 
			f1.write(pch_h_contents);
			f1.close();

			# win8 game source file
			print("Create WP8 Game Class...");
			f1 = open(self.ark2d_dir + "/lib/windows-phone/" + projectTemplateFolder + "/" + projectTemplateClassName + ".cpp", "r");
			game_cpp_contents = f1.read();
			f1.close(); 

			game_cpp_contents = self.str_replace(game_cpp_contents, [("%GAME_NAME%", game_name)]);
			game_cpp_contents = self.str_replace(game_cpp_contents, [("%GAME_CLASS_NAME%", self.game_class_name)]);
			game_cpp_contents = self.str_replace(game_cpp_contents, [("%GAME_CLEAR_COLOR%", self.game_clear_color)]);
			game_cpp_contents = self.str_replace(game_cpp_contents, [("%GAME_WIDTH%", str(self.wp8_config['game_width']))]);
			game_cpp_contents = self.str_replace(game_cpp_contents, [("%GAME_HEIGHT%", str(self.wp8_config['game_height']))]);

			f1 = open(output_folder + "/" + projectTemplateClassName + ".cpp", "w");
			f1.write(game_cpp_contents);
			f1.close();

			# win8 game source file
			print("Create WP8 Game Header...");
			f1 = open(self.ark2d_dir + "/lib/windows-phone/" + projectTemplateFolder + "/" + projectTemplateClassName + ".h", "r");
			pch_h_contents = f1.read();
			f1.close(); 
			f1 = open(output_folder + "/" + projectTemplateClassName + ".h", "w");
			f1.write(pch_h_contents);
			f1.close();

			# copy resources in to wp8 game project folder.
			print("Copying data in...");
			shutil.copytree(game_resources_dir, output_folder + "/data/");

			# copy ark2d.dll in 
			print("Copying ARK2D data in..."); 

			# WP8 Live tiles
			shutil.copytree(self.ark2d_dir + "\\lib\\windows-phone\\" + projectTemplateFolder + "\\" + projectTemplateShortFolder, output_folder + "/data/ark2d/" + projectTemplateShortFolder + "/");

				# custom icon / live tile
			if "icon" in self.wp8_config:
				for icontype in wp8_iconTypes:
					if icontype in self.wp8_config['icon']:
						icontype_full = self.wp8_config['icon'][icontype];
						icontype_filename = wp8_iconTypeMap[icontype]; #self.get_str_filename2(icontype_name);
						icontype_full = self.str_replace(icontype_full, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%GAME_DIR%", self.game_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
						shutil.copy(icontype_full, output_folder + "/data/ark2d/" + projectTemplateShortFolder + "/" + icontype_filename);

			# Shaders!
			mkdirs = [];
			mkdirs.extend([output_folder + "\\data\\ark2d\\shaders"]);
			mkdirs.extend([output_folder + "\\ARM"]);
			self.makeDirectories(mkdirs);
			shutil.copy(self.ark2d_dir + "\\build\\windows-phone\\ARM\\Debug\\ARK2D\\geometry-dx11-pixel.cso", output_folder + "/data/ark2d/shaders/geometry-dx11-pixel.cso");
			shutil.copy(self.ark2d_dir + "\\build\\windows-phone\\ARM\\Debug\\ARK2D\\geometry-dx11-vertex.cso", output_folder + "/data/ark2d/shaders/geometry-dx11-vertex.cso");
			shutil.copy(self.ark2d_dir + "\\build\\windows-phone\\ARM\\Debug\\ARK2D\\texture-dx11-pixel.cso", output_folder + "/data/ark2d/shaders/texture-dx11-pixel.cso");
			shutil.copy(self.ark2d_dir + "\\build\\windows-phone\\ARM\\Debug\\ARK2D\\texture-dx11-vertex.cso", output_folder + "/data/ark2d/shaders/texture-dx11-vertex.cso");

			# DLLs
			shutil.copy(self.ark2d_dir + "\\build\\windows-phone\\ARM\\Debug\\ARK2D\\ARK2D_arm.dll", self.game_dir + "\\" + self.build_folder + self.ds + self.platform + self.ds + "ARK2D_arm.dll");
			shutil.copy(self.ark2d_dir + "\\build\\windows-phone\\ARM\\Debug\\ARK2D\\ARK2D_arm.lib", self.game_dir + "\\" + self.build_folder + self.ds + self.platform + self.ds + "ARK2D_arm.lib");

			shutil.copy(self.ark2d_dir + "\\build\\windows-phone\\Debug\\ARK2D\\ARK2D_x86.dll", self.game_dir + "\\" + self.build_folder + self.ds + self.platform + self.ds + "ARK2D_x86.dll");
			shutil.copy(self.ark2d_dir + "\\build\\windows-phone\\Debug\\ARK2D\\ARK2D_x86.lib", self.game_dir + "\\" + self.build_folder + self.ds + self.platform + self.ds + "ARK2D_x86.lib");

			# Copy in additional DLLs.
			# for (extra_dll in extra_dlls_arm):
			#	shutil.copy(extra_dll, self.game_dir + "\\" + self.build_folder + self.ds + self.platform + self.ds + );#extra_dll);

			# copy any other dll dependencies...
			for extradll in extra_dlls_arm: 
				print(extradll);
				extradll_name = self.get_str_filename2(extradll);
				print(extradll_name);
				
				extradll2 = self.str_replace(extradll, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%GAME_DIR%", self.game_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
				extradll2 = self.fixLocalPath(extradll2);
				print(extradll2);
				
				shutil.copy(extradll2, self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + extradll_name);

			#for extradll in extra_dlls_x86: 
			#	print(extradll);
			#	extradll_name = self.get_str_filename(extradll);
			#	shutil.copy(extradll, self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\X86\\" + extradll_name);


			print("One final header...");
			self.generateARKH(output_folder + "/ARK.h");

			print("Done!");

	def startWindowsStore(self):
		print("Building for Current Platform (" + self.platform + ")");

		if (self.building_library):
			pass;
		else:
			print("Cannot build games for Windows Store yet.");

		pass;

	def startWindowsVS2(self):
		print("Building for Current Platform (" + self.platform + ")");
		print("ark2d dir: " + self.ark2d_dir);

		# open config
		#f = open(self.ark2d_dir + "/config.json", "r");
		#fcontents = f.read();
		#f.close(); 
		#self.config = json.loads(fcontents);

		if (self.building_library):
			print("Building Windows dll.");

			# output directory
			output_folder = self.ark2d_dir + "/build/windows";# + self.output;

			# make directories
			mkdirs = [];
			mkdirs.extend(self.mkdirs);
			self.makeDirectories(mkdirs);

			sln_contents = ""; ####
			vcxproj_contents = "";

			# dll sln and vcxproj files
			f1 = open(self.ark2d_dir + "/lib/windows/dll-template/dll-template.sln", "r");
			f2 = open(self.ark2d_dir + "/lib/windows/dll-template/dll-template.vcxproj", "r");
			sln_contents = f1.read();
			vcxproj_contents = f2.read();
			f1.close(); 
			f2.close(); 

			# modify sln/vcxproj files
			vcxproj_headerfiles = "";
			vcxproj_sourcefiles = "";
			for srcfile in self.src_files:  

				#check if src file has a corresponding .h file. add that to the project...
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newfh = srcfile[0:findex] + ".h";
				newfhfull = self.ark2d_dir + self.ds + newfh; 
				if (os.path.exists(newfhfull)): 
					vcxproj_headerfiles += "<ClInclude Include=\"../../"+newfh+"\" /> \n";

				if h_ext == "c" or h_ext == "cpp":
					vcxproj_sourcefiles += "<ClCompile Include=\"../../"+srcfile+"\" /> \n";

				

			vcxproj_contents = self.str_replace(vcxproj_contents, [("%COMPILE_HEADER_FILES%", vcxproj_headerfiles)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%COMPILE_SOURCE_FILES%", vcxproj_sourcefiles)]);

			# write sln file
			print("Write sln file...");
			f1 = open(output_folder + "/libARK2D.sln", "w");
			f1.write(sln_contents);
			f1.close();

			# write vcxproj file
			print("Write vcxproj file...");
			f1 = open(output_folder + "/libARK2D.vcxproj", "w");
			f1.write(vcxproj_contents);
			f1.close();




		else:
			# self.startWindowsVS();

			print("Building Windows Desktop game.");

			# output directory
			output_folder = self.game_dir + "/build/" + self.output;
			game_name = self.game_name; #config['game_name'];
			game_name_safe = self.game_name_safe; #config['game_name_safe'];
			#game_short_name = self.game_short_name; #config['game_short_name'];
			game_description = self.game_description; #config['game_description'];
			game_resources_dir = self.game_resources_dir; #config['windows']['game_resources_dir'];
			#game_name_safe
			#game_class_name

			# make directories
			print("Making directories...");
			mkdirs = [];
			mkdirs.extend(self.mkdirs);
			mkdirs.extend([output_folder + "/Debug"]);
			mkdirs.extend([output_folder + "/Release"]);
			#mkdirs.extend([output_folder + "/Debug/data/ark2d"]);
			#mkdirs.extend([output_folder + "/Release/data/ark2d"]);
			self.makeDirectories(mkdirs);

			# get list of .libs and .dlls
			extra_dlls = [];
			extra_libs = [];
			extra_dlls = self.addLibrariesToArray(extra_dlls, self.libs, "dll");
			extra_libs = self.addLibrariesToArray(extra_libs, self.libs, "lib");

			sln_contents = "";
			vcxproj_contents = "";

			# dll sln and vcxproj files 
			print("Making sln and vcxproj...");
			f1 = open(self.ark2d_dir + "\\lib\\windows\\project-template\\project-template.sln", "r");
			f2 = open(self.ark2d_dir + "/lib/windows/project-template/project-template.vcxproj", "r");
			f3 = open(self.ark2d_dir + "/lib/windows/project-template/project-template.vcxproj.user", "r");
			sln_contents = f1.read();
			vcxproj_contents = f2.read(); 
			vcxprojuser_contents = f3.read(); 
			f1.close(); 
			f2.close(); 
			f3.close(); 

			# modify sln strings
			print("Configuring sln file...");
			sln_contents = self.str_replace(sln_contents, [("%GAME_SHORT_NAME%", game_name_safe)]);

			# make spritesheets
			print("Generating spritesheets...");
			self.generateSpriteSheets();

			# resources to copy to game project. gotta do this early to generate the VS project
			game_resources_list = [];
			image_resources = [];
			audio_resources = [];
			document_resources = [];
			filesToCopy = self.listFiles(game_resources_dir, False);
			#print(filesToCopy);
			for file in filesToCopy:
				fromfile = game_resources_dir + self.ds + file;
				tofile = output_folder + "/data/" + file;

				game_resources_list.extend(["data\\" + file]);

				file_ext = self.get_str_extension(file);
				if (self.is_image_extension(file_ext)):
					image_resources.extend(["data\\" + file]);
				elif (self.is_audio_extension(file_ext)):
					# what do we do when it's an audio file idon't know?!
					pass;
				else:
					document_resources.extend(["data\\" + file]);

			# list of sourcefiles
			print("Creating list of Source Files...");
			vcxproj_headerfiles = "";
			vcxproj_sourcefiles = "";
			vcxproj_resourcefiles = "";

			for srcfile in self.src_files:  

				#check if src file has a corresponding .h file. add that to the project...
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newfh = srcfile[0:findex] + ".h";
				newfhfull = self.ark2d_dir + self.ds + newfh; 
				if (os.path.exists(newfhfull)): 
					vcxproj_headerfiles += "<ClInclude Include=\"../../"+newfh+"\" /> \n";

				if h_ext == "c":
					vcxproj_sourcefiles += "<ClCompile Include=\"../../"+srcfile+"\" /> \n";
				elif h_ext == "hlsl":
					hlsltype = "";
					if "pixel" in srcfile:
						hlsltype = "Pixel";
					elif "vertex" in srcfile:
						hlsltype = "Vertex";

					vcxproj_sourcefiles += "	<FxCompile Include=\"../../"+srcfile+"\"> \n ";
					vcxproj_sourcefiles += "		<FileType>Document</FileType> \n";
					vcxproj_sourcefiles += "		<DisableOptimizations Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</DisableOptimizations>\n"; # was true...
					vcxproj_sourcefiles += "		<EnableDebuggingInformation Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">false</EnableDebuggingInformation>\n";
					vcxproj_sourcefiles += "		<ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">" + hlsltype + "</ShaderType>\n";
					vcxproj_sourcefiles += "		<ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">" + hlsltype + "</ShaderType>\n";
					vcxproj_sourcefiles += "	</FxCompile> \n";
					
				elif h_ext == "rc":
					vcxproj_resourcefiles += "	<ResourceCompile Include=\"../../"+srcfile+"\" />";
				else:
					vcxproj_sourcefiles += "<ClCompile Include=\"../../"+srcfile+"\" /> \n";


			# extra include dirs
			vcxproj_AdditionalIncludeDirs = ""; 
			#if "include_dirs" in config['windows']:
			for includedir in self.include_dirs: #config['windows']['include_dirs']:
				includedir_actual = self.str_replace(includedir, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
				vcxproj_AdditionalIncludeDirs += includedir_actual + ";";

			vcxproj_AdditionalPreprocessorDefinitions = "";
			for ppdefinition in self.preprocessor_definitions:
				ppdefinition_actual = ppdefinition;
				vcxproj_AdditionalPreprocessorDefinitions += ppdefinition_actual + ";";

			# additional lib files
			vcxproj_AdditionalDotLibs = "";
			for extra_lib in extra_libs:
				vcxproj_AdditionalDotLibs += ";" + extra_lib;



			# modify vcxproj strings 
			game_image_resources_str = "";
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%ARK2D_DIR%", self.ark2d_dir)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%GAME_SHORT_NAME%", game_name_safe)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%GAME_RESOURCES%", vcxproj_resourcefiles)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%GAME_IMAGE_RESOURCES%", game_image_resources_str)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%COMPILE_HEADER_FILES%", vcxproj_headerfiles)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%COMPILE_SOURCE_FILES%", vcxproj_sourcefiles)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%ADDITIONAL_PREPROCESSOR_DEFINITIONS%", vcxproj_AdditionalPreprocessorDefinitions)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%ADDITIONAL_INCLUDE_DIRECTORIES%", vcxproj_AdditionalIncludeDirs)]);
			vcxproj_contents = self.str_replace(vcxproj_contents, [("%ADDITIONAL_DOTLIB_FILES%", vcxproj_AdditionalDotLibs)]);

			# write sln file
			print("Write sln file...");
			f1 = open(output_folder + "/" + game_name_safe + ".sln", "w");
			f1.write(sln_contents);
			f1.close();

			# write vcxproj file
			print("Write vcxproj file...");
			f1 = open(output_folder + "/" + game_name_safe + ".vcxproj", "w");
			f1.write(vcxproj_contents);
			f1.close();

			# write vcxproj.user file
			print("Write vcxproj.user file...");
			f1 = open(output_folder + "/" + game_name_safe + ".vcxproj.user", "w");
			f1.write(vcxprojuser_contents);
			f1.close();

			
			# copy resources in to wp8 game project folder.
			print("Copying ark2d data in...");
			shutil.copytree(self.ark2d_dir + "/data/", game_resources_dir + "/ark2d/");

			print("Copying data in..."); 
			#shutil.copytree(game_resources_dir, output_folder + "/Development/data");
			shutil.copytree(game_resources_dir, output_folder + "/Debug/data"); ## disable this until it works
			shutil.copytree(game_resources_dir, output_folder + "/Release/data/");

			

			# copy ark2d.dll in 
			print("Copying ARK2D data in..."); 

			
			# DLLs
			shutil.copy(self.ark2d_dir + "\\build\\windows\\Debug\\libARK2D.dll", self.game_dir + "\\" + self.build_folder + self.ds + self.output + self.ds + "Debug\\libARK2D.dll");
			shutil.copy(self.ark2d_dir + "\\build\\windows\\Debug\\libARK2D.lib", self.game_dir + "\\" + self.build_folder + self.ds + self.output + self.ds + "Debug\\libARK2D.lib");

			#shutil.copy(self.ark2d_dir + "\\build\\windows\\Development\\libARK2D.dll", self.game_dir + "\\" + self.build_folder + self.ds + self.output + self.ds + "Development\\libARK2D.dll");
			#shutil.copy(self.ark2d_dir + "\\build\\windows\\Development\\libARK2D.lib", self.game_dir + "\\" + self.build_folder + self.ds + self.output + self.ds + "Development\\libARK2D.lib");

			shutil.copy(self.ark2d_dir + "\\build\\windows\\Release\\libARK2D.dll", self.game_dir + "\\" + self.build_folder + self.ds + self.output + self.ds + "Release\\libARK2D.dll");
			shutil.copy(self.ark2d_dir + "\\build\\windows\\Release\\libARK2D.lib", self.game_dir + "\\" + self.build_folder + self.ds + self.output + self.ds + "Release\\libARK2D.lib");

			# ARK Dependency DLLs
			# debug 
			
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\alut.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\alut.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\freetype6.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\freetype6.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\glew32.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\glew32.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\libcurl.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\curllib.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\wrap_oal.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\wrap_oal.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\zlib1.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\zlib1.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\OpenAL32.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\OpenAL32.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\vs2013\\x86\\msvcr120d.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\Debug\\msvcr120d.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\vs2013\\x86\\msvcp120d.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\Debug\\msvcp120d.dll");
			#shutil.copy(self.ark2d_dir + "\\lib\\windows\\vs2013\\x86\\msvcr120.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\msvcr120.dll");
			#shutil.copy(self.ark2d_dir + "\\lib\\windows\\vs2013\\x86\\msvcp120.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\msvcp120.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\angelscript\\angelscriptd.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\angelscriptd.dll");
			

			#development 
			"""
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\alut.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\alut.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\freetype6.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\freetype6.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\glew32.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\glew32.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\libcurl.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\curllib.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\wrap_oal.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\wrap_oal.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\zlib1.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\zlib1.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\OpenAL32.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\OpenAL32.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\vs2013\\x86\\msvcr120.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\msvcr120.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\vs2013\\x86\\msvcp120.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\msvcp120.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\angelscript\\angelscript.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\angelscript.dll");
			"""
		
			#release 
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\alut.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\alut.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\freetype6.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\freetype6.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\glew32.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\glew32.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\libcurl.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\curllib.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\wrap_oal.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\wrap_oal.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\zlib1.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\zlib1.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\OpenAL32.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\OpenAL32.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\vs2013\\x86\\msvcr120.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\msvcr120.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\vs2013\\x86\\msvcp120.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\msvcp120.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\angelscript\\angelscript.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\angelscript.dll");
		
			# copy any other dll dependencies...
			for extradll in extra_dlls: 
				print(extradll);
				extradll_name = self.get_str_filename(extradll);
				# shutil.copy(extradll, self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Development\\" + extradll_name);
				shutil.copy(extradll, self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Debug\\" + extradll_name);
				shutil.copy(extradll, self.game_dir + self.ds + self.build_folder + self.ds + self.output + "\\Release\\" + extradll_name);


			print("One final header...");
			self.generateARKH(output_folder + "/ARK.h");

			print("Done!");

	"""
	def startWindowsVS(self):
		print("Building for Current Platform (" + self.platform + ")");

		# open config
		f = open(self.ark2d_dir + "/config.json", "r");
		fcontents = f.read();
		f.close(); 
		self.config = json.loads(fcontents);

		gyp_executable = self.config['windows']['gyp_executable']; 

		

		if (self.building_library):

			print("making directories");
			flascc_folder = self.config['windows']['ark2d_dir'] + "/build/windows";
			mkdirs = [];
			mkdirs.extend(self.mkdirs);
			self.makeDirectories(mkdirs);

			#projectname ark2d
			projectname = "ark2d";

			# generate gyp file.
			print("creating gyp file");
			gypfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + projectname + ".gyp";

			gypfile = {};
			#gypfile['variables'] = ["GYP_MSVS_VERSION"];
			gypfile['defines'] = ['ARK2D_WINDOWS', "ARK2D_WINDOWS_DLL",  "_WIN32"]; #'ARK2D_IPHONE'];
			
			gypfile['targets'] = [];

			gypfiletarget = {};
			gypfiletarget['target_name'] = "libARK2D";
			gypfiletarget['type'] = "shared_library";
			gypfiletarget['include_dirs'] = [
				'../../src/ARK2D/vendor/windows', 
				'../../lib/windows/includes',
				'../../src/ARK2D/vendor/spine/includes'

				];
			gypfiletarget['sources'] = [];

			#gypfiletarget['sources'].extend([
			#	self.ark2d_dir + self.ds + "src\\ARK2D\\vendor\\glew-vs.c"
			#])

			for srcfile in self.config['src_files']['windows']: 
				gypfiletarget['sources'].extend(["../../"+srcfile]);

				#check if src file has a corresponding .h file. add that to the project...
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newfh = srcfile[0:findex] + ".h";
				newfhfull = self.game_dir + self.ds + newfh; 
				if (os.path.exists(newfhfull)): 
					gypfiletarget['sources'].extend(["../../"+newfh]);	

			for srcfile in self.config['src_files']['all']:
				gypfiletarget['sources'].extend(["../../"+srcfile]);

				#check if src file has a corresponding .h file. add that to the project...
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newfh = srcfile[0:findex] + ".h";
				newfhfull = self.game_dir + self.ds + newfh; 
				if (os.path.exists(newfhfull)): 
					gypfiletarget['sources'].extend(["../../"+newfh]);	

			gypfiletarget['sources!'] = [];
			gypfiletarget['dependencies'] = [];
			gypfiletarget['conditions'] = [];
			gypfiletargetcondition = {};
			gypfiletargetcondition['defines'] = ['ARK2D_WINDOWS',  "ARK2D_WINDOWS_DLL",  '_WIN32']; #, 'CF_EXCLUDE_CSTD_HEADERS'];
			gypfiletargetcondition['sources'] = [];
			gypfiletargetcondition['sources!'] = [];
			gypfiletargetcondition['link_settings'] = {}; 
			gypfiletargetcondition['link_settings']['libraries'] = [

				#'/Developer/SDKs/MacOSX10.7.sdk/System/Library/Frameworks/IOKit.framework',
				
				#config['dynamic_libraries']['windows'] + "/lib/osx/freetype/libfreetype.a",
			#	config['osx']['ark2d_dir'] + "/lib/osx/libcurl.a" 
			];
			
			#for dylib in self.config['dynamic_libraries']['windows']:
			#	gypfiletargetcondition['link_settings']['libraries'].extend( [self.config['windows']['ark2d_dir'] + self.ds + dylib] );			
 
			gypfiletargetcondition['link_settings']['libraries'].extend( [
				self.config['windows']['ark2d_dir'] + self.ds + "lib\\windows\\alut.lib",
				self.config['windows']['ark2d_dir'] + self.ds + "lib\\windows\\OpenAL32.lib",
				self.config['windows']['ark2d_dir'] + self.ds + "lib\\windows\\freetype6.lib",
				self.config['windows']['ark2d_dir'] + self.ds + "lib\\windows\\zlib1.lib",
				#self.config['windows']['ark2d_dir'] + self.ds + "lib\\windows\\glew32s.lib",
				self.config['windows']['ark2d_dir'] + self.ds + "lib\\windows\\curl\\Debug\\curllib.lib",
				"opengl32.lib",
				"msvcrt.lib",
				"winmm.lib"
				
			] );
 
			
			#"glew32.lib"

			#'../../lib/iphone/libfreetype.a
			
			if (self.debug):
				gypfiletargetcondition['defines'].extend(['ARK2D_DEBUG']);

			gypfiletargetcondition['ldflags'] = [
				"-lbz2", 
				"-lcurl",
				"-lz",
				#"-L" + self.ark2d_dir + "/lib/osx",
				#"-L/usr/lib"
			]; 

			gypfiletargetcondition['link_settings!'] = [];
			

			gypfiletargetcondition['include_dirs'] = []; # ['../../src/ARK2D/vendor/windows'];
			
			# TODO:
			# make it generate VS2013 projects... :/
			
			# GYP_MSVS_VERSION=2013
			# or 
			# GYP_MSVS_VERSION=2013e

			# -G msvs_version=2013
			
			windcondition = [];
			windcondition.extend(["OS == 'win'"]);
			windcondition.extend([gypfiletargetcondition]);
			gypfiletarget['conditions'].extend([windcondition]);
			
			gypfile['targets'].extend([gypfiletarget]);

			print("saving gyp file: " + gypfilename);
			f = open(gypfilename, "w")
			f.write(json.dumps(gypfile, sort_keys=True, indent=4));
			f.close();

			
			#delete xcode project? 
			try:
				print("deleting visual studio project");
				os.system("rmdir -r -d " + self.build_folder + self.ds + self.platform + self.ds);
			except:
				print("could not delete visual studio project");
 
			# run gyp file.
			os.environ["GYP_MSVS_VERSION"] = "2013";
			# os.system("python " + gyp_executable + ".bat " + gypfilename + " --depth=../../src");
			os.system(gyp_executable + ".bat  --depth=../../src " + gypfilename);

			#exit(0);
			
			# do a bit of hacking to put the ignored libs in there.		
			# edit libARK2D.vcxproj	and replace </OutputFile> with more stuff.
			vsprojectfilename = self.build_folder + self.ds + self.platform + self.ds + "libARK2D.vcxproj";
			print("hack the vs2012 project: " + vsprojectfilename);
			vsf = open(vsprojectfilename, "r")
			vsprojectcontents = vsf.read();
			vsf.close();

			#set libs
			vsprojectcontents = self.str_replace(vsprojectcontents, [("</OutputFile>", "</OutputFile><IgnoreSpecificDefaultLibraries>libc.lib;libcmt.lib;libcd.lib;libcmtd.lib;msvcrtd.lib;%(IgnoreSpecificDefaultLibraries)</IgnoreSpecificDefaultLibraries>")]);
			
			# debug build?
			if (self.debug):
				vsprojectcontents = self.str_replace(vsprojectcontents, [("</PreprocessorDefinitions>", "</PreprocessorDefinitions><DebugInformationFormat>EditAndContinue</DebugInformationFormat><Optimization>Disabled</Optimization>")]);
				vsprojectcontents = self.str_replace(vsprojectcontents, [("</Link>", "<GenerateDebugInformation>true</GenerateDebugInformation> <AssemblyDebug>true</AssemblyDebug> <ImageHasSafeExceptionHandlers>false</ImageHasSafeExceptionHandlers></Link>")]);
				

			# set shaders as HLSL type
			vsprojectcontents = self.str_replace(vsprojectcontents, [("<None Include=\"..\..\src\ARK2D\Graphics\HLSL\geometry-dx11-pixel.hlsl\"/>", "<FxCompile Include=\"..\..\src\ARK2D\Graphics\HLSL\geometry-dx11-pixel.hlsl\"><FileType>Document</FileType><ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Default|Win32'\">Pixel</ShaderType></FxCompile>")]);
			vsprojectcontents = self.str_replace(vsprojectcontents, [("<None Include=\"..\..\src\ARK2D\Graphics\HLSL\geometry-dx11-vertex.hlsl\"/>", "<FxCompile Include=\"..\..\src\ARK2D\Graphics\HLSL\geometry-dx11-vertex.hlsl\"><FileType>Document</FileType><ShaderType Condition=\"'$(Configuration)|$(Platform)'=='Default|Win32'\">Vertex</ShaderType></FxCompile>")]);
			
			vsf = open(vsprojectfilename, "w")
			vsf.write(vsprojectcontents);
			vsf.close();
			
		
			print("done. now compile with the xcode project.");

			exit(0); 

		else: #building game

			print("making directories");
			flascc_folder = self.config['windows']['ark2d_dir'] + "/build/windows";
			mkdirs = [];
			mkdirs.extend(self.mkdirs);
			self.makeDirectories(mkdirs);

			f = open(self.game_dir + "/config.json", "r");
			fcontents = f.read();
			f.close(); 
			config = json.loads(fcontents);

			#projectname ark2d
			projectnameunsafe = config['game_name'];
			projectname = config['game_short_name'];

			# generate gyp file.
			print("creating gyp file");
			gypfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + projectname + ".gyp";

			#print(gypfilename);
			#exit(0);

			gypfile = {};
			gypfile['defines'] = ['ARK2D_WINDOWS', "_WIN32"]; #'ARK2D_IPHONE'];
			
			gypfile['targets'] = [];

			gypfiletarget = {};
			gypfiletarget['target_name'] = projectnameunsafe;
			gypfiletarget['type'] = "executable";
			gypfiletarget['include_dirs'] = [
				self.config['windows']['ark2d_dir'] + "\\src\\ARK2D",
				self.config['windows']['ark2d_dir'] + "\\src\\ARK2D\\vendor\\windows",
				self.config['windows']['ark2d_dir'] + "\\src\\ARK2D\\vendor\\spine\\includes",
				self.config['windows']['ark2d_dir'] + "\\lib\\windows\\includes"
			];
			gypfiletarget['sources'] = [];


			# custom include dirs
			if "include_dirs" in config['windows']: 
				for includedir in config['windows']['include_dirs']:
					includedir_actual = self.str_replace(includedir, [("%PREPRODUCTION_DIR%", config['windows']['game_preproduction_dir']), ("%ARK2D_DIR%", config['windows']['ark2d_dir'])]);
					gypfiletarget['include_dirs'].extend([includedir_actual]);

			#gypfiletarget['sources'].extend([
			#	self.ark2d_dir + self.ds + "src\\ARK2D\\vendor\\glew-vs.c"
			#])

			for srcfile in config['game_src_files']: 
				gypfiletarget['sources'].extend(["../../"+srcfile]);

				#check if src file has a corresponding .h file. add that to the project...
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newfh = srcfile[0:findex] + ".h";
				newfhfull = self.game_dir + self.ds + newfh; 
				if (os.path.exists(newfhfull)): 
					gypfiletarget['sources'].extend(["../../"+newfh]);	

			#for srcfile in config['src_files']['all']:
			#	gypfiletarget['sources'].extend(["../../"+srcfile]);
#
				#check if src file has a corresponding .h file. add that to the project...
#				findex = srcfile.rfind('.');
#				h_ext = srcfile[findex+1:len(srcfile)];
#				newfh = srcfile[0:findex] + ".h";
#				newfhfull = self.game_dir + self.ds + newfh; 
#				if (os.path.exists(newfhfull)): 
#					gypfiletarget['sources'].extend(["../../"+newfh]);	

			gypfiletarget['sources!'] = [];
			gypfiletarget['dependencies'] = [];
			gypfiletarget['conditions'] = [];
			gypfiletargetcondition = {};
			gypfiletargetcondition['defines'] = ['ARK2D_WINDOWS', '_WIN32']; #, 'CF_EXCLUDE_CSTD_HEADERS'];
			gypfiletargetcondition['sources'] = [];
			gypfiletargetcondition['sources!'] = [];
			gypfiletargetcondition['link_settings'] = {}; 
			gypfiletargetcondition['link_settings']['libraries'] = [

				#'/Developer/SDKs/MacOSX10.7.sdk/System/Library/Frameworks/IOKit.framework',
				
				#config['dynamic_libraries']['windows'] + "/lib/osx/freetype/libfreetype.a",
			#	config['osx']['ark2d_dir'] + "/lib/osx/libcurl.a" 
			];
			
			#for dylib in self.config['dynamic_libraries']['windows']:
			#	gypfiletargetcondition['link_settings']['libraries'].extend( [self.config['windows']['ark2d_dir'] + self.ds + dylib] );			
 
			gypfiletargetcondition['link_settings']['libraries'].extend( [
				self.config['windows']['ark2d_dir'] + self.ds + "build\\windows\\Release\\libARK2D.lib"
				
				
			] );
			#"glew32.lib"
				
			
			#'../../lib/iphone/libfreetype.a
			
			if (self.debug):
				gypfiletargetcondition['defines'].extend(['ARK2D_DEBUG']);

			gypfiletargetcondition['ldflags'] = [
				#"-lbz2", 
				#"-lcurl",
				#"-lz",
				#"-L" + self.ark2d_dir + "/lib/osx",
				#"-L/usr/lib"
			]; 

			gypfiletargetcondition['link_settings!'] = [ 

	        ];
			gypfiletargetcondition['include_dirs'] = []; # ['../../src/ARK2D/vendor/windows'];
			
			
			
			windcondition = [];
			windcondition.extend(["OS == 'win'"]);
			windcondition.extend([gypfiletargetcondition]);
			gypfiletarget['conditions'].extend([windcondition]);
			
			gypfile['targets'].extend([gypfiletarget]);

			print("saving gyp file: " + gypfilename);
			f = open(gypfilename, "w")
			f.write(json.dumps(gypfile, sort_keys=True, indent=4));
			f.close();

			
			#delete xcode project? 
			try:
				print("deleting visual studio project");
				os.system("rmdir -r -d " + self.build_folder + self.ds + self.platform + self.ds);
			except:
				print("could not delete visual studio project");
 
			# run gyp file.
			os.environ["GYP_MSVS_VERSION"] = "2013";
			# os.system("python " + gyp_executable + ".bat " + gypfilename + " --depth=../../src");
			os.system(gyp_executable + ".bat  --depth=../../src " + gypfilename);
			
			# modify the vs project a bit
			# do a bit of hacking to put the ignored libs in there.		
			# edit libARK2D.vcxproj	and replace </OutputFile> with more stuff.
			
			
			vsprojectfilename = self.build_folder + self.ds + self.platform + self.ds + self.game_short_name + ".vcxproj";
			print("hack the vs2012 project: " + vsprojectfilename);
			vsf = open(vsprojectfilename, "r")
			vsprojectcontents = vsf.read();
			vsf.close();

			# hack the vs2013 project
			vsprojectcontents = self.str_replace(vsprojectcontents, [("</OutputFile>", "</OutputFile><SubSystem>Windows</SubSystem><EntryPointSymbol>main</EntryPointSymbol>")]);

			# debug build?
			if (self.debug):
				vsprojectcontents = self.str_replace(vsprojectcontents, [("</PreprocessorDefinitions>", "</PreprocessorDefinitions><DebugInformationFormat>EditAndContinue</DebugInformationFormat><Optimization>Disabled</Optimization>")]);
				vsprojectcontents = self.str_replace(vsprojectcontents, [("</Link>", "<GenerateDebugInformation>true</GenerateDebugInformation> <AssemblyDebug>true</AssemblyDebug> <ImageHasSafeExceptionHandlers>false</ImageHasSafeExceptionHandlers></Link>")]);

			#vsprojectcontents = self.str_replace(vsprojectcontents, [("<SubSystem>", "<SubSystem>Windows")]);
			vsf = open(vsprojectfilename, "w")
			vsf.write(vsprojectcontents);
			vsf.close();
			

			# do asset ting
			print("copying assets");	
			self.generateSpriteSheets();
		
			if (self.game_resources_dir != ''):
				print("copying game resources in to project:");
				#cpy_game_res = 'copy "' + self.game_resources_dir.replace('\\\\','\\') + '" "' + self.game_dir.replace('\\\\','\\') + '\\' + self.build_folder + '\\' + self.platform + '\\"';
				#print(cpy_game_res);
				#subprocess.call([cpy_game_res], shell=True);
				thisgameresdir = self.game_resources_dir.replace('\\\\','\\');
				thisgameresdir2 = self.game_dir.replace('\\\\','\\') + '\\' + self.build_folder + '\\' + self.platform + '\\data';
				try:
					os.system("rmdir /S /Q " + thisgameresdir2); 
					shutil.copytree(thisgameresdir, thisgameresdir2);
				except:
					print("could not copy resources [from:"+thisgameresdir+",to:"+thisgameresdir2);
					pass; 

			print("copying dlls... ");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\alut.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\alut.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\freetype6.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\freetype6.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\glew32.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\glew32.dll");
			shutil.copy(self.ark2d_dir + "\\build\\windows\\Release\\libARK2D.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\libARK2D.dll");
			# shutil.copy(self.ark2d_dir + "\\lib\\windows\\curl\\Debug\\curllib.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\curllib.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\libcurl.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\curllib.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\wrap_oal.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\wrap_oal.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\zlib1.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\zlib1.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\OpenAL32.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\OpenAL32.dll");

			# visual studio 2013 dlls
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\vs2013\\x86\\msvcr120.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\msvcr120.dll");
			shutil.copy(self.ark2d_dir + "\\lib\\windows\\vs2013\\x86\\msvcp120.dll", self.game_dir + self.ds + self.build_folder + self.ds + self.platform + "\\msvcp120.dll");
			
		
			print("done. now compile with the visual studio project.");
 
			exit(0);



		print(gyp_executable);

		return;
	"""

	def startWindows(self):
		print("Building for Current Platform (" + self.platform + ")");
		
		#prepare dirs
		print("---");
		print("Making Directories");
		for h in self.mkdirs:
			#h = "'" + h + "'";
			
			#subprocess.call(['mkdir C:\\' + h], shell=True);
			try:
				print("mkdir " + self.game_dir + self.ds + h);
				os.makedirs(self.game_dir + self.ds + h);
			except OSError as exc: 
				if exc.errno == errno.EEXIST:
					pass
				else: raise	
		
		# make sure cache file exists
		
		# compile cache thing
		cachefilename = "";
		#if (self.building_game):
			#cachefilename += self.game_dir + self.ds;
			
		cachefilename += self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "build-cache" + self.ds  + "compiled.json";
		
		print("---");
		print("Loading build cache file: " + cachefilename);
		
		self.createCacheFile(cachefilename);
		f = open(cachefilename, "r")
		fcontents = f.read();
		f.close();
		fjson = json.loads(fcontents);
		fchanged = False;
		
		# TODO: generate block
		#if (sys.platform == "win32"):
			

		print("Loaded build cache file: " + cachefilename);
			
		print("---"); 
		print("Compiling Source Files: ");
		#print(self.src_files);

		#if (self.platform == "windows"): 
		#	if (not self.building_game):
				#self.src_files.extend([
				#	"src\\ARK2D\\vendor\\glew-mingw.c"
				#]); 
		


		#compile
		for h in self.src_files:
			compileStr = "";
			
			findex = h.rfind('.');
			h_ext = h[findex+1:len(h)];
			newf = h[0:findex] + ".o";
			newfd = h[0:findex] + ".d";
			
			
			if h_ext == 'c':
				compileStr += self.gccCompiler;
			elif h_ext == 'cpp':
				compileStr += self.gppCompiler;
			elif h_ext == 'mm':
				compileStr += self.objcCompiler;
			elif h_ext == 'rc':
				compileStr += "windres ";
				
			if (not h in fjson or fjson[h]['date_modified'] < os.stat(self.game_dir + self.ds + h).st_mtime):
				
				processThisFile = True;

				print(h);

				if (h_ext == 'c' or h_ext == 'cpp' or h_ext == 'mm'):
					compileStr += " -O3 -Wall -c -fmessage-length=0 ";
					if (sys.platform == "darwin"): #compiling on mac
						if not "vendor" in newf:
							compileStr += " -mmacosx-version-min=10.6 -DARK2D_MACINTOSH -DARK2D_DESKTOP -DMAC_OS_X_VERSION_MIN_REQUIRED=1060 -x objective-c++  ";

							# warnings
							compileStr += " -Wall -Winit-self -Woverloaded-virtual -Wuninitialized  "; #  -Wold-style-cast -Wmissing-declarations

							#compileStr += "-I /usr/X11/include "; 
							#compileStr += "-I /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk/usr/include ";
							compileStr += "-I /usr/include ";
							compileStr += "-I " + self.ark2d_dir + "/lib/includes";
							#compileStr += "-I /Developer/SDKs/MacOSX10.7.sdk/usr/include ";
							#compileStr += "-I /Developer/SDKs/MacOSX10.7.sdk/usr/include/c++/4.2.1 ";
							#compileStr += "-I /Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/Foundation.framework/Headers ";
							#compileStr += "-I /Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/ApplicationServices.framework/Headers ";
							#compileStr += "-I /Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/Cocoa.framework/Headers ";
							#compileStr += "-I /Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/OpenAL.framework/Headers ";
							#compileStr += "-I /Developer/SDKs/MacOSX10.6.sdk/System/Library/Frameworks/OpenGL.framework/Headers ";
							#compileStr += " -framework Foundation -framework ApplicationServices -framework Cocoa -framework OpenGL -framework OpenAL  ";
							#compileStr += " -stdlib=libc++ ";

 
						#  compileStr += " -march=i386 ";
						# compileStr += " -march=i386 ";
						#-march=i386 "; # i386
						#-arch i386
						compileStr += " -o \"";
						compileStr += self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + newf + "\" \"" + self.game_dir + self.ds + h + "\" ";
					elif (sys.platform == "win32"):
						#compileStr += " -o \"" + self.build_folder + self.ds + self.platform + self.ds + newf + "\" \"" + h + "\" ";
						#compileStr += " -Wno-strict-aliasing -Wno-unused-but-set-variable -fpermissive ";
						#compileStr += " -o " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + newf + " " + self.game_dir + self.ds + h + " ";
						compileStr += "-DARK2D_WINDOWS ";
						compileStr += "-DARK2D_DESKTOP ";
						if self.debug:
							compileStr += " -DARK2D_DEBUG ";

						compileStr += " -o " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + newf + " " + self.game_dir + self.ds + h + " ";
					else:
						compileStr += " -o " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + newf + " " + self.game_dir + self.ds + h + " ";			
					
				elif h_ext == 'rc':
					if (sys.platform == "win32"):
						#compileStr += self.game_dir + self.ds + h + " " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + newf + " ";
						compileStr += self.game_dir + self.ds + h + " " +  self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + newf + " ";
					else:
						processThisFile = False; 
			
				if (processThisFile):
					fjson[h] = {"date_modified": os.stat(self.game_dir + self.ds + h).st_mtime };
					
					#print(compileStr);
					#subprocess.call(["dir"], shell=True);
					#subprocess.call([compileStr], shell=True);	 
					
					# the above did not work on win7 64bit.
					os.system(compileStr);
					fchanged = True;
					
					
	
		# update compile cache thing
		if (fchanged == True):
			f = open(self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "build-cache" + self.ds + "compiled.json", "w")
			f.write(json.dumps(fjson, sort_keys=True, indent=4));
			f.close();
		else:
			print("...nothing to compile!");
	
		#link
		self.doLink();
		
	def str_replace(self, str, edits):
		for search, replace in edits:
			str = str.replace(search, replace);
		return str;
		
	def doLink(self):
		
		print("---")
		print("Linking Binary");
		if (sys.platform=="win32"):
		 
			# copy ark2d/build/windows in to tmpdir
			builddirsorc = self.game_dir + self.ds + self.build_folder + self.ds+ self.platform;
			builddirdest = self.ark2d_tmpdir;
			try:
				#os.system("rmdir /S " + builddirdest + self.ds + self.build_folder + self.ds + self.platform);
				os.system("rmdir /S /Q " + builddirdest );
				shutil.copytree(builddirsorc, builddirdest);
			except:
				print("could not copy sources [from:"+builddirsorc+",to:"+builddirdest);
				return;
				pass;
			
			linkingStr = "";
			linkingStr += self.gppCompiler + " " + self.mingw_link + " " + self.linkingFlags + " -o" + self.game_dir + self.ds + self.build_artifact + "";
			#linkingStr += self.gppCompiler + " " + self.mingw_link + " -o" + self.build_artifact + "";
		
			for h in self.src_files:
				findex = h.rfind('.');
				newf = h[0:findex] + ".o";
				#print(newf);
				h_ext = h[findex+1:len(h)];
				linkingStr += " " + self.ark2d_tmpdir + self.ds + newf;
			
			for f in self.dll_files:
				if (self.building_library):
					#linkingStr += " " + self.game_dir + self.ds + f;
					linkingStr += " " + f;				
				else:
					linkingStr += " " + f;				

					
			for f in self.static_libraries:
				linkingStr += " -l" + f;
				#pass;
				
			#print(linkingStr);
			
			#subprocess.call([linkingStr], shell=True);	
			#print(len(linkingStr));
			#os.system(linkingStr);
			#subprocess.call([ linkingStr ], shell=True);	
					
			editsStrReplace = [("\\", "\\")];
			linkingStrClean = self.str_replace(linkingStr, editsStrReplace);
					
			linkbat = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "dolink.bat";
			f = open(linkbat, "w")
			f.write(linkingStrClean);
			f.close();
			
			subprocess.call([ linkbat ], shell=True);	
			
			
			
			
			
			
			#copy game resources in to .build
			if(self.building_game):
			
				#copying dll in to project.
				dll = self.ark2d_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + 'libARK2D.dll'
				dlldest = self.game_dir.replace('\\\\','\\') + '\\' + self.build_folder + '\\' + self.platform;
				try:
					shutil.copy(dll, dlldest);
				except:
					print("could not copy " + dll + " into " + dlldest);
					pass;
					
				#copying other dlls in to project.
				try:
					otherdlls = ['alut.dll', 'freetype6.dll', 'glew32.dll', 'OpenAL32.dll', 'wrap_oal.dll', 'zlib1.dll', 'libcurl.dll'];
					for one in otherdlls:
						one = self.ark2d_dir + self.ds + 'lib' + self.ds + self.platform + self.ds + one;
						shutil.copy(one, self.game_dir.replace('\\\\','\\') + '\\' + self.build_folder + '\\' + self.platform);
				except:
					pass;

				#generate spritesheets
				self.generateSpriteSheets();
			
				if (self.game_resources_dir != ''):
					print("copying game resources in to project:");
					#cpy_game_res = 'copy "' + self.game_resources_dir.replace('\\\\','\\') + '" "' + self.game_dir.replace('\\\\','\\') + '\\' + self.build_folder + '\\' + self.platform + '\\"';
					#print(cpy_game_res);
					#subprocess.call([cpy_game_res], shell=True);
					thisgameresdir = self.game_resources_dir.replace('\\\\','\\');
					thisgameresdir2 = self.game_dir.replace('\\\\','\\') + '\\' + self.build_folder + '\\' + self.platform + '\\data';
					try:
						os.system("rmdir /S /Q " + thisgameresdir2); 
						shutil.copytree(thisgameresdir, thisgameresdir2);
					except:
						print("could not copy resources [from:"+thisgameresdir+",to:"+thisgameresdir2);
						pass;
				 
			
		elif(sys.platform=="darwin"): 
			
			
			if (self.building_library):
				linkingStr = ""; 
				linkingStr += self.gppCompiler + " -L " + self.game_dir + self.ds + "lib/osx/freetype -L /Developer/SDKs/MacOSX10.6.sdk/usr/lib -framework OpenGL -framework OpenAL -framework Foundation -framework Cocoa -framework ApplicationServices -lobjc -install_name @executable_path/../Frameworks/libARK2D.dylib " + self.linkingFlags + "  -dynamiclib -o " + self.game_dir + self.ds + self.build_artifact;
				#linkingStr += " -march=i386 ";
			
				for h in self.src_files:
					findex = h. rfind('.');
					newf = h[0:findex] + ".o";
					linkingStr += " " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + newf;
				
				for f in self.dll_files:
					linkingStr += " " + f;
					
				for f in self.static_libraries:
					linkingStr += " -l" + f;
					
				print(linkingStr);
				
				subprocess.call([linkingStr], shell=True);	
					
			elif(self.building_game):
				
				# we need to make the dirs for the .app file.
				gn = self.game_name.replace(' ', '\ ');
				app_folder = self.game_dir + self.ds + self.build_folder + self.ds + gn + ".app";
				contents_folder = app_folder + self.ds + "Contents";
				frameworks_folder = app_folder + self.ds + "Contents" + self.ds + "Frameworks";
				resources_folder = app_folder + self.ds + "Contents" + self.ds + "Resources";
				subprocess.call(['mkdir ' + app_folder], shell=True);
				subprocess.call(['mkdir ' + contents_folder ], shell=True);
				subprocess.call(['mkdir ' + contents_folder + self.ds + "MacOS"], shell=True);
				subprocess.call(['mkdir ' + resources_folder], shell=True);
				subprocess.call(['mkdir ' + resources_folder + self.ds + "data"], shell=True);
				subprocess.call(['mkdir ' + frameworks_folder], shell=True);
				
				#copying dylib in to project.
				dylibsrc = self.ark2d_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + 'libARK2D.dylib'
				subprocess.call(['cp ' + dylibsrc + ' ' + frameworks_folder + self.ds + 'libARK2D.dylib'], shell=True);
				
				
				#copy ark2d resources in to .app
				print("---");
				print("Copying ARK2D resources in to project:");
				cpyark2dres = 'cp -r ' + self.ark2d_dir + self.ds + 'data ' + resources_folder + self.ds + 'data' + self.ds + 'ark2d';
				print(cpyark2dres);
				subprocess.call([cpyark2dres], shell=True);
				
				#generate game spritesheets
				self.generateSpriteSheets();

				#copy game resources in to .app
				if (self.game_resources_dir != ''):
					print("---");
					print("Copying game resources in to project:");
					gme_rs_dir = self.game_resources_dir.replace(' ', '\ ').replace('&', '\&');
					cpy_game_res = 'cp -r ' + gme_rs_dir + ' ' + resources_folder;
					print(cpy_game_res);
					subprocess.call([cpy_game_res], shell=True);
				
				
				#copy icns in to .app folder
				if (self.mac_game_icns != ''):
					subprocess.call(['cp ' + self.mac_game_icns.replace(' ', '\ ').replace('&', '\&') + ' ' + resources_folder + self.ds + gn +'.icns'], shell=True);
				else:
					subprocess.call(['cp ' + resources_folder + self.ds + 'ark2d' + self.ds + 'icon.icns ' + resources_folder + self.ds + gn +'.icns'], shell=True);
				
				print("---");
				print("Generating plist file");
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
				infoplistcontents += "		<key>CFBundleGetInfoString</key>" + cr;
				infoplistcontents += "		<string>" + self.config['game_name'] + ". Copyright " + self.developer_name + ".</string>" + cr;
				#infoplistcontents += "		<key>NSHumanReadableCopyright</key>" + cr;
				#infoplistcontents += "		<string>Copyright YEAR Thoname</string>" + cr;
				infoplistcontents += "		<key>CFBundleIconFile</key>" + cr;
				infoplistcontents += "		<string>" + self.game_name + "</string>" + cr;
				infoplistcontents += "	</dict>" + cr;
				infoplistcontents += "</plist>" + cr;
				
				f = open(contents_folder.replace('\ ', ' ') + self.ds + "Info.plist", "w");
				f.write(infoplistcontents);
				f.close();
				
				self.build_artifact = app_folder + self.ds + "Contents" + self.ds + "MacOS" + self.ds + gn;
				
				print("---");
				print("Creating Executable");

				linkingStr = "";
				linkingStr += self.gppCompiler + " -o " + self.build_artifact + " "; 
				for h in self.src_files: 
					findex = h.rfind('.');
					newf = h[0:findex] + ".o";
					h_ext = h[findex+1:len(h)];
					if (h_ext != 'rc'):
						linkingStr += " " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + newf;
				
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

				print("---");
				print("Build Complete");
				print("---");
			
		
		
	def startMac(self):
		self.startWindows();

	def startFlascc(self):
		print("flascc build");

		# open config
		print("opening config");
		f = open(self.game_dir + "/config.json", "r");
		fcontents = f.read();
		f.close();
		config = json.loads(fcontents);

		print("assigning vars");
		ark2d_dir = config['osx']['ark2d_dir'];
		flascc_dir = "/Users/ashleygwinnell/crossbridge/sdk";
		flexsdk_dir = "/Users/ashleygwinnell/flex_sdk_4_6";
		gsl3d_dir = "/Users/ashleygwinnell/Dropbox/ark2d/ARK2D/lib/flascc/GLS3D";

		if (self.building_library):

			print("making directories");
			flascc_folder = config['osx']['ark2d_dir'] + "/build/flascc";
			mkdirs = [];
			mkdirs.extend(self.mkdirs);
			self.makeDirectories(mkdirs);

			print("sorting out cache thing")
			cacheJSON = self.openCache("compiled");
			cacheChanged = False;

			#subprocess.call(["cp -r " + config['osx']['ark2d_dir'] + "/lib/flascc/Makefile " + flascc_folder+"/Makefile"], shell=True);

			#flascc_dir = "~/flascc/sdk";
			#flexsdk_dir = "~/flex_sdk";

			

			
			#LOCAL_C_INCLUDES := /Users/ashleygwinnell/Dropbox/ark2d/ARK2D/src/ARK2D/vendor/libcurl;
			print("generating makefile");
			#-Werror  
			makefileStr = "";
			makefileStr += """ 

ARK2D:=""" + ark2d_dir + """
FLASCC:=""" + flascc_dir + """
GLS3D:=""" + gsl3d_dir + """

BASE_CFLAGS:= -Wall -Wno-write-strings -Wno-trigraphs

build: 
	echo "Compile for flascc!"
	 
	echo "Now compile a SWC"

	# compiling ark2d src files...\r\n""";

			thissourcefiles = [];
			thissourcefiles.extend(config['src_files']['flascc']);
			thissourcefiles.extend(config['src_files']['all']);

			
			srcindex = 0;
			srccount = len(thissourcefiles);
			for srcfile in thissourcefiles:
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newf = srcfile[0:findex] + ".o";

				if (not srcfile in cacheJSON or cacheJSON[srcfile]['date_modified'] < os.stat(ark2d_dir + self.ds + srcfile).st_mtime):

					compiler = "g++";
					if (h_ext == "c"):
						compiler = "gcc";
					#makefileStr += "	@echo \"---\"\r\n";
					makefileStr += "	@echo " + str(srcindex+1) + " of " + str(srccount) + ": \""+ srcfile + " \"\r\n";
					makefileStr += "	@\"$(FLASCC)/usr/bin/" + compiler + "\" $(BASE_CFLAGS) -O4 -DARK2D_FLASCC ";
					makefileStr += "-I$(GLS3D)/install/usr/include ";
					makefileStr += "-I$(ARK2D)/src/ARK2D/vendor/libcurl ";
					makefileStr += "-I$(ARK2D)/src/ARK2D/vendor/android/freetype/jni/include "
					makefileStr += "-I$(ARK2D)/src/ARK2D/vendor/spine/includes "
					makefileStr += "$(ARK2D)/" + srcfile + " ";
					makefileStr += "-c -o $(ARK2D)/build/flascc/" + newf + "";
					makefileStr += "\r\n";

					cacheJSON[srcfile] = {"date_modified": os.stat(ark2d_dir + self.ds + srcfile).st_mtime };
					cacheChanged = True;

				else:
					#print("skipping. no changes in this file. ");
					pass;

				srcindex += 1;
				pass;
			
			
			"""
			makefileStr += "	# linking (swc)...\r\n";
			linkingStr = "	@\"$(FLASCC)/usr/bin/g++\" $(BASE_CFLAGS) -O4 -DARK2D_FLASCC -o $(ARK2D)/build/flascc/libark2d.a ";
			for srcfile in thissourcefiles:
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newf = srcfile[0:findex] + ".o";
				linkingStr += " $(ARK2D)/build/flascc/" + newf + " ";
				pass; 

			#linkingStr += " --enable-static --disable-shared ";
			linkingStr += " --disable-static --enable-shared -dynamiclib ";
			linkingStr += " -L$(GLS3D)/install/usr/lib/ -L$(ARK2D)/lib/flascc/install/usr/lib/";
			linkingStr += " -lGL -lfreetype ";
			#linkingStr += " -emit-swc=org.ashleygwinnell.ark2d  ";
			makefileStr += linkingStr;
			"""

			staticlibStr = "";
			staticlibStr += "	$(FLASCC)/usr/bin/ar cr $(ARK2D)/build/flascc/libark2d.a "; 
			for srcfile in thissourcefiles:
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newf = srcfile[0:findex] + ".o";
				staticlibStr += " $(ARK2D)/build/flascc/" + newf + " ";
				pass;
			makefileStr += staticlibStr;
			#
			#	makefileStr += "/Users/ashleygwinnell/Dropbox/ark2d/ARK2D/" + srcfile + " \\";
			#	makefileStr += "\r\n";
			

			makefileStr += """ \
			

clean:
	#rm -f *.swf *.swc *.bc *.exe

""";

			f = open(flascc_folder+"/Makefile", "w");
			f.write(makefileStr);
			f.close();

			makeCmd = "make --directory=/Users/ashleygwinnell/Dropbox/ark2d/ARK2D/build/ -f /Users/ashleygwinnell/Dropbox/ark2d/ARK2D/build/flascc/Makefile";
			print("running makefile");
			print(makeCmd);
			subprocess.call([makeCmd], shell=True);	

			# update compile cache thing
			print("saving cache file");
			if (cacheChanged == True):
				self.saveCache("compiled", cacheJSON);
			else:
				print("...nothing to compile!");
			
			pass;
		elif (self.building_game):
			print("----------");
			print("building game ");
			print("----------");

			game_dir = config['osx']['game_dir'];
			game_resources_dir = config['osx']['game_resources_dir'];
			flascc_folder = game_dir + "/build/flascc";

			print("----------");
			print("making directories");
			print("----------");
			mkdirs = [];
			mkdirs.extend(self.mkdirs);
			mkdirs.extend([ flascc_folder+"/gamevfs" ]);
			mkdirs.extend([ flascc_folder+"/data" ]);
			mkdirs.extend([ flascc_folder+"/data/ark2d" ]);
			mkdirs.extend([ flascc_folder+"/data/ark2d/fonts" ]);
			#mkdirs.extend([ flascc_folder+"/gamevfs/compiled" ]);
			self.makeDirectories(mkdirs);

			directoriesToCreate = self.listDirectories(game_resources_dir, False);
			for dir in directoriesToCreate:
				self.makeDirectories([game_dir + "/build/flascc/data/" + dir]);

			moreDirectoriesToCreate = self.listDirectories(game_dir+"/src", False);
			for dir in moreDirectoriesToCreate:
				self.makeDirectories([game_dir + "/build/flascc/src/" + dir]);

			print("----------");
			print("copying Preloader and Console files");
			print("----------");
			subprocess.call(["cp -r " + ark2d_dir + "/lib/flascc/PreLoader.as " + game_dir + "/build/flascc/PreLoader.as"], shell=True);
			subprocess.call(["cp -r " + ark2d_dir + "/lib/flascc/Console.as " + game_dir + "/build/flascc/Console.as"], shell=True);

			print("----------");
			print("copying Flascc .C file glue");
			print("----------"); 
			subprocess.call(["cp -r " + ark2d_dir + "/lib/flascc/flascc.cpp " + game_dir + "/src/flascc.cpp"], shell=True);
 
			#print("copying game files to build dir");
			#subprocess.call(["cp -r " + game_dir + "/data/ " + game_dir + "/build/flascc/data"], shell=True);

			print("----------");
			print("Creating/opening assets cache file...");
			print("----------");
			assetsCache = flascc_folder + "/build-cache/assets.json";
			assetsJson = self.openCacheFile(assetsCache);
			assetsChanged = False; 

			#print("make mp3s and replace the oggs...");
			#game_resources_dir = game_dir + "/data";
			print("----------");
			print("Cool, now copying (game) files");
			print("----------");
			filesToCopy = self.listFiles(game_resources_dir, False);
			print(filesToCopy);
			for file in filesToCopy:
				fromfile = game_resources_dir + self.ds + file;
				tofile = game_dir + "/build/flascc/data/" + file;
				
				#replace spaces in paths on max osx with slash-spaces
				#fromfile = fromfile.replace(" ", "\ ");
				#tofile = tofile.replace(" ", "\ ");
				
				if (not fromfile in assetsJson or assetsJson[fromfile]['date_modified'] < os.stat(fromfile).st_mtime):
					file_ext = self.get_str_extension(file);
					if (file_ext == "ogg"): # resample
						print("resampling audio file from: " + fromfile + " to: " + tofile); 
						#% cat inputfile | lame [options] - - > output
						subprocess.call(["oggdec "+fromfile+" --quiet --output=- | lame --quiet -h - - > "+tofile+""], shell=True);
					elif (file_ext == "wav"): # resample
						print("resampling audio file from: " + fromfile + " to: " + tofile); 
						#% cat inputfile | lame [options] - - > output
						subprocess.call(["lame -V3 --quiet " +fromfile+ " "+tofile+""], shell=True);
					else: # standard copy
						print("copying file from: " + fromfile + " to: " + tofile);
						subprocess.call(["cp -r " + fromfile + " " + tofile], shell=True);
						
					assetsJson[fromfile] = {"date_modified": os.stat(fromfile).st_mtime };
					assetsChanged = True;
					
			if (assetsChanged == True):
				f = open(assetsCache, "w")
				f.write(json.dumps(assetsJson, sort_keys=True, indent=4));
				f.close();


			print("----------");
			print("Cool, now copy in ARK2D files...");
			subprocess.call(["cp -r " + ark2d_dir + "/data/ " + game_dir + "/build/flascc/data/ark2d"], shell=True);

			print("----------");
			print("generating index.html"); 
			game_width = config['flascc']['game_width'];
			game_height = config['flascc']['game_height'];
			indexpagestr = "";
			editsStrReplace = [("%GAME_NAME%", config['game_name']), ("%GAME_DESCRIPTION%", config['game_description']), ("%GAME_WIDTH%", str(game_width)), ("%GAME_HEIGHT%", str(game_height)), ("%COMPANY_NAME%", self.developer_name) ];
			f = open(ark2d_dir+"/lib/flascc/index.html", "r");
			indexpagestr = f.read();
			f.close();
			indexpagestr = self.str_replace(indexpagestr, editsStrReplace);
			f = open(flascc_folder+"/index.html", "w");
			f.write(indexpagestr);
			f.close();	

			print("----------");
			print("generating makefile");
			print("----------");
			makefileStr = "";  
			makefileStr += """ 

ARK2D:=""" + ark2d_dir + """
GAMEDIR:=""" + game_dir + """
FLASCC:=""" + flascc_dir + """
GLS3D:=""" + gsl3d_dir + """

BASE_CFLAGS:= -Wall -Wno-write-strings -Wno-trigraphs

build: 
	@echo "----------"
	@echo "Compile for flascc!"
	@echo "----------"
	@echo "Generate Virtual FileSystem"
	$(FLASCC)/usr/bin/genfs --type=embed --name=gamevfs $(GAMEDIR)/build/flascc/data/ $(GAMEDIR)/build/flascc/gamevfs/gamevfs
	@echo "----------"
	@echo "Compile Virtual FileSystem"
	java -Xmx2g -jar $(FLASCC)/usr/lib/asc2.jar -merge -md -AS3 -strict -optimize \
		-import $(FLASCC)/usr/lib/builtin.abc \
		-import $(FLASCC)/usr/lib/playerglobal.abc \
		-import $(FLASCC)/usr/lib/BinaryData.abc \
		-import $(FLASCC)/usr/lib/ISpecialFile.abc \
		-import $(FLASCC)/usr/lib/IBackingStore.aBC \
		-import $(FLASCC)/usr/lib/IVFS.abc \
		-import $(FLASCC)/usr/lib/InMemoryBackingStore.abc \
		-import $(FLASCC)/usr/lib/PlayerKernel.abc \
		$(GAMEDIR)/build/flascc/gamevfs/gamevfs*.as -outdir $(GAMEDIR)/build/flascc/ -out gamevfs

	@echo "----------"
	@echo "Compile Console and Preloader"
	java -jar $(FLASCC)/usr/lib/asc2.jar -merge -md -AS3 -strict -optimize \
		-import $(FLASCC)/usr/lib/builtin.abc \
		-import $(FLASCC)/usr/lib/playerglobal.abc \
		-import $(GLS3D)/install/usr/lib/libGL.abc \
		-import $(FLASCC)/usr/lib/ISpecialFile.abc \
		-import $(FLASCC)/usr/lib/IBackingStore.abc \
		-import $(FLASCC)/usr/lib/IVFS.abc \
		-import $(FLASCC)/usr/lib/InMemoryBackingStore.abc \
		-import $(FLASCC)/usr/lib/AlcVFSZip.abc \
		-import $(FLASCC)/usr/lib/CModule.abc \
		-import $(FLASCC)/usr/lib/C_Run.abc \
		-import $(FLASCC)/usr/lib/BinaryData.abc \
		-import $(FLASCC)/usr/lib/PlayerKernel.abc \
		-import $(GAMEDIR)/build/flascc/gamevfs.abc \
		$(FLASCC)/usr/share/LSOBackingStore.as $(GAMEDIR)/build/flascc/Console.as -outdir $(GAMEDIR)/build/flascc -out Console

	java -jar $(FLASCC)/usr/lib/asc2.jar -merge -md -AS3 -strict -optimize \
		-import $(FLASCC)/usr/lib/builtin.abc \
		-import $(FLASCC)/usr/lib/playerglobal.abc \
		-import $(GLS3D)/install/usr/lib/libGL.abc \
		-import $(FLASCC)/usr/lib/ISpecialFile.abc \
		-import $(FLASCC)/usr/lib/IBackingStore.abc \
		-import $(FLASCC)/usr/lib/IVFS.abc \
		-import $(FLASCC)/usr/lib/CModule.abc \
		-import $(FLASCC)/usr/lib/C_Run.abc \
		-import $(FLASCC)/usr/lib/BinaryData.abc \
		-import $(FLASCC)/usr/lib/PlayerKernel.abc \
		-import $(GAMEDIR)/build/flascc/Console.abc \
		-import $(GAMEDIR)/build/flascc/gamevfs.abc \
		$(GAMEDIR)/build/flascc/PreLoader.as -swf com.adobe.flascc.preloader.PreLoader,"""+str(game_width)+""","""+str(game_height)+""",60 -outdir $(GAMEDIR)/build/flascc -out PreLoader

	@echo "----------"
""";
			
			thissourcefiles = [];
			thissourcefiles.extend(config['game_src_files']);
			thissourcefiles.extend([ "src/flascc.cpp" ]);
			
			srcindex = 0;
			srccount = len(thissourcefiles);
			for srcfile in thissourcefiles:
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newf = srcfile[0:findex] + ".o";

				compiler = "g++";
				if (h_ext == "c"):
					compiler = "gcc";
				elif (h_ext == "rc"):
					continue;

				makefileStr += "	@echo " + str(srcindex+1) + " of " + str(srccount) + ": \""+ srcfile + " \"\r\n";
				makefileStr += "	@\"$(FLASCC)/usr/bin/" + compiler + "\" $(BASE_CFLAGS) -O4 -DARK2D_FLASCC ";
				makefileStr += "-I$(GLS3D)/install/usr/include ";
				makefileStr += "-I$(ARK2D)/src/ARK2D/vendor/libcurl ";
				makefileStr += "-I$(ARK2D)/src/ARK2D/vendor/android/freetype/jni/include  "
				makefileStr += "-I$(ARK2D)/src/ARK2D/vendor/spine/includes "
				makefileStr += "$(GAMEDIR)/" + srcfile + " ";
				makefileStr += "-c -o $(GAMEDIR)/build/flascc/" + newf;
				makefileStr += "\r\n";

				srcindex += 1;
				pass;

			makefileStr += "	# linking...\r\n";
			linkingStr = "";
			#linkingStr += "\"" + flexsdk_dir + "/bin/mxmlc\" -static-link-runtime-shared-libraries -compiler.omit-trace-statements=false -library-path=MurmurHash.swc -debug=false swcdemo.as -o swcdemo.swf
			linkingStr += "	@\"$(FLASCC)/usr/bin/g++\" $(BASE_CFLAGS) -O4 -DARK2D_FLASCC ";
			for srcfile in thissourcefiles:
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newf = srcfile[0:findex] + ".o";

				if (h_ext == "rc"):
					continue;

				linkingStr += " $(GAMEDIR)/build/flascc/" + newf + " ";
				pass;

			
			print("opening config");
			f2 = open(self.ark2d_dir + "/config.json", "r");
			f2contents = f2.read();
			f2.close();
			arkconfig = json.loads(f2contents);
			arksrcfiles = []
			arksrcfiles.extend(arkconfig['src_files']['flascc']);
			arksrcfiles.extend(arkconfig['src_files']['all']);
			for srcfile in arksrcfiles:
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newf = srcfile[0:findex] + ".o";
				if ("src/main" in srcfile[0:findex]): 
					continue;
				else:
					linkingStr += " $(ARK2D)/build/flascc/" + newf + " ";
				pass;

			

			linkingStr += " -L$(GLS3D)/install/usr/lib/ ";
			linkingStr += " -L/Users/ashleygwinnell/Dropbox/ark2d/ARK2D/lib/flascc/install/usr/lib/ ";
			#linkingStr += " -L$(ARK2D)/build/flascc ";
			#linkingStr += " -lark2d ";
			linkingStr += " -lGL -lfreetype -lAS3++ -lFlash++ ";
			#linkingStr += " -lcurl ";
			linkingStr += " -pthread ";
			#-emit-swf=org.ashleygwinnell.defaultgame



			linkingStr += " $(FLASCC)/usr/lib/AlcVFSZip.abc $(GAMEDIR)/build/flascc/gamevfs.abc -swf-preloader=$(GAMEDIR)/build/flascc/PreLoader.swf -swf-version=18 -symbol-abc=$(GAMEDIR)/build/flascc/Console.abc -jvmopt=-Xmx4G ";
			linkingStr += " -swf-size="+str(game_width)+"x"+str(game_height)+" -emit-swf -o $(GAMEDIR)/build/flascc/game.swf -j4 ";
			makefileStr += linkingStr;
 
 
			f = open(flascc_folder+"/Makefile", "w");
			f.write(makefileStr);
			f.close(); 

			makeCmd = "make --directory=" + game_dir + "/build/ -f " + game_dir + "/build/flascc/Makefile";
			print("----------");
			print("running makefile");
			print("----------");
			print(makeCmd);
			subprocess.call([makeCmd], shell=True);	

			print("----------");
			print("DONE! >:D");
			print("----------");

			pass;
 
		pass;

	def startUbuntuLinux(self):

		print("-------------------------");
		print("Linux");
		root_dir = "";
		if self.building_library:
			root_dir = self.config['linux']['ark2d_dir'];
		else:
			root_dir = self.game_dir;

		print("-------------------------");		
		print("Make directories...")
		mkdirs = [];
		mkdirs.extend(self.mkdirs);

		if self.building_game:
			self.generateARKH(root_dir + "/src/ARK.h");

			mkdirs.extend([root_dir + "/build/" + self.output + "/data"]);
			mkdirs.extend([root_dir + "/build/" + self.output + "/data/ark2d"]);
			mkdirs.extend([root_dir + "/data/ark2d"]);

			directoriesToCreate = self.listDirectories(root_dir + "/src/", False);
			for dir in directoriesToCreate:
				self.makeDirectories([root_dir + "/build/" + self.output + "/src/" + dir]);


		self.makeDirectories(mkdirs);

		# Do we want to do the SDL build?
		useSDL2 = True;

		print("-------------------------");
		print("Loading build cache...")
		cacheJSON = self.openCache("compiled");
		cacheChanged = False;
		
		print("-------------------------");
		print("Compiling Source Files...");
		print("-------------------------");
		for srcFile in self.src_files:
			
			srcFileIndex = srcFile.rfind('.');
			srcFileExtension = srcFile[srcFileIndex+1:len(srcFile)];
			srcFileNew = srcFile[0:srcFileIndex] + ".o";

			compileStr = "";
			if srcFileExtension == 'c':
				compileStr += self.gccCompiler;
			elif srcFileExtension == 'cpp':
				compileStr += self.gppCompiler;
			elif srcFileExtension == 'rc':
				continue;
			
			compileStr += " -DARK2D_UBUNTU_LINUX "; 
			compileStr += " -DARK2D_DESKTOP "; 
			compileStr += " -DGL_GLEXT_PROTOTYPES ";

			if (useSDL2):
				compileStr += " -DARK2D_SDL2 ";

			compileStr += " -Wno-trigraphs -Wno-deprecated-declarations -Wreturn-type -fexceptions -fno-builtin-exit ";
			if self.building_library:
				compileStr += " -fPIC ";

			if srcFileExtension == 'c':
				compileStr += " -Wno-implicit-function-declaration  -Wno-implicit ";
			elif srcFileExtension == "cpp":
				compileStr += " -fpermissive ";

			compileStr += " -c ";
			if ("vendor" not in srcFile):
				compileStr += " -O3 ";

			compileStr += " -I /usr/include ";
			compileStr += " -I " + self.ark2d_dir + "/src/ARK2D/vendor/angelscript ";
			compileStr += " -I " + self.ark2d_dir + "/src/ARK2D/vendor/iphone ";
			compileStr += " -I " + self.ark2d_dir + "/src/ARK2D/vendor/spine/includes ";
			compileStr += " -I " + self.ark2d_dir + "/lib/ubuntu-linux/include ";
			compileStr += " -I " + self.ark2d_dir + "/src/ ";
			if (useSDL2):
				compileStr += " -I " + self.ark2d_dir + "/src/ ";


			if self.building_game: 
				
				for includedir in self.include_dirs: 
					includedir_actual = self.str_replace(includedir, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					compileStr += " -I " + includedir_actual + " ";


			compileStr += " -o \"" + root_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + srcFileNew + "\"  \"" + srcFile + "\" ";
			compileStr += " -lrt ";
			
			if (not srcFile in cacheJSON or cacheJSON[srcFile]['date_modified'] < os.stat(srcFile).st_mtime):
				cacheJSON[srcFile] = {"date_modified": os.stat(srcFile).st_mtime };
				cacheChanged = True;

				print("Compiling " + srcFileNew);
				#print(compileStr);
				os.system(compileStr);

		if (cacheChanged == True):
			self.saveCache("compiled", cacheJSON);
		
		if (self.building_library):
			print("-------------------------");
			print("Creating Shared Object");
			linkStr = "";
			linkStr += "gcc -shared -o " + self.ark2d_dir + self.ds + "build/linux/libark2d.so ";
			for srcFile in self.src_files:
				srcFileIndex = srcFile.rfind('.');
				srcFileExtension = srcFile[srcFileIndex+1:len(srcFile)];
				srcFileNew = srcFile[0:srcFileIndex] + ".o";

				linkStr += self.ark2d_dir + "/" + self.build_folder + "/" + self.platform + "/" + srcFileNew + " ";
			linkStr += self.ark2d_dir + "/lib/ubuntu-linux/libangelscript.a ";

			print(linkStr);
			os.system(linkStr);
		elif (self.building_game):

			print("-------------------------");
			print("Copying Libraries ");
			os.system("cp " + self.ark2d_dir + "/build/linux/libark2d.so " + root_dir + "/build/" + self.output + "/libark2d.so");
			for libhere in self.libs:
				print("Copying " + libhere);
				os.system("cp " + libhere + " " + root_dir + "/build/" + self.output + "/ ");

			#if "libs" in self.config['linux']:
			#	for libhere in self.config['linux']['libs']:
			#		libhere_actual = self.str_replace(libhere, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir]), ("%ARK2D_DIR%", self.ark2d_dir])]);
			#		print("Copying " + libhere_actual);
			#		os.system("cp " + libhere_actual + " " + root_dir + "/build/" + self.output + "/ ");

			print("-------------------------");
			print("Creating Executable");
			executableStr = "";
			executableStr += "gcc -o " + root_dir + self.ds + "build/" + self.output + "/game ";
			for srcFile in self.src_files:
				srcFileIndex = srcFile.rfind('.');
				srcFileExtension = srcFile[srcFileIndex+1:len(srcFile)];
				srcFileNew = srcFile[0:srcFileIndex] + ".o";

				if srcFileExtension == 'rc':
					continue;

				executableStr += root_dir + "/" + self.build_folder + "/" + self.output + "/" + srcFileNew + " ";

			executableStr += " -L" + root_dir + "/" + self.build_folder + "/" + self.output + " ";
			executableStr += " -lark2d -lstdc++ ";
			executableStr += " -lm -lGL -lalut -lcurl -lGLU -lgcc ";
			if (useSDL2):
				executableStr += " -lSDL2 ";
			else:
				executableStr += " -lXinerama ";


			#if "libs" in self.config['linux']:
			#	for libhere in self.config['linux']['libs']:
			#		libhere_actual = self.str_replace(libhere, [("%PREPRODUCTION_DIR%", config['linux']['game_preproduction_dir']), ("%ARK2D_DIR%", config['linux']['ark2d_dir'])]);
			#		libhere_actual2 = libhere_actual[libhere_actual.rfind("/")+4:libhere_actual.rfind(".")];
			#		executableStr += " -l" + libhere_actual2 + " ";
			for libhere in self.libs:
				libhere_name = libhere[libhere.rfind("/")+4:libhere.rfind(".")];
				print(libhere_name);
				if (libhere_name.find(".") == -1):
					executableStr += " -l" + libhere_name + " ";

			#executableStr += " -Wl,-Bstatic -langelscript ";#" -Wl,-Bdynamic  ";


			print(executableStr);
			os.system(executableStr);

			print("-------------------------");
			print("Copying ARK2D Data Files ");
			os.system("cp -r " + self.ark2d_dir + "/data/* " + root_dir + "/data/ark2d ");
			os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libalut.so " + root_dir + "/build/" + self.output + "/libalut.so ");
			os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libalut.so.0 " + root_dir + "/build/" + self.output + "/libalut.so.0 ");
			os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libalut.so.0.1.0 " + root_dir + "/build/" + self.output + "/libalut.so.0.1.0 ");
			os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libopenal.so " + root_dir + "/build/" + self.output + "/libopenal.so ");
			os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libopenal.so.1 " + root_dir + "/build/" + self.output + "/libopenal.so.1 ");
			os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libopenal.so.1.14.0 " + root_dir + "/build/" + self.output + "/libopenal.so.1.14.0 ");
			if (useSDL2):
				os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libSDL2.so " + root_dir + "/build/" + self.output + "/libSDL2.so ");
				os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libSDL2-2.0.so " + root_dir + "/build/" + self.output + "/libSDL2-2.0.so");
				os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libSDL2-2.0.so.0 " + root_dir + "/build/" + self.output + "/libSDL2-2.0.so.0 ");
				os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libSDL2-2.0.so.0.2.0 " + root_dir + "/build/" + self.output + "/libSDL2-2.0.so.0.2.0 ");

				os.system("cp -r /usr/lib/x86_64-linux-gnu/libXss.so " + root_dir + "/build/" + self.output + "/libXss.so ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libXss.so.1 " + root_dir + "/build/" + self.output + "/libXss.so.1 ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libXss.so.1.0.0 " + root_dir + "/build/" + self.output + "/libXss.so.1.0.0 ");

				os.system("cp -r /usr/lib/x86_64-linux-gnu/libwayland-egl.so " + root_dir + "/build/" + self.output + "/libwayland-egl.so ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libwayland-egl.so.1 " + root_dir + "/build/" + self.output + "/libwayland-egl.so.1 ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libwayland-egl.so.1.0.0 " + root_dir + "/build/" + self.output + "/libwayland-egl.so.1.0.0 ");

				os.system("cp -r /usr/lib/x86_64-linux-gnu/libwayland-client.so " + root_dir + "/build/" + self.output + "/libwayland-client.so ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libwayland-client.so.0 " + root_dir + "/build/" + self.output + "/libwayland-client.so.0 ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libwayland-client.so.0.2.0 " + root_dir + "/build/" + self.output + "/libwayland-client.so.0.2.0 ");

				os.system("cp -r /usr/lib/x86_64-linux-gnu/libwayland-cursor.so " + root_dir + "/build/" + self.output + "/libwayland-cursor.so ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libwayland-cursor.so.0 " + root_dir + "/build/" + self.output + "/libwayland-cursor.so.0 ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libwayland-cursor.so.0.0.0 " + root_dir + "/build/" + self.output + "/libwayland-cursor.so.0.0.0 ");

				os.system("cp -r /usr/lib/x86_64-linux-gnu/libxkbcommon.so " + root_dir + "/build/" + self.output + "/libxkbcommon.so ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libxkbcommon.so.0 " + root_dir + "/build/" + self.output + "/libxkbcommon.so.0 ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libxkbcommon.so.0.0.0 " + root_dir + "/build/" + self.output + "/libxkbcommon.so.0.0.0 ");

				os.system("cp -r /usr/lib/x86_64-linux-gnu/libffi.so " + root_dir + "/build/" + self.output + "/libffi.so ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libffi.so.6 " + root_dir + "/build/" + self.output + "/libffi.so.6 ");
				os.system("cp -r /usr/lib/x86_64-linux-gnu/libffi.so.6.0.1 " + root_dir + "/build/" + self.output + "/libffi.so.6.0.1 ");

				#os.system("cp -r " + self.ark2d_dir + "/lib/ubuntu-linux/libangelscript_s.so " + root_dir + "/build/" + self.output + "/libangelscript_s.so ");

			print("-------------------------");
			print("Copying Game Data Files ");
			os.system("cp -r " + root_dir + "/data/* " + root_dir + "/build/" + self.output + "/data ");

			
			print("-------------------------");
			print("Generating run.sh");
			run_sh = "";
			run_sh += "#/bin/sh \n";
			run_sh += "export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH\n";
			run_sh += "exec ./game";	
			f = open(root_dir + "/build/" + self.output + "/run.sh", "w");
			f.write(run_sh); 
			f.close();

			# make the sh file executable?
			os.system("chmod 0777 " + root_dir + "/" + self.build_folder + "/" + self.output + "/run.sh");
			os.system("chmod 0777 " + root_dir + "/" + self.build_folder + "/" + self.output + "/game");

			print("-------------------------");
			print("Finished!");
			print("-------------------------");



	def startHTML5(self):

		print("-------------------------");
		print("HTML5");
		ark2d_dir = self.config[self.platformOn]['ark2d_dir'];
		root_dir = self.config[self.platformOn]['ark2d_dir'] if self.building_library else self.config[self.platformOn]['game_dir'];
		emscripten_dir = self.config['html5'][self.platformOn]['emscripten_dir'];
		em_gcc = emscripten_dir + self.ds + "emscripten" + self.ds + "1.22.0" + self.ds + "emcc";
		em_gpp = emscripten_dir + self.ds + "emscripten" + self.ds + "1.22.0" + self.ds + "em++";

		print("-------------------------");		
		print("Make directories...")
		mkdirs = [];
		mkdirs.extend(self.mkdirs);
		mkdirs.extend([root_dir + self.ds + self.build_folder + self.ds + self.platform]);

		if self.building_game:
			mkdirs.extend([root_dir + "/data/ark2d"]);
			mkdirs.extend([root_dir + "/build/html5/data"]);
			mkdirs.extend([root_dir + "/build/html5/data/ark2d"]);

		self.makeDirectories(mkdirs);

		print("-------------------------");
		print("Loading build cache...")
		cacheJSON = self.openCache("compiled");
		cacheChanged = False;

		print("-------------------------");	
		print("Compiling Sources");	
		for srcFile in self.src_files:
			
			srcFileIndex = srcFile.rfind('.');
			srcFileExtension = srcFile[srcFileIndex+1:len(srcFile)];
			srcFileNew = srcFile[0:srcFileIndex] + ".o";

			compileStr = "";
			if srcFileExtension == 'c':
				compileStr += em_gcc;
			elif srcFileExtension == 'cpp':
				compileStr += em_gpp;
			elif srcFileExtension == 'rc':
				continue;
			
			compileStr += " -DARK2D_EMSCRIPTEN_JS "; 
			compileStr += " -DGL_GLEXT_PROTOTYPES ";
			"""compileStr += " -Wno-trigraphs -Wno-deprecated-declarations -Wreturn-type -fexceptions "; #-fno-builtin-exit ";
			if self.building_library:
				compileStr += " -fPIC ";

			if srcFileExtension == 'c':
				compileStr += " -Wno-implicit-function-declaration  -Wno-implicit ";
			#elif srcFileExtension == "cpp":
				#compileStr += " -fpermissive ";
				"""

			if (self.platformOn == "osx"):
				compileStr += " -DARK2D_EMSCRIPTEN_JS_ON_MACINTOSH ";

			compileStr += " -Wno-literal-conversion "
			compileStr += " -c ";
			if ("vendor" not in srcFile):
				compileStr += " -O3 ";

			#compileStr += " -s ALLOW_MEMORY_GROWTH=1 "
			#compileStr += " -s TOTAL_MEMORY=16777216 "
			compileStr += " -s TOTAL_MEMORY=134217728 "

			if self.building_game:
				compileStr += " -ffunction-sections ";

			compileStr += " -I /usr/include ";
			compileStr += " -I " + self.ark2d_dir + "/src/ARK2D/vendor/iphone ";
			compileStr += " -I " + self.ark2d_dir + "/src/ARK2D/vendor/spine/includes ";
			#compileStr += " -I " + self.ark2d_dir + "/lib/ubuntu-linux/include ";
			if self.building_game:
				if "include_dirs" in self.config['html5']:
					for includedir in self.config['html5']['include_dirs']:
						includedir_actual = self.str_replace(includedir, [("%PREPRODUCTION_DIR%", config[self.platformOn]['game_preproduction_dir']), ("%ARK2D_DIR%", config[self.platformOn]['ark2d_dir'])]);
						compileStr += " -I " + includedir_actual + " ";


			compileStr += " -o \"" + root_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + srcFileNew + "\"  \"" + srcFile + "\" ";
			#compileStr += " -lrt ";
			
			if (not srcFile in cacheJSON or cacheJSON[srcFile]['date_modified'] < os.stat(srcFile).st_mtime):
				cacheJSON[srcFile] = {"date_modified": os.stat(srcFile).st_mtime };
				cacheChanged = True;

				print("Compiling " + srcFileNew);
				#print(compileStr);
				os.system(compileStr);

		if (cacheChanged == True):
			self.saveCache("compiled", cacheJSON);
		
		
		if (self.building_library):
			print("-------------------------");
			print("Creating Shared Object");
			linkStr = "";
			linkStr += em_gcc + " -s FULL_ES2=1 -shared -o " + self.ark2d_dir + self.ds + "build/html5/libark2d.so ";
			for srcFile in self.src_files:
				srcFileIndex = srcFile.rfind('.');
				srcFileExtension = srcFile[srcFileIndex+1:len(srcFile)];
				srcFileNew = srcFile[0:srcFileIndex] + ".o";

				linkStr += self.ark2d_dir + "/" + self.build_folder + "/" + self.platform + "/" + srcFileNew + " ";

			print(linkStr);
			os.system(linkStr);
		elif (self.building_game):
			print("-------------------------");
			print("Copying Libraries ");
			os.system("cp " + self.ark2d_dir + "/build/html5/libark2d.so " + root_dir + "/build/html5/libark2d.so");
			
			print("-------------------------");
			print("Copying Game Data Files ");
			os.system("cp -r " + root_dir + "/data/* " + root_dir + "/build/html5/data ");

			print("-------------------------");
			print("Copying ARK2D Data Files ");
			os.system("cp -r " + ark2d_dir + "/data/* " + root_dir + "/data/ark2d ");

			print("-------------------------");
			print("Creating Executable");
			executableStr = "";
			executableStr += em_gcc;
			executableStr += " -s FULL_ES2=1 ";
			executableStr += " -s TOTAL_MEMORY=134217728 ";
			executableStr += "-o " + root_dir + self.ds + "build/html5/game.html ";
			for srcFile in self.src_files:
				srcFileIndex = srcFile.rfind('.');
				srcFileExtension = srcFile[srcFileIndex+1:len(srcFile)];
				srcFileNew = srcFile[0:srcFileIndex] + ".o";

				if srcFileExtension == 'rc':
					continue;

				executableStr += root_dir + "/" + self.build_folder + "/" + self.platform + "/" + srcFileNew + " ";

			executableStr += " -Wl ";
			executableStr += " --memory-init-file 1  "; 

			executableStr += " -L" + root_dir + "/" + self.build_folder + "/" + self.platform + " ";
			executableStr += " -lark2d -lstdc++ -lm -lGLESv2 -lEGL "; #-lalut -lcurl -lX11 ";

			
			#executableStr += " --preload-file ./data/ark2d/fonts/default.fnt "
			#####
			filesToBundle = self.listFiles(root_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "data", False);
			print(filesToBundle);
			for file in filesToBundle:
				thefile = "./data/" + file;
				executableStr += " --preload-file " + thefile + " ";

			print(executableStr);
			os.system(executableStr);

			print("-------------------------");
			print("Creating index.html ");
			game_width = config['html5']['game_width'];
			game_height = config['html5']['game_height'];
			indexpagestr = "";
			editsStrReplace = [("%GAME_NAME%", config['game_name']), ("%GAME_DESCRIPTION%", config['game_description']), ("%GAME_WIDTH%", str(game_width)), ("%GAME_HEIGHT%", str(game_height)), ("%GAME_HEIGHT_CENTER%", str((game_height/2)-10)), ("%COMPANY_NAME%", self.developer_name) ];
			f = open(ark2d_dir+"/lib/html5/index.html", "r");
			indexpagestr = f.read();
			f.close();
			indexpagestr = self.str_replace(indexpagestr, editsStrReplace);
			f = open(root_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "index.html", "w");
			f.write(indexpagestr);
			f.close();	
			

		print("-------------------------");
		print("Finished!");
		print("-------------------------");
			
		return;

	def listDirectories(self, dir, usefullname=True, appendStr = ""):
		thelist = [];
		for name in os.listdir(dir):
			full_name = os.path.join(dir, name);
			
			#thelist.extend([full_name]);
			if os.path.isdir(full_name):
				if usefullname==True:
					thelist.extend([appendStr + full_name]);
				else:
					thelist.extend([appendStr + name]);
					
				#thelist.extend([full_name]); 
				thelist.extend(self.listDirectories(full_name, usefullname, appendStr+name+self.ds));
			#else:
			#	os.remove(full_name);
		return thelist;
	
	def listFiles(self, dir, usefullname=True, appendStr = ""):
		thelist = [];
		for name in os.listdir(dir):
			if (self.get_str_extension(name) == "DS_Store"): 
				continue;
			 
			full_name = os.path.join(dir, name);
			#print(full_name);
			
			if os.path.isdir(full_name):
				thelist.extend(self.listFiles(full_name, usefullname, appendStr+name+self.ds));
			else:
				if usefullname==True:
					thelist.extend([appendStr + full_name]);
				else:
					thelist.extend([appendStr + name]);
		return thelist;
	
	def makeDirectories(self, dir):
		for thisstr in dir:
			print("mkdir " + thisstr);
			try:
				os.makedirs(thisstr);
			except OSError as exc: 
				if exc.errno == errno.EEXIST:
					pass
				else: raise
		pass;
		
	def clean(self):
		print("---");
		print("Cleaning Build");
		cm = "";
		if (self.platform == "xcode" or self.platform == "osx"):
			cm = "rm -r -d " + self.game_dir + self.ds + self.build_folder + self.ds + self.output;
		elif(self.platform == "windows"):
			cm = "rmdir /S /Q " + self.game_dir + self.ds + self.build_folder + self.ds + self.output;
		
		#subprocess.call([], shell=True);
		print(cm);
		try:
			buildresp = os.system(cm);
		except:
			print("could not remove dir");
			pass;

		if (self.platform == "windows"):
			cm = "rmdir /S /Q " + self.game_resources_dir + self.ds + "ark2d";
			print(cm);
			try:
				buildresp = os.system(cm);
			except:
				print("could not remove dir");
				pass;

	def processAssets(self):

		# copy /data folder into project, basically. 

		if (self.platform == "ios"): 
			#subprocess.call(["cp -r " + config['osx']['game_resources_dir'] + " " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "data/"], shell=True); #libark2d
			
			# define variables
			game_dir = self.game_dir;
			game_resources_dir = self.game_resources_dir;
			audio_quality = 5;

			print("Creating/opening assets cache file...");
			assetsCache = game_dir + self.ds + "build" + self.ds + self.platform + self.ds + "build-cache" + self.ds + "assets.json";
			assetsJson = self.openCacheFile(assetsCache);
			fchanged = False;
			
			print("Start copying files...")
			filesToCopy = self.listFiles(game_resources_dir, False);
			print(filesToCopy);
			for file in filesToCopy:
				fromfile = game_resources_dir + self.ds + file;
				tofile = game_dir + self.ds + "build" + self.ds + self.platform + self.ds + "data" + self.ds + file;
				
				#replace spaces in paths on max osx with slash-spaces
				#fromfile = fromfile.replace(" ", "\ ");
				#tofile = tofile.replace(" ", "\ ");
				 
				if (not fromfile in assetsJson or assetsJson[fromfile]['date_modified'] < os.stat(fromfile).st_mtime):
					file_ext = self.get_str_extension(file); 
					if (file_ext == "ogg"): # resample
						print("resampling audio file from: " + fromfile + " to: " + tofile);
						subprocess.call(["oggdec "+fromfile+" --quiet --output=- | oggenc --raw --quiet --quality=" + str(audio_quality) + " --output="+tofile+" -"], shell=True);
					else: # standard copy
						print("copying file from: " + fromfile + " to: " + tofile);

						# check directory exists. if not, make it. 
						tofiledir = tofile[0:tofile.rfind("/")+1];
						if (not os.path.exists(tofiledir)): 
							os.makedirs(tofiledir);
						
						#subprocess.call(["cp -r " + fromfile + " " + tofile], shell=True);
						shutil.copy2(fromfile, tofile);
						
					assetsJson[fromfile] = {"date_modified": os.stat(fromfile).st_mtime };
					fchanged = True;
					
			if (fchanged == True):
				f = open(assetsCache, "w")
				f.write(json.dumps(assetsJson, sort_keys=True, indent=4));
				f.close();


	def generateStrings(self):
		if ("resources" in self.config and "strings" in self.config["resources"]):

			print("Generating Strings files...");

			if (sys.platform == "win32"):
				print("TODO win32");
			else:
				stringsJSON = str(json.dumps(self.config["resources"]["strings"], separators=(',',':')));#.replace("\"", "\\\"");
				compileLine = r"python " + self.ark2d_dir + r"/__preproduction/polyglot/build.py '" + stringsJSON + r"' ";

				print(compileLine);
				subprocess.call([compileLine], shell=True);
			
			


	def generateSpriteSheets(self):
		# print(self.config);
		if ("resources" in self.config and "spritesheets" in self.config["resources"]):

			#load cache file
			cacheJSON = self.openCache("spritesheets");
			cacheChanged = False;

			#generate sheets
			print("Generating SpriteSheets...");
			for spritesheet in self.config["resources"]["spritesheets"]:

				spritesheetName = spritesheet["name"];
				generateThisSheet = False;
				for imageFile in spritesheet["files"]:
					#print( imageFile + " : " + str(cacheJSON[spritesheetName][imageFile]['date_modified']) + " : " + str(os.stat(self.game_dir + self.ds + imageFile).st_mtime) );
					#print(cacheJSON[spritesheetName]);
					if (not spritesheetName in cacheJSON or not imageFile in cacheJSON[spritesheetName] or cacheJSON[spritesheetName][imageFile]['date_modified'] < os.stat(self.game_dir + self.ds + imageFile).st_mtime):
						generateThisSheet = True;
						break;

				#only generate this sheets if it has a change in one of it's files.
				if (generateThisSheet == True):
					print("Generating SpriteSheet: " + spritesheet["output"]);
					spritesheet['game_dir'] = self.game_dir;
					spritesheet['game_preproduction_dir'] = self.game_preproduction_dir;

					#Previos implementation did not work correctly on win 7 64 bit. Escape character (Image\ Packer)					
					#was ineffective. Solution is string literals (r + string) and calling arguments seperatly.
					#JSON format also changed, to remove escape characters.
					if(sys.platform == "win32"):
						spritesheetJSON = str(json.dumps(spritesheet, separators=(',',':')));
						print (spritesheetJSON);
						compileLine = self.ark2d_dir + r"\..\Tools\Image Packer\build\jar\ImagePacker.jar"
						subprocess.call(["java", "-jar", compileLine, spritesheetJSON], shell=True);
					#Old method, assumed to work on other platforms
					else:
						spritesheetJSON = str(json.dumps(spritesheet, separators=(',',':'))).replace("\"", "\\\"");
						compileLine = "java -jar " + self.ark2d_dir + "/../Tools/Image\ Packer/build/jar/ImagePacker.jar \"" + spritesheetJSON + "\"";
						print(compileLine);
						subprocess.call([compileLine], shell=True); # player 
						
					#redocache
					cacheChanged = True;
					cacheJSON[spritesheetName] = {};
					for imageFile in spritesheet["files"]:
						cacheJSON[spritesheetName][imageFile] = {"date_modified": os.stat(self.game_dir + self.ds + imageFile).st_mtime };

					# if platform is android and etc1 is set. make a pkm file.
					if (self.platform == "android" and "etc1" in spritesheet and spritesheet['etc1'] == True):
						print("Generating PKM file for Android build...");
						pkmfile = self.game_dir + self.ds + "data" + self.ds + spritesheetName + ".png";
						#pkmstr = self.android_sdkdir + self.ds + "android-sdks" + self.ds + "tools" + self.ds + "etc1tool --encodeNoHeader " + pkmfile;
						pkmstr = self.android_sdkdir  + self.ds + "tools" + self.ds + "etc1tool " + pkmfile;
						print(pkmstr);
						subprocess.call([pkmstr], shell=True); #herpherpherp 

						pkmbase = os.path.splitext(pkmfile)[0];
						os.rename(pkmbase + ".pkm", pkmbase + ".pkm_png")


				pass;

			if (cacheChanged == True):
				self.saveCache("spritesheets", cacheJSON);
			
		pass;

	def openCache(self, filename):
		cachefilename = "";		
		cachefilename += self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "build-cache" + self.ds  + filename + ".json";
		self.createCacheFile(cachefilename);
		f = open(cachefilename, "r");
		fcontents = f.read();
		f.close();
		fjson = json.loads(fcontents);
		return fjson;

	def saveCache(self, filename, data):
		f = open(self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "build-cache" + self.ds + filename + ".json", "w");
		f.write(json.dumps(data, sort_keys=True, indent=4));
		f.close();
		return;
	
	def startXcode(self): 
		print("Building for XCode");

		# open config
		f = open(self.ark2d_dir + "/config.json", "r");
		fcontents = f.read();
		f.close(); 
		config = json.loads(fcontents);
		



		gyp_executable = config['osx']['gyp_executable']; 

		#f = open(self.game_dir + "/config.json", "r");
		#fcontents = f.read();
		#f.close(); 
		#config = json.loads(fcontents);

		print('hello');

		if (self.building_library):

			print("making directories");
			flascc_folder = self.ark2d_dir + self.ds + self.build_folder + "/osx"; #config['osx']['ark2d_dir'] + "/build/xcode";
			mkdirs = [];
			mkdirs.extend(self.mkdirs);
			self.makeDirectories(mkdirs);

			#projectname ark2d
			projectname = "ark2d";

			# generate gyp file.
			print("creating gyp file");
			gypfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + projectname + ".gyp";

			gypfile = {};
			gypfile['defines'] = []; #'ARK2D_IPHONE'];
			
			gypfile['targets'] = [];

			gypfiletarget = {};
			gypfiletarget['target_name'] = "ark2d-OSX";
			gypfiletarget['type'] = "shared_library";
			gypfiletarget['include_dirs'] = [];
			gypfiletarget['sources'] = [];

			for srcfile in config['src_files']['osx']:
				gypfiletarget['sources'].extend(["../../"+srcfile]);

				#check if src file has a corresponding .h file. add that to the project...
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newfh = srcfile[0:findex] + ".h";
				newfhfull = self.game_dir + self.ds + newfh; 
				if (os.path.exists(newfhfull)): 
					gypfiletarget['sources'].extend(["../../"+newfh]);	

			for srcfile in config['src_files']['all']:
				gypfiletarget['sources'].extend(["../../"+srcfile]);

				#check if src file has a corresponding .h file. add that to the project...
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newfh = srcfile[0:findex] + ".h";
				newfhfull = self.game_dir + self.ds + newfh; 
				if (os.path.exists(newfhfull)): 
					gypfiletarget['sources'].extend(["../../"+newfh]);	

			gypfiletarget['sources!'] = [];
			gypfiletarget['dependencies'] = [];
			gypfiletarget['conditions'] = [];
			gypfiletargetcondition = {};
			gypfiletargetcondition['defines'] = ['ARK2D_MACINTOSH', 'ARK2D_DESKTOP']; #, 'CF_EXCLUDE_CSTD_HEADERS'];
			gypfiletargetcondition['sources'] = [];
			gypfiletargetcondition['sources!'] = [];
			gypfiletargetcondition['link_settings'] = {}; 
			gypfiletargetcondition['link_settings']['libraries'] = [
				
				'$(SDKROOT)/System/Library/Frameworks/IOKit.framework',
				'$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
				'$(SDKROOT)/System/Library/Frameworks/Foundation.framework',
				'$(SDKROOT)/System/Library/Frameworks/CoreFoundation.framework',
				'$(SDKROOT)/System/Library/Frameworks/CoreMIDI.framework',
				'$(SDKROOT)/System/Library/Frameworks/QuartzCore.framework',
				'$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
				'$(SDKROOT)/System/Library/Frameworks/OpenAL.framework',#,
				'$(SDKROOT)/System/Library/Frameworks/QTKit.framework',
				config['osx']['ark2d_dir'] + "/lib/osx/libangelscript.a"#,
			#	config['osx']['ark2d_dir'] + "/lib/osx/freetype/libfreetype.a",
			#	config['osx']['ark2d_dir'] + "/lib/osx/libcurl.a" 
			];
			#'../../lib/iphone/libfreetype.a
			
			if (self.debug):
				gypfiletargetcondition['defines'].extend(['ARK2D_DEBUG']);

			gypfiletargetcondition['ldflags'] = [
				"-lbz2", 
				"-lcurl",
				"-lz",
				"-L" + self.ark2d_dir + "/lib/osx",
				"-langelscript"
				#"-L/usr/lib"
			]; 

			gypfiletargetcondition['link_settings!'] = [ 
				'$(SDKROOT)/System/Library/Frameworks/IOKit.framework',
				'$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/Foundation.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/CoreFoundation.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/CoreMIDI.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/QuartzCore.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/ApplicationServices.framework'];
			gypfiletargetcondition['include_dirs'] = [
				'../../src/ARK2D/vendor/angelscript',
				'../../src/ARK2D/vendor/iphone',
				'../../src/ARK2D/vendor/spine/includes',
			];
			
			gypfiletargetcondition['xcode_settings'] = {};
			gypfiletargetcondition['xcode_settings']['ARCHS'] = "i386 x86_64"; 
			gypfiletargetcondition['xcode_settings']['MAC_OS_X_VERSION_MIN_REQUIRED'] = '1070';
			gypfiletargetcondition['xcode_settings']['SDKROOT'] = "macosx"; 
			#gypfiletargetcondition['xcode_settings']['TARGETED_DEVICE_FAMILY'] = "1,2";
			gypfiletargetcondition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] = "ARK2D_MACINTOSH ARK2D_DESKTOP ";
			gypfiletargetcondition['xcode_settings']['GCC_OPTIMIZATION_LEVEL'] = "0";
			gypfiletargetcondition['xcode_settings']['MACOSX_DEPLOYMENT_TARGET'] = "10.7"; 

			if (self.debug):
				gypfiletargetcondition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] += " ARK2D_DEBUG ";


			#cflags -DNS_BLOCK_ASSERTIONS=1

			#'xcode_settings': {
	        #  'INFOPLIST_FILE' : '../experimental/iOSSampleApp/iOSSampleApp-Info.plist',
	        #  'ARCHS' : 'armv6 armv7',
	        #  'IPHONEOS_DEPLOYMENT_TARGET' : '4.2',
	        #  'SDKROOT' : 'iphoneos',
	        #  'TARGETED_DEVICE_FAMILY' : '1,2',
	        #  'USER_HEADER_SEARCH_PATHS' : '../../gpu/include/** ../../include/**',
	        #  'CODE_SIGN_IDENTITY' : 'iPhone Developer',
	        #  'GCC_PREPROCESSOR_DEFINITIONS' : 'ARK2D_IPHONE',
	        #  'GCC_OPTIMIZATION_LEVEL' : '0',
			#},
			#xcconfigfile = self.game_dir + self.ds + "lib/iphone/ARK2D-Base.xcconfig";
			xcconfigfilesimple = projectname + ".xcconfig";
			xcconfigfile = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + xcconfigfilesimple;

			gypfiletargetcondition['xcode_config_file'] = xcconfigfilesimple;
			gypfiletargetcondition['mac_bundle_resources'] = [];

			iphonecondition = [];
			iphonecondition.extend(["OS == 'mac'"]);
			iphonecondition.extend([gypfiletargetcondition]);
			gypfiletarget['conditions'].extend([iphonecondition]);
			
			gypfile['targets'].extend([gypfiletarget]);

			print("saving gyp file: " + gypfilename);
			f = open(gypfilename, "w")
			f.write(json.dumps(gypfile, sort_keys=True, indent=4));
			f.close();

			#exit(0);
			#pchfilename = self.game_dir + self.ds + "lib/iphone/" + projectname + "-Prefix.pch";
			pchfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + projectname + "-Prefix.pch";

			#delete xcode project? 
			try:
				print("deleting xcode project");
				os.system("rm -r -d " + pchfilename);
				os.system("rm -r -d " + xcconfigfile);
				os.system("rm -r -d " + self.build_folder + self.ds + self.output + self.ds + "DerivedData");
			except:
				print("could not delete xcode project");

			# run gyp file.
			os.system("python " + gyp_executable + "_main.py " + gypfilename + " --depth=../../src");
			
			# add precompiled headers file
			print("generating pch file");
			
			nl = "\r\n";
			pcheaderfile = "";
			#pcheaderfile += "#ifdef __OBJC__" + nl;
			pcheaderfile += "	#import <Foundation/Foundation.h>" + nl;
			pcheaderfile += "	#import <CoreFoundation/CoreFoundation.h>" + nl + nl;
			
			#pcheaderfile += "	#import <UIKit/UIKit.h>" + nl + nl;

			pcheaderfile += "	#import <OpenAL/al.h>" + nl;
			pcheaderfile += "	#import <OpenAL/alc.h>" + nl + nl
 
			#pcheaderfile += "	#import <QuartzCore/QuartzCore.h>" + nl + nl;

			#pcheaderfile += "	#import <OpenGL/OpenGL.h>" + nl;
			pcheaderfile += "	#import <OpenGL/gl3.h>" + nl;
			#pcheaderfile += "	#import <OpenGL/gltypes.h>" + nl;
			#pcheaderfile += "	#import <OpenGL/glu.h>" + nl;
			pcheaderfile += "	#import <OpenGL/gl3ext.h>" + nl + nl; 

			#pcheaderfile += "	#import <CoreFoundation/CFBundle.h>" + nl;
			#pcheaderfile += "	#import <CoreFoundation/CoreFoundation.h>" + nl;

			#pcheaderfile += "#endif";

			print("saving pch file: " + pchfilename);
			f = open(pchfilename, "w")
			f.write(pcheaderfile);
			f.close();

			#print("set the pch manually in xcode, under Apple LLVM compiler 4.1 - Language...");
			#print("set YES and ../../lib/iphone/" + projectname + "-Prefx.pch");

			#create xcconfig file
			print("generating xcconfig file:");
			xcconfigfilecontents = "";
			xcconfigfilecontents += "GCC_PRECOMPILE_PREFIX_HEADER = YES;" + nl;
			xcconfigfilecontents += "GCC_PREFIX_HEADER = " + pchfilename + ";" + nl;
			xcconfigfilecontents += "SRCROOT = " + self.game_dir + self.ds + "src" + self.ds + "ARK2D" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_FLAT_ENTRIES_FOR_TARGET_BEING_BUILT = NO;" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_PROJECT_HEADERS = NO;" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_FRAMEWORK_ENTRIES_FOR_ALL_PRODUCT_TYPES = NO;" + nl;
			xcconfigfilecontents += "ALWAYS_SEARCH_USER_PATHS = NO;" + nl;
			xcconfigfilecontents += "OTHER_CFLAGS = -x objective-c;" + nl;  
			xcconfigfilecontents += "OTHER_CPLUSPLUSFLAGS = -x objective-c++;" + nl;
			#xcconfigfilecontents += "OTHER_LDFLAGS = -L/usr/lib -L" + self.ark2d_dir + "/lib/osx -L" + self.ark2d_dir + "/lib/osx/freetype -lbz2 -lcurl -lz -lfreetype -install_name @executable_path/../Resources/data/ark2d/libark2d.dylib " + nl;
			xcconfigfilecontents += "OTHER_LDFLAGS = -L" + self.ark2d_dir + "/lib/osx -L" + self.ark2d_dir + "/lib/osx/system -L" + self.ark2d_dir + "/lib/osx/freetype -lbz2 -lcurl -lz -lfreetype -install_name @executable_path/../Resources/data/ark2d/libark2d.dylib " + nl;
 
			 

			
			print("saving xcconfig file: " + xcconfigfile);
			f = open(xcconfigfile, "w")
			f.write(xcconfigfilecontents);
			f.close();

			print("done. now compile with the xcode project.");
 
			exit(0);

		else:




			mkdirs = [];
			mkdirs.extend(self.mkdirs);
			#mkdirs.extend([self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "data" + self.ds + "ark2d"]);
			#mkdirs.extend()
			self.makeDirectories(mkdirs);

			#projectname ark2d
			projectname = self.game_name_safe; #config['game_short_name'];
			projectnameunsafe = self.game_name; #config['game_name'];

			# generate gyp file.
			print("creating gyp file");
			gypfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + projectname + ".gyp";

			gypfile = {};
			gypfile['defines'] = []; #'ARK2D_IPHONE'];
			gypfile['defines'].extend(["ARK_INCLUDES_HEADER="+self.ark2d_dir + "/src/ARK.h"]);
			gypfile['defines'].extend(self.preprocessor_definitions);
			
			gypfile['targets'] = [];

			gypfiletarget = {};
			gypfiletarget['target_name'] = projectnameunsafe;# + "-OSX"; # we know it's OSX. HAH. 
			gypfiletarget['type'] = "executable";
			gypfiletarget['mac_bundle'] = 1;
			#'mac_bundle': 1,
			gypfiletarget['include_dirs'] = [];
			gypfiletarget['include_dirs'].extend(self.include_dirs);
			gypfiletarget['sources'] = [];

			for srcfile in self.src_files: #config['game_src_files']:
				gypfiletarget['sources'].extend(["../../"+srcfile]);

				#check if src file has a corresponding .h file. add that to the project...
				findex = srcfile.rfind('.');
				h_ext = srcfile[findex+1:len(srcfile)];
				newfh = srcfile[0:findex] + ".h";
				newfhfull = self.game_dir + self.ds + newfh; 
				if (os.path.exists(newfhfull)): 
					gypfiletarget['sources'].extend(["../../"+newfh]);	 	
			
			

			gypfiletarget['sources!'] = [];
			gypfiletarget['dependencies'] = [];
			gypfiletarget['conditions'] = [];
			gypfiletargetcondition = {};
			gypfiletargetcondition['defines'] = ['ARK2D_MACINTOSH', 'ARK2D_DESKTOP']; #, 'CF_EXCLUDE_CSTD_HEADERS'];
			gypfiletargetcondition['defines'].extend(self.preprocessor_definitions);
			gypfiletargetcondition['sources'] = [];
			gypfiletargetcondition['sources!'] = []; 
			gypfiletargetcondition['link_settings'] = {};
			gypfiletargetcondition['link_settings']['libraries'] = [
				'$(SDKROOT)/System/Library/Frameworks/IOKit.framework',
				'$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
				'$(SDKROOT)/System/Library/Frameworks/CoreFoundation.framework',
				'$(SDKROOT)/System/Library/Frameworks/QuartzCore.framework',
				'$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
				'$(SDKROOT)/System/Library/Frameworks/OpenAL.framework',
				'$(SDKROOT)/System/Library/Frameworks/QTKit.framework',
				
	          	#'../../lib/iphone/libfreetype.a'
	          	#config['osx']['ark2d_dir'] + '/lib/osx/freetype/libfreetype.a',
	          	#config['osx']['ark2d_dir'] + '/lib/osx/libcurl.a',
	          	#config['osx']['ark2d_dir'] + '/build/xcode/XcodeData/ark2d/Build/Products/Default/libark2d-OSX.dylib'
	          	#self.ark2d_dir + '/lib/osx/libangelscript.a',
	          	self.game_dir + '/build/' + self.output + '/data/ark2d/libark2d.dylib'
			]; 
			gypfiletargetcondition['link_settings']['libraries'] = self.addLibrariesToArray(gypfiletargetcondition['link_settings']['libraries'], self.libs);
			

			if (self.debug):
				gypfiletargetcondition['defines'].extend(['ARK2D_DEBUG']);
 
			gypfiletargetcondition['ldflags'] = [ ];

			gypfiletargetcondition['link_settings!'] = [];

			ark2ddir = self.ark2d_dir + "/src/ARK2D"; #config['osx']['ark2d_dir'] + "/src/ARK2D";
			gypfiletargetcondition['include_dirs'] = [ 
				self.ark2d_dir + "/src/",
				self.ark2d_dir + "/src/ARK2D",
				self.ark2d_dir + "/src/ARK2D/vendor/iphone",
				self.ark2d_dir + "/src/ARK2D/vendor/spine/includes"
			];

			# custom include dirs
			#if "include_dirs" in config['osx']: 
			#	for includedir in config['osx']['include_dirs']:
			#		includedir_actual = self.str_replace(includedir, [("%PREPRODUCTION_DIR%", config['osx']['game_preproduction_dir']), ("%ARK2D_DIR%", config['osx']['ark2d_dir'])]);
			#		gypfiletargetcondition['include_dirs'].extend([includedir_actual]);
			gypfiletargetcondition['include_dirs'].extend(self.include_dirs);
			
			# we can set any of these! 
			# https://developer.apple.com/library/mac/documentation/DeveloperTools/Reference/XcodeBuildSettingRef/1-Build_Setting_Reference/build_setting_ref.html 
			gypfiletargetcondition['xcode_settings'] = {};
			gypfiletargetcondition['xcode_settings']['ARCHS'] = "$(ARCHS_STANDARD)"; #"i386 x86_64"; # or  $(ARCHS_STANDARD_32_64_BIT)
			gypfiletargetcondition['xcode_settings']['SDKROOT'] = "macosx";
			gypfiletargetcondition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] = "ARK2D_MACINTOSH ARK2D_DESKTOP";
			gypfiletargetcondition['xcode_settings']['GCC_OPTIMIZATION_LEVEL'] = "0";
			gypfiletargetcondition['xcode_settings']['MACOSX_DEPLOYMENT_TARGET'] = "10.7";
			gypfiletargetcondition['xcode_settings']['LD_RUNPATH_SEARCH_PATHS'] = "@executable_path/";

			if (self.debug):
				gypfiletargetcondition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] += " ARK2D_DEBUG ";
			
			xcconfigfilesimple = projectname + ".xcconfig";
			xcconfigfile = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + xcconfigfilesimple;

			gypfiletargetcondition['xcode_config_file'] = xcconfigfilesimple;
			gypfiletargetcondition['mac_bundle_resources'] = [
				"data/"#,				#ark2d and game data 
				
				# "Icon.png",				#iphone
				# "Icon@2x.png",			#iphone-retina
				# "Icon-Small.png",			#iphone-spotlight
				# "Icon-Small@2x.png",		#iphone-spotlight-retina

				# "Default.png" 			#iphone-launch-image (320x480)
				# "Default@2x.png"			#iphone-launch-image-retina (640x960)
				# "Default-568h@2x.png" 	#iphone5-launch-image-retina (640x1136)

				# "Icon-72.png",			#ipad
				# "Icon-72@2x.png",			#ipad-retina
				# "Icon-Small-50.png",		#ipad-spotlight
				# "Icon-Small-50@2x.png",	#ipad-spotlight-retina

				# "Default-Portrait.png" 		#ipad-launch-image-portrait (768x1024)
				# "Default-Landscape.png" 		#ipad-launch-image-landscape (1024x768)
				# "Default-Portrait@2x.png" 	#ipad-launch-image-portrait (1536x2048)
				# "Default-Landscape@2x.png" 	#ipad-launch-image-landscape (2048x1536)

				# "iTunesArtwork", 		#app-store-icon
				# "iTunesArtwork@2x", 	#app-store-icon-retina
			];

			iphonecondition = [];
			iphonecondition.extend(["OS == 'mac'"]);
			iphonecondition.extend([gypfiletargetcondition]);
			gypfiletarget['conditions'].extend([iphonecondition]);
			
			gypfile['targets'].extend([gypfiletarget]);

			print("saving gyp file: " + gypfilename);
			f = open(gypfilename, "w")
			f.write(json.dumps(gypfile, sort_keys=True, indent=4));
			f.close();



			#delete xcode project? 
			pchfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + projectname + "-Prefix.pch";
			info_plist_filename = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + projectname + "-Info.plist";
			try:
				print("deleting xcode project"); 
				os.system("rm -r -d " + pchfilename);
				os.system("rm -r -d " + info_plist_filename);
				os.system("rm -r -d " + xcconfigfile);
				os.system("rm -r -d " + self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "DerivedData");
			except:
				print("could not delete xcode project");

			# run gyp file.
			os.system("python " + gyp_executable + "_main.py " + gypfilename + " --depth=../../src");
 
			# hack into xcode project to add steam / other libs that need to go next to the executable.
			#if "libs" in config['osx']:
			if (len(self.libs) > 0):

				f = open(self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + projectname + ".xcodeproj" + self.ds + "project.pbxproj", "r");
				pbxproj = f.read();
				f.close();

				copyFilesId = "FFFFFFF3198988B100BDD086";

				PBXBuildFileExtraContents = "";
				libfilecount = 0;
				for libfile in self.libs: #config['osx']['libs']:
					libfilename = self.get_str_filename(libfile);
				
					PBXBuildFileExtraContents += "		FFFFFFFFFFFF00000000000" + str(libfilecount) + " /* " + libfilename + " in CopyFiles */ = {isa = PBXBuildFile; fileRef = FFFFFFFEFFFF00000000000" + str(libfilecount) + " /* " + libfilename + " */; };\n";
					libfilecount += 1;

				pbjxprojCopyLibs_contents = "";
				pbjxprojCopyLibs_contents += PBXBuildFileExtraContents;
				pbjxprojCopyLibs_contents += "/* End PBXBuildFile section */\n";
				pbjxprojCopyLibs_contents += "/* Begin PBXCopyFilesBuildPhase section */\n";
				pbjxprojCopyLibs_contents += copyFilesId + " /* CopyFiles */ = {\n";
				pbjxprojCopyLibs_contents += "	isa = PBXCopyFilesBuildPhase;\n";
				pbjxprojCopyLibs_contents += "	buildActionMask = 2147483647;\n";
				pbjxprojCopyLibs_contents += "	dstPath = \"\";\n";
				pbjxprojCopyLibs_contents += "	dstSubfolderSpec = 6;\n";
				pbjxprojCopyLibs_contents += "	files = (\n";

				libfilecount = 0; 
				for libfile in self.libs: #config['osx']['libs']:
					libfilename = self.get_str_filename(libfile);
					pbjxprojCopyLibs_contents += "		FFFFFFFFFFFF00000000000" + str(libfilecount) + " /* " + libfilename + " in CopyFiles */,\n";
					libfilecount += 1;
				
				pbjxprojCopyLibs_contents += "	);\n";
				pbjxprojCopyLibs_contents += "	runOnlyForDeploymentPostprocessing = 0;\n";
				pbjxprojCopyLibs_contents += "};\n";
				pbjxprojCopyLibs_contents += "/* End PBXCopyFilesBuildPhase section */\n\n";

				pbxproj = self.str_replace(pbxproj, [("/* End PBXBuildFile section */", pbjxprojCopyLibs_contents)]);
				pbxproj = self.str_replace(pbxproj, [("buildPhases = (", "buildPhases = (\n 	" + copyFilesId + " /* CopyFiles */,")]);
				
				things2 = "";
				libfilecount = 0;
				addplace3index = pbxproj.find("/* Source */,") - 1 - 24;
				for libfile in self.libs: #config['osx']['libs']:
					libfilename = self.get_str_filename(libfile);
					things2 += "FFFFFFFEFFFF00000000000" + str(libfilecount) + " /* " + libfilename + " */,\n";
					libfilecount += 1;

				newpbxproj = pbxproj[0:addplace3index] + things2 + "				" + pbxproj[addplace3index:];

				# add file references
				libfilecount = 0;
				PBXFileReferenceExtraContents = "";
				for libfile in self.libs: #config['osx']['libs']:
					libfilename = self.get_str_filename(libfile);
					libfile_actual = libfile; 
					libfile_actual_relative = os.path.relpath(libfile_actual, self.game_dir + self.ds + self.build_folder + self.ds + projectname + "xcodeproj" + self.ds);
					PBXFileReferenceExtraContents += "FFFFFFFEFFFF00000000000" + str(libfilecount) + " /* " + libfilename + " */ = {isa = PBXFileReference; lastKnownFileType = \"compiled.mach-o.dylib\"; name = " + libfilename + "; path = " + libfile_actual_relative + "; sourceTree = \"<group>\"; };";

				newpbxproj = self.str_replace(newpbxproj, [("/* End PBXFileReference section */", PBXFileReferenceExtraContents)]);
				
				f = open(self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + projectname + ".xcodeproj" + self.ds + "project.pbxproj", "w")
				f.write(newpbxproj);
				f.close();
 
			
			# add precompiled headers file
			print("generating pch file");
			
			nl = "\r\n";
			pcheaderfile = "";
			#pcheaderfile += "#ifdef __OBJC__" + nl;
			# pcheaderfile += "	#import <Foundation/Foundation.h>" + nl;
			# pcheaderfile += "	#import <UIKit/UIKit.h>" + nl + nl;


			# pcheaderfile += "	#import <OpenAL/al.h>" + nl;
			# pcheaderfile += "	#import <OpenAL/alc.h>" + nl + nl

			# pcheaderfile += "	#import <OpenGLES/EAGL.h>" + nl;
			# pcheaderfile += "	#import <QuartzCore/QuartzCore.h>" + nl + nl;

			# pcheaderfile += "	#import <OpenGLES/ES1/gl.h>" + nl;
			# pcheaderfile += "	#import <OpenGLES/ES1/glext.h>" + nl;
			# pcheaderfile += "	#import <OpenGLES/ES2/gl.h>" + nl;
			# pcheaderfile += "	#import <OpenGLES/ES2/glext.h>" + nl;
			# pcheaderfile += "	#import <OpenGLES/EAGLDrawable.h>" + nl + nl;

			# pcheaderfile += "	#import <CoreFoundation/CFBundle.h>" + nl;
			# pcheaderfile += "	#import <CoreFoundation/CoreFoundation.h>" + nl;

			pcheaderfile += "	#import <Foundation/Foundation.h>" + nl;

			pcheaderfile += "	#import <OpenAL/al.h>" + nl;
			pcheaderfile += "	#import <OpenAL/alc.h>" + nl + nl
 
			#pcheaderfile += "	#import <QuartzCore/QuartzCore.h>" + nl + nl;

			#pcheaderfile += "	#import <OpenGL/OpenGL.h>" + nl;
			pcheaderfile += "	#import <OpenGL/gl3.h>" + nl;
			#pcheaderfile += "	#import <OpenGL/gltypes.h>" + nl;
			#pcheaderfile += "	#import <OpenGL/glu.h>" + nl;
			pcheaderfile += "	#import <OpenGL/gl3ext.h>" + nl + nl;  

			#pcheaderfile += "	#import <CoreFoundation/CFBundle.h>" + nl;
			pcheaderfile += "	#import <CoreFoundation/CoreFoundation.h>" + nl;

			#pcheaderfile += "#endif";

			print("saving pch file: " + pchfilename);
			f = open(pchfilename, "w")
			f.write(pcheaderfile);
			f.close();

			print ("generating project-Info.plist file");
			info_plist_contents = "";
			info_plist_contents += '<?xml version="1.0" encoding="UTF-8"?>' + nl;
			info_plist_contents += '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' + nl;
			info_plist_contents += '<plist version="1.0">' + nl;
			info_plist_contents += '<dict>' + nl;
			info_plist_contents += '	<key>CFBundleDevelopmentRegion</key>' + nl;
			info_plist_contents += '	<string>en</string>' + nl;
			info_plist_contents += '	<key>CFBundleDisplayName</key>' + nl;
			info_plist_contents += '	<string>' + self.game_name + '</string>' + nl;
			info_plist_contents += "	<key>CFBundleGetInfoString</key>" + nl;
			info_plist_contents += "	<string>" + self.game_name + ". Copyright " + self.developer_name + "</string>" + nl;
			info_plist_contents += '	<key>CFBundleExecutable</key>' + nl;
			info_plist_contents += '	<string>${EXECUTABLE_NAME}</string>' + nl;

			# info_plist_contents += '	<key>CFBundleIconFiles</key>' + nl;
			# info_plist_contents += '	<array>' + nl;
			# info_plist_contents += '		<string></string>' + nl;
			# info_plist_contents += '		<string>Icon.png</string>' + nl;					#iphone
			# info_plist_contents += '		<string>Icon@2x.png</string>' + nl;					#iphone-retina
			# info_plist_contents += '		<string>Icon-Small.png</string>' + nl;				#iphone-spotlight
			# info_plist_contents += '		<string>Icon-Small@2x.png</string>' + nl;			#iphone-spotlight-retina
			# info_plist_contents += '		<string>Icon-72.png</string>' + nl;					#ipad
			# info_plist_contents += '		<string>Icon-72@2x.png</string>' + nl;				#ipad-retina
			# info_plist_contents += '		<string>Icon-Small-50.png</string>' + nl;			#ipad-spotlight
			# info_plist_contents += '		<string>Icon-Small-50@2x.png</string>' + nl;		#ipad-spotlight-retina
			# info_plist_contents += '	</array>' + nl;

			# info_plist_contents += '	<key>CFBundleIcons</key>' + nl;
			# info_plist_contents += '	<dict>' + nl;
			# info_plist_contents += '		<key>CFBundlePrimaryIcon</key>' + nl;
			# info_plist_contents += '		<dict>' + nl;
			# info_plist_contents += '			<key>CFBundleIconFiles</key>' + nl;
			# info_plist_contents += '			<array>' + nl;
			# info_plist_contents += '				<string></string>' + nl;
			# info_plist_contents += '				<string>Icon.png</string>' + nl;				#iphone
			# info_plist_contents += '				<string>Icon@2x.png</string>' + nl;				#iphone-retina
			# info_plist_contents += '				<string>Icon-Small.png</string>' + nl;			#iphone-spotlight
			# info_plist_contents += '				<string>Icon-Small@2x.png</string>' + nl;		#iphone-spotlight-retina
			# info_plist_contents += '				<string>Icon-72.png</string>' + nl;				#ipad
			# info_plist_contents += '				<string>Icon-72@2x.png</string>' + nl;			#ipad-retina
			# info_plist_contents += '				<string>Icon-Small-50.png</string>' + nl;		#ipad-spotlight
			# info_plist_contents += '				<string>Icon-Small-50@2x.png</string>' + nl;	#ipad-spotlight-retina
			# info_plist_contents += '			</array>' + nl;
			# info_plist_contents += '		</dict>' + nl;
			# info_plist_contents += '	</dict>' + nl;
 
			info_plist_contents += '	<key>CFBundleIconFile</key>' + nl;
			info_plist_contents += '	<string>data/icon.icns</string>' + nl; 
			info_plist_contents += '	<key>CFBundleIdentifier</key>' + nl;
			info_plist_contents += '	<string>org.' + self.developer_name_safe + '.' + projectname + '</string>' + nl;
			info_plist_contents += '	<key>CFBundleInfoDictionaryVersion</key>' + nl;
			info_plist_contents += '	<string>6.0</string>' + nl;
			info_plist_contents += '	<key>CFBundleName</key>' + nl;
			info_plist_contents += '	<string>${PRODUCT_NAME}</string>' + nl;
			info_plist_contents += '	<key>CFBundlePackageType</key>' + nl;
			info_plist_contents += '	<string>APPL</string>' + nl;
			info_plist_contents += '	<key>CFBundleShortVersionString</key>' + nl;
			info_plist_contents += '	<string>1.0</string>' + nl;
			info_plist_contents += '	<key>CFBundleSignature</key>' + nl;
			info_plist_contents += '	<string>????</string>' + nl;
			info_plist_contents += '	<key>CFBundleVersion</key>' + nl;
			info_plist_contents += '	<string>1.0</string>' + nl;

			
			info_plist_contents += '</dict>' + nl;
			info_plist_contents += '</plist>';

			print("saving project-Info.plist file");
			f = open(info_plist_filename, "w")
			f.write(info_plist_contents);
			f.close();


			#print("set the pch manually in xcode, under Apple LLVM compiler 4.1 - Language...");
			#print("set YES and ../../lib/iphone/" + projectname + "-Prefx.pch");

			#create xcconfig file
			print("generating xcconfig file:");
			xcconfigfilecontents = "";
			xcconfigfilecontents += "GCC_PRECOMPILE_PREFIX_HEADER = YES;" + nl;
			xcconfigfilecontents += "GCC_PREFIX_HEADER = " + pchfilename + ";" + nl;
			xcconfigfilecontents += "SRCROOT = " + self.game_dir + self.ds + "src" + self.ds + "ARK2D" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_FLAT_ENTRIES_FOR_TARGET_BEING_BUILT = NO;" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_PROJECT_HEADERS = NO;" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_FRAMEWORK_ENTRIES_FOR_ALL_PRODUCT_TYPES = NO;" + nl;
			xcconfigfilecontents += "ALWAYS_SEARCH_USER_PATHS = NO;" + nl;
			xcconfigfilecontents += "OTHER_CFLAGS = -x objective-c;" + nl;
			xcconfigfilecontents += "OTHER_CPLUSPLUSFLAGS = -x objective-c++;" + nl;
			#xcconfigfilecontents += "OTHER_LDFLAGS = -lbz2 -lcurl -lz;" + nl;
			xcconfigfilecontents += "INFOPLIST_FILE = " + info_plist_filename;
			# -L" + self.ark2d_dir + "/lib/osx -L" + self.ark2d_dir + "/lib/osx/system -L" + self.ark2d_dir + "/lib/osx/freetype
				
			
			print("saving xcconfig file: " + xcconfigfile);
			f = open(xcconfigfile, "w")
			f.write(xcconfigfilecontents);
			f.close();
 
			print("generating/updating spritesheets");
			self.generateSpriteSheets();

			print("generating/updating strings");
			self.generateStrings();

			#copy ark2d resources in to project data folder.
			print("Copy game resources in to xcode dir");
			game_resources_copy = "cp -r " + self.game_resources_dir + "/ " + self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "data/";
			print(game_resources_copy);
			subprocess.call([game_resources_copy], shell=True); #libark2d
			
			print("Copy ark2d resources in to xcode dir");
			subprocess.call(["cp -r " + self.ark2d_dir + "/data " + self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "data/ark2d/"], shell=True); #libark2d
			
			print("Copy ark2d dylib in to xcode dir");
			subprocess.call(["cp " + self.ark2d_dir + "/build/osx/DerivedData/ark2d/Build/Products/Default/libark2d-OSX.dylib " + self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "data/ark2d/libark2d.dylib"], shell=True); #libark2d
			 
			#generate icons 
			"""
			if "iphone" in config: 
				if "master_icon" in config['iphone']:
					if (config['iphone']['use_master_icon'] == True): 
						iconinterpolation = config['iphone']['master_icon_interpolation'];
						icongenarr = [];
						icongenobj = {};
						icongenobj['from'] = config['iphone']['master_icon'];
						icongenobj['from'] = self.str_replace(icongenobj['from'], [("%PREPRODUCTION_DIR%", config['osx']['game_preproduction_dir'])]);
						icongenobj['to'] = [
							# iPhone Icon
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "Icon.png",
								"width" : 57,
								"height": 57, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "Icon@2x.png",
								"width" : 114,
								"height": 114, 
								"interpolation": iconinterpolation
							},
							# iPhone Spotlight Icon
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "Icon-Small.png",
								"width" : 29,
								"height": 29, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "Icon-Small@2x.png",
								"width" : 58,
								"height": 58, 
								"interpolation": iconinterpolation
							},
							# iPad Icon
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "Icon-72.png",
								"width" : 72,
								"height": 72, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "Icon-72@2x.png",
								"width" : 144,
								"height": 144, 
								"interpolation": iconinterpolation
							},
							# iPad Spotlight Icon
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "Icon-Small-50.png",
								"width" : 50,
								"height": 50, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "Icon-Small-50@2x.png",
								"width" : 100,
								"height": 100, 
								"interpolation": iconinterpolation
							},
							# app store icon
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "iTunesArtwork",
								"width" : 512,
								"height": 512, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "iTunesArtwork@2x",
								"width" : 1024,
								"height": 1024, 
								"interpolation": iconinterpolation
							}
							
							
						];
						icongenarr.extend([icongenobj]);
						#icongenstr = json.dumps(icongenarr, sort_keys=True, indent=0, new);

						
						icongenstr = str(json.dumps(icongenarr, separators=(',',':'))).replace("\"", "\\\"");
						icongenLINE = "java -jar " + self.ark2d_dir + "/../Tools/ARK2D\ Image\ Resizer/build/jar/Resizer.jar \"" + icongenstr + "\"";
						print(icongenLINE);
						subprocess.call([icongenLINE], shell=True); 


						#os.system();
						pass;
					pass;
				pass;
			"""

			
			pass;


	def addLibrariesToArray(self, arr, lbs, extension=""): #, extension2=""):
		
		if (extension == ""): 
			if (self.platform == "windows"):
				extension = "dll";
			elif (self.platform == "osx"):
				extension = "dylib";
			elif (self.platform == "linux"):
				extension = "so";

		for lib in lbs:
			file_ext = self.get_str_extension(lib);
			if file_ext == extension: #"dll":
				arr.extend([lib]); 
			

		return arr;


	def startIOS(self):
		print("Building iOS");

		# open ARK2d config
		f = open(self.ark2d_dir + "/config.json", "r");
		fcontents = f.read();
		f.close();
		ark2dconfig = json.loads(fcontents);

		self.makeDirectories([self.game_dir + self.ds + self.build_folder + self.ds + self.output]);

		gyp_executable = ark2dconfig['osx']['gyp_executable']; #"/Users/ashleygwinnell/Documents/gyp-read-only/gyp";

		if (self.building_library):

			#projectname ark2d
			projectname = "ark2d";

			# generate gyp file.
			print("creating gyp file");
			gypfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + projectname + ".gyp";

			gypfile = {};
			gypfile['defines'] = []; #'ARK2D_IPHONE'];
			
			gypfile['targets'] = [];

			gypfiletarget = {};
			gypfiletarget['target_name'] = "ark2d-iPhone";
			gypfiletarget['type'] = "static_library";
			gypfiletarget['include_dirs'] = [];
			gypfiletarget['sources'] = [];

			for srcfile in ark2dconfig['src_files']['ios']:
				gypfiletarget['sources'].extend(["../../"+srcfile]);

			for srcfile in ark2dconfig['src_files']['all']:
				gypfiletarget['sources'].extend(["../../"+srcfile]);

			gypfiletarget['sources!'] = [];
			gypfiletarget['dependencies'] = [];
			gypfiletarget['conditions'] = [];
			gypfiletargetcondition = {};
			gypfiletargetcondition['defines'] = ['ARK2D_IPHONE', "ARK2D_IOS"]; #, 'CF_EXCLUDE_CSTD_HEADERS'];

			if self.debug:
				gypfiletargetcondition['defines'].extend(["ARK2D_DEBUG"]);
			

			gypfiletargetcondition['sources'] = [];
			gypfiletargetcondition['sources!'] = [];
			gypfiletargetcondition['link_settings'] = {};
			gypfiletargetcondition['link_settings']['libraries'] = [
				#'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/CoreFoundation.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/CoreGraphics.framework',
	          	#'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/CoreText.framework',
	          	#'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/UIKit.framework',
	          	"/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS6.0.sdk/System/Library/Frameworks/UIKit.framework",
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/Foundation.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/QuartzCore.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/OpenGLES.framework'
					              ];
					              #'../../lib/iphone/libfreetype.a
			

			gypfiletargetcondition['ldflags'] = [
				"-lbz2",
				"-lcurl",
				"-lz",
				"-L" + self.ark2d_dir + "/lib/iphone",
				"-L" + self.ark2d_dir + "/lib/osx",
				"-L/usr/lib"
			];

			gypfiletargetcondition['link_settings!'] = [
				'$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/Foundation.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/QuartzCore.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/OpenGL.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/ApplicationServices.framework'
	        	'$(SDKROOT)/System/Library/Frameworks/AVFoundation.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/CoreAudio.framework',
	        	'$(SDKROOT)/System/Library/Frameworks/AudioToolbox.framework'
	        ];
	        
			ark2ddir = self.ark2d_dir + "/src/ARK2D";
			gypfiletargetcondition['include_dirs'] = [
				ark2ddir + '/vendor/iphone',
				ark2ddir + '/vendor/spine/includes'
			];
			
			gypfiletargetcondition['xcode_settings'] = {};
			gypfiletargetcondition['xcode_settings']['ARCHS'] = "armv6 armv7 arm64";
			gypfiletargetcondition['xcode_settings']['IPHONEOS_DEPLOYMENT_TARGET'] = '6.0';
			gypfiletargetcondition['xcode_settings']['SDKROOT'] = "iphoneos";
			gypfiletargetcondition['xcode_settings']['TARGETED_DEVICE_FAMILY'] = "1,2";
			gypfiletargetcondition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] = "ARK2D_IPHONE;ARK2D_IOS";
			gypfiletargetcondition['xcode_settings']['GCC_OPTIMIZATION_LEVEL'] = "0";

			if self.debug:
				gypfiletargetcondition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] += " ARK2D_DEBUG";

			#cflags -DNS_BLOCK_ASSERTIONS=1

			#'xcode_settings': {
	        #  'INFOPLIST_FILE' : '../experimental/iOSSampleApp/iOSSampleApp-Info.plist',
	        #  'ARCHS' : 'armv6 armv7',
	        #  'IPHONEOS_DEPLOYMENT_TARGET' : '6.0',
	        #  'SDKROOT' : 'iphoneos',
	        #  'TARGETED_DEVICE_FAMILY' : '1,2',
	        #  'USER_HEADER_SEARCH_PATHS' : '../../gpu/include/** ../../include/**',
	        #  'CODE_SIGN_IDENTITY' : 'iPhone Developer',
	        #  'GCC_PREPROCESSOR_DEFINITIONS' : 'ARK2D_IPHONE',
	        #  'GCC_OPTIMIZATION_LEVEL' : '0',
			#},
			#xcconfigfile = self.game_dir + self.ds + "lib/iphone/ARK2D-Base.xcconfig";
			xcconfigfilesimple = projectname + ".xcconfig";
			xcconfigfile = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + xcconfigfilesimple;

			gypfiletargetcondition['xcode_config_file'] = xcconfigfilesimple;
			gypfiletargetcondition['mac_bundle_resources'] = [];

			iphonecondition = [];
			iphonecondition.extend(["OS == 'mac'"]);
			iphonecondition.extend([gypfiletargetcondition]);
			gypfiletarget['conditions'].extend([iphonecondition]);
			
			gypfile['targets'].extend([gypfiletarget]);

			print("saving gyp file: " + gypfilename);
			f = open(gypfilename, "w")
			f.write(json.dumps(gypfile, sort_keys=True, indent=4));
			f.close();

			#exit(0);
			#pchfilename = self.game_dir + self.ds + "lib/iphone/" + projectname + "-Prefix.pch";
			pchfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + projectname + "-Prefix.pch";

			#delete xcode project? 
			try:
				print("deleting xcode project");
				os.system("rm -r -d " + pchfilename);
				os.system("rm -r -d " + xcconfigfile);
				#os.system("rm -r -d " + self.build_folder + self.ds + self.platform + self.ds + "XcodeData");
				os.system("rm -r -d " + self.build_folder + self.ds + self.platform + self.ds + "DerivedData");
			except:
				print("could not delete xcode project");

			# run gyp file.
			#os.system("python " + gyp_executable + " " + gypfilename + " --depth=../../src");
			os.system("python " + gyp_executable + "_main.py " + gypfilename + " --depth=../../src");
			
			# add precompiled headers file
			print("generating pch file");
			
			nl = "\r\n";
			pcheaderfile = "";
			#pcheaderfile += "#ifdef __OBJC__" + nl;
			pcheaderfile += "	#import <Foundation/Foundation.h>" + nl;
			pcheaderfile += "	#import <AVFoundation/AVFoundation.h>" + nl;
			pcheaderfile += "	#import <UIKit/UIKit.h>" + nl + nl;


			pcheaderfile += "	#import <OpenAL/al.h>" + nl;
			pcheaderfile += "	#import <OpenAL/alc.h>" + nl + nl

			pcheaderfile += "	#import <OpenGLES/EAGL.h>" + nl;
			pcheaderfile += "	#import <QuartzCore/QuartzCore.h>" + nl + nl;

			pcheaderfile += "	#import <OpenGLES/ES1/gl.h>" + nl;
			pcheaderfile += "	#import <OpenGLES/ES1/glext.h>" + nl;
			pcheaderfile += "	#import <OpenGLES/ES2/gl.h>" + nl;
			pcheaderfile += "	#import <OpenGLES/ES2/glext.h>" + nl;
			pcheaderfile += "	#import <OpenGLES/EAGLDrawable.h>" + nl + nl;

			pcheaderfile += "	#import <CoreFoundation/CFBundle.h>" + nl;
			pcheaderfile += "	#import <CoreFoundation/CoreFoundation.h>" + nl;

			#pcheaderfile += "#endif";

			print("saving pch file: " + pchfilename);
			f = open(pchfilename, "w")
			f.write(pcheaderfile);
			f.close();

			#print("set the pch manually in xcode, under Apple LLVM compiler 4.1 - Language...");
			#print("set YES and ../../lib/iphone/" + projectname + "-Prefx.pch");

			#create xcconfig file
			print("generating xcconfig file:");
			xcconfigfilecontents = "";
			xcconfigfilecontents += "GCC_PRECOMPILE_PREFIX_HEADER = YES;" + nl;
			xcconfigfilecontents += "GCC_PREFIX_HEADER = " + pchfilename + ";" + nl;
			xcconfigfilecontents += "SRCROOT = " + self.game_dir + self.ds + "src" + self.ds + "ARK2D" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_FLAT_ENTRIES_FOR_TARGET_BEING_BUILT = NO;" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_PROJECT_HEADERS = NO;" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_FRAMEWORK_ENTRIES_FOR_ALL_PRODUCT_TYPES = NO;" + nl;
			xcconfigfilecontents += "ALWAYS_SEARCH_USER_PATHS = NO;" + nl;
			xcconfigfilecontents += "OTHER_CFLAGS = -x objective-c;" + nl;
			xcconfigfilecontents += "OTHER_CPLUSPLUSFLAGS = -x objective-c++;" + nl;
			

			
			print("saving xcconfig file: " + xcconfigfile);
			f = open(xcconfigfile, "w")
			f.write(xcconfigfilecontents);
			f.close();

			print("done. now compile with the xcode project.");

			exit(0);

		else:
			#building game.
			if "ios" not in self.target_config:
				print("no iOS configuration details");
				return;

			#mkdirs
			try:
				self.makeDirectories([self.game_dir + self.ds + self.build_folder + self.ds + self.output]);
				self.makeDirectories([self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "data"]);
				self.makeDirectories([self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "build-cache"]);
			except OSError as exc: 
				print("could not mkdirs (iphone) ");
				exit(0);
				pass;

			#projectname ark2d
			projectname = self.game_name_safe; # config['game_short_name'];
			companynamesafe = self.developer_name_safe; #config['company_name_safe'];

			# generate gyp file. 
			print("creating gyp file");
			gypfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + projectname + ".gyp";

			gypfile = {};
			gypfile['defines'] = []; #'ARK2D_IPHONE'];
			
			gypfile['targets'] = [];

			gypfiletarget = {};
			gypfiletarget['target_name'] = projectname + "-iPhone";
			gypfiletarget['type'] = "executable";
			gypfiletarget['mac_bundle'] = 1;
			#'mac_bundle': 1,
			gypfiletarget['include_dirs'] = [];
			gypfiletarget['sources'] = [];

			for srcfile in self.game_config['src_files']:
				gypfiletarget['sources'].extend(["../../"+srcfile]);

			gypfiletarget['sources!'] = [];
			gypfiletarget['dependencies'] = [];
			gypfiletarget['conditions'] = [];
			gypfiletargetcondition = {};
			gypfiletargetcondition['defines'] = ['ARK2D_IPHONE', 'ARK2D_IOS']; #, 'CF_EXCLUDE_CSTD_HEADERS'];

			if self.debug:
				gypfiletargetcondition['defines'].extend(["ARK2D_DEBUG"]);

			gypfiletargetcondition['sources'] = [];
			gypfiletargetcondition['sources!'] = [];
			gypfiletargetcondition['link_settings'] = {};
			gypfiletargetcondition['link_settings']['libraries'] = [
				'$(SDKROOT)/System/Library/Frameworks/AVFoundation.framework',
				'$(SDKROOT)/System/Library/Frameworks/CoreAudio.framework',
				'$(SDKROOT)/System/Library/Frameworks/AudioToolbox.framework',
				'$(SDKROOT)/System/Library/Frameworks/CoreFoundation.framework',
	          	'$(SDKROOT)/System/Library/Frameworks/CoreGraphics.framework',
	          	'$(SDKROOT)/System/Library/Frameworks/Foundation.framework',
	          	'$(SDKROOT)/System/Library/Frameworks/QuartzCore.framework',
	          	'$(SDKROOT)/System/Library/Frameworks/OpenAL.framework',
	          	'$(SDKROOT)/System/Library/Frameworks/GameKit.framework',
	          	'$(SDKROOT)/System/Library/Frameworks/SystemConfiguration.framework',
	          	'$(SDKROOT)/System/Library/Frameworks/CoreData.framework',
	          	'$(SDKROOT)/System/Library/Frameworks/OpenGLES.framework',
	          	'$(SDKROOT)/System/Library/Frameworks/UIKit.framework',
	          	self.ark2d_dir + '/lib/iphone/libfreetype.a',
	          	self.ark2d_dir + '/lib/iphone/libGoogleAnalyticsServices.a',
	          	self.ark2d_dir + '/build/ios/DerivedData/ark2d/Build/Products/Default-iphoneos/libark2d-iPhone.a'
			];

			print("Add libraries to project ")
			"""
			if "game_libraries" in config:
				for gamelibrary in config['game_libraries']:
					gamelibraryname = gamelibrary['name'];
					if "iphone" in gamelibrary:
						if "os" in gamelibrary['iphone']:
							gypfiletargetcondition['link_settings']['libraries'].extend([self.game_dir + self.ds + gamelibrary['iphone']['os']]);
			"""
			if "native_libraries" in self.ios_config:
				for nativelib in self.ios_config['native_libraries']:
					if "os" in nativelib:
						for lib in nativelib['os']:
							gypfiletargetcondition['link_settings']['libraries'].extend([self.game_dir + self.ds + lib]);




			gypfiletargetcondition['ldflags'] = [ ];

			gypfiletargetcondition['link_settings!'] = [];

			ark2ddir = self.ark2d_dir + "/src/ARK2D";
			gypfiletargetcondition['include_dirs'] = [ 
				ark2ddir, 
				ark2ddir + '/vendor/iphone',
				ark2ddir + '/vendor/spine/includes'
			];
			
			gypfiletargetcondition['xcode_settings'] = {};
			gypfiletargetcondition['xcode_settings']['ARCHS'] = "armv6 armv7 arm64";
			gypfiletargetcondition['xcode_settings']['IPHONEOS_DEPLOYMENT_TARGET'] = '6.0';
			gypfiletargetcondition['xcode_settings']['SDKROOT'] = "iphoneos";
			gypfiletargetcondition['xcode_settings']['TARGETED_DEVICE_FAMILY'] = "1,2";
			gypfiletargetcondition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] = "ARK2D_IPHONE;ARK2D_IOS";
			gypfiletargetcondition['xcode_settings']['GCC_OPTIMIZATION_LEVEL'] = "0";

			if self.debug: 
				gypfiletargetcondition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] += " ARK2D_DEBUG";
			
			xcconfigfilesimple = projectname + ".xcconfig";
			xcconfigfile = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + xcconfigfilesimple;

			gypfiletargetcondition['xcode_config_file'] = xcconfigfilesimple;
			gypfiletargetcondition['mac_bundle_resources'] = [
				"data/",				#ark2d and game data

				"Icon.png",				#iphone
				"Icon@2x.png",			#iphone-retina
				"Icon-Small.png",		#iphone-spotlight
				"Icon-Small@2x.png",	#iphone-spotlight-retina

				"Icon-72.png",			#ipad
				"Icon-72@2x.png",		#ipad-retina
				"Icon-Small-50.png",	#ipad-spotlight
				"Icon-Small-50@2x.png",	#ipad-spotlight-retina

				"Icon-40.png", 			#ios7
				"Icon-80.png", 			#ios7
				"Icon-120.png", 		#ios7 

				"Icon-76.png", 			#ios7
				"Icon-152.png", 		#ios7

				"iTunesArtwork", 		#app-store-icon
				"iTunesArtwork@2x", 	#app-store-icon-retina
			];

			iphonecondition = [];
			iphonecondition.extend(["OS == 'mac'"]);
			iphonecondition.extend([gypfiletargetcondition]);
			gypfiletarget['conditions'].extend([iphonecondition]);
			
			gypfile['targets'].extend([gypfiletarget]);


			# simulator target
			"""
			gypfiletarget_simulator = {};
			gypfiletarget_simulator['target_name'] = projectname + "-iPhoneSimulator";
			gypfiletarget_simulator['type'] = "executable";
			gypfiletarget_simulator['mac_bundle'] = 1;
			#'mac_bundle': 1,
			gypfiletarget_simulator['include_dirs'] = [];
			gypfiletarget_simulator['sources'] = [];

			for srcfile in config['game_src_files']:
				gypfiletarget_simulator['sources'].extend(["../../"+srcfile]);

			gypfiletarget_simulator['sources!'] = [];
			gypfiletarget_simulator['dependencies'] = [];
			gypfiletarget_simulator['conditions'] = [];
			gypfiletarget_simulator_condition = {};
			gypfiletarget_simulator_condition['defines'] = ['ARK2D_IPHONE']; #, 'CF_EXCLUDE_CSTD_HEADERS'];

			if self.debug:
				gypfiletarget_simulator_condition['defines'].extend(["ARK2D_DEBUG"]);

			gypfiletarget_simulator_condition['sources'] = [];
			gypfiletarget_simulator_condition['sources!'] = [];
			gypfiletarget_simulator_condition['link_settings'] = {};
			gypfiletarget_simulator_condition['link_settings']['libraries'] = [
				'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/AVFoundation.framework',
				'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/CoreAudio.framework',
				'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/AudioToolbox.framework',
				'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/CoreFoundation.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/CoreGraphics.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/UIKit.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/Foundation.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/QuartzCore.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/OpenGLES.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/OpenAL.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/GameKit.framework',
	          	#'../../lib/iphone/libfreetype.a'
	          	config['osx']['ark2d_dir'] + '/lib/iphone/libfreetype.a',
	          	config['osx']['ark2d_dir'] + '/lib/osx/libcurl.a',
	          	
	          	config['osx']['ark2d_dir'] + '/build/iphone/DerivedData/ark2d/Build/Products/Default-iphonesimulator/libark2d-iPhone.a'
					              ];
					              #'../../lib/iphone/libfreetype.a
			

			gypfiletarget_simulator_condition['ldflags'] = [ ];

			gypfiletarget_simulator_condition['link_settings!'] = [];

			ark2ddir = config['osx']['ark2d_dir'] + "/src/ARK2D";
			gypfiletarget_simulator_condition['include_dirs'] = [ 
				ark2ddir, 
				ark2ddir + '/vendor/iphone' 
			];
			
			gypfiletarget_simulator_condition['xcode_settings'] = {};
			gypfiletarget_simulator_condition['xcode_settings']['ARCHS'] = "armv6 armv7";
			gypfiletarget_simulator_condition['xcode_settings']['IPHONEOS_DEPLOYMENT_TARGET'] = '6.0';
			gypfiletarget_simulator_condition['xcode_settings']['SDKROOT'] = "iphonesimulator";
			gypfiletarget_simulator_condition['xcode_settings']['TARGETED_DEVICE_FAMILY'] = "1,2";
			gypfiletarget_simulator_condition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] = "ARK2D_IPHONE";
			gypfiletarget_simulator_condition['xcode_settings']['GCC_OPTIMIZATION_LEVEL'] = "0";

			if self.debug: 
				gypfiletarget_simulator_condition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] += " ARK2D_DEBUG";
			
			xcconfigfilesimple = projectname + ".xcconfig";
			xcconfigfile = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + xcconfigfilesimple;

			gypfiletarget_simulator_condition['xcode_config_file'] = xcconfigfilesimple;
			gypfiletarget_simulator_condition['mac_bundle_resources'] = [
				"data/",				#ark2d and game data

				"Icon.png",				#iphone
				"Icon@2x.png",			#iphone-retina
				"Icon-Small.png",		#iphone-spotlight
				"Icon-Small@2x.png",	#iphone-spotlight-retina

				"Icon-72.png",			#ipad
				"Icon-72@2x.png",		#ipad-retina
				"Icon-Small-50.png",	#ipad-spotlight
				"Icon-Small-50@2x.png",	#ipad-spotlight-retina

				"iTunesArtwork", 		#app-store-icon
				"iTunesArtwork@2x", 	#app-store-icon-retina
			];

			iphonecondition_simulator = [];
			iphonecondition_simulator.extend(["OS == 'mac'"]);
			iphonecondition_simulator.extend([gypfiletarget_simulator_condition]);
			gypfiletarget_simulator['conditions'].extend([iphonecondition_simulator]);
			
			gypfile['targets'].extend([gypfiletarget_simulator]);
			"""

			print("saving gyp file: " + gypfilename);
			f = open(gypfilename, "w")
			f.write(json.dumps(gypfile, sort_keys=True, indent=4));
			f.close();



			#delete xcode project? 
			pchfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + projectname + "-Prefix.pch";
			info_plist_filename = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + projectname + "-Info.plist";
			try:
				print("deleting xcode project");
				os.system("rm -r -d " + pchfilename);
				os.system("rm -r -d " + info_plist_filename);
				os.system("rm -r -d " + xcconfigfile);
				os.system("rm -r -d " + self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "DerivedData");
			except:
				print("could not delete xcode project");

			# run gyp file.
			#os.system("python " + gyp_executable + " " + gypfilename + " --depth=../../src");
			os.system("python " + gyp_executable + "_main.py " + gypfilename + " --depth=../../src");
			
			# add precompiled headers file
			print("generating pch file");
			
			nl = "\r\n";
			pcheaderfile = "";
			#pcheaderfile += "#ifdef __OBJC__" + nl;
			pcheaderfile += "	#import <Foundation/Foundation.h>" + nl;
			pcheaderfile += "	#import <AVFoundation/AVFoundation.h>" + nl;
			pcheaderfile += "	#import <UIKit/UIKit.h>" + nl + nl;


			pcheaderfile += "	#import <OpenAL/al.h>" + nl;
			pcheaderfile += "	#import <OpenAL/alc.h>" + nl + nl

			pcheaderfile += "	#import <OpenGLES/EAGL.h>" + nl;
			pcheaderfile += "	#import <QuartzCore/QuartzCore.h>" + nl + nl;

			pcheaderfile += "	#import <OpenGLES/ES1/gl.h>" + nl;
			pcheaderfile += "	#import <OpenGLES/ES1/glext.h>" + nl;
			pcheaderfile += "	#import <OpenGLES/ES2/gl.h>" + nl;
			pcheaderfile += "	#import <OpenGLES/ES2/glext.h>" + nl;
			pcheaderfile += "	#import <OpenGLES/EAGLDrawable.h>" + nl + nl;

			pcheaderfile += "	#import <CoreFoundation/CFBundle.h>" + nl;
			pcheaderfile += "	#import <CoreFoundation/CoreFoundation.h>" + nl;

			#pcheaderfile += "#endif";

			print("saving pch file: " + pchfilename);
			f = open(pchfilename, "w")
			f.write(pcheaderfile);
			f.close();

			print ("generating project-Info.plist file");
			info_plist_contents = "";
			info_plist_contents += '<?xml version="1.0" encoding="UTF-8"?>' + nl;
			info_plist_contents += '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' + nl;
			info_plist_contents += '<plist version="1.0">' + nl;
			info_plist_contents += '<dict>' + nl;
			info_plist_contents += '	<key>UIViewControllerBasedStatusBarAppearance</key>' + nl;
			info_plist_contents += '	<false/>' + nl;
			info_plist_contents += '	<key>CFBundleDevelopmentRegion</key>' + nl;
			info_plist_contents += '	<string>en</string>' + nl;
			info_plist_contents += '	<key>CFBundleDisplayName</key>' + nl;
			info_plist_contents += '	<string>' + self.game_name + '</string>' + nl;
			info_plist_contents += '	<key>CFBundleExecutable</key>' + nl;
			info_plist_contents += '	<string>${EXECUTABLE_NAME}</string>' + nl;

			info_plist_contents += '	<key>CFBundleIconFile</key>' + nl;
			info_plist_contents += '	<string></string>' + nl;

			info_plist_contents += '	<key>CFBundleIconFiles</key>' + nl;
			info_plist_contents += '	<array>' + nl;
			info_plist_contents += '		<string></string>' + nl;
			info_plist_contents += '		<string>Icon.png</string>' + nl;					#iphone
			info_plist_contents += '		<string>Icon@2x.png</string>' + nl;					#iphone-retina
			info_plist_contents += '		<string>Icon-Small.png</string>' + nl;				#iphone-spotlight
			info_plist_contents += '		<string>Icon-Small@2x.png</string>' + nl;			#iphone-spotlight-retina
			info_plist_contents += '		<string>Icon-72.png</string>' + nl;					#ipad
			info_plist_contents += '		<string>Icon-72@2x.png</string>' + nl;				#ipad-retina
			info_plist_contents += '		<string>Icon-Small-50.png</string>' + nl;			#ipad-spotlight
			info_plist_contents += '		<string>Icon-Small-50@2x.png</string>' + nl;		#ipad-spotlight-retina
			info_plist_contents += '		<string>Icon-40.png</string>' + nl;					#ios7
			info_plist_contents += '		<string>Icon-80.png</string>' + nl;					#ios7
			info_plist_contents += '		<string>Icon-120.png</string>' + nl;				#ios7
			info_plist_contents += '		<string>Icon-76.png</string>' + nl;					#ios7
			info_plist_contents += '		<string>Icon-152.png</string>' + nl;				#ios7
			info_plist_contents += '	</array>' + nl;

			info_plist_contents += '	<key>CFBundleIcons</key>' + nl;
			info_plist_contents += '	<dict>' + nl;
			info_plist_contents += '		<key>CFBundlePrimaryIcon</key>' + nl;
			info_plist_contents += '		<dict>' + nl;
			info_plist_contents += '			<key>CFBundleIconFiles</key>' + nl;
			info_plist_contents += '			<array>' + nl;
			info_plist_contents += '				<string></string>' + nl;
			info_plist_contents += '				<string>Icon.png</string>' + nl;				#iphone
			info_plist_contents += '				<string>Icon@2x.png</string>' + nl;				#iphone-retina
			info_plist_contents += '				<string>Icon-Small.png</string>' + nl;			#iphone-spotlight
			info_plist_contents += '				<string>Icon-Small@2x.png</string>' + nl;		#iphone-spotlight-retina
			info_plist_contents += '				<string>Icon-72.png</string>' + nl;				#ipad
			info_plist_contents += '				<string>Icon-72@2x.png</string>' + nl;			#ipad-retina
			info_plist_contents += '				<string>Icon-Small-50.png</string>' + nl;		#ipad-spotlight
			info_plist_contents += '				<string>Icon-Small-50@2x.png</string>' + nl;	#ipad-spotlight-retina
			info_plist_contents += '				<string>Icon-40.png</string>' + nl;					#ios7
			info_plist_contents += '				<string>Icon-80.png</string>' + nl;					#ios7
			info_plist_contents += '				<string>Icon-120.png</string>' + nl;				#ios7
			info_plist_contents += '				<string>Icon-76.png</string>' + nl;					#ios7
			info_plist_contents += '				<string>Icon-152.png</string>' + nl;				#ios7
			info_plist_contents += '			</array>' + nl;

			info_plist_contents += '			<key>UIPrerenderedIcon</key>' + nl;
			if self.ios_config['icon']['prerendered']:
				info_plist_contents += '			<true/>' + nl;
			
			info_plist_contents += '		</dict>' + nl;
			info_plist_contents += '	</dict>' + nl;

			
			info_plist_contents += '	<key>CFBundleIdentifier</key>' + nl;
			info_plist_contents += '	<string>org.' + companynamesafe + '.' + projectname + '</string>' + nl;
			info_plist_contents += '	<key>CFBundleInfoDictionaryVersion</key>' + nl;
			info_plist_contents += '	<string>6.0</string>' + nl;
			info_plist_contents += '	<key>CFBundleName</key>' + nl;
			info_plist_contents += '	<string>${PRODUCT_NAME}</string>' + nl;
			info_plist_contents += '	<key>CFBundlePackageType</key>' + nl;
			info_plist_contents += '	<string>APPL</string>' + nl;
			info_plist_contents += '	<key>CFBundleShortVersionString</key>' + nl;
			info_plist_contents += '	<string>1.0</string>' + nl;
			info_plist_contents += '	<key>CFBundleSignature</key>' + nl;
			info_plist_contents += '	<string>????</string>' + nl;
			info_plist_contents += '	<key>CFBundleVersion</key>' + nl;
			info_plist_contents += '	<string>1.0</string>' + nl;
			info_plist_contents += '	<key>LSRequiresIPhoneOS</key>' + nl;
			info_plist_contents += '	<true/>' + nl;
			info_plist_contents += '	<key>UIPrerenderedIcon</key>' + nl;
			if self.ios_config['icon']['prerendered']:
				info_plist_contents += '		<true/>' + nl;
			else:
				info_plist_contents += '		<false/>' + nl;

			info_plist_contents += '	<key>UIStatusBarHidden</key>' + nl;
			info_plist_contents += '	<true/>' + nl;
			info_plist_contents += '	<key>UISupportedInterfaceOrientations</key>' + nl;
			info_plist_contents += '	<array>' + nl;
			if (self.game_orientation == "portrait"):
				info_plist_contents += '	<string>UIInterfaceOrientationPortrait</string>' + nl;
			elif(self.game_orientation == "landscape"):
				info_plist_contents += '	<string>UIInterfaceOrientationLandscapeRight</string>' + nl;
			
			info_plist_contents += '	</array>' + nl;
			info_plist_contents += '</dict>' + nl;
			info_plist_contents += '</plist>';

			print("saving project-Info.plist file");
			f = open(info_plist_filename, "w")
			f.write(info_plist_contents);
			f.close();


			#print("set the pch manually in xcode, under Apple LLVM compiler 4.1 - Language...");
			#print("set YES and ../../lib/iphone/" + projectname + "-Prefx.pch");

			#create xcconfig file
			print("generating xcconfig file:");
			xcconfigfilecontents = "";
			xcconfigfilecontents += "GCC_PRECOMPILE_PREFIX_HEADER = YES;" + nl;
			xcconfigfilecontents += "GCC_PREFIX_HEADER = " + pchfilename + ";" + nl;
			xcconfigfilecontents += "SRCROOT = " + self.game_dir + self.ds + "src" + self.ds + "ARK2D" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_FLAT_ENTRIES_FOR_TARGET_BEING_BUILT = NO;" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_PROJECT_HEADERS = NO;" + nl;
			xcconfigfilecontents += "HEADERMAP_INCLUDES_FRAMEWORK_ENTRIES_FOR_ALL_PRODUCT_TYPES = NO;" + nl;
			xcconfigfilecontents += "ALWAYS_SEARCH_USER_PATHS = NO;" + nl;
			xcconfigfilecontents += "OTHER_CFLAGS = -x objective-c;" + nl;
			xcconfigfilecontents += "OTHER_CPLUSPLUSFLAGS = -x objective-c++;" + nl;
			xcconfigfilecontents += "OTHER_LDFLAGS = -lbz2 -lcurl -lz;" + nl;
			xcconfigfilecontents += "INFOPLIST_FILE = " + info_plist_filename;
				
			
			print("saving xcconfig file: " + xcconfigfile);
			f = open(xcconfigfile, "w")
			f.write(xcconfigfilecontents);
			f.close();

			# copy game resources in to project data folder.
			#subprocess.call(["cp -r " + config['osx']['game_resources_dir'] + " " + self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "data/"], shell=True); #libark2d
			self.processAssets();
			
			# copy game resources in to project data folder
			subprocess.call(["cp -r " + self.ark2d_dir + "/data " + self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "data/ark2d/"], shell=True); #libark2d
			
			# generate icons
			if "icon" in self.ios_config:
				if "master_icon" in self.ios_config['icon']:
					if (self.ios_config['icon']['use_master_icon'] == True): 
						iconinterpolation = self.ios_config['icon']['master_icon_interpolation'];
						icongenarr = [];
						icongenobj = {};
						icongenobj['from'] = self.ios_config['icon']['master_icon'];
						icongenobj['from'] = self.str_replace(icongenobj['from'], [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
						icongenobj['to'] = [
							# iPhone Icon
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon.png",
								"width" : 57,
								"height": 57, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon@2x.png",
								"width" : 114,
								"height": 114, 
								"interpolation": iconinterpolation
							},
							# iPhone Spotlight Icon
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-Small.png",
								"width" : 29,
								"height": 29, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-Small@2x.png",
								"width" : 58,
								"height": 58, 
								"interpolation": iconinterpolation
							},
							# iPad Icon
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-72.png",
								"width" : 72,
								"height": 72, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-72@2x.png",
								"width" : 144,
								"height": 144, 
								"interpolation": iconinterpolation
							},
							# iPad Spotlight Icon
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-Small-50.png",
								"width" : 50,
								"height": 50, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-Small-50@2x.png",
								"width" : 100,
								"height": 100, 
								"interpolation": iconinterpolation
							},
							# app store icon
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "iTunesArtwork",
								"width" : 512,
								"height": 512, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "iTunesArtwork@2x",
								"width" : 1024,
								"height": 1024, 
								"interpolation": iconinterpolation
							},

							# iOS 7 shit
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-40.png",
								"width" : 40, 
								"height": 40, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-80.png",
								"width" : 80,
								"height": 80, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-120.png",
								"width" : 120,
								"height": 120, 
								"interpolation": iconinterpolation
							},
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-76.png",
								"width" : 76,
								"height": 76, 
								"interpolation": iconinterpolation
							}, 
							{
								"filename": self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds + "Icon-152.png",
								"width" : 152,
								"height": 152, 
								"interpolation": iconinterpolation
							}
							 
							
						];
						icongenarr.extend([icongenobj]);
						#icongenstr = json.dumps(icongenarr, sort_keys=True, indent=0, new);

						
						icongenstr = str(json.dumps(icongenarr, separators=(',',':'))).replace("\"", "\\\"");
						icongenLINE = "java -jar -Xmx512m " + self.ark2d_dir + "/../Tools/ARK2D\ Image\ Resizer/build/jar/ImageResizer.jar \"" + icongenstr + "\"";
						print(icongenLINE);
						subprocess.call([icongenLINE], shell=True); 


						#os.system();
						pass;
					#else:
					pass;
						#	if (config['osx']['android']['icon'] != ''):
						#	subprocess.call(['cp ' + config['osx']['android']['icon'] + " " + rootPath+"/build/android/project/res/drawable/ic_launcher.png"], shell=True);
						#else:
						#	subprocess.call(['cp ' + ark2ddir + "/__preproduction/icon/512.png " + rootPath+"/build/android/project/res/drawable/ic_launcher.png"], shell=True);
				else:

					# copy individual files
					print("Copying individual icon files.");
					iphonedir = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds;
					
					icon_72 = self.ios_config['icon']['icon_72'];
					icon_72_2x = self.ios_config['icon']['icon_72_2x'];
					icon_small_50 = self.ios_config['icon']['icon_small_50'];
					icon_small_50_2x = self.ios_config['icon']['icon_small_50_2x'];
					icon_small = self.ios_config['icon']['icon_small'];
					icon_small_2x = self.ios_config['icon']['icon_small_2x'];
					icon = self.ios_config['icon']['icon'];
					icon_2x = self.ios_config['icon']['icon_2x'];
					itunes_artwork = self.ios_config['icon']['itunes_artwork'];
					itunes_artwork_2x = self.ios_config['icon']['itunes_artwork_2x'];
					
					icon_40 = self.ios_config['icon']['icon_40'];
					icon_80 = self.ios_config['icon']['icon_80'];
					icon_120 = self.ios_config['icon']['icon_120'];
					icon_76 = self.ios_config['icon']['icon_76'];
					icon_152 = self.ios_config['icon']['icon_152'];
					
					

					icon_72 = self.str_replace(icon_72, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon_72_2x = self.str_replace(icon_72_2x, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon_small_50 = self.str_replace(icon_small_50, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon_small_50_2x = self.str_replace(icon_small_50_2x, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon_small = self.str_replace(icon_small, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon_small_2x = self.str_replace(icon_small_2x, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon = self.str_replace(icon, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon_2x = self.str_replace(icon_2x, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					itunes_artwork = self.str_replace(itunes_artwork, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					itunes_artwork_2x = self.str_replace(itunes_artwork_2x, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					
					icon_40 = self.str_replace(icon_40, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon_80 = self.str_replace(icon_80, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon_120 = self.str_replace(icon_120, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon_76 = self.str_replace(icon_76, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					icon_152 = self.str_replace(icon_152, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
					 
					subprocess.call(["cp -r " + icon_72 + " " + iphonedir + "Icon-72.png"], shell=True);
					subprocess.call(["cp -r " + icon_72_2x + " " + iphonedir + "Icon-72@2x.png"], shell=True);
					subprocess.call(["cp -r " + icon_small_50 + " " + iphonedir + "Icon-Small-50.png"], shell=True);
					subprocess.call(["cp -r " + icon_small_50_2x + " " + iphonedir + "Icon-Small-50@2x.png"], shell=True);
					subprocess.call(["cp -r " + icon_small + " " + iphonedir + "Icon-Small.png"], shell=True);
					subprocess.call(["cp -r " + icon_small_2x + " " + iphonedir + "Icon-Small@2x.png"], shell=True);
					subprocess.call(["cp -r " + icon + " " + iphonedir + "Icon.png"], shell=True);
					subprocess.call(["cp -r " + icon_2x + " " + iphonedir + "Icon@2x.png"], shell=True);
					subprocess.call(["cp -r " + itunes_artwork + " " + iphonedir + "iTunesArtwork"], shell=True);
					subprocess.call(["cp -r " + itunes_artwork_2x + " " + iphonedir + "iTunesArtwork@2x"], shell=True);
					
					subprocess.call(["cp -r " + icon_40 + " " + iphonedir + "Icon-40.png"], shell=True);
					subprocess.call(["cp -r " + icon_80 + " " + iphonedir + "Icon-80.png"], shell=True);
					subprocess.call(["cp -r " + icon_120 + " " + iphonedir + "Icon-120.png"], shell=True);
					subprocess.call(["cp -r " + icon_76 + " " + iphonedir + "Icon-76.png"], shell=True);
					subprocess.call(["cp -r " + icon_152 + " " + iphonedir + "Icon-152.png"], shell=True);


			# generate defaults
			print("Generating defaults...")
			if "defaults" in self.ios_config:
				if "use_master" in self.ios_config['defaults']:
					if (self.ios_config['defaults']['use_master'] == True): 
						# do
						startdir = self.game_dir + self.ds + self.build_folder + self.ds + self.output + self.ds;
						defaultsgenarr = [];
						defaultsgenobj = {};
						defaultsgenobj['from'] = self.ios_config['defaults']['master'];
						defaultsgenobj['from'] = self.str_replace(defaultsgenobj['from'], [("%PREPRODUCTION_DIR%", self.game_preproduction_dir), ("%ARK2D_DIR%", self.ark2d_dir)]);
						defaultsgenobj['to'] = [
							{
								"filename": startdir + "Default.png",
								"width" : 320,
								"height": 480, 
								"interpolation": "nearest_neighbour"
							},
							{
								"filename": startdir + "Default@2x.png",
								"width" : 640,
								"height": 960, 
								"interpolation": "nearest_neighbour"
							},
							{
								"filename": startdir + "Default-568h@2x.png",
								"width" : 640,
								"height": 1136, 
								"interpolation": "nearest_neighbour"
							},
							{
								"filename": startdir + "Default-Landscape.png",
								"width" : 1024,
								"height": 768, 
								"interpolation": "nearest_neighbour"
							},
							{
								"filename": startdir + "Default-Landscape@2x.png",
								"width" : 2048,
								"height": 1536, 
								"interpolation": "nearest_neighbour"
							},
							{
								"filename": startdir + "Default-Portrait.png",
								"width" : 768,
								"height": 1024, 
								"interpolation": "nearest_neighbour"
							},
							{
								"filename": startdir + "Default-Portrait@2x.png",
								"width" : 1536,
								"height": 2048, 
								"interpolation": "nearest_neighbour"
							}
						];
						defaultsgenarr.extend([defaultsgenobj]);

						defaultsgenstr = str(json.dumps(defaultsgenarr, separators=(',',':'))).replace("\"", "\\\"");
						defaultsgenLINE = "java -jar -Xmx512m " + self.ark2d_dir + "/../Tools/ARK2D\ Image\ Resizer/build/jar/ImageResizer.jar \"" + defaultsgenstr + "\"";
						print(defaultsgenLINE);
						subprocess.call([defaultsgenLINE], shell=True); 
					pass;
				pass;
			print("Done generating defaults.");

			
				
			#exit(0);
			pass;


		"""
		# mkdirs
		print("Making Directories...");
		for h in self.mkdirs:
			#h = "'" + h + "'";
			#print("mkdir " + h);
			#subprocess.call(['mkdir C:\\' + h], shell=True);
			try:
				os.makedirs(h);
			except OSError as exc: 
				if exc.errno == errno.EEXIST:
					pass
				else: raise
		
		# compile cache thing
		cachefilename = "";		
		cachefilename += self.build_folder + self.ds + self.platform + self.ds + "build-cache" + self.ds  + "compiled.json";
		self.createCacheFile(cachefilename);
		f = open(cachefilename, "r")
		fcontents = f.read();
		f.close();
		fjson = json.loads(fcontents);
		fchanged = False;
		

		# Compile files
		varROOTDIR = "/Developer";
		varARCH = "-arch i386";
		varMINVERSION = "3.2";
		varMAXVERSION = "4.3";
		varDEVROOT = varROOTDIR + "/Platforms/" + self.varPLATFORM + ".platform/Developer";
		varSDKROOT = varDEVROOT + "/SDKs/" + self.varPLATFORM + varMAXVERSION + ".sdk";

		
		print("-------------------------");
		print("Compiling Source Files...");
		print("-------------------------");
		lala = 0;
		for srcFile in self.src_files:
			#if (lala == 4): break;
			compileStr = "";

			srcFileIndex = srcFile.rfind('.');
			srcFileExtension = srcFile[srcFileIndex+1:len(srcFile)];
			srcFileNew = srcFile[0:srcFileIndex] + ".o";

			
			if srcFileExtension == 'c':
				compileStr += self.gccCompiler;
			elif srcFileExtension == 'cpp':
				compileStr += self.gppCompiler;
			elif srcFileExtension == 'mm':
				compileStr += self.objcCompiler;	

			#compileStr += "i686-apple-darwin11-llvm-g++-4.2 ";

			
			compileStr += " -DARK2D_IPHONE -c -fmessage-length=0 ";
			# -mmacosx-version-min=10.5
			if ("vendor" not in srcFile):
				compileStr += " -O3 -march=i386 -pipe -gdwarf-2 -no-cpp-precomp -mthumb -isysroot " + varSDKROOT + " -m" + self.varPLATFORMsml + "-version-min=" + varMINVERSION + "  ";
				compileStr += " -x objective-c++ ";
				compileStr += "  -Wno-trigraphs -fpascal-strings -Wmissing-prototypes -Wreturn-type -Wunused-variable -DDEBUG=1 -fexceptions -fasm-blocks -fobjc-abi-version=2 -fobjc-legacy-dispatch ";
			else:
				compileStr += " -arch i386 ";
			compileStr += " -I " + self.ark2d_dir + "/src/ARK2D/vendor/iphone ";
			#compileStr += " -I /Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin:/Developer/Platforms/iPhoneSimulator.platform/Developer/usr/include:/Developer/usr/bin:/usr/bin:/bin:/usr/sbin:/sbin ";
			#compileStr += " -framework Kernel "; 
			#compileStr += " -platform iphoneos4.3 ";
			#compileStr += " -D__IPHONE_OS_VERSION_MIN_REQUIRED=40300 ";
			#compileStr += " -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk ";
			#  
			
			#compileStr += " -I /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/UIKit.framework/Headers "; 
			#compileStr += " -framework /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/OpenGLES.framework "; 
			#compileStr += " -framework Foundation -framework CoreGraphics -framework QuartzCore -framework OpenGLES -framework Kernel";
			compileStr += " -o \"" + self.build_folder + self.ds + self.platform + self.ds + srcFileNew + "\"  \"" + srcFile + "\" ";
			#compileStr += " -framework OpenGL -framework OpenAL -framework Foundation -framework Cocoa -lobjc ";

			lala += 1; 

			if (not srcFile in fjson or fjson[srcFile]['date_modified'] < os.stat(srcFile).st_mtime):
				
				processThisFile = True;
		
				print(compileStr);
				print("-------------------------");
				os.system(compileStr);

				fjson[srcFile] = {"date_modified": os.stat(srcFile).st_mtime };
				fchanged = True;
		
			pass;

		#
		
		
		

		# update compile cache thing
		if (fchanged == True):
			f = open(self.build_folder + self.ds + self.platform + self.ds + "build-cache" + self.ds + "compiled.json", "w")
			f.write(json.dumps(fjson, sort_keys=True, indent=4));
			f.close();

		print("-------------------------");
		print("Linking in to Library...");
		print("-------------------------");
		linkingStr = "";
		linkingStr += "/Developer/Platforms/iPhoneSimulator.platform/Developer/usr/bin/g++-4.2" + " -arch i386 -isysroot " + varSDKROOT + " -framework QuartzCore  -framework OpenGLES -framework OpenAL -framework Foundation -framework CoreGraphics -framework UIKit -m" + self.varPLATFORMsml + "-version-min=" + varMINVERSION + "   -lobjc -dynamiclib -o " + self.build_artifact;
		linkingStr += " -L" + self.ark2d_dir + "/lib/iphone -L" + self.ark2d_dir + "/lib/osx -L/usr/lib ";
		# -framework CoreGraphics
		# -framework Kernel
		#linkingStr += " -march=i386 ";
		linkingStr += " -Wl,-no_pie ";
		linkingStr += " -Wl,-no_compact_unwind ";
		linkingStr += " -lbz2 -lcurl -lz ";

	
		for h in self.src_files:
			findex = h. rfind('.');
			newf = h[0:findex] + ".o";
			linkingStr += " " + self.build_folder + self.ds + self.platform + self.ds + newf;
		
		for f in self.dll_files:
			linkingStr += " " + f;
			
		for f in self.static_libraries:
			linkingStr += " -l" + f;
			
		print(linkingStr);
		
		subprocess.call([linkingStr], shell=True);	
		print("Done!");

		#copy files...
		# TODO: 
		"""

	def startAndroid(self):
		print("Building Android");
		
		# open config
		
		
		# define vars.	
		nl = "\r\n";
		
		
		
		#self.android_sdkdir = self.android_sdkdir; #config[self.platformOn]['android']['sdk_dir'];
		#ndkdir = config[self.platformOn]['android']['ndk_dir'];

		
		if (self.building_game):

			#sdk version, not NDK.
			appplatformno = self.android_config['sdk_version'];
			appplatform= "android-" + str(appplatformno);
			ndkappplatformno = self.android_config['ndk_version'];
			ndkappplatform = "android-" + str(ndkappplatformno);

			print ("Android sdk version: " + str(appplatformno));

			#game specific vars
			#game_name = #config['game_name'];
			#game_name_safe = config['game_name_safe'];
			#game_short_name = config['game_short_name'];
			#game_description = config['game_description'];
			#game_resources_dir = config[self.platformOn]['game_resources_dir'];
			#company_name = config['company_name'];
			#company_name_safe = config['company_name_safe'];
			javaPackageName = "org." + self.developer_name_safe + "." + self.game_name_safe;
			
			audio_quality = self.android_config['audio_quality'];
			
			rootPath = self.game_dir;

			ndkprojectpath = self.game_dir;
			thisCreateDirs = [self.game_dir + self.ds + "build"];
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "build-cache"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "assets"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "assets" + self.ds + "ark2d"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "src"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "gen"]);
			#thisCreateDirs.extend([rootPath+"/build/android/project/lib"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "libs"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "libs" + self.ds + "armeabi"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "libs" + self.ds + "armeabi-v7a"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "libs" + self.ds + "x86"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "obj"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "obj" + self.ds + "local"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "obj" + self.ds + "local" + self.ds + "armeabi"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "obj" + self.ds + "local" + self.ds + "armeabi-v7a"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "obj" + self.ds + "local" + self.ds + "x86"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "res"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "raw"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable-mdpi"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable-hdpi"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable-xhdpi"]); 
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable-xxhdpi"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "values"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "src" + self.ds + "org"]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "src" + self.ds + "org" + self.ds + "" + self.developer_name_safe]);
			thisCreateDirs.extend([rootPath+self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "src" + self.ds + "org" + self.ds + "" + self.developer_name_safe + self.ds + self.game_name_safe]);
			pass;
		else:

			#sdk version, not NDK.
			appplatformno = self.android_config['sdk_version'];
			appplatform= "android-" + str(appplatformno);
			ndkappplatformno = self.android_config['ndk_version'];
			ndkappplatform = "android-" + str(ndkappplatformno);

			rootPath = self.ark2d_dir; 

			ndkprojectpath = self.ark2d_dir;
			thisCreateDirs = [ndkprojectpath + self.ds + "build"];
			pass;
		
		appbuilddir = rootPath + self.ds + "build" + self.ds + self.output;
		appbuildscript = rootPath + self.ds + "build" + self.ds + self.output + self.ds + "Android.mk";
		jnifolder = rootPath + self.ds + "jni";
		appbuildscript3 = rootPath + self.ds + "jni" + self.ds + "Application.mk";
		
		#check for spaces
		if (" " in ndkprojectpath) or (" " in self.android_ndkdir):
			print("Android build paths (and ndk directory) may not contain spaces.");
			return;
		
		
		thisCreateDirs.extend([appbuilddir, jnifolder]);
		
		# make some directories...
		self.makeDirectories(thisCreateDirs);
		#for thisstr in thisCreateDirs:
		#	print("mkdir " + thisstr);
		#	try:
		#		os.makedirs(thisstr);
		#	except OSError as exc: 
		#		if exc.errno == errno.EEXIST:
		#			pass
		#		else: raise
		
		if (self.building_game):

			if (self.platform == "android" and self.ouya == True):
				print("copying Ouya libs"); 
				shutil.copy(self.ouya_config['sdk'] + "/libs/ouya-sdk.jar", self.game_dir + self.ds + "build" + self.ds + self.output + self.ds + "project" + self.ds + "libs" + self.ds + "ouya-sdk.jar");

				print("copying Ouya store graphic");
				#if "ouya" in config:
				if "store_icon" in self.ouya_config:
					ouya_icon_full = self.str_replace(self.ouya_config['store_icon'], [("%PREPRODUCTION_DIR%", self.game_preproduction_dir)]);
					shutil.copy(ouya_icon_full, self.game_dir + self.ds + "build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable-xhdpi" + self.ds + "ouya_icon.png");
	
			if (self.platform == "android" and self.firetv == True):
				print("TODO: copy firetv libs...");
				# ... 
			 
			#make android.mk
			pnl = nl;
			nl = "\n";

			#if "libs" not in config['android']:
			#	config['android']['libs'] = [];

			print("Creating Android.mk");
			android_make_file = "";
			android_make_file += "LOCAL_PATH := $(call my-dir)/../../" + nl + nl;
			android_make_file += "include $(CLEAR_VARS)" + nl+nl;
			android_make_file += "LOCAL_MODULE    := " + self.game_name_safe +  nl+nl; # Here we give our module name and source file(s)
			android_make_file += "LOCAL_C_INCLUDES := " + self.ark2d_dir + "/src/ARK2D/vendor/android/libzip/jni/ " + self.ark2d_dir + "/src/ARK2D/vendor/spine/includes " + nl;
			
			android_make_file += "LOCAL_CFLAGS := -DARK2D_ANDROID "; 
			if (self.platformOn == "windows"):
				android_make_file += "-DARK2D_ANDROID_ON_WINDOWS ";
			elif (self.platformOn == "osx"):
				android_make_file += "-DARK2D_ANDROID_ON_MACINTOSH ";

			if (self.ouya == True):
				android_make_file += "-DARK2D_OUYA ";
			if (self.firetv == True):
				android_make_file += "-DARK2D_AMAZON ";

			android_make_file += "-fno-exceptions -fno-rtti -Wno-psabi ";
			if (self.debug):
				android_make_file += " -DARK2D_DEBUG -DDEBUG -DNDK_DEBUG -O0 ";
			else:
				android_make_file += " -O3 "; #-fno-strict-aliasing -mfpu=vfp -mfloat-abi=softfp ";
			android_make_file += nl+nl;

			android_make_file += "LOCAL_DEFAULT_CPP_EXTENSION := cpp" + nl+nl; 
			android_make_file += "LOCAL_SRC_FILES := \\" + nl;
			for h in self.src_files: #foreach file on config...
				if (self.get_str_extension(h)!= "rc"):
					android_make_file += "	" + h + " \\" + nl;
					
			android_make_file += "	src/jni.cpp \\ " + nl;
				
			android_make_file += nl;  
			#android_make_file += "LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz -lfreetype -lopenal -lark2d" + nl+nl;
			#android_make_file += "LOCAL_LDLIBS := -lGLESv2 -lGLESv1_CM -ldl -llog -lz -lfreetype -lopenal -lark2d" + nl+nl;
			android_make_file += "LOCAL_LDLIBS := -lGLESv2 -lEGL -ldl -llog -lz -lopenal -lark2d "; #-lfreetype

			# custom game libraries (fmod, and things)
			lib_search_path = "";
			edits_shared_libraries = [];
			if "native_libraries" in self.android_config:
				for gamelibrary in self.android_config['native_libraries']:
					gamelibraryname = gamelibrary['name'];
					
					edits_shared_libraries.extend([gamelibraryname]);
					android_make_file += " -l" + gamelibraryname + " ";

					if "jars" in gamelibrary:
						for gamelibraryjar in gamelibrary['jars']:
							self.android_libs.extend([gamelibraryjar]);

					if "armeabi" in gamelibrary:
						path = gamelibrary['armeabi'];
						path = path[:path.rfind("/")];
						lib_search_path += "-L"+self.game_dir+self.ds+path + " ";
					if "armeabi-v7a" in gamelibrary:
						path = gamelibrary['armeabi-v7a'];
						path = path[:path.rfind("/")];
						lib_search_path += "-L"+self.game_dir+self.ds+path + " ";
					if "x86" in gamelibrary:
						path = gamelibrary['x86']; 
						path = path[:path.rfind("/")];
						lib_search_path += "-L"+self.game_dir+self.ds+path + " ";


			android_make_file += lib_search_path;
			android_make_file += nl+nl;
			


			android_make_file += "include $(BUILD_SHARED_LIBRARY)" + nl;
			f = open(appbuildscript, "w");
			f.write(android_make_file);
			f.close();
			
			#make application.mk
			print("Creating Application.mk");
			application_make_file = "";
			application_make_file += "APP_PROJECT_PATH := " + ndkprojectpath + nl;
			application_make_file += "APP_BUILD_SCRIPT := " + appbuildscript + nl;
			application_make_file += "NDK_APP_OUT=" + appbuilddir + nl;
			application_make_file += "NDK_PROJECT_PATH=" + ndkprojectpath + nl;
			#application_make_file += "APP_ABI := all" + nl;
			#application_make_file += "APP_ABI := armeabi"; # armeabi-v7a x86" + nl;
			
			#next 7 fix
			if (self.debug):
				application_make_file += "APP_ABI := armeabi-v7a " + nl; 
			else:
				application_make_file += "APP_ABI := armeabi armeabi-v7a ";
				application_make_file += "x86 ";
				application_make_file += nl;  
			
			application_make_file += "APP_STL := stlport_static" + nl; 
			f = open(appbuildscript3, "w");
			f.write(application_make_file);
			f.close();

			nl = pnl;
			
			#copy ark2d in to project
			print("Copying ARK2D in to android sdk");
			#subprocess.call(["cp -r " +self.ark2d_dir + "/build/android/libs/armeabi-v7a/ " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib/"], shell=True);
			#subprocess.call(["cp -r " +self.ark2d_dir + "/build/android/libs/armeabi-v7a/ " + ndkdir + "/platforms/"+ndkappplatform+"/arch-x86/usr/lib/"], shell=True);
			
			shutil.copy2(self.ark2d_dir + self.ds + "build" + self.ds + "android" + self.ds + "libs" + self.ds + "armeabi-v7a" + self.ds + "libark2d.so", self.android_ndkdir + self.ds + "platforms"+self.ds+ndkappplatform+self.ds+"arch-arm" + self.ds + "usr" + self.ds + "lib" + self.ds);
			shutil.copy2(self.ark2d_dir + self.ds + "build" + self.ds + "android" + self.ds + "libs" + self.ds + "x86" + self.ds + "libark2d.so", self.android_ndkdir + self.ds + "platforms"+self.ds+ndkappplatform+self.ds+"arch-x86" + self.ds + "usr" + self.ds + "lib" + self.ds);

			#print ("Copying game libraries into android sdk"):


			#return;

			#copy resources in to "assets" dir.
			#print("Copying game resources in to project");
			#subprocess.call(["cp -r " + game_resources_dir + "/ " + rootPath + "/build/android/project/assets/"], shell=True);
			
			print("Making new asset directories");
			directoriesToCreate = self.listDirectories(self.game_resources_dir, False);
			for dir in directoriesToCreate:
				self.makeDirectories([rootPath + self.ds+"build" + self.ds + self.output + self.ds + "project" + self.ds + "assets" + self.ds + dir]);
			#self.makeDirectories(directoriesToCreate);

			#print("making directories for the 'lib's to copy in");
			#if ("libs" in config['android']):
			#	for lib in config['android']['libs']:
			#		td = lib[0:lib.rfind(self.ds)];
			#		self.makeDirectories([rootPath + "/build/android/project/" + td]);


			#generate game spritesheets
			self.generateSpriteSheets();

			print("Creating/opening assets cache file...");
			assetsCache = rootPath + self.ds + "build" + self.ds + self.output + self.ds + "build-cache" + self.ds + "assets.json";
			assetsJson = self.openCacheFile(assetsCache);
			fchanged = False;
			
			print("Cool, now copying files")
			filesToCopy = self.listFiles(self.game_resources_dir, False);
			print(filesToCopy);
			for file in filesToCopy:
				fromfile = self.game_resources_dir + self.ds + file;
				tofile = rootPath + self.ds + "build" + self.ds + self.output + self.ds + "project" + self.ds + "assets" + self.ds + file;
				
				#replace spaces in paths on max osx with slash-spaces
				#fromfile = fromfile.replace(" ", "\ ");
				#tofile = tofile.replace(" ", "\ ");
				 
				if (not fromfile in assetsJson or assetsJson[fromfile]['date_modified'] < os.stat(fromfile).st_mtime):
					file_ext = self.get_str_extension(file); 
					if (file_ext == "ogg"): # resample
						print("resampling audio file from: " + fromfile + " to: " + tofile);
						subprocess.call(["oggdec "+fromfile+" --quiet --output=- | oggenc --raw --quiet --quality=" + str(audio_quality) + " --output="+tofile+" -"], shell=True);
					else: 
						print("copying file from: " + fromfile + " to: " + tofile);
						#subprocess.call(["cp -r " + fromfile + " " + tofile], shell=True);
						shutil.copy2(fromfile, tofile);
						
					assetsJson[fromfile] = {"date_modified": os.stat(fromfile).st_mtime };
					fchanged = True;
					
			if (fchanged == True):
				f = open(assetsCache, "w")
				f.write(json.dumps(assetsJson, sort_keys=True, indent=4));
				f.close();
					
			
			editsSharedLibraries = "";
			for edit_shared_lib in edits_shared_libraries:
				editsSharedLibraries += "System.loadLibrary(\"" + edit_shared_lib + "\");" + nl + nl;
			

			#copy sample game c++/jni files...
			print("generating game jni files");
			print("	generating jni.h");
			editsStrReplace = [
				("%GAME_CLASS_NAME%", self.game_class_name), 
				("%GAME_SHORT_NAME%", self.game_name_safe), 
				("%COMPANY_NAME%", self.developer_name_safe), 
				("%PACKAGE_DOT_NOTATION%", javaPackageName), 
				("%GAME_WIDTH%", str(self.android_config['game_width'])), 
				("%GAME_HEIGHT%", str(self.android_config['game_height'])), 
				("%GAME_ORIENTATION%", self.config['game']['orientation']), 
				("%GAME_CLEAR_COLOR%", self.config['game']['clearcolor']), 
				("%GAME_SHARED_LIBRARIES%", editsSharedLibraries)
			];
			
			# game services bits in java files.
			editsGameServices = [("%GAME_SERVICES_BLOCKSTART%", "/*"), ("%GAME_SERVICES_BLOCKEND%", "*/") ];
			if "game_services" in self.android_config:
				if (self.android_config['game_services']['use']):
					editsGameServices = [("%GAME_SERVICES_BLOCKSTART%", ""), ("%GAME_SERVICES_BLOCKEND%", "") ];

			# stuff to hide from old android
			editsOldAndroid23 = [
				("%ANDROIDSDK16_BLOCKSTART%", "/*"),
				("%ANDROIDSDK16_BLOCKEND%", "*/"),
				("%ANDROIDSDK16_LINECOMMENT%", "//"),
				("%!ANDROIDSDK16_LINECOMMENT%", "")
			];


			if (appplatformno >= 16):
				print("Android sdk version is 16. We can use new Android features.")
				editsOldAndroid23 = [
					("%ANDROIDSDK16_BLOCKSTART%", ""),
					("%ANDROIDSDK16_BLOCKEND%", ""),
					("%ANDROIDSDK16_LINECOMMENT%", ""),
					("%!ANDROIDSDK16_LINECOMMENT%", "//")
				];

			# ouya things
			editsOuya = [("%OUYA_BLOCKSTART%", "/*"), ("%OUYA_BLOCKEND%", "*/"), ("%!OUYA_BLOCKSTART%", ""), ("%!OUYA_BLOCKEND%", ""), ("%OUYA_DEVELOPERID%", ""), ("%OUYA_ENTITLEMENT_ID%", "") ];
			if (self.ouya == True):
				editsOuya = [("%OUYA_BLOCKSTART%", ""), ("%OUYA_BLOCKEND%", ""), ("%!OUYA_BLOCKSTART%", "/*"), ("%!OUYA_BLOCKEND%", "*/"), ("%OUYA_DEVELOPERID%", self.ouya_config['developer_id']), ("%OUYA_ENTITLEMENT_ID%", self.ouya_config['ouya']['entitlement_id']) ];	
				
			editsFireTV = [("%FIRETV_BLOCKSTART%", "/*"), ("%FIRETV_BLOCKEND%", "*/"), ("%!FIRETV_BLOCKSTART%", ""), ("%!FIRETV_BLOCKEND%", "")];
			if (self.firetv == True):
				editsFireTV = [("%FIRETV_BLOCKSTART%", ""), ("%FIRETV_BLOCKEND%", ""), ("%!FIRETV_BLOCKSTART%", "/*"), ("%!FIRETV_BLOCKEND%", "*/")];

			# ouya key.der signature file
			if (self.ouya == True):
				if "key_file" in self.ouya_config:
					actual_key_file = self.str_replace(self.ouya_config['key_file'], [("%PREPRODUCTION_DIR%", self.game_preproduction_dir)]);
					actual_kf_copy_cmd = "cp " + actual_key_file + " " + rootPath+"/build/" + self.output + "/project/res/raw/key.der";
					print("Copying key file");
					print(actual_kf_copy_cmd);
					subprocess.call([actual_kf_copy_cmd], shell=True);
			elif (self.firetv == True):
				#... 
				print("firetv key things");


			f = open(self.ark2d_dir+"/lib/android/jni.h", "r");
			fgamejnih = f.read();
			f.close();
			fgamejnih = self.str_replace(fgamejnih, editsStrReplace);
			f = open(rootPath+"/src/jni.h", "w");
			f.write(fgamejnih);
			f.close();	
			
			print("	generating jni.cpp");
			f = open(self.ark2d_dir+"/lib/android/jni.cpp", "r");
			fgamejnicpp = f.read();
			f.close();
			fgamejnicpp = self.str_replace(fgamejnicpp, editsStrReplace);
			f = open(rootPath+"/src/jni.cpp", "w");
			f.write(fgamejnicpp);
			f.close();	
			
			#build game apk.
			buildline = self.android_ndkdir + "/ndk-build";
			buildline += " NDK_PROJECT_PATH=" + ndkprojectpath;
			buildline += " NDK_APP_OUT=" + appbuilddir;
			buildline += " APP_PROJECT_PATH=" + ndkprojectpath;
			buildline += " APP_BUILD_SCRIPT=" + appbuildscript;
			buildline += " APP_PLATFORM=" + ndkappplatform;

			#nexus-7 fix.
			#application_make_file += "APP_ABI := armeabi armeabi-v7a " + nl; #x86" + nl;

			#buildline += " NDK_LOG=1";
			print("Building game");
			print(buildline);
			subprocess.call([buildline], shell=True);	
			
			#print("Moving output to build folder");
			libsdir = ndkprojectpath + "/libs";
			
			print("removing temp folders");
			self.rmdir_recursive(jnifolder);
			#self.rmdir_recursive(libsdir);

			print("done c++ bits.");
			print("do androidy-javay bits...");
			try:
				print("using custom AndroidManifest.xml");
				androidManifestPath = self.android_config['override_manifest'];
				subprocess.call(['cp ' + androidManifestPath + " " + rootPath+"/build/" + self.output + "/project/AndroidManifest.xml"], shell=True);
				#todo: copy this to rootPath+"/build/android/project/AndroidManifest.xml";
			except:

				minSdkVersion = str(appplatformno);
				targetSdkVersion = str(16);

				if (self.firetv):
					minSdkVersion = str(17);
					targetSdkVersion = str(17);

				print("generating default AndroidManifest.xml");
				androidManifestContents = "";
				androidManifestContents += "<?xml version=\"1.0\" encoding=\"utf-8\"?>" + nl;
				androidManifestContents += "<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"" + nl;
				androidManifestContents += "	package=\"" + javaPackageName + "\" " + nl;
				if (self.android_config['app_to_sd'] == True):
					androidManifestContents += "	android:installLocation=\"preferExternal\"" + nl;
				else:
					androidManifestContents += "	android:installLocation=\"internalOnly\"" + nl;
				androidManifestContents += "	android:versionCode=\"1\" " + nl; 
				androidManifestContents += "	android:versionName=\"1.0\"> " + nl;
				androidManifestContents += "	<uses-sdk android:minSdkVersion=\"" + minSdkVersion + "\" android:targetSdkVersion=\"" + targetSdkVersion + "\"/>" + nl;
				androidManifestContents += "	<uses-feature android:glEsVersion=\"0x00020000\" android:required=\"true\" />" + nl;
				for permission in self.android_config['permissions']:
					androidManifestContents += "	<uses-permission android:name=\"android.permission." + permission + "\" />" + nl;
				androidManifestContents += "	<application" + nl;

				if self.ouya == True or self.firetv == True:
					androidManifestContents += "		android:icon=\"@drawable/app_icon\" " + nl;
					androidManifestContents += "		android:label=\"@string/application_name\"> " + nl;
				else:
					androidManifestContents += "		android:icon=\"@drawable/ic_launcher\" " + nl;
					androidManifestContents += "		android:label=\"@string/application_name\" " + nl;
					androidManifestContents += ">" + nl;
					#androidManifestContents += "		android:debuggable=\"true\"> " + nl;

				#game services
				#if "game_services" in self.android_config:
				#	if (self.android_config['game_services']['use']):
				#		androidManifestContents += "	<meta-data android:name=\"com.google.android.gms.games.APP_ID\" android:value=\"@string/app_id\" />" + nl;
				androidManifestContents += "	<meta-data android:name=\"com.google.android.gms.games.APP_ID\" android:value=\"@string/app_id\" />" + nl;

				if (self.firetv):
					androidManifestContents += "	<activity android:name=\"com.amazon.ags.html5.overlay.GameCircleUserInterface\" android:theme=\"@style/GCOverlay\" android:hardwareAccelerated=\"false\"></activity>" + nl;
					androidManifestContents += "	<activity " + nl;
					androidManifestContents += "		android:name=\"com.amazon.identity.auth.device.authorization.AuthorizationActivity\" " + nl;
					androidManifestContents += "		android:theme=\"@android:style/Theme.NoDisplay\" " + nl;
					androidManifestContents += "		android:allowTaskReparenting=\"true\" " + nl;
					androidManifestContents += "		android:launchMode=\"singleTask\">" + nl; 
					androidManifestContents += "		<intent-filter>" + nl; 
					androidManifestContents += "			<action android:name=\"android.intent.action.VIEW\" /> " + nl;
					androidManifestContents += "			<category android:name=\"android.intent.category.DEFAULT\" /> " + nl;
					androidManifestContents += "			<category android:name=\"android.intent.category.BROWSABLE\" /> " + nl;
					androidManifestContents += "			<data android:host=\"" + javaPackageName + "\" android:scheme=\"amzn\" /> " + nl;
					androidManifestContents += "		</intent-filter> " + nl;
					androidManifestContents += "	</activity> " + nl;
					androidManifestContents += "	<activity android:name=\"com.amazon.ags.html5.overlay.GameCircleAlertUserInterface\" " + nl;
					androidManifestContents += "		android:theme=\"@style/GCAlert\" " + nl;
					androidManifestContents += "		android:hardwareAccelerated=\"false\"></activity> " + nl;
					androidManifestContents += "	<receiver " + nl;
					androidManifestContents += "		android:name=\"com.amazon.identity.auth.device.authorization.PackageIntentReceiver\" " + nl;
					androidManifestContents += "		android:enabled=\"true\"> " + nl;
					androidManifestContents += "		<intent-filter> " + nl;
					androidManifestContents += "			<action android:name=\"android.intent.action.PACKAGE_INSTALL\" /> " + nl;
					androidManifestContents += "			<action android:name=\"android.intent.action.PACKAGE_ADDED\" /> " + nl;
					androidManifestContents += "			<data android:scheme=\"package\" /> " + nl;
					androidManifestContents += "		</intent-filter> " + nl;
					androidManifestContents += "	</receiver>";

				androidManifestContents += "		<activity" + nl;
				androidManifestContents += "			android:name=\"." + self.game_class_name + "Activity\" " + nl;
				androidManifestContents += "			android:label=\"@string/application_name\" " + nl;
				if self.firetv == True:
					androidManifestContents += "			android:configChanges=\"keyboard|keyboardHidden|navigation\" " + nl;
				else:
					androidManifestContents += "			android:configChanges=\"orientation|keyboardHidden\" " + nl;	
				androidManifestContents += "			android:theme=\"@android:style/Theme.NoTitleBar.Fullscreen\"> " + nl;
				androidManifestContents += "			<intent-filter>" + nl;
				androidManifestContents += "				<action android:name=\"android.intent.action.MAIN\" /> " + nl;
				androidManifestContents += "				<category android:name=\"android.intent.category.LAUNCHER\" /> " + nl;

				if self.ouya == True:
					androidManifestContents += "				<category android:name=\"tv.ouya.intent.category.GAME\" /> " + nl;   

				androidManifestContents += "			</intent-filter>" + nl;
				androidManifestContents += "		</activity>" + nl;
				androidManifestContents += "	</application>" + nl;
				androidManifestContents += "</manifest>" + nl;
				f = open(rootPath+"/build/" + self.output + "/project/AndroidManifest.xml", "w");
				f.write(androidManifestContents);
				f.close();
				androidManifestPath = rootPath+"/build/" + self.output + "/project/AndroidManifest.xml";
			
			# ...	 
			ic_launcher_name = "ic_launcher.png";
			if self.ouya == True or self.firetv == True:
				ic_launcher_name = "app_icon.png";

			print("copying icon in to project: ");
			if ("icon" in self.android_config): 
				if not self.android_config['icon']['use_master_icon']:
					icon_xxhdpi = self.android_config['icon']['xxhdpi'];
					icon_xhdpi = self.android_config['icon']['xhdpi'];
					icon_hdpi = self.android_config['icon']['hdpi']; 
					icon_mdpi = self.android_config['icon']['mdpi'];
					icon_nodpi = self.android_config['icon']['nodpi']; 
					
					icon_xxhdpi = self.str_replace(icon_xxhdpi, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir)]);
					icon_xhdpi = self.str_replace(icon_xhdpi, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir)]);
					icon_hdpi = self.str_replace(icon_hdpi, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir)]);
					icon_mdpi = self.str_replace(icon_mdpi, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir)]);
					icon_nodpi = self.str_replace(icon_nodpi, [("%PREPRODUCTION_DIR%", self.game_preproduction_dir)]);

					#subprocess.call(['cp ' + icon_xxhdpi + " " + rootPath+"/build/android/project/res/drawable-xxhdpi/ic_launcher.png"], shell=True);
					#subprocess.call(['cp ' + icon_xhdpi + " " + rootPath+"/build/android/project/res/drawable-xhdpi/ic_launcher.png"], shell=True);
					#subprocess.call(['cp ' + icon_hdpi + " " + rootPath+"/build/android/project/res/drawable-hdpi/ic_launcher.png"], shell=True);
					#subprocess.call(['cp ' + icon_mdpi + " " + rootPath+"/build/android/project/res/drawable-mdpi/ic_launcher.png"], shell=True);
					#subprocess.call(['cp ' + icon_nodpi + " " + rootPath+"/build/android/project/res/drawable/ic_launcher.png"], shell=True);

					shutil.copy2(icon_xxhdpi, rootPath + self.ds + "build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable-xxhdpi" + self.ds + ic_launcher_name);
					shutil.copy2(icon_xhdpi, rootPath + self.ds + "build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable-xhdpi" + self.ds + ic_launcher_name);
					shutil.copy2(icon_hdpi, rootPath + self.ds + "build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable-hdpi" + self.ds + ic_launcher_name);
					shutil.copy2(icon_mdpi, rootPath + self.ds + "build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable-mdpi" + self.ds + ic_launcher_name);
					shutil.copy2(icon_nodpi, rootPath + self.ds + "build" + self.ds + self.output + self.ds + "project" + self.ds + "res" + self.ds + "drawable" + self.ds + ic_launcher_name);

				else:
					if (self.android_config['icon']['icon'] != ''):
						subprocess.call(['cp ' + self.android_config['icon']['icon'] + " " + rootPath+"/build/" + self.output + "/project/res/drawable/" + ic_launcher_name], shell=True);
					#else:
					#	subprocess.call(['cp ' + self.ark2d_dir + "/__preproduction/icon/512.png " + rootPath+"/build/android/project/res/drawable/ic_launcher.png"], shell=True);


			# adding default libs & library projects
			self.android_config['ark_libs'] = [];
			self.android_config['ark_libs'].extend([self.ark2d_dir + "/lib/android/libs/android-support-v4.jar"]);
			self.android_config['ark_libs'].extend([self.ark2d_dir + "/lib/android/libs/libGoogleAnalyticsV2.jar"]);
			
			
			self.android_config['ark_library_projects'] = [];
			temp_library_projects = [];
			temp_library_projects.extend([self.ark2d_dir + "/lib/android/library_projects/play_licensing/library"]);
			temp_library_projects.extend([self.ark2d_dir + "/lib/android/library_projects/play_services"]);

			if (self.firetv == True):
				temp_library_projects.extend([self.ark2d_dir + "/lib/android/library_projects/GameCircleSDK"]);

			# we need the relative path stupidly. :/
			# count number of forward slashes in project path and in ark2d path and then we have 
			#  the difference number of ../s
			#count_slashes_in_ark2d = self.ark2d_dir.count(self.ds);
			#count_slashes_in_project = rootPath.count(self.ds);
			for library_project in temp_library_projects:
				this_path = "../../../" + os.path.relpath(library_project, rootPath);
				print(this_path);
				self.android_config['ark_library_projects'].extend([this_path]);
				#this_library_project = "../../../";
				#for each_slash in range(count_slashes_in_project - count_slashes_in_ark2d)
				#	this_library_project += "../";

			#print("AHAHA");
			#return;
				
			#config['android']['ark_library_projects']


				
			print("------");
			print("Copying libs to project..."); 
			#if ("libs" in self.android_config):
			if (len(self.android_libs) > 0):
				for lib in self.android_libs: #config['libs']:
					libf = lib[lib.rfind(self.ds)+1:len(lib)];
					print("	copying " + lib + " in to project as " + libf + "...");

					#subprocess.call(['cp ' + rootPath + "/" + lib + " " + rootPath+"/build/android/project/libs/" + libf], shell=True);
					shutil.copy2(lib, rootPath + self.ds + "build" + self.ds + self.output + self.ds + "project" + self.ds + "libs" + self.ds + libf);

			print("------");
			print("Copying ark libs to project...")
			for lib in self.android_config['ark_libs']:
				libf = lib[lib.rfind(self.ds)+1:len(lib)];
				print("	copying " + lib + " in to project as " + libf + "...");
				shutil.copy2(lib, rootPath + self.ds + "build" + self.ds + self.output + self.ds + "project" + self.ds + "libs" + self.ds + libf);

			print("------");
			print("generating project.properties");
			projectPropertiesContents = "";

			if (self.firetv):
				projectPropertiesContents += "target=Amazon.com:Amazon Fire TV SDK Addon:17";
			else:
				projectPropertiesContents += "target=" + appplatform;

			library_project_count = 1;

			if len(self.android_libraryprojects) > 0:
				projectPropertiesContents += nl;
				for library_project in self.android_libraryprojects: #android_config['library_projects']:  
					projectPropertiesContents += "android.library.reference." + str(library_project_count) + "=../../../" + library_project;
					projectPropertiesContents += nl;
					library_project_count += 1;

			if "ark_library_projects" in self.android_config:
				projectPropertiesContents += nl;
				for library_project in self.android_config['ark_library_projects']:  
					projectPropertiesContents += "android.library.reference." + str(library_project_count) + "=" + library_project;
					projectPropertiesContents += nl;
					library_project_count += 1;


			f = open(rootPath+"/build/" + self.output + "/project/project.properties", "w");
			f.write(projectPropertiesContents);
			f.close();

			#generate ids.xml
			print("------");
			print("create ids.xml");
			if "override_ids" in self.android_config:
				print("using custom ids.xml");
				androidIdsPath = self.android_config['override_ids'];
				subprocess.call(['cp ' + androidIdsPath + " " + rootPath+"/build/" + self.output + "/project/res/values/ids.xml"], shell=True);
				#todo: copy this to rootPath+"/build/android/project/res/values/ids.xml";
			else:
				print("generating default ids.xml");
			
				androidStringsXmlContents = "";
				androidStringsXmlContents += "<?xml version=\"1.0\" encoding=\"utf-8\"?>" + nl;
				androidStringsXmlContents += "<resources>" + nl;
				androidStringsXmlContents += "	<string name=\"app_id\">";

				didgameservices = False;
				if "game_services" in self.android_config:
					if (self.android_config['game_services']['use']):
						androidStringsXmlContents += str(self.android_config['game_services']['app_id']);
						didgameservices = True;

				if (didgameservices == False):
					androidStringsXmlContents += "1";

				androidStringsXmlContents += "</string>" + nl;
				androidStringsXmlContents += "	<string name=\"achievement_id_test\">1</string>" + nl;
				androidStringsXmlContents += "</resources>" + nl;
				f = open(rootPath+"/build/" + self.output + "/project/res/values/ids.xml", "w");
				f.write(androidStringsXmlContents);
				f.close(); 

			
			#generate strings.xml
			print("------");
			print("create strings.xml");
			if "override_strings" in self.android_config:
				print("using custom strings.xml");
				androidStringsPath = self.android_config['override_strings'];
				subprocess.call(['cp ' + androidStringsPath + " " + rootPath+"/build/" + self.output + "/project/res/values/strings.xml"], shell=True);
				#todo: copy this to rootPath+"/build/android/project/res/values/ids.xml";

			else:
				game_name_forstrings = self.game_name.replace("'", "\\'");
				game_description_forstrings = self.game_description.replace("'", "\\'").replace("-", ":");
				androidStringsXmlContents = "";
				androidStringsXmlContents += "<?xml version=\"1.0\" encoding=\"utf-8\"?>" + nl;
				androidStringsXmlContents += "<resources>" + nl;
				androidStringsXmlContents += "	<string name=\"application_name\">" + game_name_forstrings + "</string>" + nl;
				androidStringsXmlContents += "	<string name=\"application_description\">" + game_description_forstrings + "</string>" + nl;
				androidStringsXmlContents += "	<string name=\"gamehelper_sign_in_failed\">Failed to sign in. Please check your network connection and try again.</string>" + nl;
				androidStringsXmlContents += "	<string name=\"gamehelper_app_misconfigured\">The application is incorrectly configured. Check that the package name and signing certificate match the client ID created in Developer Console. Also, if the application is not yet published, check that the account you are trying to sign in with is listed as a tester account. See logs for more information.</string>" + nl;
				androidStringsXmlContents += "	<string name=\"gamehelper_license_failed\">License check failed.</string>" + nl;
				androidStringsXmlContents += "	<string name=\"gamehelper_unknown_error\">Unknown error.</string>" + nl;
				androidStringsXmlContents += "</resources>" + nl;
				f = open(rootPath+"/build/" + self.output + "/project/res/values/strings.xml", "w");
				f.write(androidStringsXmlContents); 
				f.close(); 
			
			#generate project .classpath file
			print("------");
			print("create project .classpath file");
			androidProjectClasspathContents = "";
			androidProjectClasspathContents += "<?xml version=\"1.0\" encoding=\"UTF-8\"?> " + nl;
			androidProjectClasspathContents += "<classpath> " + nl;
			androidProjectClasspathContents += "	<classpathentry kind=\"src\" path=\"src\"/> " + nl;
			androidProjectClasspathContents += "	<classpathentry kind=\"src\" path=\"gen\"/> " + nl;
			androidProjectClasspathContents += "	<classpathentry kind=\"con\" path=\"com.android.ide.eclipse.adt.ANDROID_FRAMEWORK\"/> " + nl;
			androidProjectClasspathContents += "	<classpathentry exported=\"true\" kind=\"con\" path=\"com.android.ide.eclipse.adt.LIBRARIES\"/> " + nl;
			androidProjectClasspathContents += " 	<classpathentry kind=\"con\" path=\"com.android.ide.eclipse.adt.DEPENDENCIES\" /> ";

			if ("libs" in self.android_config):
				for lib in self.android_libs: #_config['libs']:
					libf = lib[lib.rfind(self.ds)+1:len(lib)];
					androidProjectClasspathContents += "<classpathentry exported=\"true\" kind=\"lib\" path=\"libs/" + libf + "\"/>" + nl;

			if ("ark_libs" in self.android_config):
				for lib in self.android_config['ark_libs']:
					libf = lib[lib.rfind(self.ds)+1:len(lib)];
					androidProjectClasspathContents += "<classpathentry exported=\"true\" kind=\"lib\" path=\"libs/" + libf + "\"/>" + nl;

			# force ouya-sdk lib into classpath
			if self.ouya == True:
				androidProjectClasspathContents += "<classpathentry exported=\"true\" kind=\"lib\" path=\"libs/ouya-sdk.jar\"/>" + nl;

			androidProjectClasspathContents += "	<classpathentry kind=\"output\" path=\"bin/classes\"/> " + nl;
			androidProjectClasspathContents += "</classpath> " + nl;
			f = open(rootPath+"/build/"+ self.output + "/project/.classpath", "w");
			f.write(androidProjectClasspathContents);
			f.close(); 

				
			#copy sample game java files...
			fgamefile = "";
			fgamecontents = "";
			try: 
				print("using custom GameActivity.java");
				fgamefile = self.android_config['override_activity'];

			except:
				print("using default (generated) GameActivity.java");
				fgamefile = self.ark2d_dir + "/lib/android/GameActivity.java";
				pass;

			f = open(fgamefile, "r");
			fgamecontents = f.read(); f.close(); #fgamecontents = fgamecontents.decode("utf8");
			fgamecontents = self.str_replace(fgamecontents, editsStrReplace);
			fgamecontents = self.str_replace(fgamecontents, editsOldAndroid23);
			fgamecontents = self.str_replace(fgamecontents, editsGameServices);
			fgamecontents = self.str_replace(fgamecontents, editsOuya);
			fgamecontents = self.str_replace(fgamecontents, editsFireTV);
			f = open(rootPath+"/build/" + self.output + "/project/src/org/" + self.developer_name_safe + self.ds + self.game_name_safe + self.ds + self.game_class_name + "Activity.java", "w");
			f.write(fgamecontents);
			f.close();		

			# if not overriding 
			if "override_activity" not in self.android_config:
				#ARK Game Helper
				print("using GameHelper.java, BaseGameActivity.java, etc.");

				f = open(self.ark2d_dir + "/lib/android/GameHelper.java", "r");
				fgamecontents = f.read(); f.close();
				fgamecontents = self.str_replace(fgamecontents, editsStrReplace);
				fgamecontents = self.str_replace(fgamecontents, editsOldAndroid23);
				fgamecontents = self.str_replace(fgamecontents, editsGameServices);
				f = open(rootPath+"/build/" + self.output + "/project/src/org/" + self.developer_name_safe + self.ds + self.game_name_safe + self.ds +  "GameHelper.java", "w");
				f.write(fgamecontents);
				f.close();		

				# Google play game.
				f = open(self.ark2d_dir + "/lib/android/BaseGameActivity.java", "r"); 
				fgamecontents = f.read(); f.close();
				fgamecontents = self.str_replace(fgamecontents, editsStrReplace);
				fgamecontents = self.str_replace(fgamecontents, editsOldAndroid23);
				fgamecontents = self.str_replace(fgamecontents, editsGameServices);
				f = open(rootPath+"/build/" + self.output + "/project/src/org/" + self.developer_name_safe + self.ds + self.game_name_safe + self.ds +  "BaseGameActivity.java", "w");
				f.write(fgamecontents);
				f.close();	

				# copy android-support-v4 in.
				f = open(self.ark2d_dir + "/lib/android/libs/android-support-v4.jar", "r"); 
				fgamecontents = f.read(); f.close();
				f = open(rootPath+"/build/" + self.output + "/project/libs/android-support-v4.jar", "w");
				f.write(fgamecontents);
				f.close();	
				

			# additional source files
			print("copying additional source files");
			if "src_files" in self.android_config:
				for src_file in self.android_srcfiles: #config['src_files']:
					f = open(src_file, "r");
					extrasrccontents = f.read();
					f.close();
					extrasrccontents = self.str_replace(extrasrccontents, editsStrReplace);
					extrasrccontents = self.str_replace(extrasrccontents, editsOldAndroid23);


					findex = src_file.rfind('/'); 
					small_src_file = src_file[findex+1:len(src_file)];
					#newf = src_file[0:findex] + ".o";
					#newfd = src_file[0:findex] + ".d";
					
					f = open(rootPath+"/build/" + self.output + "/project/src/org/" + self.developer_name_safe + self.ds + self.game_name_safe + self.ds + small_src_file, "w");
					f.write(extrasrccontents);
					f.close();		

			
			#copying library/s in to project.
			print("Copying ark2d and game.so in to project.");
			ds = self.ds;


			#
			# freetype
			#
			print("copying in freetype library");
			"""
			#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/freetype/obj/local/armeabi-v7a/ " + rootPath+"/build/android/project/libs/armeabi-v7a"], shell=True); #libfreetype
			#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/freetype/obj/local/armeabi-v7a/ " + rootPath+"/build/android/project/obj/local/armeabi-v7a"], shell=True);
			self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"freetype"+ds+"obj"+ds+"local"+ds+"armeabi-v7a", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"armeabi-v7a");
			self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"freetype"+ds+"obj"+ds+"local"+ds+"armeabi-v7a", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"armeabi-v7a");
			if (not self.debug):
				#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/freetype/obj/local/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libfreetype
				#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/freetype/obj/local/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
				self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"freetype"+ds+"obj"+ds+"local"+ds+"armeabi", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"armeabi");
				self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"freetype"+ds+"obj"+ds+"local"+ds+"armeabi", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"armeabi");
			"""


			#
			# openal
			#
			print("copying in openal library");
			#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/openal/libs/armeabi-v7a/ " + rootPath+"/build/android/project/libs/armeabi-v7a"], shell=True); #libfreetype
			#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/openal/libs/armeabi-v7a/ " + rootPath+"/build/android/project/obj/local/armeabi-v7a"], shell=True);
			self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"openal"+ds+"libs"+ds+"armeabi-v7a", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"armeabi-v7a");
			self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"openal"+ds+"libs"+ds+"armeabi-v7a", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"armeabi-v7a");
			if (not self.debug):
				#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/openal/libs/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libfreetype
				#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/openal/libs/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
				self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"openal"+ds+"libs"+ds+"armeabi", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"armeabi");
				self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"openal"+ds+"libs"+ds+"armeabi", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"armeabi");
				self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"openal"+ds+"libs"+ds+"x86", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"x86");
				self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"openal"+ds+"libs"+ds+"x86", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"x86");
			

			#
			# libzip
			# 
			print("copying in libzip library");
			#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/libzip/libs/armeabi-v7a/ " + rootPath+"/build/android/project/libs/armeabi-v7a"], shell=True); #libzip
			#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/libzip/libs/armeabi-v7a/ " + rootPath+"/build/android/project/obj/local/armeabi-v7a"], shell=True);
			self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"libzip"+ds+"libs"+ds+"armeabi-v7a", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"armeabi-v7a");
			self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"libzip"+ds+"libs"+ds+"armeabi-v7a", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"armeabi-v7a");
			if (not self.debug):
				#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/libzip/libs/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libzip
				#subprocess.call(["cp -r " +self.ark2d_dir + "/src/ARK2D/vendor/android/libzip/libs/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
				self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"libzip"+ds+"libs"+ds+"armeabi", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"armeabi");
				self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"libzip"+ds+"libs"+ds+"armeabi", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"armeabi");
				self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"libzip"+ds+"libs"+ds+"x86", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"x86");
				self.mycopytree(self.ark2d_dir+ds+"src"+ds+"ARK2D"+ds+"vendor"+ds+"android"+ds+"libzip"+ds+"libs"+ds+"x86", rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"x86");
			
			# 
			# custom libraries e.g. fmod, spine...  
			# 
			print("copying in custom libraries"); 
			if "native_libraries" in self.android_config:
				for gamelibrary in self.android_config['native_libraries']:
					gamelibraryname = gamelibrary['name'];
					
					print("copying in " + gamelibraryname + " library");
					shutil.copy(self.game_dir+ds+gamelibrary['armeabi-v7a'], rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"armeabi-v7a");
					shutil.copy(self.game_dir+ds+gamelibrary['armeabi-v7a'], rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"armeabi-v7a");
					if (not self.debug and "armeabi" in gamelibrary):
						shutil.copy(self.game_dir+ds+gamelibrary['armeabi'], rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"armeabi");
						shutil.copy(self.game_dir+ds+gamelibrary['armeabi'], rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"armeabi");

					if (not self.debug and "x86" in gamelibrary):
						shutil.copy(self.game_dir+ds+gamelibrary['x86'], rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"libs"+ds+"x86");
						shutil.copy(self.game_dir+ds+gamelibrary['x86'], rootPath+ds+"build"+ds+self.output+ds+"project"+ds+"obj"+ds+"local"+ds+"x86");						
					

			#
			# ark2d
			#
			#subprocess.call(["cp -r " +self.ark2d_dir + "/build/android/libs/armeabi-v7a/ " + rootPath+"/build/android/project/libs/armeabi-v7a"], shell=True); #libark2d
			#subprocess.call(["cp -r " +self.ark2d_dir + "/build/android/libs/armeabi-v7a/ " + rootPath+"/build/android/project/obj/local/armeabi-v7a"], shell=True);
			print("copying in ark2d libraries"); 
			self.mycopytree(self.ark2d_dir+self.ds+"build"+self.ds+"android"+self.ds+"libs"+self.ds+"armeabi-v7a", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"libs"+self.ds+"armeabi-v7a");
			self.mycopytree(self.ark2d_dir+self.ds+"build"+self.ds+"android"+self.ds+"libs"+self.ds+"armeabi-v7a", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"obj"+self.ds+"local"+self.ds+"armeabi-v7a");
			if (not self.debug):
				#subprocess.call(["cp -r " +self.ark2d_dir + "/build/android/libs/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libark2d
				#subprocess.call(["cp -r " +self.ark2d_dir + "/build/android/libs/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
				self.mycopytree(self.ark2d_dir+self.ds+"build"+self.ds+"android"+self.ds+"libs"+self.ds+"armeabi", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"libs"+self.ds+"armeabi");
				self.mycopytree(self.ark2d_dir+self.ds+"build"+self.ds+"android"+self.ds+"libs"+self.ds+"armeabi", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"obj"+self.ds+"local"+self.ds+"armeabi");
				self.mycopytree(self.ark2d_dir+self.ds+"build"+self.ds+"android"+self.ds+"libs"+self.ds+"x86", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"libs"+self.ds+"x86");
				self.mycopytree(self.ark2d_dir+self.ds+"build"+self.ds+"android"+self.ds+"libs"+self.ds+"x86", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"obj"+self.ds+"local"+self.ds+"x86");



			#
			# ark2d (local)
			#
			#subprocess.call(["cp -r " +rootPath + "/build/android/local/armeabi-v7a/ " + rootPath+"/build/android/project/libs/armeabi-v7a"], shell=True); #libgamename
			#subprocess.call(["cp -r " +rootPath + "/build/android/local/armeabi-v7a/ " + rootPath+"/build/android/project/obj/local/armeabi-v7a"], shell=True);
			print("copying in ark2d libraries (local)"); 
			self.mycopytree(rootPath+self.ds+"build"+self.ds+self.output+self.ds+"local"+self.ds+"armeabi-v7a", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"libs"+self.ds+"armeabi-v7a");
			self.mycopytree(rootPath+self.ds+"build"+self.ds+self.output+self.ds+"local"+self.ds+"armeabi-v7a", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"obj"+self.ds+"local"+self.ds+"armeabi-v7a");
			if (not self.debug):
				#	subprocess.call(["cp -r " +rootPath + "/build/android/local/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libgamename
				#	subprocess.call(["cp -r " +rootPath + "/build/android/local/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
				self.mycopytree(rootPath+self.ds+"build"+self.ds+self.output+self.ds+"local"+self.ds+"armeabi", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"libs"+self.ds+"armeabi");
				self.mycopytree(rootPath+self.ds+"build"+self.ds+self.output+self.ds+"local"+self.ds+"armeabi", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"obj"+self.ds+"local"+self.ds+"armeabi");
				self.mycopytree(rootPath+self.ds+"build"+self.ds+self.output+self.ds+"local"+self.ds+"x86", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"libs"+self.ds+"x86");
				self.mycopytree(rootPath+self.ds+"build"+self.ds+self.output+self.ds+"local"+self.ds+"x86", rootPath+self.ds+"build"+self.ds+self.output+self.ds+"project"+self.ds+"obj"+self.ds+"local"+self.ds+"x86");
			
			#
			# copying ark2d resources in to assets folder.
			#
			#subprocess.call(["cp -r " +self.ark2d_dir + "/data/ " + rootPath+"/build/android/project/assets/ark2d"], shell=True);
			print("copying in ark2d resources"); 
			self.mycopytree(self.ark2d_dir + self.ds + "data", rootPath + self.ds + "build"+self.ds+ self.output + self.ds + "project" + self.ds + "assets" + self.ds + "ark2d");

			#print("removing temp libs dir?"); 
			try:
				self.rmdir_recursive(libsdir);  
			except:
				print("could not remove libsdir. this probably means compilation failed.");
				exit(0);
			
		elif (self.building_library):

			f = open(self.ark2d_dir + self.ds + "config.json", "r");
			fcontents = f.read();
			f.close();
			config = json.loads(fcontents); 

			ndkdir = config[self.platformOn]['android']['ndk_dir'];
			
			"""
			#copy stuff in vendor to ndk directory. 
			#freetype
			print("copying vendor headers (freetype)");
			#copyfreetype1 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/freetype/jni/include ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/";
			#copyfreetype2 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/freetype/jni/include ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-x86/usr/";
			#subprocess.call([copyfreetype1], shell=True);
			#subprocess.call([copyfreetype2], shell=True);
			self.mycopytree(ndkprojectpath + self.ds + 'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'android' + self.ds + 'freetype' + self.ds + 'jni' + self.ds + 'include', self.android_ndkdir + self.ds + "platforms"+self.ds + ndkappplatform+self.ds+"arch-arm" + self.ds + "usr" + self.ds + "include");
			self.mycopytree(ndkprojectpath + self.ds + 'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'android' + self.ds + 'freetype' + self.ds + 'jni' + self.ds + 'include', self.android_ndkdir + self.ds + "platforms"+self.ds + ndkappplatform+self.ds+"arch-x86" + self.ds + "usr" + self.ds + "include");
			
			
			
			print("Compiling vendor sources (freetype)");
			libfreetypedir = ndkprojectpath + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "android" + self.ds + "freetype";
			compilefreetype1 = self.android_ndkdir + self.ds + "ndk-build NDK_PROJECT_PATH=" + libfreetypedir +" APP_PROJECT_PATH=" + libfreetypedir + " APP_BUILD_SCRIPT=" + libfreetypedir + self.ds + "jni" + self.ds + "Android.mk APP_PLATFORM=" + ndkappplatform;  
			print(compilefreetype1);
			subprocess.call([compilefreetype1], shell=True);
			#subprocess.call(['cp -r ' + libfreetypedir + "/obj/local/armeabi-v7a/libfreetype.a " + self.android_ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib"], shell=True);
			shutil.copy(libfreetypedir + self.ds + "obj" + self.ds + "local" + self.ds + "armeabi-v7a" + self.ds + "libfreetype.a", self.android_ndkdir + self.ds + "platforms" + self.ds + ndkappplatform+ self.ds + "arch-arm" + self.ds + "usr" + self.ds + "lib" + self.ds);

			#return;
			"""
			
			#openal
			print("copying vendor headers (openal)");
			#copyopenal1 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/openal/jni/include ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/";
			#copyopenal2 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/openal/jni/include ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-x86/usr/";
			#subprocess.call([copyopenal1], shell=True);
			#subprocess.call([copyopenal2], shell=True);
			self.mycopytree(ndkprojectpath + self.ds + 'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'android' + self.ds + 'openal' + self.ds + 'jni' + self.ds + 'include', self.android_ndkdir + self.ds + "platforms"+self.ds + ndkappplatform+self.ds+"arch-arm" + self.ds + "usr" + self.ds + "include");
			self.mycopytree(ndkprojectpath + self.ds + 'src' + self.ds + 'ARK2D' + self.ds + 'vendor' + self.ds + 'android' + self.ds + 'openal' + self.ds + 'jni' + self.ds + 'include', self.android_ndkdir + self.ds + "platforms"+self.ds + ndkappplatform+self.ds+"arch-x86" + self.ds + "usr" + self.ds + "include");
			
			print("Compiling vendor sources (openal)");
			libopenaldir = ndkprojectpath + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "android" + self.ds + "openal";
			compileopenal1 = self.android_ndkdir + self.ds + "ndk-build NDK_PROJECT_PATH=" + libopenaldir +" APP_PROJECT_PATH=" + libopenaldir + " APP_BUILD_SCRIPT=" + libopenaldir + self.ds +"jni" + self.ds + "Android.mk APP_PLATFORM=" + ndkappplatform;  
			print(compileopenal1);
			subprocess.call([compileopenal1], shell=True);
			#subprocess.call(['cp -r ' + libopenaldir + "/libs/armeabi-v7a/libopenal.so " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib"], shell=True);
			#nexus7update subprocess.call(['cp -r ' + libopenaldir + "/libs/armeabi-v7a/libopenal.so " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib"], shell=True);
			shutil.copy(libopenaldir + self.ds + "libs" + self.ds + "armeabi-v7a" + self.ds + "libopenal.so", self.android_ndkdir + self.ds + "platforms" + self.ds + ndkappplatform+ self.ds + "arch-arm" + self.ds + "usr" + self.ds + "lib" + self.ds);
			shutil.copy(libopenaldir + self.ds + "libs" + self.ds + "x86" + self.ds + "libopenal.so", self.android_ndkdir + self.ds + "platforms" + self.ds + ndkappplatform+ self.ds + "arch-x86" + self.ds + "usr" + self.ds + "lib" + self.ds);
			
			#openal
			#print("copying vendor headers (libzip)");
			#print("NOT");
			#copyopenal1 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/libzip/jni/zip.h ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/include/";
			#copyopenal2 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/libzip/jni/zip.h ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-x86/usr/include/";
			#subprocess.call([copyopenal1], shell=True);
			#subprocess.call([copyopenal2], shell=True);
			
			print("Compiling vendor sources (libzip)");
			libzipdir = ndkprojectpath + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "android" + self.ds + "libzip";
			compilelibzip1 = self.android_ndkdir + self.ds + "ndk-build NDK_PROJECT_PATH=" + libzipdir +" APP_PROJECT_PATH=" + libzipdir + " APP_BUILD_SCRIPT=" + libzipdir + self.ds + "jni" + self.ds + "Android.mk APP_PLATFORM=" + ndkappplatform;  
			print(compilelibzip1);
			subprocess.call([compilelibzip1], shell=True);
			#subprocess.call(['cp -r ' + libzipdir + "/libs/armeabi-v7a/libzip.so " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib"], shell=True);
			shutil.copy(libzipdir + self.ds + "libs" + self.ds + "armeabi-v7a" + self.ds + "libzip.so", self.android_ndkdir + self.ds + "platforms" + self.ds + ndkappplatform+ self.ds + "arch-arm" + self.ds + "usr" + self.ds + "lib" + self.ds);
			shutil.copy(libzipdir + self.ds + "libs" + self.ds + "x86" + self.ds + "libzip.so", self.android_ndkdir + self.ds + "platforms" + self.ds + ndkappplatform+ self.ds + "arch-x86" + self.ds + "usr" + self.ds + "lib" + self.ds);
			
			
			#libcurl
			#print("Compiling vendor sources (libcurl)");
			#libcurldir = ndkprojectpath + "/src/ARK2D/vendor/android/curl-android";
			#compilelibcurl1 = ndkdir + "/ndk-build NDK_PROJECT_PATH=" + libcurldir +" APP_PROJECT_PATH=" + libcurldir + " APP_BUILD_SCRIPT=" + libcurldir + "/Android.mk APP_PLATFORM=" + ndkappplatform;  
			#print(compilelibcurl1);
			#subprocess.call([compilelibcurl1], shell=True);
			#subprocess.call(['cp -r ' + libcurldir + "/libs/armeabi/libcurl.so " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib"], shell=True);
			

			nl = "\n";
			mds = "/";
			#make android.mk
			print("Creating Android.mk");
			android_make_file = "";
			android_make_file += "LOCAL_PATH := $(call my-dir)/../../" + nl + nl; 
			android_make_file += "include $(CLEAR_VARS)" + nl+nl;
			android_make_file += "LOCAL_MODULE    := ark2d" + nl+nl; # Here we give our module name and source file(s)
			#android_make_file += "LOCAL_C_INCLUDES := $(LOCAL_PATH)/../libzip/ $(LOCAL_PATH)/../libpng/" + nl;
			android_make_file += "LOCAL_C_INCLUDES := $(LOCAL_PATH)" + mds + "src" + mds + "ARK2D" + mds + "vendor" + mds + "android" + mds + "libzip" + mds + "jni" + mds + " $(LOCAL_PATH)/src/ARK2D/vendor/spine/includes " + nl;
			#android_make_file += "LOCAL_STATIC_LIBRARIES := libzip libpng" + nl;
			#android_make_file += "LOCAL_C_INCLUDES += external/stlport/stlport" + nl+nl;
			android_make_file += "LOCAL_SHARED_LIBRARIES += libstdc++ " + nl+nl;
			
			android_make_file += "LOCAL_CFLAGS := -DARK2D_ANDROID ";
			if (self.platformOn == "windows"):
				android_make_file += "-DARK2D_ANDROID_ON_WINDOWS ";
			elif (self.platformOn == "osx"):
				android_make_file += "-DARK2D_ANDROID_ON_MACINTOSH ";

			if (self.ouya == True):
				android_make_file += "-DARK2D_OUYA ";
			if (self.firetv == True):
				android_make_file += "-DARK2D_AMAZON ";

			android_make_file += "-DDISABLE_IMPORTGL -fno-exceptions -fno-rtti -Wno-psabi ";
			if (self.debug):
				android_make_file += " -DARK2D_DEBUG -DDEBUG -DNDK_DEBUG -O0 ";
			else:
				android_make_file += " -O3 "; #-fno-strict-aliasing -mfpu=vfp -mfloat-abi=softfp ";
			android_make_file += nl+nl;

			android_make_file += "LOCAL_DEFAULT_CPP_EXTENSION := cpp" + nl+nl; 
			android_make_file += "LOCAL_SRC_FILES := \\" + nl;
			for h in self.src_files: #foreach file on config...
				android_make_file += "	" + h + " \\" + nl; 
			android_make_file += nl;
			#android_make_file += "LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz -lfreetype -lopenal -lzip" + nl+nl;
			#android_make_file += "LOCAL_LDLIBS := -lGLESv2 -lGLESv1_CM -ldl -llog -lz -lfreetype -lopenal -lzip" + nl+nl;
			android_make_file += "LOCAL_LDLIBS := -lGLESv2 -lEGL -ldl -llog -lz -lopenal -lzip " + nl+nl; # -lfreetype
			#android_make_file += "LOCAL_SHARED_LIBRARIES :=   " + nl+nl;


			android_make_file += "include $(BUILD_SHARED_LIBRARY)" + nl;
			f = open(appbuildscript, "w");
			f.write(android_make_file);
			f.close();
			
			#make application.mk
			print("Creating Application.mk");
			application_make_file = "";
			application_make_file += "APP_PROJECT_PATH := " + ndkprojectpath + nl;
			application_make_file += "APP_BUILD_SCRIPT := " + appbuildscript + nl;
			application_make_file += "NDK_APP_OUT=" + appbuilddir + nl;
			application_make_file += "NDK_PROJECT_PATH=" + ndkprojectpath + nl;
			

			# nexus7 fixes?! 
			#application_make_file += "APP_ABI := all" + nl;
			if (self.debug):
				application_make_file += "APP_ABI := armeabi-v7a " + nl; #x86" + nl;
			else:
				application_make_file += "APP_ABI := armeabi armeabi-v7a ";
				#if (ndkappplatformno >= )
				application_make_file += "x86 ";
				application_make_file += nl; #x86" + nl;
			


			application_make_file += "APP_STL := stlport_static" + nl; 
			f = open(appbuildscript3, "w");
			f.write(application_make_file);
			f.close();
			
			buildline = ndkdir + self.ds + "ndk-build";
			buildline += " NDK_PROJECT_PATH=" + ndkprojectpath;
			buildline += " NDK_APP_OUT=" + appbuilddir;
			buildline += " APP_PROJECT_PATH=" + ndkprojectpath;
			buildline += " APP_BUILD_SCRIPT=" + appbuildscript;
			buildline += " APP_PLATFORM=" + ndkappplatform;
			#buildline += " NDK_LOG=1";
			print("Building library");
			print(buildline);
			subprocess.call([buildline], shell=True);
			
			print("Moving output to build folder");
			libsdir = ndkprojectpath + self.ds + "libs"; 
			#libsdir2 = ndkprojectpath + self.ds + "libs";
			#subprocess.call(["cp -r " + libsdir + " " + appbuilddir + self.ds ], shell=True);
			print("libsdir: " + libsdir);
			self.mycopytree( libsdir, appbuilddir + self.ds + "libs");
			 
			print("removing temp folders");
			self.rmdir_recursive(libsdir);  
			self.rmdir_recursive(jnifolder);
			
			print("done!");
			
		else:
			pass;



	def mycopytree(self, src, dst, symlinks=False, ignore=None):
		if not os.path.exists(dst):
			os.makedirs(dst)

		for itemm in os.listdir(src):
			s = os.path.join(src, itemm)
			d = os.path.join(dst, itemm)
			if os.path.isdir(s):
				self.mycopytree(s, d, symlinks, ignore)
			else:
				if not os.path.exists(d) or os.stat(src).st_mtime - os.stat(dst).st_mtime > 1:
					shutil.copy2(s, d)


	def fixLocalPath(self, str):
		if (self.platform == "windows" or self.platform == "windows-phone"):
			str = self.str_replace(str, [("/", "\\")]);
			if (not str.startswith("C:\\") and not str.startswith("D:\\")):
				str = self.game_dir + self.ds + str;
		elif (self.platform == "osx" or self.platform == "linux"):
			if (not str.startswith("/") and not str.startswith("../")):
				str = self.game_dir + self.ds + str;
		return str;

		
	def start(self):
	
		if (self.platform == "android" or self.platform == "ouya" or self.platform == "firetv"):
			self.startAndroid();
		elif (self.platform == "ios"): 
			self.startIOS();
		elif (self.platform == "windows-old"):
			self.startWindows();
		elif (self.platform == "windows"):
			self.startWindowsVS2();
		elif (self.platform == "windows-store"):
			self.startWindowsStore();
		elif (self.platform == "windows-phone"):
			self.startWindowsPhone();
		#elif(self.platform == "osx"):
		#	self.startMac();
		elif(self.platform == "ubuntu-linux" or self.platform == "linux"):
			self.startUbuntuLinux();
		elif(self.platform == "osx"):
			self.startXcode();
		elif(self.platform == "flascc"):
			self.startFlascc();
		elif(self.platform == "html5"):
			self.startHTML5();
		else:
			print(self.platform); 
			print("platform " + self.platform + " is not supported yet");

	
	pass;
	
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
#
#
# 	params:
#		NAME 		DESC 												OPTIONS 						REQUIRED
#		type 		building a game or the framework?					library | game 					defaults to "game"
#		target 		the platform to build for.							default | android | ios 		defaults to "default"
#		dir 		directory of the game. must not contain spaces. 	directory						defaults to cwd
#		clean 		clean project or not. 								true | false
#		debug 		debug project or not. 								true | false
#
##	
if __name__ == "__main__":
	
	print("---");
	print("---");
	print("Starting");

	type = "library";
	target = "default";
	dir = sys.path[0];
	clean = False;
	debug = False; 
	onlyspritesheets = False;
	onlyassets = False;
	newconfig = False;

	count = 0;
	print("args: " + str(len(sys.argv)));
	for item in sys.argv:
		print("lol item: " + item);
		if count == 0:
			count += 1;
			continue;

		parts = item.split("=");
		if len(parts) > 2:
			print("item " + item + " is invalid");
			continue;
		if parts[0] == "type":
			type = parts[1];
		elif parts[0] == "target":
			target = parts[1];
		elif parts[0] == "dir":
			dir = parts[1];
		elif parts[0] == "debug":
			if (parts[1]=="true"):
				debug = True;
			else:
				debug = False
		elif parts[0] == "clean":
			if (parts[1]=="true"):
				clean = True;
		elif parts[0] == "spritesheets":
			if (parts[1]=="true"):
				onlyspritesheets = True;
			else:
				onlyspritesheets = False;
			
		elif parts[0] == "assets":
			if (parts[1]=="true"):
				onlyassets = True;
			else:
				onlyassets = False;

		elif parts[0] == "newconfig":
			if(parts[1] == "true"):
				newconfig = True;
			else: 
				newconfig = False;

		count += 1;
		
	print("---");
	print("type: " + type);
	print("target: " + target);
	print("dir: " + dir);
	print("debug: " + str(debug));
	print("clean: " + str(clean));
	print("newconfig: " + str(newconfig));
	
	if (sys.platform == "win32"):
		arkPlatform = "windows";
	elif(sys.platform == "darwin"):
		arkPlatform = "osx";
	elif(sys.platform == "linux2"):
		arkPlatform = "linux";
	else: 
		print("platform " + sys.platform + " not supported");
		exit(0);

	# have to read json and override some values for back-compatibility.
	if (newconfig and type == "game"):
		try:
			dir = os.getcwd();
			ark2d_dir = os.path.dirname(os.path.realpath(__file__));
			print("---");
			print("current file:" + ark2d_dir);
			print("current working directory:" + dir);

			print("---");
			print("Opening ark2d config file: ");
			f = open(ark2d_dir + "/config.json");
			fcontents = f.read();
			f.close();
			ark_config = json.loads(fcontents);
			
			print("---");
			print("Opening game config file: ");
			print(dir + "/configs/game.json");

			f = open(dir + "/configs/game.json");
			fcontents = f.read();
			f.close();
			game_config = json.loads(fcontents);

			print("---");
			print("Opening target config file: ");
			print(dir + "/configs/" + target);

			f = open(dir + "/configs/" + target);
			fcontents = f.read();
			f.close();
			target_config = json.loads(fcontents);

			#print(game_config);
			#print(target_config);
			#print(game_config['game']['name']);
			#if (target_config["platform"] == "windows"):
			
			a = ARK2DBuildSystem();
			a.newconfig = True;
			a.debug = debug;
			a.platform = target_config['platform'];
			a.output = target_config['folder'];

			a.ark2d_dir = ark2d_dir;
			a.game_dir = dir;
			a.game_name = game_config['game']['name'];
			a.game_name_safe = game_config['game']['name_safe'];
			#a.game_short_name = game_config['game']['class_name'];
			a.game_class_name = game_config['game']['class_name'];
			a.game_description = game_config['game']['description'];
			a.game_resources_dir = a.game_dir + a.ds + game_config['game']['folders']['resources'];
			a.game_preproduction_dir = a.game_dir + a.ds + game_config['game']['folders']['preproduction'];
			a.game_clear_color = game_config['game']['clearcolor'];
			a.game_orientation = game_config['game']['orientation'];
			a.developer_name = game_config['developer']['name'];
			a.developer_name_safe = game_config['developer']['name_safe'];


			a.src_files.extend(game_config["src_files"]);
			a.game_mkdirs = game_config['mkdirs'];
			a.build_artifact = "";
			a.config = game_config;
			a.ark_config = ark_config
			a.game_config = game_config;
			a.target_config = target_config;

			if (clean == True or target_config['clean'] == True):
				a.clean();
				#exit(0);
			
			a.gamePreInit();
			
			a.mingw_link = "";

			# Add libs to ark2d build system
			if ("libs" in target_config):
				for lib in target_config['libs']:
					lib2 = a.str_replace(lib, [("%PREPRODUCTION_DIR%", a.game_preproduction_dir), ("%ARK2D_DIR%", a.ark2d_dir)]);
					lib2 = a.fixLocalPath(lib2);
					a.libs.extend([lib2]);

			if ("include_dirs" in game_config):
				a.include_dirs.extend(game_config['include_dirs']);

			if ("include_dirs" in target_config):
				for idir in target_config['include_dirs']:
					idir = a.str_replace(idir, [("%PREPRODUCTION_DIR%", a.game_preproduction_dir), ("%ARK2D_DIR%", a.ark2d_dir)]);
					idir = a.fixLocalPath(idir);
					a.include_dirs.extend([idir]);

			if ("defines" in game_config):
				a.preprocessor_definitions.extend(game_config['defines']);

			if ("defines" in target_config):
				a.preprocessor_definitions.extend(target_config['defines']);

			if (a.platform == "firetv"):
				a.firetv = True;
			
			if (a.platform == "ouya"):
				a.ouya = True;
				a.ouya_config = game_config['ouya'];

			if (a.platform == "ios" or a.platform == "iphone" or a.platform == "ipad"):
				a.ios_config = target_config['ios'];

			if (a.platform == "android" or a.platform == "ouya" or a.platform == "firetv"):
				print("Opening android.json");
				f = open(dir + "/configs/android.json");
				fcontents = f.read();
				f.close();
				a.android_config = json.loads(fcontents)['android'];

				a.android_sdkdir = a.android_config['sdk_dir'];
				a.android_ndkdir = a.android_config['ndk_dir'];
				a.android_srcfiles = [];
				a.android_libs = [];
				a.android_libraryprojects = [];

				if "src_files" in a.android_config:
					for src in a.android_config['src_files']:
						src_actual = a.str_replace(src, [("%PREPRODUCTION_DIR%", a.game_preproduction_dir), ("%ARK2D_DIR%", a.ark2d_dir)]);
						a.android_srcfiles.extend([src_actual]);

				if "libs" in a.android_config:
					for lib in a.android_config['libs']:
						lib_actual = a.str_replace(lib, [("%PREPRODUCTION_DIR%", a.game_preproduction_dir), ("%ARK2D_DIR%", a.ark2d_dir)]);
						a.android_libs.extend([lib_actual]);

				# library projects 
				if "library_projects" in a.android_config:
					for libproj in a.android_config['library_projects']:
						libproj_actual = a.str_replace(libproj, [("%PREPRODUCTION_DIR%", a.game_preproduction_dir), ("%ARK2D_DIR%", a.ark2d_dir)]);
						a.android_libraryprojects.extend([libproj_actual]);

				if "android" in target_config and "library_projects" in target_config['android']:
					for libproj in target_config['android']['library_projects']:
						print("library project: " + libproj);
						libproj_actual = a.str_replace(libproj, [("%PREPRODUCTION_DIR%", a.game_preproduction_dir), ("%ARK2D_DIR%", a.ark2d_dir)]);
						a.android_libraryprojects.extend([libproj_actual]); 
				
				if "override_activity" in a.android_config:
					a.android_config["override_activity"] = a.str_replace(a.android_config["override_activity"], [("%PREPRODUCTION_DIR%", a.game_preproduction_dir), ("%ARK2D_DIR%", a.ark2d_dir)]);

				if "override_manifest" in a.android_config:
					a.android_config["override_manifest"] = a.str_replace(a.android_config["override_manifest"], [("%PREPRODUCTION_DIR%", a.game_preproduction_dir), ("%ARK2D_DIR%", a.ark2d_dir)]);

				if "override_ids" in a.android_config:
					a.android_config["override_ids"] = a.str_replace(a.android_config["override_ids"], [("%PREPRODUCTION_DIR%", a.game_preproduction_dir), ("%ARK2D_DIR%", a.ark2d_dir)]);

				if "override_strings" in a.android_config:
					a.android_config["override_strings"] = a.str_replace(a.android_config["override_strings"], [("%PREPRODUCTION_DIR%", a.game_preproduction_dir), ("%ARK2D_DIR%", a.ark2d_dir)]);

			# Windows Phone 8 config.
			if (a.platform == "windows-phone"):
				print("Opening windows-phone.json");
				f = open(dir + "/configs/windows-phone.json");
				fcontents = f.read();
				f.close();
				a.wp8_config = json.loads(fcontents)['windows-phone'];

			
			"""if(sys.platform=="win32"):
				a.dll_files.append(a.ark2d_dir + a.ds + a.build_folder + a.ds + a.platform + a.ds + 'libARK2D.dll');
				a.linkingFlags += "-mwindows ";
				#a.linkingFlags += "-static-libgcc -static-libstdc++ "; 
				a.linkingFlags += "-enable-auto-import ";
			elif(sys.platform=="darwin"):
				a.dll_files.append(a.ark2d_dir + a.ds + a.build_folder + a.ds + a.platform + a.ds + 'libARK2D.dylib');
				
				if ('mac_game_icns' in config[arkPlatform]):
					a.mac_game_icns = config[arkPlatform]['mac_game_icns']; 
					
			
			if ('game_resources_dir' in config[arkPlatform]):
				a.game_resources_dir = config[arkPlatform]['game_resources_dir'];
			"""		

			
			
			a.gamePostInit();

			if (onlyspritesheets):
				a.generateSpriteSheets();
				exit(0);

			if (onlyassets):
				a.processAssets();
				exit(0);

			a.start();


			print("---");
			print("Done");
			print("---");

		except Exception as exc: 
			print("configs/" + target + " is invalid or does not exist.");
			print(exc);
			exit(1);
		except:
			print("configs/" + target + " is invalid or does not exist.");
			#print(e);
			exit(1);

		exit(0);
		


	if (type == "library"):

		print("---");
		print("Building Library");
		ark2d_dir = os.path.dirname(os.path.realpath(__file__));

		a = ARK2DBuildSystem();
		a.debug = debug;
		a.ark2d_dir = ark2d_dir;

		if (target=='iphone' or target=="ios"):
			a.platform = 'ios';
		elif (target=='android'):
			a.platform = 'android';
		elif (target=='ouya'):
			a.platform = 'android';
			a.ouya = True;
		elif (target=='flascc'):
			a.platform = 'flascc';
		elif (target=='xcode' or target == "osx"):
			a.platform = 'osx';
		elif (target=='windows' or target =="windows-vs"):
			a.platform = 'windows';
		elif (target=='windows-phone'):
			a.platform = 'windows-phone';
		elif (target=='windows-store'):
			a.platform = 'windows-store';
		elif (target=='linux' or target=='ubuntu-linux'):
			a.platform = 'linux';
		elif (target=='html5' or target=='emscripten'): 
			a.platform = 'html5';

		a.output = a.platform;

		print("---");
		print("Opening config file: " + dir + "/config.json");
		f = open(dir + "/config.json", "r");
		fcontents = f.read();
		f.close();
		config = False; 
		
		try:
			a.config = json.loads(fcontents);
			a.android_config = a.config['android'];
			a.android_sdkdir = a.config[a.platformOn]['android']['sdk_dir'];
			a.android_ndkdir = a.config[a.platformOn]['android']['ndk_dir'];
		except:
			print("Config.json invalid.");
			exit(1);

		a.dllInit();

		if (clean):
			a.clean();
			exit(0);

		a.start();	

	else:

		print("---");		
		print("Must use new configs for game building? ");
		exit(0);

		print("---");
		print("Building game");

		print("---");
		print("Opening config file: " + dir + "/config.json");
		f = open(dir + "/config.json", "r");
		fcontents = f.read();
		f.close();
		
		#print(fcontents);
		config = False; 
		
		try:
			config = json.loads(fcontents);
			blah = config["game_name"];

			if (config == False):
				print("Config.json invalid or trying to build game from framework path.");
				exit(1);

			a = ARK2DBuildSystem();
			a.debug = debug;

			if (target=='iphone' or target=="ios"):
				a.platform = 'ios';
			elif (target=='android'):
				a.platform = 'android';
			elif (target=='ouya'): 
				a.platform = 'android';
				a.ouya = True;
			elif (target=='flascc'):
				a.platform = 'flascc';
			elif (target=='xcode' or target=='osx'):
				a.platform = 'osx';
			elif (target=='windows-vs'):
				a.platform = 'windows';
			elif (target=='windows-phone'):
				a.platform = 'windows-phone';
			elif (target=='ubuntu-linux'):
				a.platform = 'ubuntu-linux';
			elif (target=='html5' or target=='emscripten'):
				a.platform = 'html5';

			a.output = a.platform;

			a.ark2d_dir = config[arkPlatform]["ark2d_dir"];
			a.game_name = config["game_name"];
			a.game_short_name = config['game_short_name'];
			a.game_dir  = config[arkPlatform]["game_dir"];
			a.src_files.extend(config["game_src_files"]);
			a.game_mkdirs = config['game_mkdirs'];
			a.build_artifact = "";
			a.config = config;

			if ("libs" in config[arkPlatform]):
				a.libs.extend(config[arkPlatform]["libs"]);
			if ("include_dirs" in config[arkPlatform]):
				a.include_dirs.extend(config[arkPlatform]['include_dirs']);
			
			if (clean == True):
				a.clean();
				#exit(0);
			
			a.gamePreInit();
			
			a.mingw_link = "";
			
			if(sys.platform=="win32"):
				a.dll_files.append(a.ark2d_dir + a.ds + a.build_folder + a.ds + a.platform + a.ds + 'libARK2D.dll');
				a.linkingFlags += "-mwindows ";
				#a.linkingFlags += "-static-libgcc -static-libstdc++ "; 
				a.linkingFlags += "-enable-auto-import ";
			elif(sys.platform=="darwin"):
				a.dll_files.append(a.ark2d_dir + a.ds + a.build_folder + a.ds + a.platform + a.ds + 'libARK2D.dylib');
				
				if ('mac_game_icns' in config[arkPlatform]):
					a.mac_game_icns = config[arkPlatform]['mac_game_icns']; 
					
			
			if ('game_resources_dir' in config[arkPlatform]):
				a.game_resources_dir = config[arkPlatform]['game_resources_dir'];
					
			
			a.gamePostInit();

			if (onlyspritesheets):
				a.generateSpriteSheets();
				exit(0);

			if (onlyassets):
				a.processAssets();
				exit(0);

			a.start();

		except Exception as exc: 
			print(exc);
			print("Config.json invalid or trying to build game from framework path.");
			exit(1);
		except:
			print("Done");
			pass;

		
		
		

		
