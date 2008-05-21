#ifndef _FLOOD_SERVER_DATA_TEACHERINTERFACE_H__
#define _FLOOD_SERVER_DATA_TEACHERINTERFACE_H__

#include <string>

#include "object/teacher.h"
#include "data/connectionfactory.h"
using namespace std;

class TeacherInterface: public ConnectionFactory{
public:
  TeacherInterface() {
  }
  
  ~TeacherInterface() {
  }

  int addTeacher(const Teacher&);
  int updateTeacher(const Teacher&);
  int disableTeacher(const string& user_id, bool available);
  int addControlClass(const string& user_id, const Class& mclass);
  int deleteControlClass(const string& user_id, const Class& mclass);

  static TeacherInterface& getInstance(){
    if(instance == NULL)
      instance = new TeacherInterface;
    return *instance;
  }

  static void destroy(){
    if(instance)
      delete instance;
    instance = NULL;
  }
private:
  static TeacherInterface * instance;
  
};


#endif
