#ifndef _FLOOD_SERVER_DATA_USER_H__
#define _FLOOD_SERVER_DATA_USER_H__

#include <string>
#include <iostream>
using namespace std;

#include "list.h"

class User{
public:
  User(const string& user_id = "NULL") :user_id_(user_id){
  }

  User(const string& user_id,
       const string& password,
       const string& email,
       bool show_email,
       const string& nickname,
       const string& school,
       int submit,
       int solved,
       bool share_code,
       int volume,
       int language,
       bool available,
       string last_login_ip,
       string last_login_time,
       string reg_time,
       int permission,
       string indentify_code);
  string getId() const;
  void setId(const string& id);
  string getPassword() const;
  void setPassword(const string& password);
  string getEmail() const;
  void setEmail(const string& email);
  bool getShowEmail() const;
  void setShowEmail(bool show_email);
  string getNickname() const;
  void setNickname(const string& nickname);
  string getSchool() const;
  void setSchool(const string& school);
  int getSubmit() const;
  void setSubmit(int submit);
  int getSolved() const;
  void setSolved(int solved);
  bool getShareCode() const;
  void setShareCode(bool share_code);
  int getVolume() const;
  void setVolume(int volume);
  int getLanguage() const;
  void setLanguage(int language);
  bool getAvailable() const;
  void setAvailable(bool available);
  string getLastLoginIp() const;
  void setLastLoginIp(const string& last_login_ip);
  string getLastLoginTime() const;
  void setLastLoginTime(const string& last_login_time);
  string getRegTime() const;
  void setRegTime(const string& reg_time);
  int getPermission() const;
  void setPermission(int permission);
  string getIndentifyCode() const;
  void setIndentifyCode(const string& indentify_code);
  UserListItem getUserListItem() const;
  RankListItem getRankListItem() const;

private:
  string user_id_;
  string password_;
  string email_;
  bool show_email_;
  string nickname_;
  string school_;
  int submit_;
  int solved_;
  bool share_code_;
  int volume_;
  int language_;
  bool available_;
  string last_login_ip_;
  string last_login_time_;
  string reg_time_;
  int permission_;
  string indentify_code_;
};

#endif
