import sys
import os
import subprocess
import json 

def main():
	build_folder = "build_release";
	build_artifact = build_folder + "\\libARK2D.dll"
	src_files = [
		'src\\main.cpp',
		'src\\ARK2D\\ogg\\bitwise.c',
		'src\\ARK2D\\ogg\\framing.c',
		'src\\ARK2D\\vorbis\\analysis.c',
		'src\\ARK2D\\vorbis\\barkmel.c',
		'src\\ARK2D\\vorbis\\bitrate.c',
		'src\\ARK2D\\vorbis\\block.c',
		'src\\ARK2D\\vorbis\\codebook.c',
		'src\\ARK2D\\vorbis\\envelope.c',
		'src\\ARK2D\\vorbis\\floor0.c',
		'src\\ARK2D\\vorbis\\floor1.c',
		'src\\ARK2D\\vorbis\\info.c',
		'src\\ARK2D\\vorbis\\lookup.c',
		'src\\ARK2D\\vorbis\\lpc.c',
		'src\\ARK2D\\vorbis\\lsp.c',
		'src\\ARK2D\\vorbis\\mapping0.c',
		'src\\ARK2D\\vorbis\\mdct.c',
		'src\\ARK2D\\vorbis\\psy.c',
		'src\\ARK2D\\vorbis\\registry.c',
		'src\\ARK2D\\vorbis\\res0.c',
		'src\\ARK2D\\vorbis\\sharedbook.c',
		'src\\ARK2D\\vorbis\\smallft.c',
		'src\\ARK2D\\vorbis\\synthesis.c',
		'src\\ARK2D\\vorbis\\tone.c',
		'src\\ARK2D\\vorbis\\vorbisfile.c',
		'src\\ARK2D\\vorbis\\window.c',
		'src\\ARK2D\\zlib123\\adler32.c',
		'src\\ARK2D\\zlib123\\compress.c',
		'src\\ARK2D\\zlib123\\crc32.c',
		'src\\ARK2D\\zlib123\\deflate.c',
		'src\\ARK2D\\zlib123\\gzio.c',
		'src\\ARK2D\\zlib123\\infback.c',
		'src\\ARK2D\\zlib123\\inffast.c',
		'src\\ARK2D\\zlib123\\inflate.c',
		'src\\ARK2D\\zlib123\\inftrees.c',
		'src\\ARK2D\\zlib123\\trees.c',
		'src\\ARK2D\\zlib123\\uncompr.c',
		'src\\ARK2D\\zlib123\\zutil.c'
	];
	mingw_dir = "C:\\MinGW";
	dll_files = [
		'lib\\kernel32.dll', 
		'lib\\glew32.dll', 
		'lib\\OpenAL32.dll', 
		'lib\\alut.dll', 
		'lib\\winmm.dll'
	];
	static_libraries = [
		'glu32', 
		'winmm', 
		'opengl32', 
		'mingw32'
	];
	
	if (sys.platform == "win32"):
		print("Hurray for windows");
		
		#prepare dirs
		subprocess.call(["mkdir " + build_folder + ""], shell=True);
		subprocess.call(["mkdir " + build_folder + "\\src"], shell=True);
		subprocess.call(["mkdir " + build_folder + "\\src\\ARK2D"], shell=True);
		subprocess.call(["mkdir " + build_folder + "\\src\\ARK2D\\ogg"], shell=True);
		subprocess.call(["mkdir " + build_folder + "\\src\\ARK2D\\vorbis"], shell=True);
		subprocess.call(["mkdir " + build_folder + "\\src\\ARK2D\\zlib123"], shell=True);
		
		# compile cache thing
		subprocess.call(["mkdir " + build_folder + "\\build-cache"], shell=True);
		f = open(build_folder + "\\build-cache\\compiled.json", "r")
		fcontents = f.read();
		f.close();
		fjson = json.loads(fcontents);
		fchanged = False;
		
			
		#compile
		for h in src_files:
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
				compileStr += build_folder + "\\" + newf + " " + h + " ";
			
				fjson[h] = {"date_modified": os.stat(h).st_mtime };
			
				print(compileStr);
				subprocess.call([compileStr], shell=True);	
				fchanged = True;
	
	
		# update compile cache thing
		if (fchanged == True):
			f = open(build_folder + "\\build-cache\\compiled.json", "w")
			f.write(json.dumps(fjson, sort_keys=True, indent=4));
			f.close();
	
		#link
		print("---");
		
		linkingStr = "";
		linkingStr += "g++ -L" + mingw_dir + "\\lib -mwindows -shared -o" + build_artifact;
		for h in src_files:
			findex = h.rfind('.');
			newf = h[0:findex] + ".o";
			#print(newf);
			linkingStr += " " + build_folder + "\\" + newf;
			
		"""
		
		linkingStr += build_folder + "\\src\\main.o " + build_folder + "\\src\\ARK2D\\zlib123\\zutil.o " + build_folder + "\\src\\ARK2D\\zlib123\\uncompr.o " + build_folder + "\\src\\ARK2D\\zlib123\\trees.o " + build_folder + "\\src\\ARK2D\\zlib123\\inftrees.o " + build_folder + "\\src\\ARK2D\\zlib123\\inflate.o " + build_folder + "\\src\\ARK2D\\zlib123\\inffast.o " + build_folder + "\\src\\ARK2D\\zlib123\\infback.o " + build_folder + "\\src\\ARK2D\\zlib123\\gzio.o " + build_folder + "\\src\\ARK2D\\zlib123\\deflate.o " + build_folder + "\\src\\ARK2D\\zlib123\\crc32.o " + build_folder + "\\src\\ARK2D\\zlib123\\compress.o " + build_folder + "\\src\\ARK2D\\zlib123\\adler32.o " + build_folder + "\\src\\ARK2D\\vorbis\\window.o " + build_folder + "\\src\\ARK2D\\vorbis\\vorbisfile.o " + build_folder + "\\src\\ARK2D\\vorbis\\tone.o " + build_folder + "\\src\\ARK2D\\vorbis\\synthesis.o " + build_folder + "\\src\\ARK2D\\vorbis\\smallft.o " + build_folder + "\\src\\ARK2D\\vorbis\\sharedbook.o " + build_folder + "\\src\\ARK2D\\vorbis\\res0.o " + build_folder + "\\src\\ARK2D\\vorbis\\registry.o " + build_folder + "\\src\\ARK2D\\vorbis\\psy.o " + build_folder + "\\src\\ARK2D\\vorbis\\mdct.o " + build_folder + "\\src\\ARK2D\\vorbis\\mapping0.o " + build_folder + "\\src\\ARK2D\\vorbis\\lsp.o " + build_folder + "\\src\\ARK2D\\vorbis\\lpc.o " + build_folder + "\\src\\ARK2D\\vorbis\\lookup.o " + build_folder + "\\src\\ARK2D\\vorbis\\info.o " + build_folder + "\\src\\ARK2D\\vorbis\\floor1.o " + build_folder + "\\src\\ARK2D\\vorbis\\floor0.o " + build_folder + "\\src\\ARK2D\\vorbis\\envelope.o " + build_folder + "\\src\\ARK2D\\vorbis\\codebook.o " + build_folder + "\\src\\ARK2D\\vorbis\\block.o " + build_folder + "\\src\\ARK2D\\vorbis\\bitrate.o " + build_folder + "\\src\\ARK2D\\vorbis\\barkmel.o " + build_folder + "\\src\\ARK2D\\vorbis\\analysis.o " + build_folder + "\\src\\ARK2D\\tinyxml\\tinyxmlparser.o " + build_folder + "\\src\\ARK2D\\tinyxml\\tinyxmlerror.o " + build_folder + "\\src\\ARK2D\\tinyxml\\tinyxml.o " + build_folder + "\\src\\ARK2D\\tinyxml\\tinystr.o " + build_folder + "\\src\\ARK2D\\particles\\ParticleSystem.o " + build_folder + "\\src\\ARK2D\\particles\\ParticlePool.o " + build_folder + "\\src\\ARK2D\\particles\\ParticleIO.o " + build_folder + "\\src\\ARK2D\\particles\\Particle.o " + build_folder + "\\src\\ARK2D\\particles\\ConfigurableEmitter.o " + build_folder + "\\src\\ARK2D\\ogg\\framing.o " + build_folder + "\\src\\ARK2D\\ogg\\bitwise.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngwutil.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngwtran.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngwrite.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngwio.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngtrans.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngset.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngrutil.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngrtran.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngrio.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngread.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngpread.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngmem.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngget.o " + build_folder + "\\src\\ARK2D\\lpng151\\pngerror.o " + build_folder + "\\src\\ARK2D\\lpng151\\png.o " + build_folder + "\\src\\ARK2D\\libJSON\\jsonmain.o " + build_folder + "\\src\\ARK2D\\libJSON\\JSON_Worker.o " + build_folder + "\\src\\ARK2D\\libJSON\\JSONNode.o " + build_folder + "\\src\\ARK2D\\glew.o " + build_folder + "\\src\\ARK2D\\UISimpleButton.o " + build_folder + "\\src\\ARK2D\\UIButton.o " + build_folder + "\\src\\ARK2D\\Timeline.o " + build_folder + "\\src\\ARK2D\\TiledMapTileset.o " + build_folder + "\\src\\ARK2D\\TiledMapTile.o " + build_folder + "\\src\\ARK2D\\TiledMapProperty.o " + build_folder + "\\src\\ARK2D\\TiledMapObjectGroup.o " + build_folder + "\\src\\ARK2D\\TiledMapObject.o " + build_folder + "\\src\\ARK2D\\TiledMapLayer.o " + build_folder + "\\src\\ARK2D\\TiledMap.o " + build_folder + "\\src\\ARK2D\\Texture.o " + build_folder + "\\src\\ARK2D\\TargaImage.o " + build_folder + "\\src\\ARK2D\\StringUtil.o " + build_folder + "\\src\\ARK2D\\StringStore.o " + build_folder + "\\src\\ARK2D\\SpriteSheetDescriptionItem.o " + build_folder + "\\src\\ARK2D\\SpriteSheetDescription.o " + build_folder + "\\src\\ARK2D\\SpriteSheet.o " + build_folder + "\\src\\ARK2D\\SoundStore.o " + build_folder + "\\src\\ARK2D\\Sound.o " + build_folder + "\\src\\ARK2D\\Sort.o " + build_folder + "\\src\\ARK2D\\Shader.o " + build_folder + "\\src\\ARK2D\\PNGImage.o " + build_folder + "\\src\\ARK2D\\OutputWrapper.o " + build_folder + "\\src\\ARK2D\\MathUtil.o " + build_folder + "\\src\\ARK2D\\LocalHighscores.o " + build_folder + "\\src\\ARK2D\\Input.o " + build_folder + "\\src\\ARK2D\\Image.o " + build_folder + "\\src\\ARK2D\\Graphics.o " + build_folder + "\\src\\ARK2D\\GigaRectangleF.o " + build_folder + "\\src\\ARK2D\\GigaRectangle.o " + build_folder + "\\src\\ARK2D\\Gamepad.o " + build_folder + "\\src\\ARK2D\\GameTimer.o " + build_folder + "\\src\\ARK2D\\GameContainerWindows.o " + build_folder + "\\src\\ARK2D\\GameContainerLinux.o " + build_folder + "\\src\\ARK2D\\GameContainer.o " + build_folder + "\\src\\ARK2D\\Game.o " + build_folder + "\\src\\ARK2D\\FileUtil.o " + build_folder + "\\src\\ARK2D\\Event.o " + build_folder + "\\src\\ARK2D\\ErrorDialog.o " + build_folder + "\\src\\ARK2D\\Color.o " + build_folder + "\\src\\ARK2D\\Circle.o " + build_folder + "\\src\\ARK2D\\CameraShake.o " + build_folder + "\\src\\ARK2D\\BMPImage.o " + build_folder + "\\src\\ARK2D\\BMFont.o " + build_folder + "\\src\\ARK2D\\Animation.o " + build_folder + "\\src\\ARK2D\\ARKThread.o " + build_folder + "\\src\\ARK2D\\ARKString.o " + build_folder + "\\src\\ARK2D\\ARKMutex.o " + build_folder + "\\src\\ARK2D\\ARKGameObject.o " + build_folder + "\\src\\ARK2D\\ARKException.o " + build_folder + "\\src\\ARK2D\\ARK2D_util\\VerticalMenuItem.o " + build_folder + "\\src\\ARK2D\\ARK2D_util\\VerticalMenu.o " + build_folder + "\\src\\ARK2D\\ARK2D_state\\TranslateOutInTransition.o " + build_folder + "\\src\\ARK2D\\ARK2D_state\\Transition.o " + build_folder + "\\src\\ARK2D\\ARK2D_state\\StateBasedGame.o " + build_folder + "\\src\\ARK2D\\ARK2D_state\\SlideRectanglesAcrossTransition.o " + build_folder + "\\src\\ARK2D\\ARK2D_state\\LoadingState.o " + build_folder + "\\src\\ARK2D\\ARK2D_state\\GameState.o " + build_folder + "\\src\\ARK2D\\ARK2D_state\\FadeTransition.o src\\ARK2D\\ARK2D_state\\EmptyTransition.o " + build_folder + "\\src\\ARK2D\\ARK2D_path\\PathLoader.o " + build_folder + "\\src\\ARK2D\\ARK2D.o";
		"""
		for f in dll_files:
			linkingStr += " " + f;
			
		print(linkingStr);
		
		subprocess.call([linkingStr], shell=True);
	else:
		print("not windows. not supported yet");

		
if __name__ == "__main__":
	main()