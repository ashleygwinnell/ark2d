package org.ashleygwinnell.ark2dprojectmanager;

import java.awt.Component;
import java.awt.Dimension;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.JPanel;

public class MyProjectList extends JPanel
{
	int count = 0;
	String group = "";
	ArrayList<IconListItem> items = new ArrayList<IconListItem>();
	
	public MyProjectList() 
	{
		//setLayout(null);
		setPreferredSize(new Dimension(200, -1));
	}
	
	public MyProjectList(String group) 
	{
		//setLayout(null);
		this.group = group;
		 
		setBorder(BorderFactory.createTitledBorder(group));
		setPreferredSize(new Dimension(200, -1));
		
	}
	@Override
	public int getHeight() {
		return getPreferredSize().height;
		//return super.getHeight();
	}
	
	public Component add(Component component) {
		if (!(component instanceof IconListItem)) { return null; }
		items.add((IconListItem) component);
		count++;
		int start = (group.equals(""))?0:35;
		setPreferredSize(new Dimension(200, start + (40 * count)));
		return super.add(component);
	}
}
