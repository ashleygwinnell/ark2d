package org.ashleygwinnell.ark2dprojectmanager;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Insets;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextArea;
import javax.swing.JTextField;

public class GameDataPanel extends JPanel {

	JTextField game_name;
	JTextField game_name_safe;
	JTextField game_class_name;
	JTextArea game_description;
	
	JRadioButton gameOrientationPortrait;
	JRadioButton gameOrientationLandscape;
	
	JPanel clear_color;
	
	JTextField dir_windows;
	JTextField dir_osx;
	
	public GameDataPanel() {
		
		
		JLabel gameNameLbl = new JLabel("Name:");
		this.add(gameNameLbl);
		
		game_name = new JTextField(40);
		this.add(game_name);
		
		
		JLabel gameClassNameSafeLbl = new JLabel("Name (Safe):");
		this.add(gameClassNameSafeLbl);
		
		game_name_safe = new JTextField(40);
		this.add(game_name_safe);
		
		JLabel gameClassNameLbl = new JLabel("Class Name:");
		this.add(gameClassNameLbl);
		
		game_class_name = new JTextField(40);
		this.add(game_class_name);
		
		
		
		JLabel gameDescriptionLbl = new JLabel("Description:");
		this.add(gameDescriptionLbl);
		
		game_description = new JTextArea();
		game_description.setPreferredSize(new Dimension(480, 40));
		game_description.setBorder(BorderFactory.createLineBorder(Color.black));
		game_description.setMargin(new Insets(10,10,10,10));
		game_description.setWrapStyleWord(true);
		game_description.setLineWrap(true);
		game_description.setFont(new Font("Arial", Font.PLAIN, 12));
		this.add(game_description);
		
		ButtonGroup bg1 = new ButtonGroup();
		JLabel gameOrientationLbl = new JLabel("Orientation:");
		gameOrientationPortrait = new JRadioButton("Portrait");
		gameOrientationLandscape = new JRadioButton("Landscape");
		bg1.add(gameOrientationPortrait);
		bg1.add(gameOrientationLandscape);
		
		this.add(gameOrientationLbl);
		this.add(gameOrientationPortrait);
		this.add(gameOrientationLandscape);
		
		JPanel spacer = new JPanel();
		spacer.setPreferredSize(new Dimension(480, 5));
		add(spacer);
		
		JLabel clearColorLbl = new JLabel("Clear Color:");
		clear_color = new JPanel();
		clear_color.setPreferredSize(new Dimension(20,20));
		clear_color.setBackground(ProjectManager.hex2rgb("#333333"));
		clear_color.setBorder(BorderFactory.createLineBorder(Color.black));
		JButton clearColorBtn = new JButton("Pick Color");
		
		
		
		//JColorChooser clearColorCh = new JColorChooser(Color.DARK_GRAY);
		this.add(clearColorLbl);
		this.add(clear_color);
		//this.add(clearColorCh);
		this.add(clearColorBtn);
		 
		JPanel spacer2 = new JPanel();
		spacer2.setPreferredSize(new Dimension(480, 5));
		add(spacer2);
		
		// directory
		JLabel directoryWindowsLbl = new JLabel("Directory (Windows):");
		this.add(directoryWindowsLbl);
		
		dir_windows = new JTextField(40);
		this.add(dir_windows);
		
		JLabel directoryOSXLbl = new JLabel("Directory (OS X):");
		this.add(directoryOSXLbl);
		
		dir_osx = new JTextField(40);
		this.add(dir_osx);
		
	
		// misc
		this.setPreferredSize(new Dimension(500, 400));
		this.setBorder(BorderFactory.createTitledBorder("Game Information"));
		
		
	}
	
	
	
	
}
