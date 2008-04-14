package cn.edu.whu.flood.object;

public class User {
    
  public User() {
    
  }
    
  public User(String user_id, String password) {
    this.user_id = user_id;
    this.password = password;
  }
  
  public String getUserId() {
    return user_id;
  }
  
  public String getPassword() {
    return password;
  }
  
  public void setUserId(String user_id) {
    this.user_id = user_id;
  }
    
  public void setPassword(String password) {
    this.password = password;
  }
    
  private String user_id;
  private String password;
}
