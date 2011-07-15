import sys
import os
import subprocess
import json 

if __name__ == "__main__":
	a = dict();
	a['ark2d_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\ARK2D";
	
	a['game_name'] = "ARK2D Path Editor";
	a['game_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\Examples & Tests\\ARK2D Path Editor";
	a['game_src_files'] = ["src\\main.cpp", "src\\DefaultGame.cpp"];
	
	b = json.dumps(a, separators=(',',':')).replace(" ", "%20");
	
	subprocess.call(["C:\Python31\python.exe ../../ARK2D/build.py " + b + ""], shell=True);