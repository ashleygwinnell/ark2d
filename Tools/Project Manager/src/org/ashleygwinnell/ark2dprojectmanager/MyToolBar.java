package org.ashleygwinnell.ark2dprojectmanager;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

import org.json.JSONObject;

public class MyToolBar extends JPanel {
	
	/**
	 * AB Separator
	 * This is just a JPanel with a specified background 
	 * and size to cut down on lines.
	 * @author Ashley Gwinnell
	 */
	public class ABSeparator extends JPanel
	{
		/**
		 * Create a new separator.
		 */
		public ABSeparator()
		{
			this.setBackground(Color.LIGHT_GRAY);
			this.setPreferredSize(new Dimension(10,30));
		}
	}
	
	public MyToolBar() 
	{
		setPreferredSize(new Dimension(getWidth(), 36));
		setLayout(null);
			int toolbar_separator_num = 0;
			int toolbar_item_num = 0;
			int toolbar_item_width = 32;
			
			
			
			JButton tb_new = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Doc-Add.png")));
			tb_new.setToolTipText("New Project");
			tb_new.setBounds((toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 2, toolbar_item_width, toolbar_item_width);
			tb_new.setMargin(new Insets(1,1,1,1));
			tb_new.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) { 
				ProjectManager._instance.newProject();
			} });
			add(tb_new);
			toolbar_item_num++;
			
			JButton tb_open = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Folder.png")));
			tb_open.setToolTipText("Add Project");
			tb_open.setBounds((toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 2,toolbar_item_width, toolbar_item_width);
			tb_open.setMargin(new Insets(1,1,1,1));
			tb_open.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) {
				ProjectManager._instance.addProject();
			}});
			add(tb_open);
			toolbar_item_num++;
			
			JButton tb_del = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/User-Del.png")));
			tb_del.setToolTipText("Remove Project");
			tb_del.setBounds(	(toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 
								2,
								toolbar_item_width, 
								toolbar_item_width);
			tb_del.setMargin(new Insets(1,1,1,1));
			tb_del.addActionListener(new ActionListener() { 
				public void actionPerformed(ActionEvent e) {
					
				}
			});
			add(tb_del);
			toolbar_item_num++;
			
			ABSeparator s11 = new ABSeparator();
			s11.setBounds((toolbar_item_num*toolbar_item_width) + 5 + (7*toolbar_separator_num), 6, 1, toolbar_item_width - 8);
			add(s11);
			toolbar_separator_num++;
			
			JButton tb_save = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Save.png")));
			tb_save.setToolTipText("Save All");
			tb_save.setBounds((toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 2,toolbar_item_width, toolbar_item_width);
			tb_save.setMargin(new Insets(1,1,1,1));
			tb_save.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) {
				ProjectManager._instance.save();
			}});
			add(tb_save);
			toolbar_item_num++;
			
			
			
			ABSeparator s2 = new ABSeparator();
			s2.setBounds((toolbar_item_num*toolbar_item_width) + 5 + (7*toolbar_separator_num), 6, 1, toolbar_item_width - 8);
			add(s2);
			toolbar_separator_num++;
			
			/*JButton tb_import = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Datbase-Add.png")));
			tb_import.setToolTipText("Import Contacts From A File");
			tb_import.setBounds(	(toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 
									2,
									toolbar_item_width, 
									toolbar_item_width);
			tb_import.setMargin(new Insets(1,1,1,1));
			tb_import.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					importContacts();
				}
			});
			add(tb_import);
			toolbar_item_num++;*/
			
			//ABSeparator ss = new ABSeparator();
			//ss.setBounds((toolbar_item_num*toolbar_item_width) + 5 + (7*toolbar_separator_num), 6, 1, toolbar_item_width - 8);
			//add(ss);
			//toolbar_separator_num++;
			
			/*tb_undo = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Left.png")));
			tb_undo.setToolTipText("Undo");
			tb_undo.setBounds((toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 2,toolbar_item_width, toolbar_item_width);
			tb_undo.setMargin(new Insets(1,1,1,1));
			tb_undo.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) {
				actionStack.pop();
			}});		
			tb_undo.setEnabled(false);
			add(tb_undo);
			toolbar_item_num++;
			
			tb_redo = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Right.png")));
			tb_redo.setToolTipText("Redo");
			tb_redo.setBounds((toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 2,toolbar_item_width, toolbar_item_width);
			tb_redo.setMargin(new Insets(1,1,1,1));
			tb_redo.addActionListener(new ActionListener() { public void actionPerformed(ActionEvent e) {
				actionStack.push();
			}});	
			tb_redo.setEnabled(false);
			add(tb_redo);
			toolbar_item_num++;*/
			
			//ABSeparator s = new ABSeparator();
			//s.setBounds((toolbar_item_num*toolbar_item_width) + 5 + (7*toolbar_separator_num), 6, 1, toolbar_item_width - 8);
			//add(s);
			//toolbar_separator_num++;
			
			
			
			
			
			
			// compile & run
			JButton tb_compile = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Datbase-Add.png")));
			tb_compile.setToolTipText("Compile Project");
			tb_compile.setBounds(	(toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 
								2,
								toolbar_item_width, 
								toolbar_item_width);
			tb_compile.setMargin(new Insets(1,1,1,1));
			tb_compile.addActionListener(new ActionListener() { 
				public void actionPerformed(ActionEvent e) {
					try { 
						String key = ProjectManager.osname();
						String dir = ProjectManager._instance.selectedProject.data.getJSONObject("dir").getString(key);
							
						String cmd = "python " + dir + System.getProperty("file.separator") + "build-game.py";
						System.out.println(cmd);
					} catch (Exception ee){
						ee.printStackTrace();
					}
				}
			});
			add(tb_compile);
			toolbar_item_num++;
			
			JButton tb_run = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Application-info.png")));
			tb_run.setToolTipText("Run Project");
			tb_run.setBounds(	(toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 
								2,
								toolbar_item_width, 
								toolbar_item_width);
			tb_run.setMargin(new Insets(1,1,1,1));
			tb_run.addActionListener(new ActionListener() { 
				public void actionPerformed(ActionEvent e) {
					try { 
						String key = ProjectManager.osname();
						String name = ProjectManager._instance.selectedProject.data.getString("name");
						String dir = ProjectManager._instance.selectedProject.data.getJSONObject("dir").getString(key);
								
						String fs = System.getProperty("file.separator");
						String cmd = "open -g -a " + dir.replace(" ", "\\ ") + fs + "build" + fs + name.replace(" ", "_") + ".app "; //1>/dev/null 2>&1";
						
						String name2 = dir.replace(" ", "\\ ") + fs + "build" + fs + name.replace(" ", "\\ ") + ".app";
						String name3 = dir.replace(" ", "\\ ") + fs + "build" + fs + name.replace(" ", "_") + ".app";
						
						File file2 = new File(name2);
						File file3 = new File(name3);
						file2.renameTo(file3);
						
						
						System.out.println(cmd);
						Process pr2 = Runtime.getRuntime().exec(cmd);
						
						
						BufferedReader input = new BufferedReader(new InputStreamReader(pr2.getInputStream()));
						 
		                String line=null;
		 
		                while((line=input.readLine()) != null) {
		                    System.out.println(line);
		                }
		 
		                int exitVal = pr2.waitFor();
		                System.out.println("Exited with error code "+exitVal);
						
						/*String cd1 = "cd " + dir.replace(" ", "\\ ") + fs + "build";
						System.out.println(cd1);
						Process pr = Runtime.getRuntime().exec(cd1);
						
						String cd2 = "open " + name.replace(" ", "\\ ") + ".app";
						System.out.println(cd2);
						Process pr2 = Runtime.getRuntime().exec(cd2);*/
						
						
					} catch (Exception ee) {
						ee.printStackTrace();
					}
				}
			});
			add(tb_run);
			toolbar_item_num++;
			
			
			
			ABSeparator s3 = new ABSeparator();
			s3.setBounds((toolbar_item_num*toolbar_item_width) + 5 + (7*toolbar_separator_num), 6, 1, toolbar_item_width - 8);
			add(s3);
			toolbar_separator_num++;
			
			int xOffset = 0;
			/*JPanel pnl_search = new JPanel();
			pnl_search.setBounds(	(toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 
									2, 
									toolbar_item_width + 200, 
									toolbar_item_width);
			pnl_search.setLayout(null);
			pnl_search.setBackground(Color.white);
				JLabel lbl_icon = new JLabel(new ImageIcon(getClass().getClassLoader().getResource("files/Search.png")));
				lbl_icon.setBounds(0, 0, 32, pnl_search.getHeight());
				lbl_icon.setBorder(new MatteBorder(1, 1, 1, 0, Color.LIGHT_GRAY));
				pnl_search.add(lbl_icon);
				
				tf_search = new JTextField("");
				tf_search.setBounds(30,0, 200 + toolbar_item_width - 30, pnl_search.getHeight());
				tf_search.setBackground(Color.white);
				tf_search.setBorder(new MatteBorder(1, 0, 1, 1, Color.LIGHT_GRAY));
				tf_search.addKeyListener(new KeyListener() {
					public void keyPressed(KeyEvent e) {}
					public void keyReleased(KeyEvent e) { 
						setFilter(tf_search.getText()); 
					}
					public void keyTyped(KeyEvent e) {}
				});
				pnl_search.add(tf_search);
			add(pnl_search);
			toolbar_item_num++;
			xOffset += 200;*/
			
			ABSeparator s4 = new ABSeparator();
			s4.setBounds((toolbar_item_num*toolbar_item_width) + 5 + (7*toolbar_separator_num) + xOffset, 6, 1, toolbar_item_width - 8);
			add(s4);
			toolbar_separator_num++;
			JButton tb_about = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Info.png")));
			tb_about.setToolTipText("About Application");
			tb_about.setBounds(	(toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num) + xOffset, 2, toolbar_item_width, toolbar_item_width);
			tb_about.setMargin(new Insets(1,1,1,1));
			tb_about.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					ProjectManager._instance.about();
				}
			});
			add(tb_about);
			toolbar_item_num++;
			
			JButton tb_exit = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Delete.png")));
			tb_exit.setToolTipText("Exit Application");
			tb_exit.setBounds(	(toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num) + xOffset, 2, toolbar_item_width, toolbar_item_width);
			tb_exit.setMargin(new Insets(1,1,1,1));
			tb_exit.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					ProjectManager._instance.close();
				}
			});
			add(tb_exit);
			toolbar_item_num++;
			
			
	}
	
}
