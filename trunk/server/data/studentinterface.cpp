#include "studentinterface.h"

StudentInterface* StudentInterface::instance = NULL;

int StudentInterface::addStudent(const Student& student);

int StudentInterface::updateStudent(const Student& student);

int StudentInterface::disableStudent(const string& user_id);

