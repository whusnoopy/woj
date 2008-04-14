package cn.edu.whu.flood.object;

import java.util.*;

public class ProblemSet {
  public ProblemSet() {
    
  }
  
  public ProblemSet(int upProblem, int downProblem, Vector problemList) {
    this.upProblem = upProblem;
    this.downProblem = downProblem;
    this.problemList = new Vector(problemList);
  }
  
  public int getUpProblem() {
    return upProblem;
  }
  
  public int getDownProblem() {
    return downProblem;
  }
  
  public void setUpProblem(int upProblem) {
    this.upProblem = upProblem;
  }
  
  public void setDownProblem(int downProblem) {
    this.downProblem = downProblem;
  }
  
  public Vector getProblemList() {
    return problemList;
  }
  
  public void setProblemList(Vector problemList) {
    this.problemList = problemList;
  }
  
  public String toString() {
	 return new String(upProblem + "-" + (downProblem-1)); 
  }
  
  private int upProblem;
  private int downProblem;
  private Vector problemList;
}
