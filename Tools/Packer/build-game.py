import sys
import os
import subprocess
import json
import zlib 
import base64
import binascii

if __name__ == "__main__":
	
	a = dict();
	a['game_short_name'] ="packer";
	a['game_name'] = "Packer";
	
	if (sys.platform == "win32"):
		a['ark2d_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\ARK2D";
		a['game_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\Tools\\Packer";
		comm = "C:\Python31\python.exe ../../ARK2D/build.py ";
		ds = "\\";
	elif (sys.platform == "darwin"):
		a['ark2d_dir'] = "/Users/ashleygwinnell/Projects/C++/ark2d/ARK2D";
		a['game_dir'] = "/Users/ashleygwinnell/Projects/C++/ark2d/Tools/Packer";
		comm = "/usr/local/bin/python3.1 ../../ARK2D/build.py ";
		ds = "/";
	
	a['game_src_files'] = [
		"src" + ds + "main.cpp"
	];
	
	a['game_mkdirs'] = [];
	a['game_is_commandline'] = True;

	b = json.dumps(a, separators=(',',':')); #.replace(" ", "-"); #.replace("\"", "\\\"").replace("&", "\&");
	b = str.encode(b);
	b = base64.b64encode(b);
	b = str(b);
	#print(b);
	subprocess.call([comm + b], shell=True);
	subprocess.call(["exec /Users/ashleygwinnell/Projects/C++/ark2d/Tools/Packer/build_release/Packer.app/Contents/MacOS/Packer output.test sound1.ogg sound2.ogg"], shell=True);