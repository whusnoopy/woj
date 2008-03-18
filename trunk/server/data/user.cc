#include "user.h"

User::User(const string& user_id,
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
           string indentify_code)
:  user_id_(user_id),
   password_(password),
   email_(email),
   show_email_(show_email),
   nickname_(nickname),
   school_(school),
   submit_(submit),
   solved_(solved),
   share_code_(share_code),
   volume_(volume),
   language_(language),
   available_(available),
   last_login_ip_(last_login_ip),
   last_login_time_(last_login_time),
   reg_time_(reg_time),
   permission_(permission),
   indentify_code_(indentify_code){
}

string User::getId() const{
  return user_id_;
}
void User::setId(const string& id){
  user_id_ =  id;
}
string User::getPassword() const{
  return password_;
} 
void User::setPassword(const string& password){
  password_ =  password;
}
string User::getEmail() const{
  return email_;
}
void User::setEmail(const string& email){
  email_ = email;
}
bool User::getShowEmail() const{
  return show_email_;
}

void User::setShowEmail(bool show_email){
  show_email_=show_email;
}

string User::getNickname() const{
  return nickname_;
}

void User::setNickname(const string& nickname){
  nickname_ = nickname;
}

string User::getSchool() const{
  return school_;
}

void User::setSchool(const string& school){
  school_ = school;
}

int User::getSubmit() const{
  return submit_;
}

void User::setSubmit(int submit){
  submit_ = submit;
}

int User::getSolved() const{
  return solved_;

}

void User::setSolved(int solved){
  solved_ = solved;
}

bool User::getShareCode() const{
  return share_code_;
}

void User::setShareCode(bool share_code){
  share_code_ = share_code;
}

int User::getVolume() const{
 return volume_;
}

void User::setVolume(int volume){
  volume_ = volume;
}

int User::getLanguage() const{
  return language_;
}

void User::setLanguage(int language){
  language_ = language;
}

bool User::getAvailable() const{
  return available_;
}

void User::setAvailable(bool available){
  available_ = available;
}

string User::getLastLoginIp() const{
  return last_login_ip_;
}

void User::setLastLoginIp(const string& last_login_ip){
  last_login_ip_ = last_login_ip;
}

string User::getLastLoginTime() const{
  return last_login_time_;
}
  
void User::setLastLoginTime(const string& last_login_time){
  last_login_time_ = last_login_time;
}

string User::getRegTime() const{
  return reg_time_;
}

void User::setRegTime(const string& reg_time){
  reg_time_ = reg_time;
}

int User::getPermission() const{
  return permission_;
}

void User::setPermission(int permission){
  permission_ = permission;
}

string User::getIndentifyCode() const{
  return indentify_code_;
}

void User::setIndentifyCode(const string& indentify_code){
  indentify_code_ = indentify_code;
}

UserListItem User::getUserListItem() const{
  UserListItem item;
  item.user_id = user_id_;
  item.nickname = nickname_;
  item.last_login_time = last_login_time_;
  item.last_login_ip = last_login_ip_;
  item.reg_time = reg_time_;
  return item;
}

RankListItem User::getRankListItem() const{
  RankListItem item;
  item.user_id = user_id_;
  item.nickname = nickname_;
  item.sovled = solved_;
  item.submit = submit_;
}

