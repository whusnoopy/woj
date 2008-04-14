package cn.edu.whu.flood.ui;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.event.*;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import javax.swing.*;

import cn.edu.whu.flood.network.Connection;
import cn.edu.whu.flood.object.Contest;
import cn.edu.whu.flood.object.ProblemSet;
import cn.edu.whu.flood.object.Status;
import cn.edu.whu.flood.object.User;

import java.util.*;

public class MainFrame extends JFrame implements ActionListener {
  
  public MainFrame() {
    super();
    Vector problemSetList = new Vector();
    Vector contestList = new Vector();
    Vector contestProblemList = new Vector();
    contestProblemList.add(new Integer(123));
    contestList.add(new Contest(1, contestProblemList, "fuck"));
    initFrame(problemSetList, contestList);
    setVisible(true);
  }
  public MainFrame(User user, 
                   Vector problemSetList, 
                   Vector contestList) {
    super();
    this.user = user;
    initFrame(problemSetList, contestList);
    setVisible(true);
  }
  
  public void initFrame(Vector problemSetList, Vector contestList) {
    
    setDefaultCloseOperation(EXIT_ON_CLOSE);
    statusPanel = new StatusPanel(this, user);
    getContentPane().add(statusPanel);   
    menuBar.setLayout(new GridLayout(1,3));
    menuBar.add(systemMenu); 
    systemMenu.add(statusMenuItem);
    statusMenuItem.addActionListener(this);
    systemMenu.addSeparator();
    systemMenu.add(logout);  
    logout.addActionListener(this);
    menuBar.add(problemMenu);
    menuBar.add(contestMenu);
    initProblemMenu(problemSetList);
    initContestMenu(contestList);
    setJMenuBar(menuBar);
    setTitle("Test");
    setSize(680, 400);
    setResizable(false);
    setCenter();
  }
  
  public void initProblemMenu(Vector problemSetList) {
    for (int i = 0; i < problemSetList.size(); i++) {
      ProblemSet problemSet = (ProblemSet)problemSetList.elementAt(i);
      JMenu pMenu = new JMenu("      " + problemSet.toString() + "       ");
      problemMenu.add(pMenu);
      Vector midProblemList = problemSet.getProblemList();
      
      for (int j = 0; j < midProblemList.size(); j++) {
        ProblemSet midProblemSet = (ProblemSet)midProblemList.elementAt(j);
        JMenu midPMenu = new JMenu("      " + midProblemSet.toString() + "       ");
        pMenu.add(midPMenu);
        Vector problemList = midProblemSet.getProblemList();
        
        for (int k = 0; k < problemList.size(); k++) {
          int problemId = ((Integer)problemList.elementAt(k)).intValue();
          JMenuItem pItem = new JMenuItem("      " + problemId + "       ");
          pItem.addActionListener(new MenuItemActionListener(problemId, 
                                                             0,
                                                             "" + problemId,
                                                             ""));
          midPMenu.add(pItem);
        }
      }
    }
  }
  
  public void initContestMenu(Vector contestList) {
    if (contestList.size() == 0) {
      JMenuItem noItem = new JMenuItem("This is no starting contest now");
      contestMenu.add(noItem);
    }
    for (int i = 0; i < contestList.size(); i++) {
      Contest contest = (Contest)contestList.elementAt(i);
      int contestId = contest.getContestId();
      JMenu cMenu = new JMenu("      " + contest.getTitle() + "       ");
      contestMenu.add(cMenu);
      Vector problemList = contest.getProblemList();
      
      for (int j = 0; j < problemList.size(); j++) {
        int problemId = ((Integer)problemList.elementAt(j)).intValue();
        char c = (char) ('A' + j);
        JMenuItem pItem = new JMenuItem("          " + c + "          ");
        pItem.addActionListener(new MenuItemActionListener(problemId, 
                                                           contestId,
                                                           "" + c, 
                                                           contest.getTitle()));
        cMenu.add(pItem);
      }
    }
  }
  
