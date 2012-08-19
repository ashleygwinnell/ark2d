package org.ashleygwinnell.ark2dprojectmanager;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.Charset;
import java.util.ArrayList;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

import org.json.JSONException;
import org.json.JSONObject;


 
public class ProjectManager extends JFrame {
	
	JSONObject config;
	ArrayList<MyProjectList> projectGroups = new ArrayList<MyProjectList>();
	
	JPanel pane;
	
	ARK2DDataPanel arkpanel;
	JScrollPane arkpanelScroll;
	
	ProjectPanel projectpanel;
	JScrollPane projectpanelScroll;
	
	IconListItem selectedProject;
	
	public ProjectManager() {
		super("ARK2D Project Manager");
	}
	
	private String readFile(String path)  {
		FileInputStream stream; 
		try {  
		stream = new FileInputStream(new File(path));
		 
		    FileChannel fc = stream.getChannel();
		    MappedByteBuffer bb = fc.map(FileChannel.MapMode.READ_ONLY, 0, fc.size());
		    /* Instead of using default, pass in a decoder. */
		    stream.close();
		    return Charset.defaultCharset().decode(bb).toString();
		  }
		  catch (Exception e) {
			  e.printStackTrace();
			  return "{}";
		  }
		}
	
	public JSONObject openConfig() {
		try {
			return new JSONObject(readFile("files/config.json"));
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}
	}
	
