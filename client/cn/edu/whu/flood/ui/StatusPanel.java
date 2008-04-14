package cn.edu.whu.flood.ui;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.Enumeration;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.*;

import cn.edu.whu.flood.network.Connection;
import cn.edu.whu.flood.object.Status;
import cn.edu.whu.flood.object.User;

class StatusTableModel extends DefaultTableModel {
  /**
	 * 
	 */
  private static final long serialVersionUID = -3515977997475366695L;


  public StatusTableModel() {
    super(COLUMN_NAMES, 0);
  }
  
  public static final String[] COLUMN_NAMES = {"RunID",
		                                      "Problem",
		                                      "Contest",
		                                      "Result",
		                                      "Memory",
		                                      "Time",
		                                      "Langauge",
		                                      "Length",
		                                      "SubmitTime"};
  public static final int
	  FIRST_STATUS          = 1,
	  ACCEPTED              = 1,   // 1. Correct answer
	  WRONG_ANSWER          = 2,   // 2. Wrong answer
	  PRESENTATION_ERROR    = 3,   // 3. Presentation error
	  COMPILE_ERROR         = 4,   // 4. Compile error
	  TIME_LIMIT_EXCEEDED   = 5,   // 5. Time limit exceed
	  MEMORY_LIMIT_EXCEEDED = 6,   // 6. Memory limit exceed
	  OUTPUT_LIMIT_EXCEEDED = 7,   // 7. Output limit exceed
	  RUNTIME_ERROR_SIGSEGV = 8,   // 8. Runtime-error when buffer overflow, stack
	                               //    overflow or illegal file access
	  RUNTIME_ERROR_SIGFPE  = 9,   // 9. Runtime-error when divided by 0
	  RUNTIME_ERROR_SIGBUS  = 10,  // 10. Runtime-error when hardware error occurred
	  RUNTIME_ERROR_SIGABRT = 11,  // 11. Runtime-error when programme aborted before it
	                               //     should be finished
	  RUNTIME_ERROR_JAVA    = 12,  // 12. Runtime-error of JAVA when there is outputable
	                               //     info
	  RESTRICTED_FUNCTION   = 13,  // 13. Restricted functions, like system call
	  SYSTEM_ERROR          = 14,  // 14. Unknown system errors
	  PENDING               = 15,  // 15. Pending for Judge
	  COMPILING             = 16,  // 16. Judge is compiling the source file
	  RUNNING               = 17,  // 17. Judge is running the submit executable file
	  JUDGING               = 18,  // 18. Judge is compare the submit result and the
	                               //     standard output
	  
	  
	  LAST_JUDGE_STATUS             = 19,
	  FIRST_REPLY_STATUS            = 100,
	  READY                         = 100,   // 100. Judge is ready to judge
	  UNSUPPORTED_SOURCE_FILE_TYPE  = 101,
	  DATA_EXSIST                   = 102,
	  INVALID_FILE_SIZE             = 103,
	  LAST_STATUS                   = 104;
  public static final int
	  FIRST_SOURCE_FILE_TYPE = 0,
	  GCC                    = 0,  // 0, GNU C (.c) gcc
	  GPP                    = 1,  // 1, GNU C++ (.cc) g++
	  JAVA                   = 2,  // 2, Java (.java) SUN JDK
	  PASCAL                 = 3;  // 3, Free Pascal (.pas) fpc

  
  public void addStatus(Status status) {
    Vector row = new Vector();
    String str = new String("System Error");
    row.add(new Integer(status.getStatusId()));
    row.add(new Integer(status.getProblemId()));
    if (status.getContestId() == -1)
      row.add(new String(""));
    else 
      row.add(new Integer(status.getContestId()));
    switch (status.getResult()) {
      case ACCEPTED:
        str = "Accepted";
        break;
      case PRESENTATION_ERROR:
        str = "PE";
        break;
      case TIME_LIMIT_EXCEEDED:
        str = "TLE";
        break;
      case MEMORY_LIMIT_EXCEEDED:
        str = "MLE";
        break;
      case WRONG_ANSWER:
    	str = "WA";
        break;
      case OUTPUT_LIMIT_EXCEEDED:
        str = "OLE";
        break;
      case COMPILE_ERROR:
        str = "CE";
        break;
      case RUNTIME_ERROR_SIGSEGV:
      case RUNTIME_ERROR_SIGFPE:
      case RUNTIME_ERROR_SIGBUS:
      case RUNTIME_ERROR_SIGABRT:
      case RUNTIME_ERROR_JAVA:
        str = "RE";
        break;
    }
    row.add(str);
    row.add("" + status.getMemory()+"KB");
    row.add("" + status.getTime() + "ms");
    String language = new String("GCC");
    switch (status.getLanguage()) {
      case GCC:
       language = "GCC";
       break;
      case GPP:
       language = "G++";
       break;
      case JAVA:
       language = "Java";
       break;
      case PASCAL:
       language = "Pascal";
       break;
    }
    row.add(language);
    row.add("" + status.getCodeLength() + "B");
    row.add(status.getSubmitTime());
    addRow(row);
  }
  
}

