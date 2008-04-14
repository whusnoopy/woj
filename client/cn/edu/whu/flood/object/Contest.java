package cn.edu.whu.flood.object;

import java.util.*;

public class Contest {

  public Contest() {
    
  }
  
  public Contest(int contestId, Vector problemList, String title) {
    this.contestId = contestId;
    this.problemList = problemList;
    this.title = title;
  }
  
  public int getContestId() {
    return contestId;
  }
  public void setContestId(int contestId) {
    this.contestId = contestId;
  }
  public Vector getProblemList() {
    return problemList;
  }
  public void setProblemList(Vector problemList) {
    this.problemList = new Vector(problemList);
  }

  public String getTitle() {
    return title;
  }

public void setTitle(String title) {
    this.title = title;
  }
  
  private int contestId;
  private Vector problemList;
  private String title;

}
