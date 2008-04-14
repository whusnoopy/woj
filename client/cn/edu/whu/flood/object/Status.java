package cn.edu.whu.flood.object;

public class Status {
    
  private int statusId;
  private int problemId;
  private int contestId;
  private int result;
  private int memory;
  private int time;
  private int language;
  private int codeLength;
  private String submitTime;
  private boolean shareCode;
  
  public boolean isShareCode() {
	return shareCode;
  }


  public void setShareCode(boolean shareCode) {
	this.shareCode = shareCode;
  }


  public Status() {
      
  }
  

  public Status(int statusId, 
		        int problemId, 
		        int contestId, 
		        int result, 
		        int memory, 
		        int time, 
		        int language, 
		        int codeLength, 
		        String submitTime) {
	super();
	this.statusId = statusId;
	this.problemId = problemId;
	this.contestId = contestId;
	this.result = result;
	this.memory = memory;
	this.time = time;
	this.language = language;
	this.codeLength = codeLength;
	this.submitTime = submitTime;
  }

  public int getCodeLength() {
	return codeLength;
  }

  public void setCodeLength(int codeLength) {
	this.codeLength = codeLength;
  }

  public int getContestId() {
	return contestId;
  }

  public void setContestId(int contestId) {
	this.contestId = contestId;
  }

  public int getLanguage() {
	return language;
  }

  public void setLanguage(int language) {
	this.language = language;
  }

  public int getMemory() {
	return memory;
  }

  public void setMemory(int memory) {
	this.memory = memory;
  }

  public int getProblemId() {
	return problemId;
  }

  public void setProblemId(int problemId) {
	this.problemId = problemId;
  }

  public int getResult() {
	return result;
  }

  public void setResult(int result) {
	this.result = result;
  }

  public int getStatusId() {
	return statusId;
  }

  public void setStatusId(int statusId) {
	this.statusId = statusId;
  }

  public String getSubmitTime() {
	return submitTime;
  }

  public void setSubmitTime(String submitTime) {
	this.submitTime = submitTime;
  }

  public int getTime() {
	return time;
  }

  public void setTime(int time) {
	this.time = time;
  }
}
