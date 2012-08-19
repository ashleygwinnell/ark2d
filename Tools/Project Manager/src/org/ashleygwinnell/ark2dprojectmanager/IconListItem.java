package org.ashleygwinnell.ark2dprojectmanager;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.io.File;

import javax.imageio.ImageIO;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;

import org.json.JSONObject;

public class IconListItem extends JPanel 
{
	public boolean selected = false;
	
	JLabel iconpanel;
	JLabel namelabel;
	JSONObject data = null;
	
	public void stateChange() {
		if (selected) {
			setBackground(Color.darkGray);
			namelabel.setForeground(Color.white);
		} else {
			setBackground(Color.white);
			namelabel.setForeground(Color.black);
		}
	}
	
	public IconListItem() 
	{		
		setLayout(new BorderLayout());
		
		iconpanel = new JLabel();
		iconpanel.setLayout(new GridLayout(1,1));
		
		iconpanel.setPreferredSize(new Dimension(32,32));
		iconpanel.setBackground(Color.DARK_GRAY);
		add(iconpanel, BorderLayout.WEST);
		
		/*JPanel lab = new JPanel();
		lab.setPreferredSize(new Dimension(10,10));
		add(lab, BorderLayout.CENTER);*/
		
		namelabel = new JLabel(" Project Name");
		namelabel.setLabelFor(iconpanel);
		add(namelabel, BorderLayout.CENTER);
		
		setBackground(Color.white);
		this.setBorder(BorderFactory.createLineBorder(Color.black));
		this.setPreferredSize(new Dimension(180,34));
		
		final IconListItem thiz = this;
		this.addMouseListener(new MouseListener() {
			boolean over = false;
			@Override
			public void mouseReleased(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void mousePressed(MouseEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void mouseExited(MouseEvent arg0) {
				// TODO Auto-generated method stub
				over = false;
			}
			
			@Override
			public void mouseEntered(MouseEvent arg0) {
				// TODO Auto-generated method stub
				over = true;
			}
			
			@Override
			public void mouseClicked(MouseEvent arg0) {
				// TODO Auto-generated method stub
				ProjectManager._instance.selectProject(thiz);
			}
		});
		
		
	}

	public void parse() {
		try {
			namelabel.setText(" " + data.getString("name"));
			
			BufferedImage myPicture = ImageIO.read(new File(data.getJSONObject("dir").getString("osx") + data.getString("icon")));
			JLabel picLabel = new JLabel();
			picLabel.setIcon(new ImageIcon( myPicture ));
			picLabel.setPreferredSize(new Dimension(32, 32));
			picLabel.setBackground(Color.DARK_GRAY);
			iconpanel.add( picLabel );
		} catch (Exception e) {
			e.printStackTrace();
		}
		
	}
}