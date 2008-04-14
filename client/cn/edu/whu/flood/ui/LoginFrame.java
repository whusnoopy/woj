package cn.edu.whu.flood.ui;

import javax.swing.*;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.Vector;

import cn.edu.whu.flood.network.*;
import cn.edu.whu.flood.object.*;

public class LoginFrame extends JFrame implements ActionListener, KeyListener{
  
  private JButton submit = new JButton("Login");
  private JButton quit = new JButton("Cancel");
  private JTextField userField = new JTextField(20);
  private JPasswordField passwordField = new JPasswordField(20);
  private JLabel userLabel = new JLabel("User Id:");
  private JLabel passwordLabel = new JLabel("Password");
  private JLabel upLabel = new JLabel("Welcome to use onlinejudge client");
 
  public LoginFrame() {
    super();
    initFrame();
    setVisible(true);
    //System.out.println("ok");
  }
  
  public void initFrame() {
	JPanel p1 = new JPanel();
	p1.setLayout(new GridLayout(2, 1));
	p1.add(userLabel);
	p1.add(passwordLabel);
	JPanel p2 = new JPanel();
	p2.setLayout(new GridLayout(2, 1));
	p2.add(userField);
	p2.add(passwordField);
	JPanel p3 = new JPanel(new FlowLayout());
	p3.add(submit);
	p3.add(quit);
	JPanel p4 = new JPanel(new FlowLayout());
	p4.add(p1);
	p4.add(p2);
	JPanel p5 = new JPanel(new BorderLayout());
	p5.add(upLabel, BorderLayout.NORTH);
	upLabel.setHorizontalAlignment(SwingConstants.CENTER);
	p5.add(p4, BorderLayout.CENTER);
	p5.add(p3, BorderLayout.SOUTH);
	this.getContentPane().add(p5);
	
	userLabel.addKeyListener(this);
	passwordLabel.addKeyListener(this);
	userField.addKeyListener(this);
	passwordField.addKeyListener(this);
	submit.addKeyListener(this);
	quit.addKeyListener(this);
	p1.addKeyListener(this);
	p2.addKeyListener(this);
	p3.addKeyListener(this);
	p4.addKeyListener(this);
	p5.addKeyListener(this);
	submit.addActionListener(this);
	quit.addActionListener(this);
	addKeyListener(this);
	setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
	setTitle("Welcome to use onlinejudge client");
	pack();	
	setCenter();
  }

  public void actionPerformed(ActionEvent event) {
    if (event.getSource() == submit) {
      doLogin();
    }else if (event.getSource() == quit) {
      System.exit(0);
    }
  }
  
  public String encodingPassword(String password) {
    String str = new String("");
    //return str;
    return password;
  }
  
  public void doLogin() {
	System.out.println("doLogin");
    Connection connection = Connection.getInstance();
    User user = new User();
    user.setUserId(userField.getText().trim());
    user.setPassword(encodingPassword(new String(passwordField.getPassword())));
    boolean ret = connection.doLogin(user);
    if (!ret) {
      MessageDialog msgDialog = new MessageDialog("UserId or Password error!");
      return;
    }
    Vector problemMenuList = new Vector();
    Vector contestMenuList = new Vector();
    connection.doInitClient(problemMenuList, contestMenuList);
    this.dispose();
    MainFrame mainFrame = new MainFrame(user, 
    		                            problemMenuList, 
    		                            contestMenuList);
  }

  public void keyPressed(KeyEvent event) {
    if (event.getKeyCode() == KeyEvent.VK_ENTER) { 
      doLogin();
    }
  }

  public void keyReleased(KeyEvent event) {
    
  }

  public void keyTyped(KeyEvent event) {
    if (event.getKeyCode() == KeyEvent.VK_ENTER) {
      doLogin();
    }
  }
  
  private void setCenter() {
    Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
    int scrx, scry, frmx, frmy;
    scrx = scrSize.width;
    scry = scrSize.height;
    frmx = getWidth();
    frmy = getHeight();
    setBounds((scrx - frmx)/2, (scry - frmy)/2, frmx, frmy);
  }
}
