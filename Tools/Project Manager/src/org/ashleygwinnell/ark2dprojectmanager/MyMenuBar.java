package org.ashleygwinnell.ark2dprojectmanager;

import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JSeparator;
import javax.swing.KeyStroke;

public class MyMenuBar extends JMenuBar {
	
	public void newFile() {
		
	}
	public void open() {
		
	}
	public void close() {
		
	}
	
	public MyMenuBar() {
		JMenu menu_file = new JMenu("File");
		menu_file.setMnemonic(KeyEvent.VK_F);
		
			JMenuItem menuitem_new = new JMenuItem("New Project");
			menuitem_new.setMnemonic(KeyEvent.VK_N);
			menuitem_new.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N, InputEvent.CTRL_DOWN_MASK));
			menuitem_new.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) { 
				newFile();
			}});
			menuitem_new.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/Doc-Add.png")));
			menu_file.add(menuitem_new);
			
			menu_file.add(new JSeparator());
			
			JMenuItem menuitem_open = new JMenuItem("Add Project");
			menuitem_open.setMnemonic(KeyEvent.VK_O);
			menuitem_open.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, InputEvent.CTRL_DOWN_MASK));
			menuitem_open.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/Folder.png")));
			menuitem_open.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) {
				open();
			}}); 
			menu_file.add(menuitem_open);
			
			/*JMenu menu_recentfiles = new JMenu("Open Recent File ");
			menu_recentfiles.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/Folder.png")));
			final ArrayList<String> recentfiles = this.recentFileSet.get();
			for (int i = 0; i < recentfiles.size(); i++) {
				final int j = i;
				JMenuItem menuitem_recentfile = new JMenuItem((i + 1) + ") " + recentfiles.get(i));
				menuitem_recentfile.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						open(new File(recentfiles.get(j)));
					}
				});
				menu_recentfiles.add(menuitem_recentfile);
			}
			if (recentfiles.size() == 0) {
				menu_recentfiles.setEnabled(false);
			}
			menu_file.add(menu_recentfiles);
			*/
			//menu_file.add(new JSeparator());
			
			/*JMenuItem menuitem_save = new JMenuItem("Save");
			menuitem_save.setMnemonic(KeyEvent.VK_S);
			menuitem_save.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK));
			menuitem_save.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) {
				save();
			}});
			menuitem_save.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/Save.png")));
			menu_file.add(menuitem_save);
			
			JMenuItem menuitem_saveas = new JMenuItem("Save As");
			menuitem_saveas.setMnemonic(KeyEvent.VK_A);
			menuitem_saveas.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, InputEvent.CTRL_DOWN_MASK + InputEvent.SHIFT_DOWN_MASK));
			menuitem_saveas.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) {
				saveAs();
			}});
			menuitem_saveas.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/SaveAs.png")));
			menu_file.add(menuitem_saveas);
			
			menu_file.add(new JSeparator());
			
			JMenuItem menuitem_import = new JMenuItem("Import Contacts From File");
			menuitem_import.setMnemonic(KeyEvent.VK_I);
			menuitem_import.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_I, InputEvent.CTRL_DOWN_MASK));
			menuitem_import.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) {
				importContacts();
			}});
			menuitem_import.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/Datbase-Add.png")));
			menu_file.add(menuitem_import);
			
			JMenu menu_import_recentfiles = new JMenu("Import Contacts From Recent File ");
			menu_import_recentfiles.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/Datbase-Add.png")));
			final ArrayList<String> import_recentfiles = this.recentFileSet.get();
			for (int i = 0; i < recentfiles.size(); i++) {
				final int j = i;
				JMenuItem menuitem_recentfile = new JMenuItem((i + 1) + ") " + recentfiles.get(i));
				menuitem_recentfile.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						importContacts(new File[] {new File(recentfiles.get(j))});
					}
				});
				menu_import_recentfiles.add(menuitem_recentfile);
			}
			if (import_recentfiles.size() == 0) {
				menu_import_recentfiles.setEnabled(false);
			}
			menu_file.add(menu_import_recentfiles);
			*/
			menu_file.add(new JSeparator());
			
			JMenuItem menuitem_exit = new JMenuItem("Exit ");
			menuitem_exit.setMnemonic(KeyEvent.VK_E);
			menuitem_exit.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_W, InputEvent.CTRL_DOWN_MASK));
			menuitem_exit.addActionListener(new ActionListener() { 
				public void actionPerformed(ActionEvent e) {
					close();
				}
			});
			menuitem_exit.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/Delete.png")));
			menu_file.add(menuitem_exit);
		
		add(menu_file);
		
		/*JMenu menu_edit = new JMenu("Edit");
		menu_edit.setMnemonic(KeyEvent.VK_E);
			m_undo = new JMenuItem("Undo");
			m_undo.setEnabled(false);
			m_undo.setMnemonic(KeyEvent.VK_U);
			m_undo.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Z, InputEvent.CTRL_DOWN_MASK + InputEvent.ALT_DOWN_MASK));
			m_undo.addActionListener(new ActionListener() { 
				public void actionPerformed(ActionEvent e) {
					actionStack.pop();
				}
			});
			m_undo.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/Left.png")));
			menu_edit.add(m_undo);
			m_redo = new JMenuItem("Redo");
			m_redo.setEnabled(false);
			m_redo.setMnemonic(KeyEvent.VK_R);
			m_redo.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Y, InputEvent.CTRL_DOWN_MASK + InputEvent.ALT_DOWN_MASK));
			m_redo.addActionListener(new ActionListener() { 
				public void actionPerformed(ActionEvent e) {
					actionStack.push();
				}
			});
			m_redo.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/Right.png")));
			menu_edit.add(m_redo);				
		add(menu_edit);*/
		
		JMenu menu_help = new JMenu("Help");
		menu_help.setMnemonic(KeyEvent.VK_H);
		
			JMenuItem menuitem_about = new JMenuItem("About");
			menuitem_about.setMnemonic(KeyEvent.VK_A);
			menuitem_about.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_F1, 0));
			menuitem_about.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) { 
				
			}});
			menuitem_about.setIcon(new ImageIcon(getClass().getClassLoader().getResource("files/Info.png")));
			menu_help.add(menuitem_about);
			
			
		add(menu_help);
		
	}

}
