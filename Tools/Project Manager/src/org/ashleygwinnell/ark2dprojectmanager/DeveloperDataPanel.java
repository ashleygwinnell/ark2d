package org.ashleygwinnell.ark2dprojectmanager;

import java.awt.Dimension;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class DeveloperDataPanel extends JPanel {

	JTextField name;
	JTextField name_safe;
	JTextField website;
	
	public DeveloperDataPanel() {
		
		JPanel content = new JPanel();
		
		// developer
		JLabel companyNameLbl = new JLabel("Developer Name:");
		content.add(companyNameLbl);
		
		name = new JTextField(40);
		content.add(name);
		
		
		JLabel companyNameSafeLbl = new JLabel("Developer Name (Safe):");
		content.add(companyNameSafeLbl);
		
		name_safe = new JTextField(40);
		content.add(name_safe);
		
		
		JLabel websiteLbl = new JLabel("Developer Website:");
		content.add(websiteLbl);
		
		website = new JTextField(40);
		content.add(website);
		
		
		content.setPreferredSize(new Dimension(500, 200));
		
		this.setBorder(BorderFactory.createTitledBorder("Developer Information"));
		this.add(content);
		
	}
	
	
}
