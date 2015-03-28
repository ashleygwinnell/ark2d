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

if __name__ == "__main__":
	
	print("-----------------");
	print("Debugging");

	dir = sys.path[0];
	platform = "android";
	
	count = 0;
	print("Command line arguments: " + str(len(sys.argv)));
	for item in sys.argv:
		print(str(count) + ": " + item);

		if count == 0:
			count += 1;
			continue;

		parts = item.split("=");
		if len(parts) > 2:
			print("Item " + item + " is invalid");
			continue;
		if parts[0] == "platform":
			platform = parts[1];

		count += 1;

	print("Platform: " + platform);

	game_dir = os.getcwd();
	ark2d_dir = os.path.dirname(os.path.realpath(__file__));

	print("ark2d dir: " + ark2d_dir);
	print("game_dir dir: " + game_dir);
	#exit(0);

	if (platform == "android"):
		print("---");
		print("Opening config file: " + ark2d_dir + "/config.json");
		f = open(ark2d_dir + "/config.json", "r");
		fcontents = f.read();
		f.close();

		try:
			config = json.loads(fcontents);
		except:
			print("config.json invalid.");
			exit(1);


		sdk_dir = "";
		ndk_dir = "";
		if (sys.platform == "win32"):
			sdk_dir = config['windows']['android']['sdk_dir']; 
			ndk_dir = config['windows']['android']['ndk_dir']; 
		elif (sys.platform == "darwin"):
			sdk_dir = config['osx']['android']['sdk_dir']; 
			ndk_dir = config['osx']['android']['ndk_dir']; 

		print("sdk dir: " + sdk_dir);
		print("ndk dir: " + ndk_dir);

		print("-----------------");
		os.system(sdk_dir + "/platform-tools/adb logcat | " + ndk_dir + "/ndk-stack -sym " + game_dir + "/build/android/project/obj/local/armeabi-v7a");

	pass;
	# 





	