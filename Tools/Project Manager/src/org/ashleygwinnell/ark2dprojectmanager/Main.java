package org.ashleygwinnell.ark2dprojectmanager;

import javax.swing.JFrame;
import javax.swing.UIManager;

/*

// manager file
{
	"project_groups": [
		{
			"name": "ARK2D",
			"projects": [
				{
					"name": "ARK2D",
					"dir": {
						"windows": "",
						"osx": ""
					}
				}
			]
		},
		{
			"name": "Custom",
			"projects": [
				{
					"name": "Toast Time",
					"dir": {
						"windows": "",
						"osx": ""
					}
				}
			]
		}
	 ]
}



//new config file.
{
	"config_version": 1,
	"clean": false,
	"game": {
	    "name": {
	        "normal": "Royal Tea",
	        "safe": "royal_tea",
	        "class": "DefaultGame"
	    },
	    "description": "Play as the King of Dunn - the most royal cup of tea ever.",
	    "orientation": "portrait",
	    "clear_color": "#333333",
	    "width": "480",
	    "height": "640",
	    "dir": {
	        "windows": "C: \\Dropbox\\ark2d-examples\\RoyalTea",
	        "mac": "/Users/ashleygwinnell/Dropbox/ark2d-examples/RoyalTea"
	    },
	    "audio_quality": {
	        "windows": 9,
	        "mac": 9,
	        "linux": 9,
	        "android": 1
	    },
	    "android": {
	        "sdk": {
	            "version": 8
	        },
	        "ndk": {
	            "version": 5,
	            "dir": {
	                "windows": "",
	                "mac": "",
	                "linux": ""
	            }
	        },
	        "permissions": [
	            "WRITE_EXTERNAL_STORAGE"
	        ],
	        "app_to_sd": true
	    }
	    "deployments": {
	    	"default": {
			    "src_files": [
			        "src/main.cpp",
			        "src/DefaultGame.cpp",
			        "src/states/MenuState.cpp",
			        "src/states/InGameState.cpp",
			        "src/states/SummaryState.cpp",
			        "src/objects/Player.cpp",
			        "src/objects/Platform.cpp",
			        "src/objects/Pickup.cpp"
			    ],
			    "additional_command_line": ""
			}
		}
	},
	"developer": {
	    "name": "AshleyGwinnell",
	    "safe_name": "ashleygwinnell"
	},
	"ark2d": {
	    "dir": {
	        "windows": "C: \\Dropbox\\ARK2D\\ark2d",
	        "mac": "/Users/ashleygwinnell/Dropbox/ark2d/ARK2D",
	        "linux": ""
	    }
	},
	"python": {
	    "executable": {
	        "windows": "C: \\Python31\\python.exe",
	        "mac": "/usr/local/bin/python3.1",
	        "linux": ""
	    }
	},
	"hooks": {
	    "pre_scripts": [],
	    "post_scripts": []
	}
}


*/

public class Main {
	public static void main (String[] args) 
	{
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} 
		catch (Exception e) { }
		 
		JFrame.setDefaultLookAndFeelDecorated(false);
		
		try {
			ProjectManager._instance = new ProjectManager();
			ProjectManager._instance.init();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
}
