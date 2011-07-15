import sys
import os
import subprocess
import json 

#
# Windows requires MinGW with includes:
# 	AL/ 
#		al.h, alc.h, alext.h, alut.h
#	GL/
#		gl.h, glew.h, glext.h, glu.h, glxew.h, glxext.h, wglew.h, wglext.h
#
#	... and probably some others which I don't remember right now.
#

class ARK2DBuildSystem:

	def __init__(self):
		# something
		self.mingw_dir = "C:\\MinGW";
		self.mingw_link = "-L" + self.mingw_dir + "\\lib"
		
		self.build_folder = "build_release";
		self.build_artifact = self.build_folder + "\\libARK2D.dll";
		
		self.mkdirs = [];
		self.src_files = [];
		self.dll_files = [];
		self.static_libraries = [];
		
	def dllInit(self):
		self.mkdirs.extend([
			self.build_folder,
			self.build_folder + "\\src",
			self.build_folder + "\\src\\ARK2D", 
			self.build_folder + "\\src\\ARK2D\\ARK2D_Path",
			self.build_folder + "\\src\\ARK2D\\ARK2D_State",
			self.build_folder + "\\src\\ARK2D\\ARK2D_Util",
			self.build_folder + "\\src\\ARK2D\\particles",
			self.build_folder + "\\src\\ARK2D\\UI",
			self.build_folder + "\\src\\ARK2D\\vendor\\libJSON",
			self.build_folder + "\\src\\ARK2D\\vendor\\libJSON",
			self.build_folder + "\\src\\ARK2D\\vendor\\lpng151",
			self.build_folder + "\\src\\ARK2D\\vendor\\ogg",
			self.build_folder + "\\src\\ARK2D\\vendor\\tinyxml",
			self.build_folder + "\\src\\ARK2D\\vendor\\vorbis",
			self.build_folder + "\\src\\ARK2D\\vendor\\zlib123",
			self.build_folder + "\\build-cache" # cache folder
		]);

		self.src_files.extend([
			'src\\main.cpp',
			'src\\ARK2D\\glew.c',
			'src\\ARK2D\\ARK2D.cpp',
			'src\\ARK2D\\Animation.cpp',
			'src\\ARK2D\\ARKException.cpp',
			'src\\ARK2D\\ARKGameObject.cpp',
			'src\\ARK2D\\ARKMutex.cpp',
			'src\\ARK2D\\ARKString.cpp',
			'src\\ARK2D\\ARKThread.cpp',
			'src\\ARK2D\\BMFont.cpp',
			'src\\ARK2D\\BMPImage.cpp',
			'src\\ARK2D\\CameraShake.cpp',
			'src\\ARK2D\\Circle.cpp',
			'src\\ARK2D\\Color.cpp',
			'src\\ARK2D\\Easing.cpp',
			'src\\ARK2D\\ErrorDialog.cpp',
			'src\\ARK2D\\Event.cpp',
			'src\\ARK2D\\FileUtil.cpp',
			'src\\ARK2D\\Game.cpp',
			'src\\ARK2D\\GameContainer.cpp',
			'src\\ARK2D\\GameContainerLinux.cpp',
			'src\\ARK2D\\GameContainerWindows.cpp',
			'src\\ARK2D\\Gamepad.cpp',
			'src\\ARK2D\\GameTimer.cpp',
			'src\\ARK2D\\GigaRectangle.cpp',
			'src\\ARK2D\\GigaRectangleF.cpp',
			'src\\ARK2D\\Graphics.cpp',
			'src\\ARK2D\\Image.cpp',
			'src\\ARK2D\\Input.cpp',
			'src\\ARK2D\\LocalHighscores.cpp',
			'src\\ARK2D\\MathUtil.cpp',
			'src\\ARK2D\\OutputWrapper.cpp',
			'src\\ARK2D\\PNGImage.cpp',
			'src\\ARK2D\\Shader.cpp',
			'src\\ARK2D\\Sort.cpp',
			'src\\ARK2D\\Sound.cpp',
			'src\\ARK2D\\SoundStore.cpp',
			'src\\ARK2D\\SpriteSheet.cpp',
			'src\\ARK2D\\SpriteSheetDescription.cpp',
			'src\\ARK2D\\SpriteSheetDescriptionItem.cpp',
			'src\\ARK2D\\StringStore.cpp',
			'src\\ARK2D\\StringUtil.cpp',
			'src\\ARK2D\\TargaImage.cpp',
			'src\\ARK2D\\Texture.cpp',
			'src\\ARK2D\\TiledMap.cpp',
			'src\\ARK2D\\TiledMapLayer.cpp',
			'src\\ARK2D\\TiledMapObject.cpp',
			'src\\ARK2D\\TiledMapObjectGroup.cpp',
			'src\\ARK2D\\TiledMapProperty.cpp',
			'src\\ARK2D\\TiledMapTile.cpp',
			'src\\ARK2D\\TiledMapTileset.cpp',
			'src\\ARK2D\\Timeline.cpp',
			'src\\ARK2d\\UI\\UIComponent.cpp',
			'src\\ARK2d\\UI\\AbstractUIComponent.cpp',
			'src\\ARK2d\\UI\\Panel.cpp',
			'src\\ARK2d\\UI\\TextField.cpp',
			'src\\ARK2d\\UI\\Button.cpp',
			'src\\ARK2d\\UI\\FileDialog.cpp',
			'src\\ARK2d\\UI\\ComboBox.cpp',
			'src\\ARK2d\\UI\\ComboBoxItem.cpp',
			'src\\ARK2D\\ARK2D_Path\\PathGroup.cpp',
			'src\\ARK2D\\ARK2D_Path\\Path.cpp',
			'src\\ARK2D\\ARK2D_Path\\SubPath.cpp',
			'src\\ARK2D\\ARK2D_Path\\PathIO.cpp',
			'src\\ARK2D\\ARK2D_State\\EmptyTransition.cpp',
			'src\\ARK2D\\ARK2D_State\\FadeTransition.cpp',
			'src\\ARK2D\\ARK2D_State\\GameState.cpp',
			'src\\ARK2D\\ARK2D_State\\LoadingState.cpp',
			'src\\ARK2D\\ARK2D_State\\SlideRectanglesAcrossTransition.cpp',
			'src\\ARK2D\\ARK2D_State\\StateBasedGame.cpp',
			'src\\ARK2D\\ARK2D_State\\Transition.cpp',
			'src\\ARK2D\\ARK2D_State\\TranslateOutInTransition.cpp',
			'src\\ARK2D\\ARK2D_Util\\VerticalMenu.cpp',
			'src\\ARK2D\\ARK2D_Util\\VerticalMenuItem.cpp',
			'src\\ARK2D\\particles\\ConfigurableEmitter.cpp',
			'src\\ARK2D\\particles\\Particle.cpp',
			'src\\ARK2D\\particles\\ParticleIO.cpp',
			'src\\ARK2D\\particles\\ParticlePool.cpp',
			'src\\ARK2D\\particles\\ParticleSystem.cpp',
			'src\\ARK2D\\vendor\\libJSON\\JSON_Worker.cpp',
			'src\\ARK2D\\vendor\\libJSON\\jsonmain.cpp',
			'src\\ARK2D\\vendor\\libJSON\\JSONNode.cpp',
			'src\\ARK2D\\vendor\\lpng151\\png.c',
			'src\\ARK2D\\vendor\\lpng151\\pngerror.c',
			'src\\ARK2D\\vendor\\lpng151\\pngget.c',
			'src\\ARK2D\\vendor\\lpng151\\pngmem.c',
			'src\\ARK2D\\vendor\\lpng151\\pngpread.c',
			'src\\ARK2D\\vendor\\lpng151\\pngread.c',
			'src\\ARK2D\\vendor\\lpng151\\pngrio.c',
			'src\\ARK2D\\vendor\\lpng151\\pngrtran.c',
			'src\\ARK2D\\vendor\\lpng151\\pngrutil.c',
			'src\\ARK2D\\vendor\\lpng151\\pngset.c',
			'src\\ARK2D\\vendor\\lpng151\\pngtrans.c',
			'src\\ARK2D\\vendor\\lpng151\\pngwio.c',
			'src\\ARK2D\\vendor\\lpng151\\pngwrite.c',
			'src\\ARK2D\\vendor\\lpng151\\pngwtran.c',
			'src\\ARK2D\\vendor\\lpng151\\pngwutil.c',
			'src\\ARK2D\\vendor\\ogg\\bitwise.c',
			'src\\ARK2D\\vendor\\ogg\\framing.c',
			'src\\ARK2D\\vendor\\tinyxml\\tinystr.cpp',
			'src\\ARK2D\\vendor\\tinyxml\\tinyxml.cpp',
			'src\\ARK2D\\vendor\\tinyxml\\tinyxmlerror.cpp',
			'src\\ARK2D\\vendor\\tinyxml\\tinyxmlparser.cpp',
			'src\\ARK2D\\vendor\\vorbis\\analysis.c',
			'src\\ARK2D\\vendor\\vorbis\\barkmel.c',
			'src\\ARK2D\\vendor\\vorbis\\bitrate.c',
			'src\\ARK2D\\vendor\\vorbis\\block.c',
			'src\\ARK2D\\vendor\\vorbis\\codebook.c',
			'src\\ARK2D\\vendor\\vorbis\\envelope.c',
			'src\\ARK2D\\vendor\\vorbis\\floor0.c',
			'src\\ARK2D\\vendor\\vorbis\\floor1.c',
			'src\\ARK2D\\vendor\\vorbis\\info.c',
			'src\\ARK2D\\vendor\\vorbis\\lookup.c',
			'src\\ARK2D\\vendor\\vorbis\\lpc.c',
			'src\\ARK2D\\vendor\\vorbis\\lsp.c',
			'src\\ARK2D\\vendor\\vorbis\\mapping0.c',
			'src\\ARK2D\\vendor\\vorbis\\mdct.c',
			'src\\ARK2D\\vendor\\vorbis\\psy.c',
			'src\\ARK2D\\vendor\\vorbis\\registry.c',
			'src\\ARK2D\\vendor\\vorbis\\res0.c',
			'src\\ARK2D\\vendor\\vorbis\\sharedbook.c',
			'src\\ARK2D\\vendor\\vorbis\\smallft.c',
			'src\\ARK2D\\vendor\\vorbis\\synthesis.c',
			'src\\ARK2D\\vendor\\vorbis\\tone.c',
			'src\\ARK2D\\vendor\\vorbis\\vorbisfile.c',
			'src\\ARK2D\\vendor\\vorbis\\window.c',
			'src\\ARK2D\\vendor\\zlib123\\adler32.c',
			'src\\ARK2D\\vendor\\zlib123\\compress.c',
			'src\\ARK2D\\vendor\\zlib123\\crc32.c',
			'src\\ARK2D\\vendor\\zlib123\\deflate.c',
			'src\\ARK2D\\vendor\\zlib123\\gzio.c',
			'src\\ARK2D\\vendor\\zlib123\\infback.c',
			'src\\ARK2D\\vendor\\zlib123\\inffast.c',
			'src\\ARK2D\\vendor\\zlib123\\inflate.c',
			'src\\ARK2D\\vendor\\zlib123\\inftrees.c',
			'src\\ARK2D\\vendor\\zlib123\\trees.c',
			'src\\ARK2D\\vendor\\zlib123\\uncompr.c',
			'src\\ARK2D\\vendor\\zlib123\\zutil.c'
		]);
	
		self.dll_files.extend([
			'lib\\kernel32.dll', 
			'lib\\glew32.dll', 
			'lib\\OpenAL32.dll', 
			'lib\\alut.dll', 
			'lib\\winmm.dll'
		]);
		self.static_libraries.extend([
			'glu32', 
			'winmm', 
			'opengl32', 
			'mingw32'
		]);
		
		if (sys.platform == "win32"):
			self.linkingFlags = " -mwindows -shared ";
		
		
	def gamePreInit(self):
	
		self.src_files[:] = []; # clear the lists.
		self.dll_files[:] = [];
		self.static_libraries[:] = [];
		
		self.mkdirs.extend([
			 self.build_folder + "\\build-cache"
		]);
		
	def gamePostInit(self):
	
		if (sys.platform == "win32"):
			self.build_artifact = self.build_folder + "\\" + self.game_name.replace(" ", "%20") + ".exe";
			
			
	def createCacheFile(self, path):
		try:
			f = open(path, "r");
			f.close();
		except IOError as e:
			f = open(path, "w");
			f.write("{}");
			f.close();
		
	def start(self):
	
		if (sys.platform == "win32"):
			print("Hurray for windows");
			
			#prepare dirs
			for h in self.mkdirs:
				subprocess.call(["mkdir " + h + ""], shell=True);
			
			# make sure cache file exists
			
			# compile cache thing
			cachefilename = self.build_folder + "\\build-cache\\compiled.json";
			self.createCacheFile(cachefilename);
			f = open(cachefilename, "r")
			fcontents = f.read();
			f.close();
			fjson = json.loads(fcontents);
			fchanged = False;
			
				
			#compile
			for h in self.src_files:
				compileStr = "";
				
				findex = h.rfind('.');
				h_ext = h[findex+1:len(h)];
				newf = h[0:findex] + ".o";
				
				if h_ext == 'c':
					compileStr += "gcc";
				else:
					compileStr += "g++";
					
				if (not h in fjson or fjson[h]['date_modified'] < os.stat(h).st_mtime):
					compileStr += " -O3 -Wall -c -fmessage-length=0 -o";
					compileStr += self.build_folder + "\\" + newf + " " + h + " ";
				
					fjson[h] = {"date_modified": os.stat(h).st_mtime };
				
					print(compileStr);
					subprocess.call([compileStr], shell=True);	
					fchanged = True;
		
		
			# update compile cache thing
			if (fchanged == True):
				f = open(self.build_folder + "\\build-cache\\compiled.json", "w")
				f.write(json.dumps(fjson, sort_keys=True, indent=4));
				f.close();
		
			#link
			print("---");
			
			linkingStr = "";
			linkingStr += "g++ " + self.mingw_link + " " + self.linkingFlags + " -o" + self.build_artifact;
			for h in self.src_files:
				findex = h.rfind('.');
				newf = h[0:findex] + ".o";
				#print(newf);
				linkingStr += " " + self.build_folder + "\\" + newf;
				
			for f in self.dll_files:
				linkingStr += " " + f;
				
			for f in self.static_libraries:
				linkingStr += " -l" + f;
				
			print(linkingStr);
			
			subprocess.call([linkingStr], shell=True);
		else:
			print("not windows. not supported yet");

	
	pass;
	
		
if __name__ == "__main__":
	
	try:
		#print(sys.argv[1]);
		j = json.loads(sys.argv[1].replace("%20", " "));
		
		a = ARK2DBuildSystem();
		a.gamePreInit();
		
		a.ark2d_dir = j["ark2d_dir"]
		a.game_name = j["game_name"];
		a.game_dir  = j["game_dir"];
		a.src_files.extend(j["game_src_files"]);
		
		a.mingw_link = "";
		a.dll_files.append(a.ark2d_dir + '\\' + a.build_folder + '\\libARK2D.dll');
		a.linkingFlags = " -mwindows -enable-auto-import ";
		
		a.gamePostInit();
		a.start();
		
		#copyCommand = "copy /y " + a.ark2d_dir + '\\' + a.build_folder + '\\libARK2D.dll \"' + a.game_dir + '\\' + a.build_folder + '\\libARK2D.dll\"';
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
