import sys
import os
import subprocess
import json 

if __name__ == "__main__":
	a = dict();
	a['ark2d_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\ARK2D";
	
	a['game_name'] = "Baseball";
	a['game_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\Examples & Tests\\Baseball";
	a['game_src_files'] = [
		"src\\main.cpp", 
		"src\\Baseball.cpp", 
		"src\\BlankState.cpp", 
		"src\\Boss.cpp", 
		"src\\DefaultGame.cpp", 
		"src\\FailState.cpp", 
		"src\\GameObject.cpp", 
		"src\\InGameState.cpp", 
		"src\\InstructionsState.cpp", 
		"src\\MenuState.cpp",
		"src\\Player.cpp",
		"src\\WinState.cpp"
	];
	
	b = json.dumps(a, separators=(',',':')).replace(" ", "-");
	
	subprocess.call(["C:\Python31\python.exe ../../ARK2D/build.py " + b + ""], shell=True);