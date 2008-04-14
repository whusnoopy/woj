package cn.edu.whu.flood.ui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

public class MessageDialog extends JDialog implements ActionListener {
	
  /**
	 * 
	 */
  private static final long serialVersionUID = -3473700452431922720L;
  
  public MessageDialog (String message) {
    super();
    initDialog(message);
    setVisible(true);
  }
  
  public void initDialog(String message) {
    msgLabel = new JLabel(message);
    
    getContentPane().setLayout(new BorderLayout());
    getContentPane().add(msgLabel, BorderLayout.CENTER);
    msgLabel.setHorizontalAlignment(SwingConstants.CENTER);
    JPanel p1 = new JPanel();
    p1.add(confirm);
    getContentPane().add(p1, BorderLayout.SOUTH);
    confirm.addActionListener(this);
    setDefaultCloseOperation(DISPOSE_ON_CLOSE);
    setSize(250, 150);
    setTitle(message);
    setCenter();
    //pack();
  }
  
  public void actionPerformed(ActionEvent e) {
    if (e.getSource() == confirm) {
      this.dispose();
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

  private JButton confirm = new JButton("OK");
  private JLabel msgLabel;
}
