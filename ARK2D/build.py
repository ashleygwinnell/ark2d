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
		self.game_resources_dir = "";
		#self.mac_game_icns = "";
		
		self.build_folder = "build";
		self.arch = platform.machine();
		
		if (len(sys.argv)==3 and sys.argv[2] == "android"):
			self.platform = "android";
			if (sys.platform == "win32"):
				self.ds = "\\";
			elif(sys.platform == "darwin"):
				self.ds = "/";
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
			
		elif(sys.platform == "darwin"):
			self.ds = "/";
			self.platform = "osx";
			self.mingw_dir = ""; #/usr";
			self.mingw_link = ""; #-L" + self.mingw_dir + self.ds + "lib"
			self.gccCompiler = "i686-apple-darwin11-llvm-gcc-4.2 ";
			self.gppCompiler = "i686-apple-darwin11-llvm-g++-4.2 ";
			self.objcCompiler = "i686-apple-darwin11-llvm-g++-4.2 ";
			#self.gccCompiler = "llvm-gcc-4.2";
			#self.gppCompiler = "llvm-g++-4.2";
			#self.gccCompiler = "gcc";
			#self.gppCompiler = "g++";
			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + "libARK2D.dylib";
			self.mac_game_icns = '';
		
		self.windresources = [];
		
		self.mkdirs = [];
		self.game_mkdirs = [];
		self.src_files = [];
		self.dll_files = [];
		self.static_libraries = [];
		self.linkingFlags = "";
		
	def dllInit(self):
		
		f = open("config.json", "r");
		fcontents = f.read();
		f.close();
		config = json.loads(fcontents);
		
		self.building_library = True;
		self.building_game = False;
	
		self.mkdirs.extend([
			self.build_folder,
			self.build_folder + self.ds + self.platform,
			self.build_folder + self.ds + self.platform + self.ds + "build-cache", # cache folder
			self.build_folder + self.ds + self.platform + self.ds + "src",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D", 
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Audio",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Font",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Geometry",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Image",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Particles",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Path",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "State",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "State" + self.ds + "Transition",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tiled",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Tools",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "UI",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "Util",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "libJSON",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "lpng151",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "ogg130",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "tinyxml",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "vorbis132",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "vorbis132" + self.ds + "modes",
			self.build_folder + self.ds + self.platform + self.ds + "src" + self.ds + "ARK2D" + self.ds + "vendor" + self.ds + "zlib123"
		]);
		
		
		self.src_files.extend(config['src_files']['all']);
		
		if (self.platform == "android"):
			self.src_files.extend(config['src_files']['android']);
		elif (self.platform == "windows"):
		
			self.src_files.extend(config['src_files']['windows']);
			self.dll_files.extend(config['dynamic_libraries']['windows']);
			self.static_libraries.extend(config['static_libraries']['windows']);
			self.linkingFlags = " -mwindows -shared ";
			
		elif (self.platform == "osx"):
			
			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + "libARK2D.dylib";
			
			self.src_files.extend(config['src_files']['mac']);
			self.dll_files.extend(config['dynamic_libraries']['mac']);
			self.static_libraries.extend(config['static_libraries']['mac']);
			self.linkingFlags = "";
			
					
		
		
	def gamePreInit(self):
	
		self.building_library = False;
		self.building_game = True;
	
		#self.src_files[:] = []; # clear the lists.
		self.dll_files[:] = [];
		self.static_libraries[:] = [];
		
		self.mkdirs.extend([
			self.build_folder,
			self.build_folder + self.ds + self.platform,
			self.build_folder + self.ds + self.platform + self.ds + "src",
			self.build_folder + self.ds + self.platform + self.ds + "build-cache" # cache folder
			
		]);
		self.mkdirs.extend(self.game_mkdirs);
		
	def gamePostInit(self):
	
		if (sys.platform == "win32"):
			self.build_artifact = self.build_folder + self.ds + self.platform + self.ds + self.game_name.replace(" ", "_") + ".exe";
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
			#h = "'" + h + "'";
			print("mkdir " + h);
			#subprocess.call(['mkdir C:\\' + h], shell=True);
			try:
				os.makedirs(h);
			except OSError as exc: 
				if exc.errno == errno.EEXIST:
					pass
				else: raise
				
		
		
		# make sure cache file exists
		
		# compile cache thing
		cachefilename = "";
		#if (self.building_game):
			#cachefilename += self.game_dir + self.ds;
			
		cachefilename += self.build_folder + self.ds + self.platform + self.ds + "build-cache" + self.ds  + "compiled.json";
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
			newfd = h[0:findex] + ".d";
			
			
			if h_ext == 'c':
				compileStr += self.gccCompiler;
			elif h_ext == 'cpp':
				compileStr += self.gppCompiler;
			elif h_ext == 'mm':
				compileStr += self.objcCompiler;
			elif h_ext == 'rc':
				compileStr += "windres ";
				
			if (not h in fjson or fjson[h]['date_modified'] < os.stat(h).st_mtime):
				
				processThisFile = True;

				if (h_ext == 'c' or h_ext == 'cpp' or h_ext == 'mm'):
					compileStr += " -O3 -Wall -c -fmessage-length=0 ";
					if (sys.platform == "darwin"): #compiling on mac
						if not "vendor" in newf:
							compileStr += " -mmacosx-version-min=10.5 -DMAC_OS_X_VERSION_MIN_REQUIRED=1050 -x objective-c++ ";
							compileStr += "-I /usr/X11/include "; 
						
						#  compileStr += " -march=i386 ";
						# compileStr += " -march=i386 ";
						#-march=i386 "; # i386
						#-arch i386
						compileStr += " -o \"";
						compileStr += self.build_folder + self.ds + self.platform + self.ds + newf + "\" \"" + h + "\" ";
					else:
						#compileStr += " -o \"" + self.build_folder + self.ds + self.platform + self.ds + newf + "\" \"" + h + "\" ";
						compileStr += " -o " + self.build_folder + self.ds + self.platform + self.ds + newf + " " + h + " ";
				elif h_ext == 'rc':
					if (sys.platform == "win32"):
						compileStr += h + " " + self.build_folder + self.ds + self.platform + self.ds + newf + " ";
					else:
						processThisFile = False;
			
				if (processThisFile):
					fjson[h] = {"date_modified": os.stat(h).st_mtime };
	
					print(compileStr);
					#subprocess.call(["dir"], shell=True);
					#subprocess.call([compileStr], shell=True);	 
					
					# the above did not work on win7 64bit.
					os.system(compileStr);
					fchanged = True;
					
					
	
		# update compile cache thing
		if (fchanged == True):
			f = open(self.build_folder + self.ds + self.platform + self.ds + "build-cache" + self.ds + "compiled.json", "w")
			f.write(json.dumps(fjson, sort_keys=True, indent=4));
			f.close();
	
		#link
		self.doLink();
		
		
	def doLink(self):
		
		print("linking binary");
		if (sys.platform=="win32"):
		 
			linkingStr = "";
			linkingStr += self.gppCompiler + " " + self.mingw_link + " " + self.linkingFlags + " -o" + self.build_artifact + "";
			#linkingStr += self.gppCompiler + " " + self.mingw_link + " -o" + self.build_artifact + "";
		
			for h in self.src_files:
				findex = h.rfind('.');
				newf = h[0:findex] + ".o";
				#print(newf);
				h_ext = h[findex+1:len(h)];
				linkingStr += " " + self.build_folder + self.ds + self.platform + self.ds + newf;
			
			for f in self.dll_files:
				linkingStr += " " + f;
				
			for f in self.static_libraries:
				linkingStr += " -l" + f;
				#pass;
				
			print(linkingStr);
			
			#subprocess.call([linkingStr], shell=True);	
			#print(len(linkingStr));
			os.system(linkingStr);
			
			#copy game resources in to .build
			if(self.building_game):
			
				#copying dll in to project.
				try:
					dll = self.ark2d_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + 'libARK2D.dll'
					shutil.copy(dll, self.game_dir.replace('\\\\','\\') + '\\' + self.build_folder + '\\' + self.platform);
				except:
					pass;
					
				#copying other dlls in to project.
				try:
					otherdlls = ['alut.dll', 'freetype6.dll', 'glew32.dll', 'OpenAL32.dll', 'wrap_oal.dll', 'zlib1.dll'];
					for one in otherdlls:
						one = self.ark2d_dir + self.ds + 'lib' + self.ds + self.platform + self.ds + one;
						shutil.copy(one, self.game_dir.replace('\\\\','\\') + '\\' + self.build_folder + '\\' + self.platform);
				except:
					pass;
			
				if (self.game_resources_dir != ''):
					print("copying game resources in to project:");
					#cpy_game_res = 'copy "' + self.game_resources_dir.replace('\\\\','\\') + '" "' + self.game_dir.replace('\\\\','\\') + '\\' + self.build_folder + '\\' + self.platform + '\\"';
					#print(cpy_game_res);
					#subprocess.call([cpy_game_res], shell=True);
					try:
						shutil.copytree(self.game_resources_dir.replace('\\\\','\\'), self.game_dir.replace('\\\\','\\') + '\\' + self.build_folder + '\\' + self.platform + '\\data');
					except:
						pass;
				
			
		elif(sys.platform=="darwin"):
			
			if (self.building_library):
				linkingStr = "";
				linkingStr += self.gppCompiler + " -framework OpenGL -framework OpenAL -framework Foundation -framework Cocoa -lobjc -install_name @executable_path/../Frameworks/libARK2D.dylib " + self.linkingFlags + "  -dynamiclib -o " + self.build_artifact;
				#linkingStr += " -march=i386 ";
			
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
				
				#copying dylib in to project.
				dylibsrc = self.ark2d_dir + self.ds + self.build_folder + self.ds + self.platform + self.ds + 'libARK2D.dylib'
				subprocess.call(['cp ' + dylibsrc + ' ' + frameworks_folder + self.ds + 'libARK2D.dylib'], shell=True);
				
				
				#copy ark2d resources in to .app
				print("copying ark2d resources in to project:");
				cpyark2dres = 'cp -r ' + self.ark2d_dir + self.ds + 'data ' + resources_folder + self.ds + 'ark2d';
				print(cpyark2dres);
				subprocess.call([cpyark2dres], shell=True);
				
				#copy game resources in to .app
				if (self.game_resources_dir != ''):
					print("copying game resources in to project:");
					gme_rs_dir = self.game_resources_dir.replace(' ', '\ ').replace('&', '\&');
					cpy_game_res = 'cp -r ' + gme_rs_dir + ' ' + resources_folder;
					print(cpy_game_res);
					subprocess.call([cpy_game_res], shell=True);
				
				
				#copy icns in to .app folder
				if (self.mac_game_icns != ''):
					subprocess.call(['cp ' + self.mac_game_icns.replace(' ', '\ ').replace('&', '\&') + ' ' + resources_folder + self.ds + gn +'.icns'], shell=True);
				else:
					subprocess.call(['cp ' + resources_folder + self.ds + 'ark2d' + self.ds + 'icon.icns ' + resources_folder + self.ds + gn +'.icns'], shell=True);
				
				
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
				infoplistcontents += "		<string>" + self.game_name + "</string>" + cr;
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
					h_ext = h[findex+1:len(h)];
					if (h_ext != 'rc'):
						linkingStr += " " + self.build_folder + self.ds + self.platform + self.ds + newf;
				
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
		if (sys.platform == "darwin"):
			cm = "rm -r -d " + self.build_folder + self.ds + self.platform;
		elif(sys.platform == "win32"):
			cm = "rmdir /S /Q " + self.build_folder + self.ds + self.platform;
		
		#subprocess.call([], shell=True);
		print(cm);
		os.system(cm);
		
	def startAndroid(self):
		print("Building Android");
		
		f = open("config.json", "r");
		fcontents = f.read();
		f.close();
		config = json.loads(fcontents);
		
		nl = "\r\n";
		ndkdir = config['mac']['android']['ndk_dir'];
		ndkprojectpath = config['mac']['ark2d_dir']
		appbuilddir = ndkprojectpath+"/build/android";
		appbuildscript = ndkprojectpath+"/build/android/Android.mk";
		appbuildscript2 = ndkprojectpath+"/build/android/Application.mk";
		appplatform= "android-4";
		
		#make android.mk
		android_make_file = "";
		android_make_file += "LOCAL_PATH := $(call my-dir)/../../" + nl + nl;
		android_make_file += "include $(CLEAR_VARS)" + nl+nl;
		android_make_file += "LOCAL_MODULE    := ark2d" + nl+nl; # Here we give our module name and source file(s)
		#android_make_file += "LOCAL_C_INCLUDES := $(LOCAL_PATH)/../libzip/ $(LOCAL_PATH)/../libpng/" + nl;
		#android_make_file += "LOCAL_STATIC_LIBRARIES := libzip libpng" + nl;
		android_make_file += "LOCAL_CFLAGS := -DARK2D_ANDROID -DDISABLE_IMPORTGL -Wno-psabi" + nl+nl;
		android_make_file += "LOCAL_DEFAULT_CPP_EXTENSION := cpp" + nl+nl; 
		android_make_file += "LOCAL_SRC_FILES := \\" + nl;
		for h in self.src_files: #foreach file on config...
			android_make_file += "	" + h + " \\" + nl;
		android_make_file += nl;
		android_make_file += "LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog -lz" + nl+nl;
		android_make_file += "include $(BUILD_SHARED_LIBRARY)" + nl;
		f = open(appbuildscript, "w");
		f.write(android_make_file);
		f.close();
		
		#make application.mk
		application_make_file = "";
		#application_make_file += "APP_PROJECT_PATH := " + ndkprojectpath + nl;
		#application_make_file += "APP_BUILD_SCRIPT := " + appbuildscript + nl;
		#application_make_file += "APP_ABI := all" + nl;
		#application_make_file += "APP_ABI := armeabi armeabi-v7a x86" + nl;
		application_make_file += "APP_STL := stlport_static" + nl;
		f = open(appbuildscript2, "w");
		f.write(application_make_file);
		f.close();
		
		buildline = ndkdir + "/ndk-build NDK_PROJECT_PATH=" + ndkprojectpath + " APP_BUILD_SCRIPT=" + appbuildscript + " NDK_APP_OUT=" + appbuilddir + " APP_PLATFORM=" + appplatform;
		print(buildline);
		subprocess.call([buildline], shell=True);	
		
	def start(self):
	
		if (self.platform == "android"):
			self.startAndroid();
		elif (self.platform == "windows"):
			self.startWindows();
		elif(self.platform == "osx"):
			self.startMac();
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
##	
if __name__ == "__main__":
	
	print("Starting");
	
	if (len(sys.argv)>=2):
		if (sys.argv[1] == "library"):
			print("Building library");
			a = ARK2DBuildSystem();
			#a.clean();
			a.dllInit();
			a.start();
		else:
			print("Invalid parameters.");	
	else:
		print("Building game");
		if (sys.platform == "win32"):
			arkPlatform = "windows";
		elif(sys.platform == "darwin"):
			arkPlatform = "mac";
		
		f = open("config.json", "r");
		fcontents = f.read();
		f.close();
		
		#print(fcontents);
		j = json.loads(fcontents);
		
		try:
			blah = j["game_name"];
		except:
			print("Config.json invalid or trying to build game from framework path.");
			exit(0);
			
		a = ARK2DBuildSystem();
		a.ark2d_dir = j[arkPlatform]["ark2d_dir"];
		a.game_name = j["game_name"];
		a.game_short_name = j['game_short_name'];
		a.game_dir  = j[arkPlatform]["game_dir"];
		a.src_files.extend(j["game_src_files"]);
		a.game_mkdirs = j['game_mkdirs'];
		a.build_artifact = "";
		
		if (j['clean'] == True):
			a.clean();
		
		a.gamePreInit();
		
		a.mingw_link = "";
		
		if(sys.platform=="win32"):
			a.dll_files.append(a.ark2d_dir + a.ds + a.build_folder + a.ds + a.platform + a.ds + 'libARK2D.dll');
			a.linkingFlags += " -mwindows "; 
			a.linkingFlags += " -enable-auto-import ";
		elif(sys.platform=="darwin"):
			a.dll_files.append(a.ark2d_dir + a.ds + a.build_folder + a.ds + a.platform + a.ds + 'libARK2D.dylib');
			
			if ('mac_game_icns' in j[arkPlatform]):
				a.mac_game_icns = j[arkPlatform]['mac_game_icns'];
				
		
		if ('game_resources_dir' in j[arkPlatform]):
			a.game_resources_dir = j[arkPlatform]['game_resources_dir'];
				
		
		a.gamePostInit();
		a.start();
		
		

		
