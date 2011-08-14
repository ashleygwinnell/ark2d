import sys
import os
import subprocess
import json
import zlib 
import base64
import binascii

if __name__ == "__main__":
	
	a = dict();
	a['game_short_name'] ="hotcrossbunions";
	a['game_name'] = "HOT CROSS BUNIONS";
	
	if (sys.platform == "win32"):
		a['ark2d_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\ARK2D";
		a['game_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\Examples & Tests\\Hot Cross Bunions";
		comm = "C:\Python31\python.exe ../../ARK2D/build.py ";
		ds = "\\";
	elif (sys.platform == "darwin"):
		a['ark2d_dir'] = "/Users/ashleygwinnell/Projects/C++/ark2d/ARK2D";
		a['game_dir'] = "/Users/ashleygwinnell/Projects/C++/ark2d/Examples & Tests/Hot Cross Bunions";
		comm = "/usr/local/bin/python3.1 ../../ARK2D/build.py ";
		ds = "/";
	
	a['game_src_files'] = [
		"src" + ds + "main.cpp", 
		"src" + ds + "BlankState.cpp", 
		"src" + ds + "DefaultGame.cpp", 
		"src" + ds + "FallingPoint.cpp",
		"src" + ds + "FloatingFadingText.cpp",
		"src" + ds + "GameObject.cpp",
		"src" + ds + "HighscoresComponent.cpp",
		"src" + ds + "InGameState.cpp",
		"src" + ds + "Ingredient.cpp",
		"src" + ds + "InstructionsState.cpp",
		"src" + ds + "MenuState.cpp",
		"src" + ds + "Player.cpp",
		"src" + ds + "Recipe.cpp",
		"src" + ds + "ViewHighscoresState.cpp",
		"src" + ds + "WinState.cpp"
	];
	
	a['game_mkdirs'] = [];
	a['game_resources_dir'] = a['game_dir'] + ds + "data";
	a['mac_game_icns'] = a['game_resources_dir'] + ds + "icon.icns";
	
	b = json.dumps(a, separators=(',',':'));#.replace(" ", "-"); #.replace("\"", "\\\"").replace("&", "\&");
	b = base64.b64encode(b);
	
	subprocess.call([comm + b], shell=True);