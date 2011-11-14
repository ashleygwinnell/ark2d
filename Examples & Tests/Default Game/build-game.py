import sys
import os
import subprocess
import json
import zlib 
import base64
import binascii

if __name__ == "__main__":

	a = dict();
	a['game_short_name'] ="default_game";
	a['game_name'] = "ARK2D Default Game";
	
	if (sys.platform == "win32"):
		a['ark2d_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\ARK2D";
		a['game_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\Examples & Tests\\Default Game";
		comm = "C:\Python31\python.exe ../../ARK2D/build.py ";
		ds = "\\";
	elif (sys.platform == "darwin"):
		a['ark2d_dir'] = "/Users/ashleygwinnell/Projects/C++/ark2d/ARK2D";
		a['game_dir'] = "/Users/ashleygwinnell/Projects/C++/ark2d/Examples & Tests/Default Game";
		comm = "/usr/local/bin/python3.1 ../../ARK2D/build.py ";
		ds = "/";
		#a['mac_game_icns'] = "Game.icns"
		
	
	a['game_src_files'] = [
		"src" + ds + "main.cpp", 
		"src" + ds + "DefaultGame.cpp"
	];
	
	a['game_mkdirs'] = [];
	
	b = json.dumps(a, separators=(',',':'));#.replace(" ", "-"); #.replace("\"", "\\\"").replace("&", "\&");
	b = base64.b64encode(b);
	
	subprocess.call([comm + b], shell=True);
