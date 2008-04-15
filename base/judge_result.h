// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _BASE_JUDGE_RESULT_H__
#define _BASE_JUDGE_RESULT_H__

#include <string>

using namespace std;

enum COMMUNICATION_STATUS {
  FIRST_STATUS = 1,
  ACCEPTED = 1,           // 1. Correct answer
  WRONG_ANSWER,           // 2. Wrong answer
  PRESENTATION_ERROR,     // 3. Presentation error
  COMPILE_ERROR,          // 4. Compile error
  TIME_LIMIT_EXCEEDED,    // 5. Time limit exceed
  MEMORY_LIMIT_EXCEEDED,  // 6. Memory limit exceed
  OUTPUT_LIMIT_EXCEEDED,  // 7. Output limit exceed
  RUNTIME_ERROR_SIGSEGV,  // 8. Runtime-error when buffer overflow, stack
                          //    overflow or illegal file access
  RUNTIME_ERROR_SIGFPE,   // 9. Runtime-error when divided by 0
  RUNTIME_ERROR_SIGBUS,   // 10. Runtime-error when hardware error occurred
  RUNTIME_ERROR_SIGABRT,  // 11. Runtime-error when programme aborted before it
                          //     should be finished
  RUNTIME_ERROR_JAVA,     // 12. Runtime-error of JAVA when there is outputable
                          //     info
  RESTRICTED_FUNCTION,    // 13. Restricted functions, like system call
  SYSTEM_ERROR,           // 14. Unknown system errors
  PENDING,                // 15. Pending for Judge
  COMPILING,              // 16. Judge is compiling the source file
  RUNNING,                // 17. Judge is running the submit executable file
  JUDGING,                // 18. Judge is compare the submit result and the
                          //     standard output
  LAST_JUDGE_STATUS,
  FIRST_REPLY_STATUS = 100,
  READY = 100,            // 100. Judge is ready to judge
  UNSUPPORTED_SOURCE_FILE_TYPE,
  DATA_EXSIST,
  INVALID_FILE_SIZE,
  LAST_STATUS,
};

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
