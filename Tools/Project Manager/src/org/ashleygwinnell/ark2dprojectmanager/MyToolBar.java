package org.ashleygwinnell.ark2dprojectmanager;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

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
			
			
			
			JButton tb_clean = new JButton(new ImageIcon(getClass().getClassLoader().getResource("files/Delete.png")));
			tb_clean.setToolTipText("Clean Project");
			tb_clean.setBounds(	(toolbar_item_num*toolbar_item_width) + 2 + (7*toolbar_separator_num), 
								2,
								toolbar_item_width, 
								toolbar_item_width);
			tb_clean.setMargin(new Insets(1,1,1,1));
			tb_clean.addActionListener(new ActionListener() { 
				public void actionPerformed(ActionEvent e) {
					try { 
						final String key = ProjectManager.osname();
						final String ark2ddir = ProjectManager._instance.config.getJSONObject("ark2d").getJSONObject("dir").getString(key);
						final String dir = ProjectManager._instance.selectedProject.data.getJSONObject("dir").getString(key);
						final String pyscript = ark2ddir + System.getProperty("file.separator") + "build.py";
						
						
						System.out.println("python " + pyscript);
						ProcessBuilder pb = new ProcessBuilder("python", pyscript, "clean=true", "type=game", "dir="+dir);
						System.out.println(pb.toString());
						Process p = pb.start();
						
						
						BufferedReader input = new BufferedReader(new InputStreamReader(p.getInputStream()));
		                String line=null;
		                while((line=input.readLine()) != null) {
		                	System.out.println(line);
		                }
						
		                p.waitFor();
						//int exitCode = p.waitFor();
						//System.out.println("exit code: " + exitCode);
						//if (exitCode != 0) {
						//	JOptionPane.showMessageDialog(null, "Could not clean game. Check project path and settings.", "Error", JOptionPane.ERROR_MESSAGE );
						//}
						JOptionPane.showMessageDialog(null, "Project cleaned", "Success", JOptionPane.INFORMATION_MESSAGE );
						
					} catch (Exception ee) {
						ee.printStackTrace();
					}
				}
			}); 
			add(tb_clean);
			toolbar_item_num++;
			
			
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
						final String key = ProjectManager.osname();
						final String ark2ddir = ProjectManager._instance.config.getJSONObject("ark2d").getJSONObject("dir").getString(key);
						final String dir = ProjectManager._instance.selectedProject.data.getJSONObject("dir").getString(key);
							
						final String pyscript = ark2ddir + System.getProperty("file.separator") + "build.py";
						//pyscript += " type=game";
						//pyscript += " dir=" + dir;
						
						final JFrame frame = new JFrame("Compile Log");
						frame.setSize(1024, 768);
						frame.setLocationRelativeTo(ProjectManager._instance);
						frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
						frame.setLayout(new GridLayout(2,1));
						
						final JTextArea stdout = new JTextArea(); 
						final JScrollPane stdoutScroll = new JScrollPane(stdout);
						frame.add(stdoutScroll);
						
						final JTextArea stderr = new JTextArea();
						final JScrollPane stderrScroll = new JScrollPane(stderr);
						frame.add(stderrScroll);
						
						Thread t = new Thread(new Runnable() {
							public void run() {
								try { 
									System.out.println("python " + pyscript);
									ProcessBuilder pb = new ProcessBuilder("python", pyscript, "type=game", "dir="+dir);
									pb.redirectErrorStream(true);
									System.out.println(pb.toString());
									Process p = pb.start();
									
									
									/*BufferedReader input = new BufferedReader(new InputStreamReader(p.getInputStream()));
					                String line=null;
					                while((line=input.readLine()) != null) {
					                	System.out.println(line);
					                	stdout.setText(stdout.getText() + line + "\n");
					                	stdout.setCaretPosition(stdout.getDocument().getLength());
					                	frame.repaint();
					                }
					                
					                input = new BufferedReader(new InputStreamReader(p.getErrorStream()));
					                line=null;
					                while((line=input.readLine()) != null) { 
					                	stderr.setText(stderr.getText() + line + "\n");
					                	stderr.setCaretPosition(stderr.getDocument().getLength());
					                	frame.repaint();
					                }
									*/
									//int exitCode = p.waitFor();
									//System.out.println("exit code: " + exitCode);
									//if (exitCode != 0) {
										JOptionPane.showMessageDialog(null, "Could not compile game. Check project path and settings.", "Error", JOptionPane.ERROR_MESSAGE );
									//}
								} catch (Exception e) {
									e.printStackTrace();
								}
							}
						});
						t.start();
						frame.setVisible(true);
						
						
						
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
						String app = dir + fs + "build" + fs + name + ".app";
						
						//
						// http://stackoverflow.com/questions/7979234/java-command-runtime-getruntime-exec-in-mac-os
						//
						ProcessBuilder pb = new ProcessBuilder("open", app);
						Process p = pb.start();
						int exitCode = p.waitFor();
						System.out.println("exit code: " + exitCode);
						if (exitCode != 0) {
							JOptionPane.showMessageDialog(null, "Could not run game. Check project path and settings.", "Error", JOptionPane.ERROR_MESSAGE );
						}
						
						/*
						BufferedReader input = new BufferedReader(new InputStreamReader(pr2.getInputStream()));
		                String line=null;
		                while((line=input.readLine()) != null) {
		                    System.out.println(line);
		                }
		                int exitVal = pr2.waitFor();
		                System.out.println("Exited with error code "+exitVal);*/
						
						
						
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
