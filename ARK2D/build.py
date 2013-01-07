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
# Mac requires you do not install OpenAL from their website 
#  - i.e. you should not have Library/Frameworks/OpenAL.framework
#
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
		#self.mac_game_icns = "";

		self.build_folder = "build";
		self.arch = platform.machine();
		
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
			self.platform = "iphone";
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
			return "iphone";
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
			self.build_folder + self.ds + self.platform,
			self.build_folder + self.ds + self.platform + self.ds + "build-cache", # cache folder
			self.build_folder + self.ds + self.platform + self.ds + "src",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D", 
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Audio",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Controls",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Core",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Core" + self.ds + "Platform",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Font",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Geometry",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "GJ",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Graphics",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Graphics" + self.ds + "ImageIO",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Particles",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Path",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Pathfinding",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "State",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "State" + self.ds + "Transition",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tests",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Threading",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tiled",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tools",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tween",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "UI",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Util",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Util" + self.ds + "Containers",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "libJSON",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "lpng151",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "ogg130",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "tinyxml",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "vorbis132",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "vorbis132" + self.ds + "modes",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "zlib123"
		]);
		
		
		#self.src_files.extend(config['src_files']['all']);
		
		if (self.platform == "flascc"):
			self.src_files.extend(config['src_files']['flascc']);
			self.ark2d_dir = config[self.platformOn]['ark2d_dir'];

		elif (self.platform == "android"):
			self.src_files.extend(config['src_files']['android']);
			self.ark2d_dir = config[self.platformOn]['ark2d_dir'];

		elif (self.platform == "iphone"):
			self.src_files.extend(config['src_files']['iphone']);
			self.ark2d_dir = config["osx"]['ark2d_dir'];

			self.static_libraries.extend(config['static_libraries']['iphone']);
		elif (self.platform == "windows"):
		
			self.src_files.extend(config['src_files']['windows']);
			self.dll_files.extend(config['dynamic_libraries']['windows']);
			self.static_libraries.extend(config['static_libraries']['windows']);
			self.linkingFlags = " -mwindows -shared ";
			
			self.ark2d_dir = config["windows"]['ark2d_dir'];
			self.ark2d_tmpdir = "C:\\ark2d\\l";
				
		elif (self.platform == "osx"):
			
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
			self.build_folder + self.ds + self.platform,
			self.build_folder + self.ds + self.platform + self.ds + "src",
			self.build_folder + self.ds + self.platform + self.ds + "build-cache" # cache folder
			
		]);
		self.mkdirs.extend(self.game_mkdirs);
		
		if (self.platform == "windows"):
			self.ark2d_tmpdir = "C:\\ark2d\\gm";
		else:
			self.ark2d_tmpdir = "/ark2d/gm";
		
	def gamePostInit(self):
	
		if (sys.platform == "win32"):
			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + self.game_name.replace(" ", "_") + ".exe";
		elif(sys.platform == "darwin"):
			self.build_artifact = self.build_folder + self.ds + self.game_name.replace(" ", "_");

		# open config
		f = open(self.game_dir + "/config.json", "r");
		fcontents = f.read();
		f.close();
		config = json.loads(fcontents);

		#create ARK.h file
		nl = "\r\n";
		arkh  = "";
		arkh += "#ifdef ARK2D_WINDOWS" + nl;
		arkh += "	#include \"" + self.str_replace(config["windows"]['ark2d_dir'], [("\\", "\\\\")]) + "\\src\\ARK.h\"" + nl;
		arkh += "#else" + nl;
		arkh += "	#include \"" + config["osx"]['ark2d_dir'] + "/src/ARK.h\"" + nl;
		arkh += "#endif" + nl;
		f = open(self.game_dir + "/src/ARK.h", "w");
		f.write(arkh);
		f.close();
			
			
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
							compileStr += " -mmacosx-version-min=10.6 -DARK2D_MACINTOSH -DMAC_OS_X_VERSION_MIN_REQUIRED=1060 -x objective-c++  ";

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
					linkingStr += " " + self.game_dir + self.ds + f;
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
				infoplistcontents += "		<string>" + self.config['game_name'] + ". Copyright " + self.config['company_name'] + "</string>" + cr;
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
		flascc_dir = "/Users/ashleygwinnell/flascc/sdk";
		flexsdk_dir = "/Users/ashleygwinnell/flex_sdk";
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

			directoriesToCreate = self.listDirectories(game_dir+"/data", False);
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
			game_resources_dir = game_dir + "/data";
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
			editsStrReplace = [("%GAME_NAME%", config['game_name']), ("%GAME_DESCRIPTION%", config['game_description']), ("%GAME_WIDTH%", str(game_width)), ("%GAME_HEIGHT%", str(game_height)), ("%COMPANY_NAME%", config['company_name']) ];
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
	java -jar $(FLASCC)/usr/lib/asc2.jar -merge -md -AS3 -strict -optimize \
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
			#linkingStr += " -pthread ";
			#-emit-swf=org.ashleygwinnell.defaultgame



			linkingStr += " $(FLASCC)/usr/lib/AlcVFSZip.abc $(GAMEDIR)/build/flascc/gamevfs.abc -swf-preloader=$(GAMEDIR)/build/flascc/PreLoader.swf -swf-version=18 -symbol-abc=$(GAMEDIR)/build/flascc/Console.abc -jvmopt=-Xmx4G ";
			linkingStr += " -swf-size="+str(game_width)+"x"+str(game_height)+" -emit-swf -o $(GAMEDIR)/build/flascc/game.swf -j8 ";
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
		if (sys.platform == "darwin"):
			cm = "rm -r -d " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform;
		elif(sys.platform == "win32"):
			cm = "rmdir /S /Q " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform;
		
		#subprocess.call([], shell=True);
		print(cm);
		try:
			buildresp = os.system(cm);
		except:
			print("could not remove dir");





	def generateSpriteSheets(self):
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
					spritesheet['game_preproduction_dir'] = self.config[self.platformOn]['game_preproduction_dir'];

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
						#print(compileLine);
						subprocess.call([compileLine], shell=True); # player 
						
					#redocache
					cacheChanged = True;
					cacheJSON[spritesheetName] = {};
					for imageFile in spritesheet["files"]:
						cacheJSON[spritesheetName][imageFile] = {"date_modified": os.stat(self.game_dir + self.ds + imageFile).st_mtime };

				pass;

			if (cacheChanged == True):
				self.saveCache("spritesheets", cacheJSON);
			
		pass;

	def openCache(self, filename):
		cachefilename = "";		
		cachefilename += self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "build-cache" + self.ds  + filename + ".json";
		self.createCacheFile(cachefilename);
		f = open(cachefilename, "r")
		fcontents = f.read();
		f.close();
		fjson = json.loads(fcontents);
		return fjson;

	def saveCache(self, filename, data):
		f = open(self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "build-cache" + self.ds + filename + ".json", "w");
		f.write(json.dumps(data, sort_keys=True, indent=4));
		f.close();
		return;
	
	def startIPhone(self):	
		print("Building iPhone");

		# open config
		f = open(self.game_dir + "/config.json", "r");
		fcontents = f.read();
		f.close();
		config = json.loads(fcontents);


		gyp_executable = "/Users/ashleygwinnell/Documents/gyp-read-only/gyp";

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

			for srcfile in config['src_files']['iphone']:
				gypfiletarget['sources'].extend(["../../"+srcfile]);

			for srcfile in config['src_files']['all']:
				gypfiletarget['sources'].extend(["../../"+srcfile]);

			gypfiletarget['sources!'] = [];
			gypfiletarget['dependencies'] = [];
			gypfiletarget['conditions'] = [];
			gypfiletargetcondition = {};
			gypfiletargetcondition['defines'] = ['ARK2D_IPHONE']; #, 'CF_EXCLUDE_CSTD_HEADERS'];
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
	        	'$(SDKROOT)/System/Library/Frameworks/ApplicationServices.framework'];
			gypfiletargetcondition['include_dirs'] = ['../../src/ARK2D/vendor/iphone'];
			
			gypfiletargetcondition['xcode_settings'] = {};
			gypfiletargetcondition['xcode_settings']['ARCHS'] = "armv6 armv7";
			gypfiletargetcondition['xcode_settings']['IPHONEOS_DEPLOYMENT_TARGET'] = '4.2';
			gypfiletargetcondition['xcode_settings']['SDKROOT'] = "iphoneos";
			gypfiletargetcondition['xcode_settings']['TARGETED_DEVICE_FAMILY'] = "1,2";
			gypfiletargetcondition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] = "ARK2D_IPHONE";
			gypfiletargetcondition['xcode_settings']['GCC_OPTIMIZATION_LEVEL'] = "0";

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
				os.system("rm -r -d " + self.build_folder + self.ds + self.platform + self.ds + "XcodeData");
			except:
				print("could not delete xcode project");

			# run gyp file.
			os.system("python " + gyp_executable + " " + gypfilename + " --depth=../../src");
			
			# add precompiled headers file
			print("generating pch file");
			
			nl = "\r\n";
			pcheaderfile = "";
			#pcheaderfile += "#ifdef __OBJC__" + nl;
			pcheaderfile += "	#import <Foundation/Foundation.h>" + nl;
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

			#mkdirs
			try:
				os.makedirs(self.game_dir + self.ds + self.build_folder + self.ds + self.platform);
			except OSError as exc: 
				pass;

			#projectname ark2d
			projectname = config['game_short_name'];

			# generate gyp file.
			print("creating gyp file");
			gypfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + projectname + ".gyp";

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

			for srcfile in config['game_src_files']:
				gypfiletarget['sources'].extend(["../../"+srcfile]);

			gypfiletarget['sources!'] = [];
			gypfiletarget['dependencies'] = [];
			gypfiletarget['conditions'] = [];
			gypfiletargetcondition = {};
			gypfiletargetcondition['defines'] = ['ARK2D_IPHONE']; #, 'CF_EXCLUDE_CSTD_HEADERS'];
			gypfiletargetcondition['sources'] = [];
			gypfiletargetcondition['sources!'] = [];
			gypfiletargetcondition['link_settings'] = {};
			gypfiletargetcondition['link_settings']['libraries'] = [
				'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/CoreFoundation.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/CoreGraphics.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/UIKit.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/Foundation.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/QuartzCore.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/OpenGLES.framework',
	          	'/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS4.3.sdk/System/Library/Frameworks/OpenAL.framework',
	          	#'../../lib/iphone/libfreetype.a'
	          	config['osx']['ark2d_dir'] + '/lib/iphone/libfreetype.a',
	          	config['osx']['ark2d_dir'] + '/lib/osx/libcurl.a',
	          	
	          	config['osx']['ark2d_dir'] + '/build/iphone/XcodeData/ark2d/Build/Products/Default-iphonesimulator/libark2d-iPhone.a'
					              ];
					              #'../../lib/iphone/libfreetype.a
			

			gypfiletargetcondition['ldflags'] = [ ];

			gypfiletargetcondition['link_settings!'] = [];

			ark2ddir = config['osx']['ark2d_dir'] + "/src/ARK2D";
			gypfiletargetcondition['include_dirs'] = [ 
				ark2ddir, 
				ark2ddir + '/vendor/iphone' 
			];
			
			gypfiletargetcondition['xcode_settings'] = {};
			gypfiletargetcondition['xcode_settings']['ARCHS'] = "armv6 armv7";
			gypfiletargetcondition['xcode_settings']['IPHONEOS_DEPLOYMENT_TARGET'] = '4.2';
			gypfiletargetcondition['xcode_settings']['SDKROOT'] = "iphoneos";
			gypfiletargetcondition['xcode_settings']['TARGETED_DEVICE_FAMILY'] = "1,2";
			gypfiletargetcondition['xcode_settings']['GCC_PREPROCESSOR_DEFINITIONS'] = "ARK2D_IPHONE";
			gypfiletargetcondition['xcode_settings']['GCC_OPTIMIZATION_LEVEL'] = "0";

			
			xcconfigfilesimple = projectname + ".xcconfig";
			xcconfigfile = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + xcconfigfilesimple;

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

				"iTunesArtwork", 		#app-store-icon
				"iTunesArtwork@2x", 	#app-store-icon-retina
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
			pchfilename = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + projectname + "-Prefix.pch";
			info_plist_filename = self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + projectname + "-Info.plist";
			try:
				print("deleting xcode project");
				os.system("rm -r -d " + pchfilename);
				os.system("rm -r -d " + info_plist_filename);
				os.system("rm -r -d " + xcconfigfile);
				os.system("rm -r -d " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "XcodeData");
			except:
				print("could not delete xcode project");

			# run gyp file.
			os.system("python " + gyp_executable + " " + gypfilename + " --depth=../../src");
			
			# add precompiled headers file
			print("generating pch file");
			
			nl = "\r\n";
			pcheaderfile = "";
			#pcheaderfile += "#ifdef __OBJC__" + nl;
			pcheaderfile += "	#import <Foundation/Foundation.h>" + nl;
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
			info_plist_contents += '	<key>CFBundleDevelopmentRegion</key>' + nl;
			info_plist_contents += '	<string>en</string>' + nl;
			info_plist_contents += '	<key>CFBundleDisplayName</key>' + nl;
			info_plist_contents += '	<string>' + config['game_name'] + '</string>' + nl;
			info_plist_contents += '	<key>CFBundleExecutable</key>' + nl;
			info_plist_contents += '	<string>${EXECUTABLE_NAME}</string>' + nl;

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
			info_plist_contents += '			</array>' + nl;
			info_plist_contents += '		</dict>' + nl;
			info_plist_contents += '	</dict>' + nl;

			info_plist_contents += '	<key>CFBundleIconFile</key>' + nl;
			info_plist_contents += '	<string></string>' + nl;
			info_plist_contents += '	<key>CFBundleIdentifier</key>' + nl;
			info_plist_contents += '	<string>org.ashleygwinnell.' + projectname + '</string>' + nl;
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
			info_plist_contents += '	<key>UIStatusBarHidden</key>' + nl;
			info_plist_contents += '	<true/>' + nl;
			info_plist_contents += '	<key>UISupportedInterfaceOrientations</key>' + nl;
			info_plist_contents += '	<array>' + nl;
			if (config['game_orientation'] == "portrait"):
				info_plist_contents += '	<string>UIInterfaceOrientationPortrait</string>' + nl;
			elif(config['game_orientation'] == "landscape"):
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

			#copy ark2d resources in to project data folder.
			subprocess.call(["cp -r " + config['osx']['game_resources_dir'] + " " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "data/"], shell=True); #libark2d
			subprocess.call(["cp -r " + config['osx']['ark2d_dir'] + "/data " + self.game_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + "data/ark2d/"], shell=True); #libark2d
			
			#generate icons
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


			
			exit(0);
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
		f = open(self.game_dir + "/config.json", "r");
		fcontents = f.read();
		f.close();
		config = json.loads(fcontents);
		
		# define vars.	
		nl = "\r\n";
		
		#sdk version, not NDK.
		appplatformno = config['android']['sdk_version'];
		appplatform= "android-" + str(appplatformno);
		ndkappplatformno = config['android']['ndk_version'];
		ndkappplatform = "android-" + str(ndkappplatformno);
		
		
		ndkdir = config['osx']['android']['ndk_dir'];
		
		if (self.building_game):
			#game specific vars
			game_name = config['game_name'];
			game_name_safe = config['game_name_safe'];
			game_short_name = config['game_short_name'];
			game_description = config['game_description'];
			game_resources_dir = config['osx']['game_resources_dir'];
			company_name = config['company_name'];
			company_name_safe = config['company_name_safe'];
			javaPackageName = "org."+company_name_safe+"."+game_short_name;
			
			audio_quality = config['android']['audio_quality'];
			
			ark2ddir = config['osx']['ark2d_dir'];
			rootPath = config['osx']['game_dir'];
			ndkprojectpath = rootPath;
			thisCreateDirs = [rootPath + "/build"];
			thisCreateDirs.extend([rootPath+"/build/android/build-cache"]);
			thisCreateDirs.extend([rootPath+"/build/android/project"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/assets"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/assets/ark2d"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/src"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/gen"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/libs"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/libs/armeabi"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/obj"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/obj/local"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/obj/local/armeabi"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/res"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/res/drawable"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/res/values"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/src/org"]);
			thisCreateDirs.extend([rootPath+"/build/android/project/src/org/" + company_name_safe]);
			thisCreateDirs.extend([rootPath+"/build/android/project/src/org/" + company_name_safe + self.ds + game_short_name]);
			pass;
		else:
			rootPath = config['osx']['ark2d_dir'];
			ndkprojectpath = rootPath;
			thisCreateDirs = [ndkprojectpath + "/build"];
			pass;
		
		appbuilddir = rootPath+"/build/android";
		appbuildscript = rootPath+"/build/android/Android.mk";
		jnifolder = rootPath+"/jni";
		appbuildscript3 = rootPath+"/jni/Application.mk";
		
		#check for spaces
		if (" " in ndkprojectpath) or (" " in ndkdir):
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
			 
			#make android.mk
			print("Creating Android.mk");
			android_make_file = "";
			android_make_file += "LOCAL_PATH := $(call my-dir)/../../" + nl + nl;
			android_make_file += "include $(CLEAR_VARS)" + nl+nl;
			android_make_file += "LOCAL_MODULE    := " + game_short_name +  nl+nl; # Here we give our module name and source file(s)
			android_make_file += "LOCAL_C_INCLUDES := " + ark2ddir + "/src/ARK2D/vendor/android/libzip/jni/ " + nl;
			android_make_file += "LOCAL_CFLAGS := -DARK2D_ANDROID -fno-exceptions -fno-rtti -Wno-psabi" + nl+nl;
			android_make_file += "LOCAL_DEFAULT_CPP_EXTENSION := cpp" + nl+nl; 
			android_make_file += "LOCAL_SRC_FILES := \\" + nl;
			for h in self.src_files: #foreach file on config...
				if (self.get_str_extension(h)!= "rc"):
					android_make_file += "	" + h + " \\" + nl;
					
			android_make_file += "	src/jni.cpp \\ " + nl;
				
			android_make_file += nl; 
			android_make_file += "LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz -lfreetype -lopenal -lark2d" + nl+nl;
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
			application_make_file += "APP_ABI := armeabi armeabi-v7a " + nl; 
			
			application_make_file += "APP_STL := stlport_static" + nl; 
			f = open(appbuildscript3, "w");
			f.write(application_make_file);
			f.close();
			
			#copy ark2d in to project
			print("Copying ARK2D in to android sdk");
			#subprocess.call(["cp -r " +ark2ddir + "/build/android/libs/armeabi/ " + appbuilddir + "/local/armeabi/"], shell=True);
			subprocess.call(["cp -r " +ark2ddir + "/build/android/libs/armeabi/ " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib/"], shell=True);
			subprocess.call(["cp -r " +ark2ddir + "/build/android/libs/armeabi/ " + ndkdir + "/platforms/"+ndkappplatform+"/arch-x86/usr/lib/"], shell=True);
			
			#copy resources in to "assets" dir.
			#print("Copying game resources in to project");
			#subprocess.call(["cp -r " + game_resources_dir + "/ " + rootPath + "/build/android/project/assets/"], shell=True);
			
			print("Making new asset directories");
			directoriesToCreate = self.listDirectories(game_resources_dir, False);
			for dir in directoriesToCreate:
				self.makeDirectories([rootPath + "/build/android/project/assets/" + dir]);
			#self.makeDirectories(directoriesToCreate);
			
			#generate game spritesheets
			self.generateSpriteSheets();

			print("Creating/opening assets cache file...");
			assetsCache = rootPath + "/build/android/build-cache/assets.json";
			assetsJson = self.openCacheFile(assetsCache);
			fchanged = False;
			
			print("Cool, now copying files")
			filesToCopy = self.listFiles(game_resources_dir, False);
			print(filesToCopy);
			for file in filesToCopy:
				fromfile = game_resources_dir + self.ds + file;
				tofile = rootPath + "/build/android/project/assets/" + file;
				
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
						subprocess.call(["cp -r " + fromfile + " " + tofile], shell=True);
						
					assetsJson[fromfile] = {"date_modified": os.stat(fromfile).st_mtime };
					fchanged = True;
					
			if (fchanged == True):
				f = open(assetsCache, "w")
				f.write(json.dumps(assetsJson, sort_keys=True, indent=4));
				f.close();
					
			
			#copy sample game c++/jni files...
			print("generating game jni files");
			print("	generating jni.h");
			editsStrReplace = [("%GAME_CLASS_NAME%", game_name_safe), ("%GAME_SHORT_NAME%", game_short_name), ("%COMPANY_NAME%", company_name_safe), ("%PACKAGE_DOT_NOTATION%", javaPackageName), ("%GAME_WIDTH%", str(config['android']['game_width'])), ("%GAME_HEIGHT%", str(config['android']['game_height'])), ("%GAME_ORIENTATION%", config['game_orientation']), ("%GAME_CLEAR_COLOR%", config['game_clear_color']) ];
			f = open(ark2ddir+"/lib/android/jni.h", "r");
			fgamejnih = f.read();
			f.close();
			fgamejnih = self.str_replace(fgamejnih, editsStrReplace);
			f = open(rootPath+"/src/jni.h", "w");
			f.write(fgamejnih);
			f.close();	
			
			print("	generating jni.cpp");
			f = open(ark2ddir+"/lib/android/jni.cpp", "r");
			fgamejnicpp = f.read();
			f.close();
			fgamejnicpp = self.str_replace(fgamejnicpp, editsStrReplace);
			f = open(rootPath+"/src/jni.cpp", "w");
			f.write(fgamejnicpp);
			f.close();	
			
			#build game apk.
			buildline = ndkdir + "/ndk-build";
			buildline += " NDK_PROJECT_PATH=" + ndkprojectpath;
			buildline += " NDK_APP_OUT=" + appbuilddir;
			buildline += " APP_PROJECT_PATH=" + ndkprojectpath;
			buildline += " APP_BUILD_SCRIPT=" + appbuildscript;
			buildline += " APP_PLATFORM=" + ndkappplatform;

			#nexus-7 fix.
			#application_make_file += "APP_ABI := armeabi armeabi-v7a " + nl; #x86" + nl;

			#buildline += " NDK_LOG=1";
			print("Building library");
			print(buildline);
			subprocess.call([buildline], shell=True);	
			
			#print("Moving output to build folder");
			libsdir = ndkprojectpath + "/libs";
			
			print("removing temp folders");
			self.rmdir_recursive(jnifolder);
			self.rmdir_recursive(libsdir);

			print("done c++ bits.");
			print("do androidy-javay bits...");
			try:
				print("using custom AndroidManifest.xml");
				androidManifestPath = config['android']['manifest'];
				#todo: copy this to rootPath+"/build/android/project/AndroidManifest.xml";
			except:
				print("generating default AndroidManifest.xml");
				androidManifestContents = "";
				androidManifestContents += "<?xml version=\"1.0\" encoding=\"utf-8\"?>" + nl;
				androidManifestContents += "<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"" + nl;
				androidManifestContents += "	package=\"" + javaPackageName + "\" " + nl;
				if (config['android']['app_to_sd'] == True):
					androidManifestContents += "	android:installLocation=\"preferExternal\"" + nl;
				else:
					androidManifestContents += "	android:installLocation=\"internalOnly\"" + nl;
				androidManifestContents += "	android:versionCode=\"1\" " + nl;
				androidManifestContents += "	android:versionName=\"1.0\"> " + nl;
				androidManifestContents += "	<uses-sdk android:minSdkVersion=\"" + str(appplatformno) + "\" />" + nl;
				for permission in config['android']['permissions']:
					androidManifestContents += "	<uses-permission android:name=\"android.permission." + permission + "\" />" + nl;
				androidManifestContents += "	<application" + nl;
				androidManifestContents += "		android:icon=\"@drawable/ic_launcher\" " + nl;
				androidManifestContents += "		android:label=\"@string/application_name\" android:debuggable=\"true\"> " + nl;
				androidManifestContents += "		<activity" + nl;
				androidManifestContents += "			android:name=\"." + game_name_safe + "Activity\" " + nl;
				androidManifestContents += "			android:label=\"@string/application_name\" " + nl;
				androidManifestContents += "			android:configChanges=\"orientation|keyboardHidden\" " + nl;
				androidManifestContents += "			android:theme=\"@android:style/Theme.NoTitleBar.Fullscreen\"> " + nl;
				androidManifestContents += "			<intent-filter>" + nl;
				androidManifestContents += "				<action android:name=\"android.intent.action.MAIN\" /> " + nl;
				androidManifestContents += "				<category android:name=\"android.intent.category.LAUNCHER\" /> " + nl;
				androidManifestContents += "			</intent-filter>" + nl;
				androidManifestContents += "		</activity>" + nl;
				androidManifestContents += "	</application>" + nl;
				androidManifestContents += "</manifest>" + nl;
				f = open(rootPath+"/build/android/project/AndroidManifest.xml", "w");
				f.write(androidManifestContents);
				f.close();
				androidManifestPath = rootPath+"/build/android/project/AndroidManifest.xml";
				
			print("copying icon in to project: ");
			print("TODO: resizing for different DPIs");
			if (config['osx']['android']['icon'] != ''):
				subprocess.call(['cp ' + config['osx']['android']['icon'] + " " + rootPath+"/build/android/project/res/drawable/ic_launcher.png"], shell=True);
			else:
				subprocess.call(['cp ' + ark2ddir + "/__preproduction/icon/512.png " + rootPath+"/build/android/project/res/drawable/ic_launcher.png"], shell=True);
				
			
			print("generating project.properties");
			projectPropertiesContents = "";
			projectPropertiesContents += "target=" + appplatform;
			f = open(rootPath+"/build/android/project/project.properties", "w");
			f.write(projectPropertiesContents);
			f.close();
			
			#generate strings.xml
			androidStringsXmlContents = "";
			androidStringsXmlContents += "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
			androidStringsXmlContents += "<resources>";
			androidStringsXmlContents += "	<string name=\"application_name\">" + game_name + "</string>";
			androidStringsXmlContents += "	<string name=\"application_description\">" + game_description + "</string>";
			androidStringsXmlContents += "</resources>";
			f = open(rootPath+"/build/android/project/res/values/strings.xml", "w");
			f.write(androidStringsXmlContents);
			f.close(); 
			
			#generate project .classpath file
			androidProjectClasspathContents = "";
			androidProjectClasspathContents += "<?xml version=\"1.0\" encoding=\"UTF-8\"?> " + nl;
			androidProjectClasspathContents += "<classpath> " + nl;
			androidProjectClasspathContents += "	<classpathentry kind=\"src\" path=\"src\"/> " + nl;
			androidProjectClasspathContents += "	<classpathentry kind=\"src\" path=\"gen\"/> " + nl;
			androidProjectClasspathContents += "	<classpathentry kind=\"con\" path=\"com.android.ide.eclipse.adt.ANDROID_FRAMEWORK\"/> " + nl;
			androidProjectClasspathContents += "	<classpathentry kind=\"output\" path=\"bin/classes\"/> " + nl;
			androidProjectClasspathContents += "</classpath> " + nl;
			f = open(rootPath+"/build/android/project/.classpath", "w");
			f.write(androidProjectClasspathContents);
			f.close();

				
			#copy sample game java files...
			fgamefile = "";
			fgamecontents = "";
			try: 
				print("using custom GameActivity.java");
				fgamefile = self.game_dir + self.ds + config['android']['override_activity'];
			except:
				print("using default (generated) GameActivity.java");
				fgamefile = ark2ddir + "/lib/android/GameActivity.java";
				pass;

			f = open(fgamefile, "r");
			fgamecontents = f.read();
			f.close();
			fgamecontents = self.str_replace(fgamecontents, editsStrReplace);
			
			f = open(rootPath+"/build/android/project/src/org/" + company_name_safe + self.ds + game_short_name + self.ds + game_name_safe + "Activity.java", "w");
			f.write(fgamecontents);
			f.close();		
			
			#copying library/s in to project.
			print("Copying ark2d and game.so in to project.");
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/freetype/obj/local/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libfreetype
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/freetype/obj/local/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/freetype/obj/local/armeabi/ " + rootPath+"/build/android/project/libs/armeabi-v7a"], shell=True); #libfreetype
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/freetype/obj/local/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi-v7a"], shell=True);
			
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/openal/libs/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libfreetype
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/openal/libs/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/openal/libs/armeabi-v7a/ " + rootPath+"/build/android/project/libs/armeabi-v7a"], shell=True); #libfreetype
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/openal/libs/armeabi-v7a/ " + rootPath+"/build/android/project/obj/local/armeabi-v7a"], shell=True);
			
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/libzip/libs/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libzip
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/libzip/libs/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/libzip/libs/armeabi/ " + rootPath+"/build/android/project/libs/armeabi-v7a"], shell=True); #libzip
			subprocess.call(["cp -r " +ark2ddir + "/src/ARK2D/vendor/android/libzip/libs/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi-v7a"], shell=True);
			
			#subprocess.call(["cp -r " +ark2ddir + "/build/android/local/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libark2d
			#subprocess.call(["cp -r " +ark2ddir + "/build/android/local/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
			
			subprocess.call(["cp -r " +ark2ddir + "/build/android/libs/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libark2d
			subprocess.call(["cp -r " +ark2ddir + "/build/android/libs/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
			subprocess.call(["cp -r " +ark2ddir + "/build/android/libs/armeabi-v7a/ " + rootPath+"/build/android/project/libs/armeabi-v7a"], shell=True); #libark2d
			subprocess.call(["cp -r " +ark2ddir + "/build/android/libs/armeabi-v7a/ " + rootPath+"/build/android/project/obj/local/armeabi-v7a"], shell=True);

			subprocess.call(["cp -r " +rootPath + "/build/android/local/armeabi/ " + rootPath+"/build/android/project/libs/armeabi"], shell=True); #libgamename
			subprocess.call(["cp -r " +rootPath + "/build/android/local/armeabi/ " + rootPath+"/build/android/project/obj/local/armeabi"], shell=True);
			subprocess.call(["cp -r " +rootPath + "/build/android/local/armeabi-v7a/ " + rootPath+"/build/android/project/libs/armeabi-v7a"], shell=True); #libgamename
			subprocess.call(["cp -r " +rootPath + "/build/android/local/armeabi-v7a/ " + rootPath+"/build/android/project/obj/local/armeabi-v7a"], shell=True);
			
			#copying ark2d resources in to assets folder.
			subprocess.call(["cp -r " +ark2ddir + "/data/ " + rootPath+"/build/android/project/assets/ark2d"], shell=True);
			
		elif (self.building_library):
			
			#copy stuff in vendor to ndk directory. 
			#freetype
			print("copying vendor headers (freetype)");
			copyfreetype1 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/freetype/jni/include ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/";
			copyfreetype2 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/freetype/jni/include ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-x86/usr/";
			subprocess.call([copyfreetype1], shell=True);
			subprocess.call([copyfreetype2], shell=True);
			
			print("Compiling vendor sources (freetype)");
			libfreetypedir = ndkprojectpath + "/src/ARK2D/vendor/android/freetype";
			compilefreetype1 = ndkdir + "/ndk-build NDK_PROJECT_PATH=" + libfreetypedir +" APP_PROJECT_PATH=" + libfreetypedir + " APP_BUILD_SCRIPT=" + libfreetypedir + "/jni/Android.mk APP_PLATFORM=" + ndkappplatform;  
			print(compilefreetype1);
			subprocess.call([compilefreetype1], shell=True);
			subprocess.call(['cp -r ' + libfreetypedir + "/obj/local/armeabi/libfreetype.a " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib"], shell=True);
			
			#openal
			print("copying vendor headers (openal)");
			copyopenal1 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/openal/jni/include ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/";
			copyopenal2 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/openal/jni/include ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-x86/usr/";
			subprocess.call([copyopenal1], shell=True);
			subprocess.call([copyopenal2], shell=True);
			
			print("Compiling vendor sources (openal)");
			libopenaldir = ndkprojectpath + "/src/ARK2D/vendor/android/openal";
			compileopenal1 = ndkdir + "/ndk-build NDK_PROJECT_PATH=" + libopenaldir +" APP_PROJECT_PATH=" + libopenaldir + " APP_BUILD_SCRIPT=" + libopenaldir + "/jni/Android.mk APP_PLATFORM=" + ndkappplatform;  
			print(compileopenal1);
			subprocess.call([compileopenal1], shell=True);
			subprocess.call(['cp -r ' + libopenaldir + "/libs/armeabi/libopenal.so " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib"], shell=True);
			#nexus7update subprocess.call(['cp -r ' + libopenaldir + "/libs/armeabi-v7a/libopenal.so " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib"], shell=True);
			
			#openal
			#print("copying vendor headers (libzip)");
			#print("NOT");
			#copyopenal1 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/libzip/jni/zip.h ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/include/";
			#copyopenal2 = 'cp -r ' + ndkprojectpath + '/src/ARK2D/vendor/android/libzip/jni/zip.h ' + ndkdir + "/platforms/"+ndkappplatform+"/arch-x86/usr/include/";
			#subprocess.call([copyopenal1], shell=True);
			#subprocess.call([copyopenal2], shell=True);
			
			print("Compiling vendor sources (libzip)");
			libzipdir = ndkprojectpath + "/src/ARK2D/vendor/android/libzip";
			compilelibzip1 = ndkdir + "/ndk-build NDK_PROJECT_PATH=" + libzipdir +" APP_PROJECT_PATH=" + libzipdir + " APP_BUILD_SCRIPT=" + libzipdir + "/jni/Android.mk APP_PLATFORM=" + ndkappplatform;  
			print(compilelibzip1);
			subprocess.call([compilelibzip1], shell=True);
			subprocess.call(['cp -r ' + libzipdir + "/libs/armeabi/libzip.so " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib"], shell=True);
			
			#libcurl
			#print("Compiling vendor sources (libcurl)");
			#libcurldir = ndkprojectpath + "/src/ARK2D/vendor/android/curl-android";
			#compilelibcurl1 = ndkdir + "/ndk-build NDK_PROJECT_PATH=" + libcurldir +" APP_PROJECT_PATH=" + libcurldir + " APP_BUILD_SCRIPT=" + libcurldir + "/Android.mk APP_PLATFORM=" + ndkappplatform;  
			#print(compilelibcurl1);
			#subprocess.call([compilelibcurl1], shell=True);
			#subprocess.call(['cp -r ' + libcurldir + "/libs/armeabi/libcurl.so " + ndkdir + "/platforms/"+ndkappplatform+"/arch-arm/usr/lib"], shell=True);
			
			
			#make android.mk
			print("Creating Android.mk");
			android_make_file = "";
			android_make_file += "LOCAL_PATH := $(call my-dir)/../../" + nl + nl;
			android_make_file += "include $(CLEAR_VARS)" + nl+nl;
			android_make_file += "LOCAL_MODULE    := ark2d" + nl+nl; # Here we give our module name and source file(s)
			#android_make_file += "LOCAL_C_INCLUDES := $(LOCAL_PATH)/../libzip/ $(LOCAL_PATH)/../libpng/" + nl;
			android_make_file += "LOCAL_C_INCLUDES := $(LOCAL_PATH)/src/ARK2D/vendor/android/libzip/jni/ " + nl;
			#android_make_file += "LOCAL_STATIC_LIBRARIES := libzip libpng" + nl;
			#android_make_file += "LOCAL_C_INCLUDES += external/stlport/stlport" + nl+nl;
			android_make_file += "LOCAL_SHARED_LIBRARIES += libstdc++" + nl+nl;
			android_make_file += "LOCAL_CFLAGS := -DARK2D_ANDROID -DDISABLE_IMPORTGL -fno-exceptions -fno-rtti -Wno-psabi" + nl+nl;
			android_make_file += "LOCAL_DEFAULT_CPP_EXTENSION := cpp" + nl+nl; 
			android_make_file += "LOCAL_SRC_FILES := \\" + nl;
			for h in self.src_files: #foreach file on config...
				android_make_file += "	" + h + " \\" + nl;
			android_make_file += nl;
			android_make_file += "LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz -lfreetype -lopenal -lzip" + nl+nl;
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
			application_make_file += "APP_ABI := armeabi armeabi-v7a " + nl; #x86" + nl;
			


			application_make_file += "APP_STL := stlport_static" + nl; 
			f = open(appbuildscript3, "w");
			f.write(application_make_file);
			f.close();
			
			buildline = ndkdir + "/ndk-build";
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
			libsdir = ndkprojectpath + "/libs";
			subprocess.call(["cp -r " + libsdir + " " + appbuilddir + self.ds ], shell=True);
			
			print("removing temp folders");
			self.rmdir_recursive(libsdir);
			self.rmdir_recursive(jnifolder);
			
			print("done!");
			
		else:
			pass;
		
	def start(self):
	
		if (self.platform == "android"):
			self.startAndroid();
		elif (self.platform == "iphone"):
			self.startIPhone();
		elif (self.platform == "windows"):
			self.startWindows();
		elif(self.platform == "osx"):
			self.startMac();
		elif(self.platform == "flascc"):
			self.startFlascc();
		else:
			print(sys.platform); 
			print("platform " + sys.platform + " is not supported yet");

	
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
#		target 		the platform to build for.							default | android | iphone 		defaults to "default"
#		dir 		directory of the game. must not contain spaces. 	directory						defaults to cwd
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
		elif parts[0] == "clean":
			if (parts[1]=="true"):
				clean = True;

		count += 1;
		
	print("---");
	print("type: " + type);
	print("target: " + target);
	print("dir: " + dir);
	print("clean: " + str(clean));
	
	if (sys.platform == "win32"):
		arkPlatform = "windows";
	elif(sys.platform == "darwin"):
		arkPlatform = "osx";
	else: 
		exit(0);


	if (type == "library"):

		print("---");
		print("Building Library");
		
		a = ARK2DBuildSystem();

		if (target=='iphone'):
			a.platform = 'iphone';
		elif (target=='android'):
			a.platform = 'android';
		elif (target=='flascc'):
			a.platform = 'flascc';

		print("---");
		print("Opening config file: " + dir + "/config.json");
		f = open(dir + "/config.json", "r");
		fcontents = f.read();
		f.close();
		config = False; 
		
		try:
			a.config = json.loads(fcontents);
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

			if (target=='iphone'):
				a.platform = 'iphone';
			elif (target=='android'):
				a.platform = 'android';
			elif (target=='flascc'):
				a.platform = 'flascc';

			a.ark2d_dir = config[arkPlatform]["ark2d_dir"];
			a.game_name = config["game_name"];
			a.game_short_name = config['game_short_name'];
			a.game_dir  = config[arkPlatform]["game_dir"];
			a.src_files.extend(config["game_src_files"]);
			a.game_mkdirs = config['game_mkdirs'];
			a.build_artifact = "";
			a.config = config;
			
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
			a.start();

		except Exception as exc: 
			print(exc);
			print("Config.json invalid or trying to build game from framework path.");
			exit(1);
		except:
			print("Done");
			pass;

		
		
		

		