	public void init() throws JSONException {
		
		
		config = openConfig();
		
		setSize(800,600); 
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setMinimumSize(new Dimension(800, 600));
		setIconImage(new ImageIcon(getClass().getClassLoader().getResource("files/FrameIcon.png")).getImage());
		setLocationRelativeTo(null);
		
		// Menu bar 
		MyMenuBar menubar = new MyMenuBar();
		setJMenuBar(menubar); 
		 
		//setLayout(null);
		
		MyToolBar toolbar = new MyToolBar();
		add(toolbar, BorderLayout.NORTH);
		
		// Add shit
		pane = new JPanel();
		pane.setLayout(new BorderLayout());
		
		
			//add(new ProjectPanel());
			JPanel sideContent = new JPanel();
			sideContent.setPreferredSize(new Dimension(205, -1));
			
			// add items
			int height = 0;
			
			// ark2d setting
			MyProjectList sideContentSection = new MyProjectList("ARK2D");
			IconListItem item = new IconListItem();
			String woop = "{\"name\": \"ARK2D\", \"dir\": { \"" + 
					"windows\": \"" + config.getJSONObject("ark2d").getJSONObject("dir").getString("windows").replace("\\", "\\\\") + "\", \"" + 
					"osx\": \"" + config.getJSONObject("ark2d").getJSONObject("dir").getString("osx") + "\" }, \"icon\": \"/__preproduction/icon/32.png\" }";
			System.out.println(woop);
			JSONObject ark2dObj = new JSONObject(woop);
			item.data = ark2dObj;
			item.parse();
			sideContentSection.add(item);
			sideContent.add(sideContentSection);
			height += sideContentSection.getHeight();
			projectGroups.add(sideContentSection);
			selectedProject = item;
			
			for(int i = 0; i < config.getJSONArray("project_groups").length(); ++i) 
			{
				JSONObject projectGroup = config.getJSONArray("project_groups").getJSONObject(i);
				
				sideContentSection = new MyProjectList(projectGroup.getString("name"));
				for(int j = 0; j < projectGroup.getJSONArray("projects").length(); ++j)
				{
					JSONObject project = projectGroup.getJSONArray("projects").getJSONObject(j);
					
					item = new IconListItem();
					item.data = project;
					item.parse();
					
					sideContentSection.add(item);
				}
				sideContent.add(sideContentSection);
				
				height += sideContentSection.getHeight();
				
				projectGroups.add(sideContentSection);
			}
			//sideContent.setPreferredSize(new Dimension(205, 10 + sideContentARK2D.getHeight() + sideContentCustom.getHeight()));
			sideContent.setPreferredSize(new Dimension(205, 10 + height));
			
			JScrollPane sideList = new JScrollPane(sideContent);
			sideList.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
			sideList.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
			//sideList.setPreferredSize(new Dimension(200, 550));
			pane.add(sideList, BorderLayout.WEST);
			
			 
			
			projectpanel = new ProjectPanel();
			projectpanelScroll = new JScrollPane(projectpanel);
			pane.add(projectpanelScroll, BorderLayout.CENTER);
			//pane.remove(mainList);
			
			arkpanel = new ARK2DDataPanel();
			arkpanelScroll = new JScrollPane(arkpanel);
			//pane.add(mainList2, BorderLayout.CENTER);
			
			arkpanel.ark2d_dir_windows.setText( config.getJSONObject("ark2d").getJSONObject("dir").getString("windows") );
			arkpanel.ark2d_dir_osx.setText( config.getJSONObject("ark2d").getJSONObject("dir").getString("osx") );
			
			arkpanel.py_dir_windows.setText( config.getJSONObject("python").getJSONObject("executable").getString("windows") );
			arkpanel.py_dir_osx.setText( config.getJSONObject("python").getJSONObject("executable").getString("osx") );
			
			
		add(pane);
		

		
		setVisible(true);
	}
	public JSONObject getProject(String group, String name) {
		try { 
			for(int i = 0; i < config.getJSONArray("project_groups").length(); ++i) 
			{
				JSONObject projectGroup = config.getJSONArray("project_groups").getJSONObject(i);
				
				if (!projectGroup.getString("name").equals(group)) { continue; }
				
				for(int j = 0; j < projectGroup.getJSONArray("projects").length(); ++j)
				{
					JSONObject project = projectGroup.getJSONArray("projects").getJSONObject(j);
					
					if (!project.getString("name").equals(name)) { continue; }
					
					return project;	
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
	
	public void newProject() {
		
	}
	public void addProject() {
		
	}
	public void save() {
		
	}
	public void about() {
		
	}
	public void close() {
		this.setVisible(false);
		System.exit(0);
	}
	
	public String OSNameToARK2DOSName(String os) {
		if (os.equals("Mac OS X")) {
			return "osx";
		}
		return "windows";
	}
	
	
	public void selectProject(IconListItem project) {
		for(int i = 0; i < projectGroups.size(); i++) {
			for(int j = 0; j < projectGroups.get(i).items.size(); j++) {
				projectGroups.get(i).items.get(j).selected = false;
				projectGroups.get(i).items.get(j).stateChange();
			}
		}
		selectedProject = project;
		project.selected = true;
		project.stateChange();
		try {
			
			/*if (project.data.getString("name").equals("ARK2D")) {
				projectpanelScroll.setVisible(false);
				pane.remove(projectpanelScroll);
				
				arkpanelScroll.setVisible(true);
				pane.add(arkpanelScroll, BorderLayout.CENTER);
			} else {
				arkpanelScroll.setVisible(false);
				pane.remove(arkpanelScroll);
				
				projectpanelScroll.setVisible(true);
				pane.add(projectpanelScroll, BorderLayout.CENTER);
			}*/
			
			String fs = System.getProperty("file.separator");
			String projectFolder = project.data.getJSONObject("dir").getString(OSNameToARK2DOSName(System.getProperty("os.name")));
			projectFolder += fs + "config.json";
			
			String projectConfigJSON = readFile(projectFolder);
			JSONObject projectConfig = new JSONObject(projectConfigJSON);
			//System.out.println(projectConfigJSON);
			
			projectpanel.gamedata.game_name.setText(projectConfig.getString("game_name"));
			projectpanel.gamedata.game_name_safe.setText(projectConfig.getString("game_short_name"));
			projectpanel.gamedata.game_description.setText(projectConfig.getString("game_description"));
			projectpanel.gamedata.game_class_name.setText(projectConfig.getString("game_name_safe"));
			
			String orient = projectConfig.getString("game_orientation");
			if (orient.equals("portrait")) {
				projectpanel.gamedata.gameOrientationPortrait.setSelected(true);
			} else {
				projectpanel.gamedata.gameOrientationLandscape.setSelected(true);
			}
			
			projectpanel.gamedata.clear_color.setBackground(hex2rgb(projectConfig.getString("game_clear_color")));
			
			projectpanel.gamedata.dir_windows.setText(projectConfig.getJSONObject("windows").getString("game_dir"));
			projectpanel.gamedata.dir_osx.setText(projectConfig.getJSONObject("osx").getString("game_dir"));
			
			
			
			projectpanel.devdata.name.setText(projectConfig.getString("company_name"));
			projectpanel.devdata.name_safe.setText(projectConfig.getString("company_name_safe"));
			projectpanel.devdata.website.setText(projectConfig.getString("company_website"));
		} catch (Exception e) {
			
		}
	}
	
	public static ProjectManager _instance;
	
	public static String osname() {
		if (System.getProperty("os.name").equals("Mac OS X")) {
			return "osx";
		}
		return "windows";
	}
	public static Color hex2rgb(String colorStr) {
	    return new Color(
	            Integer.valueOf( colorStr.substring( 1, 3 ), 16 ),
	            Integer.valueOf( colorStr.substring( 3, 5 ), 16 ),
	            Integer.valueOf( colorStr.substring( 5, 7 ), 16 ) );
	}
	public static void copyFile(File sourceFile, File destFile) throws IOException {
	    if(!destFile.exists()) {
	        destFile.createNewFile();
	    }

	    FileChannel source = null;
	    FileChannel destination = null;

	    try {
	        source = new FileInputStream(sourceFile).getChannel();
	        destination = new FileOutputStream(destFile).getChannel();
	        destination.transferFrom(source, 0, source.size());
	    }
	    finally {
	        if(source != null) {
	            source.close();
	        }
	        if(destination != null) {
	            destination.close();
	        }
	    }
	}
	
}
