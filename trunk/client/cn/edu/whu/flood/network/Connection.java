package cn.edu.whu.flood.network;

import java.io.*;
import java.net.*;
import java.util.*;
import java.text.*;

import cn.edu.whu.flood.object.*;
import cn.edu.whu.flood.ui.*;

public class Connection {
  
  public Connection(String ip, int port) {
    this.ip = ip;
    this.port = port;
  }
  
  public boolean doLogin(User user) {
    char ret = 'N';
    try {
      socket = new Socket(ip, port);
      BufferedReader in = new BufferedReader(new InputStreamReader(
                                                      socket.getInputStream()));
      PrintWriter out = new PrintWriter(socket.getOutputStream());
      
      String header = new String("li");
      String data = new String(user.getUserId() + '\001' + user.getPassword());
      System.out.println(data);
      DecimalFormat df = new DecimalFormat("00000000");
      header += df.format(data.length());
      System.out.println(header);
      out.print(header);
      out.flush();
      out.print(data);
      out.flush();
      
      ret = (char)in.read();  
      in.close();
      out.close();
      
      socket.close();
    }catch(IOException e) {
       PostMessage(e);
    }
    return ret == 'Y';
  }
  
  public void doInitClient(Vector problemMenuList, Vector contestMenuList) {
    try {
      socket = new Socket(ip, port);
      BufferedReader in = new BufferedReader(new InputStreamReader(
                                                      socket.getInputStream()));
      PrintWriter out = new PrintWriter(socket.getOutputStream());
      
      
      String header = "cp00000000";
      out.print(header);
      out.flush();
      
           
      in.readLine();     
      String sep = "\001";
      String databuf = in.readLine();
      System.out.println("databuf : " + databuf);
      String data[] = databuf.split(sep);
      in.close();
      out.close();
      socket.close();
      
      int k = 0;
      
      int problemListSize = Integer.parseInt(data[k++]);
      int up = 1;
      int down = up + 25;
      int bigDown = up + 500;
      Vector problemList = new Vector();
      Vector problemBigList = new Vector();
      int problemId = 0;
      for (int i = 0; i < problemListSize; i++) {
        problemId = Integer.parseInt(data[k++]);
        if (i == problemListSize - 1) {
        	problemList.add(new Integer(problemId));
        	ProblemSet problemSet = new ProblemSet(up, down, problemList);
            problemBigList.add(problemSet);
            ProblemSet problemBigSet = new ProblemSet(bigDown - 500, 
                                                      bigDown, 
                                                      problemBigList);
            problemMenuList.add(problemBigSet);
            continue;
        }
               
        if (problemId < down) {
          problemList.add(new Integer(problemId));
        } else {
          ProblemSet problemSet = new ProblemSet(up, down, problemList);
          problemBigList.add(problemSet);
          problemList = new Vector();
          problemList.add(new Integer(problemId));
          up = down;
          down = up + 25;
        } 
        if (problemId >= bigDown) {
          ProblemSet problemBigSet = new ProblemSet(bigDown - 500, 
                                                    bigDown, 
                                                    problemBigList);
          problemMenuList.add(problemBigSet);
          problemBigList = new Vector();
          bigDown += 500;
        }
      }
      
      int contestListSize = Integer.parseInt(data[k++]);
      for (int i = 0; i < contestListSize; i++) {
        int contestId = Integer.parseInt(data[k++]);
        String title = data[k++];
        int contestProblemNum = Integer.parseInt(data[k++]);
        Vector contestProblemList = new Vector();
        for (int j = 0; j < contestProblemNum; j++) {
          int contestProblemId = Integer.parseInt(data[k++]);
          contestProblemList.add(new Integer(contestProblemId));
        }
        Contest contest = new Contest(contestId, contestProblemList, title);
        contestMenuList.add(contest);
      }
      
      
      }catch(IOException e) {
        PostMessage(e);      
      }
  }
  
  public Vector doGetStatus(User user) throws IOException{
    Vector statusList = new Vector();
    socket = new Socket(ip, port);
    BufferedReader in = new BufferedReader(new InputStreamReader(
                                                  socket.getInputStream()));
    PrintWriter out = new PrintWriter(socket.getOutputStream());
    String data = user.getUserId();
    String header = "st";
    DecimalFormat df = new DecimalFormat("00000000");
    header += df.format(data.length());
    out.print(header);
    out.flush();
    out.print(data);
    out.flush();    
  
    int length = Integer.parseInt(in.readLine());
    System.out.println("size : " + length);
    String sep = "\001";
    String databuf = in.readLine();
    System.out.println(databuf);
    String[] strSet = databuf.split(sep);
    in.close();
    out.close();
    socket.close();
    int size = Integer.parseInt(strSet[0]);
    Status status = new Status();
    for (int i = 0; i < size; i++) {
      status.setStatusId(Integer.parseInt(strSet[i*9 + 1]));
      status.setProblemId(Integer.parseInt(strSet[i*9 + 2]));
      status.setContestId(Integer.parseInt(strSet[i*9 + 3]));
      status.setResult(Integer.parseInt(strSet[i*9 + 4]));
      status.setMemory(Integer.parseInt(strSet[i*9 + 5]));
      status.setTime(Integer.parseInt(strSet[i*9 + 6]));
      status.setLanguage(Integer.parseInt(strSet[i*9 + 7]));
      status.setCodeLength(Integer.parseInt(strSet[i*9 + 8]));
      status.setSubmitTime(strSet[i*9 + 9]);
      statusList.add(status);
    }   
    return statusList;
  }
  
  public boolean doSubmit(User user, Status status, String source) {
    char ret = 'N';
    try {
      socket = new Socket(ip, port);
      BufferedReader in = new BufferedReader(new InputStreamReader(
                                                      socket.getInputStream()));
      PrintWriter out = new PrintWriter(socket.getOutputStream());
      
      String data = user.getUserId() + user.getPassword();
      DecimalFormat df = new DecimalFormat("##########");
      data += df.format(status.getProblemId());
      data += df.format(status.getContestId());
      data += df.format(status.getLanguage());
      data += df.format(status.getCodeLength());
      data += status.isShareCode()?"Y":"N";
      data += 'N';
      String header = "sm";
      df = new DecimalFormat("00000000");
      header += df.format(data.length());
      
      out.print(header);
      out.flush();
      out.print(data);
      out.flush();
      out.print(source);
      out.flush();   
      
      ret = (char)in.read();
      in.close();
      out.close();
      
      socket.close();
    }catch(IOException e) {
      PostMessage(e);      
    }
    return ret == 'Y';
  }
  
  public void PostMessage(IOException e) {
     
  }
  
  private static Connection instance = null;
  
  public static Connection getInstance() {
    if (instance == null) {
	  String ip = new String();
      int port = 3642;
	  try {
	    BufferedReader in = new BufferedReader(new FileReader("/home/flood/configure.txt"));
	    ip = in.readLine();
	    port = Integer.parseInt(in.readLine());
	    in.close();
	  } catch (Exception e) {
	    e.printStackTrace();
	  }
      instance = new Connection(ip, port);
    }
    return instance;
  }
  
  private String ip;
  private int port;
  private Socket socket;
}
