package org.ashleygwinnell.ark2dprojectmanager;

import javax.swing.JTabbedPane;

public class ProjectPanel extends JTabbedPane {

	//public ARK2DDataPanel ark2d;
	GameDataPanel gamedata;
	DeveloperDataPanel devdata;
	public ProjectPanel() {
		//add(new ARK2DDataPanel());
		//add(new PythonDataPanel());
		//add(new DeveloperDataPanel());
		//add(new GameDataPanel());
		//ark2d = new ARK2DDataPanel();
		
		//this.addTab("ARK2D", ark2d);
		gamedata = new GameDataPanel();
		devdata = new DeveloperDataPanel();
		this.addTab("Game", gamedata);
		this.addTab("Developer", devdata);
	}
	
}
