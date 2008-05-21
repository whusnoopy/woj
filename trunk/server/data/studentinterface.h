#ifndef _FLOOD_SERVER_DATA_STUDENTINTERFACE_H__
#define _FLOOD_SERVER_DATA_STUDENTINTERFACE_H__

#include <string>

#include "object/student.h"
#include "data/connectionfactory.h"
using namespace std;

class StudentInterface: public ConnectionFactory{
public:
  StudentInterface() {
  }
  
  ~StudentInterface() {
  }

  int addStudent(const Student&);
  int updateStudent(const Student&);
  int disableStudent(const string& user_id, bool available);
  StudentList getStudentList(int grade);
  Student getStudent(const string& user_id);

  static StudentInterface& getInstance(){
    if(instance == NULL)
      instance = new StudentInterface;
    return *instance;
  }

  static void destroy(){
    if(instance)
      delete instance;
    instance = NULL;
  }
private:
  static StudentInterface * instance;
  
};


#endif
