package cn.edu.whu.flood.ui;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import cn.edu.whu.flood.object.SourceFileFilter;
import cn.edu.whu.flood.object.User;

public class SubmitPanel extends JPanel implements ActionListener{
  public SubmitPanel(MainFrame father) {
    super();
    this.father = father;
    initPanel();
    setVisible(true);
  }
  
  public void initPanel() {
    Object[] lang = {"GCC", "G++", "PASCAL", "JAVA"};
    languageComboBox = new JComboBox(lang);
    Object[] share = {"Yes", "No"};
    shareCodeComboBox = new JComboBox(share);
    JPanel p1 = new JPanel();
    p1.add(fileLabel);
    p1.add(fileField);
    p1.add(fileButton);
    fileButton.addActionListener(this);
    JPanel p2 = new JPanel();
    p2.add(new JLabel("Language:  "));
    p2.add(languageComboBox);
    p2.add(new JLabel("Share Code:  "));
    p2.add(shareCodeComboBox);
    JPanel p3 = new JPanel();
    p3.add(submit);
    submit.addActionListener(father);
    p3.add(status);
    status.addActionListener(father);
    JPanel p4 = new JPanel(new BorderLayout());
    p4.add(p1, BorderLayout.CENTER);
    p4.add(p2, BorderLayout.SOUTH);
    JPanel p5 = new JPanel(new BorderLayout());
    String labelContent = "";
    if (father.getMContestId() == 0) 
      labelContent = "<html><font color = #ff0000>Problem: " + 
                     father.getProblemStr() + "</font>";
    else 
      labelContent = "<html><font color = #ff0000>Contest: " + 
                     father.getContestStr() + "</font> <br/>" + 
                     "<font color = #ff0000>Problem: " + father.getProblemStr() +
                     "</font>";
    JLabel info =new JLabel(labelContent);
    info.setHorizontalAlignment(JLabel.CENTER);
    p5.add(info, BorderLayout.NORTH);
    p5.add(p4, BorderLayout.CENTER);
    setLayout(new BorderLayout());
    add(p5, BorderLayout.CENTER);
    add(p3, BorderLayout.SOUTH);
  }
  
  public String getFilePath() {
    return fileField.getText();
  }
  
  public int getLanguage() {
    return languageComboBox.getSelectedIndex();
  }
  
  public boolean getShareCode() {
    return shareCodeComboBox.getSelectedIndex() == 0;
  }
  
  private JLabel fileLabel = new JLabel("Filename");
  private JTextField fileField = new JTextField(30);
  private JButton fileButton = new JButton("browse");
  private JButton submit = new JButton("Submit");
  private JButton status = new JButton("Status");
  private JComboBox languageComboBox;
  private JComboBox shareCodeComboBox;
  private MainFrame father;
  
  public void actionPerformed(ActionEvent e) {
    if (e.getSource() == fileButton) {
      JFileChooser chooser = new JFileChooser();
      SourceFileFilter fileFilter = new SourceFileFilter();
      fileFilter.addType(".c");
      fileFilter.addType(".pas");
      fileFilter.addType(".cc");
      fileFilter.addType(".java");
      fileFilter.setDescription("source file(.c/.cc/.pas/.java)");
      chooser.setFileFilter(fileFilter);
      
      int ret = chooser.showOpenDialog(father);
      if (ret == JFileChooser.APPROVE_OPTION) {
        fileField.setText(chooser.getSelectedFile().getAbsolutePath());
      }
    }
  }
}
