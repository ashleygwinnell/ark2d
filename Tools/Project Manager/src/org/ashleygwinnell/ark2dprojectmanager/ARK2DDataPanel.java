package org.ashleygwinnell.ark2dprojectmanager;

import java.awt.Dimension;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class ARK2DDataPanel extends JPanel 
{
	public JTextField ark2d_dir_windows;
	public JTextField ark2d_dir_osx;
	
	public JTextField py_dir_windows;
	public JTextField py_dir_osx;
	
	public ARK2DDataPanel() 
	{
		
		JPanel content = new JPanel();
		
		// ark 2d
		JLabel directoryWindowsLbl = new JLabel("ARK2D Directory (Windows):");
		content.add(directoryWindowsLbl);
		
		ark2d_dir_windows = new JTextField(40);
		content.add(ark2d_dir_windows);
		
		
		JLabel directoryOSXLbl = new JLabel("ARK2D Directory (OS X):");
		content.add(directoryOSXLbl);
		
		ark2d_dir_osx = new JTextField(40);
		content.add(ark2d_dir_osx);
		
		// breaker
		JPanel panel = new JPanel();
		panel.setPreferredSize(new Dimension(500, 30));
		content.add(panel);
		
		// python
		JLabel pydirectoryWindowsLbl = new JLabel("Python Executable (Windows):");
		content.add(pydirectoryWindowsLbl);
		
		py_dir_windows = new JTextField(40);
		content.add(py_dir_windows);
		
		JLabel pydirectoryOSXLbl = new JLabel("Python Executable (OS X):");
		content.add(pydirectoryOSXLbl);
		
		py_dir_osx = new JTextField(40);
		content.add(py_dir_osx);
		
		
		// breaker
		JPanel panel2 = new JPanel();
		panel2.setPreferredSize(new Dimension(500, 30));
		content.add(panel2);
		
		
		
		
		
		content.setPreferredSize(new Dimension(500, 460));
		this.setBorder(BorderFactory.createTitledBorder("ARK2D Information"));
		this.add(content);
	}
}
