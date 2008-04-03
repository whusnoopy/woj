
#ifndef _BASE_FLOOD_H__
#define _BASE_FLOOD_H__

const int COMPILE_TIME_LIMIT = 30;
const int MAX_PATH_LENGTH = 256;

enum SOURCE_FILE_TYPE {
  FIRST_SOURCE_FILE_TYPE = 0,
  GCC = 0,  // 0, GNU C (.c) gcc
  GPP,      // 1, GNU C++ (.cc) g++
  JAVA,     // 2, Java (.java) SUN JDK
  PASCAL,   // 3, Free Pascal (.pas) fpc
};

const string SOURCE_FILE_SUFFIX[] = {
  "c",
  "cc",
  "java",
  "pas"
};

#endif

