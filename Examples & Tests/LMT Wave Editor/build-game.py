import sys
import os
import subprocess
import json
import zlib 
import base64
import binascii

if __name__ == "__main__":
	a = dict();
	a['game_short_name'] ="lmtwaveeditor";
	a['game_name'] = "LMT Wave Editor";
	a['clean'] = False;
	
	if (sys.platform == "win32"):
		a['ark2d_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\ARK2D";
		a['game_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\Examples & Tests\\LMT Wave Editor";
		comm = "C:\Python31\python.exe ../../ARK2D/build.py ";
		ds = "\\";	
	elif (sys.platform == "darwin"):
		a['ark2d_dir'] = "/Users/ashleygwinnell/Dropbox/ark2d/ARK2D";
		a['game_dir'] = "/Users/ashleygwinnell/Dropbox/ark2d/Examples & Tests/LMT Wave Editor";
		comm = "/usr/local/bin/python3.1 ../../ARK2D/build.py ";
		ds = "/";
	
	a['game_src_files'] = [
		"src" + ds + "main.cpp", 
		"src" + ds + "DefaultGame.cpp",
		"src" + ds + "EditorEnemy.cpp",
		"src" + ds + "EditorLevel.cpp",
		"src" + ds + "EditorWave.cpp",
		"src" + ds + "EnemyButton.cpp",
		"src" + ds + "EnemySelectPanel.cpp",
		"src" + ds + "GamePanel.cpp",
		"src" + ds + "LevelEditorState.cpp",
		"src" + ds + "LevelGamePanel.cpp",
		"src" + ds + "LevelToolPanel.cpp",
		"src" + ds + "ToolPanel.cpp",
		"src" + ds + "WaveEditorState.cpp"
	];
	
	if (sys.platform == "win32"):
		a['game_src_files'].extend(["src" + ds + "Resources.rc"]);
	
	a['game_mkdirs'] = [];
	a['game_resources_dir'] = a['game_dir'] + ds + "data";
	# a['mac_game_icns'] = a['game_resources_dir'] + ds + "icon.icns";
	
	b = json.dumps(a, separators=(',',':'));
	b = str.encode(b);
	b = base64.b64encode(b);
	b = str(b);
	
	subprocess.call([comm + b], shell=True);