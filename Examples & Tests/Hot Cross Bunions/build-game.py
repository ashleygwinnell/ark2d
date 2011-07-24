import sys
import os
import subprocess
import json 

if __name__ == "__main__":
	a = dict();
	a['ark2d_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\ARK2D";
	
	a['game_name'] = "HOT CROSS BUNIONS";
	a['game_dir'] = "C:\\xampp\\htdocs\\C++\\ARK2D\\Examples & Tests\\Hot Cross Bunions";
	a['game_src_files'] = [
		"src\\main.cpp", 
		"src\\BlankState.cpp", 
		"src\\DefaultGame.cpp", 
		"src\\FallingPoint.cpp",
		"src\\FloatingFadingText.cpp",
		"src\\GameObject.cpp",
		"src\\HighscoresComponent.cpp",
		"src\\InGameState.cpp",
		"src\\Ingredient.cpp",
		"src\\InstructionsState.cpp",
		"src\\MenuState.cpp",
		"src\\Player.cpp",
		"src\\Recipe.cpp",
		"src\\ViewHighscoresState.cpp",
		"src\\WinState.cpp"
	];
	
	b = json.dumps(a, separators=(',',':')).replace(" ", "_");
	
	subprocess.call(["C:\Python31\python.exe ../../ARK2D/build.py " + b + ""], shell=True);