public class StatusPanel extends JPanel implements ActionListener {
  
  /**
	 * 
	 */
	private static final long serialVersionUID = 7202004708137649491L;
  
  public StatusPanel(MainFrame father, User user) {
    super();  
    this.user = user;
    this.father = father;
    initPanel();
  }
  
  private void initPanel() {
    setLayout(new BorderLayout());
    initTable();
    JPanel p1 = new JPanel();
    p1.add(refresh);
    add(p1, BorderLayout.SOUTH);
    
    refresh.addActionListener(this);
  }
  
  public void initTable() {
    JTable table_buf = statusTable;
    try {
      StatusTableModel statusTableModel = new StatusTableModel();  
      statusTable = new JTable(statusTableModel) {
	    public TableCellRenderer getCellRenderer(int row, int column) {
          TableCellRenderer renderer = super.getCellRenderer(row, column);
          if (renderer instanceof JLabel) {
            ((JLabel)renderer).setHorizontalAlignment(JLabel.CENTER);
          }
          return renderer;
        }
      };
      Vector statusList = Connection.getInstance().doGetStatus(user);
      Iterator iter = statusList.iterator();
      while (iter.hasNext()) {
        statusTableModel.addStatus((Status)iter.next());
      }
    
      FitTableColumns(statusTable);
      JScrollPane scrollPane = new JScrollPane(statusTable); 
      add(scrollPane, BorderLayout.CENTER);
    }catch (IOException e) {
       statusTable = table_buf;
    }
  }
  
  public void FitTableColumns(JTable myTable){
    JTableHeader header = myTable.getTableHeader();
    int rowCount = myTable.getRowCount();
    Enumeration columns = myTable.getColumnModel().getColumns();
    while(columns.hasMoreElements()){
      TableColumn column = (TableColumn)columns.nextElement();
      
      int col = header.getColumnModel().getColumnIndex(column.getIdentifier());
      int width = (int)myTable.getTableHeader()
                    .getDefaultRenderer()
                      .getTableCellRendererComponent(myTable, 
                                                     column.getIdentifier(), 
                                                     false, 
                                                     false, 
                                                     -1, 
                                                     col)
                         .getPreferredSize()
                           .getWidth();
      
      for(int row = 0; row<rowCount; row++){
        int preferedWidth = (int)myTable.getCellRenderer(row, col)
                              .getTableCellRendererComponent(myTable,
                                                             myTable
                                                               .getValueAt(row, 
                                                            		       col), 
                                                             false, 
                                                             false, 
                                                             row, 
                                                             col)
                                 .getPreferredSize()
                                   .getWidth();
        
        width = Math.max(width, preferedWidth);
      }
      
      header.setResizingColumn(column); 
      column.setWidth((int)(width * 1.4) +myTable.getIntercellSpacing().width);
    }
  }
  
  boolean hasGet = false;
  private User user;
  private JTable statusTable = new JTable(new StatusTableModel());
  private JButton refresh= new JButton("Refresh");
  private MainFrame father;

  public void actionPerformed(ActionEvent e) {
    if (e.getSource() == refresh) {
      this.remove(statusTable);
      initTable();
      father.repaint();
    }
  } 
}