  public void actionPerformed(ActionEvent e) {
    if (e.getSource() == logout) {
      this.dispose();
      LoginFrame loginFrame = new LoginFrame();
    } else if (e.getSource() == statusMenuItem) {

      this.getContentPane().removeAll();
      if (statusPanel == null) 
        System.out.println("statusPanel is empty!");
      this.getContentPane().add(statusPanel);
      setSize(680, 400);
      repaint();
      
    } else if (((JButton)e.getSource()).getText() == "Submit") {

      String filename = submitPanel.getFilePath();
      if (filename.trim().equals("")) {
        MessageDialog message = new MessageDialog("Please select file");
        return;
      }
      try {
        BufferedReader in = new BufferedReader(new FileReader(filename));
        String source = new String("");
        String line = new String("");
        while ((line = in.readLine()) != null) {
          source += line + "\n";
        }
        System.out.println(source);
        Status status = new Status();
        status.setProblemId(mProblemId);
        status.setContestId(mContestId);
        status.setCodeLength(source.length());
        status.setShareCode(submitPanel.getShareCode());
        status.setLanguage(submitPanel.getLanguage());
        System.out.println(status.getProblemId());
        System.out.println(status.getContestId());
        System.out.println(status.getCodeLength());
        System.out.println(status.isShareCode());
        System.out.println(status.getLanguage());
        Connection.getInstance().doSubmit(user, status, source);
      } catch (IOException e1) {
        e1.printStackTrace();
      }
      
      this.getContentPane().removeAll();
      statusPanel = new StatusPanel(this, user);
      this.getContentPane().add(statusPanel);
      setSize(680, 400);
      repaint();
      
    } else if (((JButton)e.getSource()).getText() == "Status") {
      this.getContentPane().removeAll();
      if (statusPanel == null) 
        System.out.println("statusPanel is empty!");
      this.getContentPane().add(statusPanel);
      setSize(680, 400);
      repaint();
    }
  }
  
  private class MenuItemActionListener implements ActionListener{
    private int problemId;
    private int contestId;
    private String problemStr;
    private String contestStr;
  
    public MenuItemActionListener(int problemId, int contestId) {
      this.problemId = problemId;
      this.contestId = contestId;
    }
  
    public MenuItemActionListener(int problemId, 
                                  int contestId, 
                                  String problemStr, 
                                  String contestStr) {
      this.problemId = problemId;
      this.contestId = contestId;
      this.problemStr = problemStr;
      this.contestStr = contestStr;
    }

    public void actionPerformed(ActionEvent e) {
      System.out.println("Contest or Problem");
      MainFrame.this.mProblemId = problemId;
      MainFrame.this.mContestId = contestId;
      MainFrame.this.problemStr = problemStr;
      MainFrame.this.contestStr = contestStr;
      System.out.println(problemId+" : "+contestId);
      MainFrame.this.getContentPane().removeAll();
      submitPanel = new SubmitPanel(MainFrame.this);
      MainFrame.this.getContentPane().add(submitPanel);
      MainFrame.this.pack();
      MainFrame.this.repaint();
    }  
  }
  
  public int getMContestId() {
    return mContestId;
  }
  public int getMProblemId() {
    return mProblemId;
  }
  public String getContestStr() {
    return contestStr;
  }
  public String getProblemStr() {
    return problemStr;
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
   
  private JMenuBar menuBar = new JMenuBar();
  private JMenu systemMenu = new JMenu("System");
  private JMenu problemMenu = new JMenu("Problems");
  private JMenu contestMenu = new JMenu("Contests");
  private JMenuItem logout = new JMenuItem("       Log Out       ");
  private JMenuItem statusMenuItem = new JMenuItem("       Status       ");
  private int mProblemId;
  private int mContestId;
  private String problemStr;
  private String contestStr;
  private User user;
  private SubmitPanel submitPanel = new SubmitPanel(this);
  private StatusPanel statusPanel;
}
