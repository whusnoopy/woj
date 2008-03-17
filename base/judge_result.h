// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _BASE_JUDGE_RESULT_H__
#define _BASE_JUDGE_RESULT_H__

enum COMMUNICATION_STATUS {
  FIRST_STATUS = 1,
  ACCEPTED = 1,           // 1. Correct answer
  PRESENTATION_ERROR,     // 2. Presentation error
  TIME_LIMIT_EXCEEDED,    // 3. Time limit exceed
  MEMORY_LIMIT_EXCEEDED,  // 4. Memory limit exceed
  WRONG_ANSWER,           // 5. Wrong answer
  OUTPUT_LIMIT_EXCEEDED,  // 6. Output limit exceed
  COMPILE_ERROR,          // 7. Compile error
  RUNTIME_ERROR_SIGSEGV,  // 8. Runtime-error when buffer overflow, stack
                          //    overflow or illegal file access
  RUNTIME_ERROR_SIGFPE,   // 9. Runtime-error when divided by 0
  RUNTIME_ERROR_SIGBUS,   // 10. Runtime-error when hardware error occurred
  RUNTIME_ERROR_SIGABRT,  // 11. Runtime-error when programme aborted before it
                          //     should be finished
  RUNTIME_ERROR_JAVA,     // 12. Runtime-error of JAVA when there is outputable
                          //     info
  RUNTIME_ERROR_PAS,      // 13. Runtime-error of PASCAL when there is
                          //     outputable info
  RESTRICTED_FUNCTION,    // 14. Restricted functions, like system call
  SYSTEM_ERROR,           // 15. Unknown system errors
  PENDING,                // 16. Pending for Judge
  COMPILING,              // 17. Judge is compiling the source file
  RUNNING,                // 18. Judge is running the submit executable file
  JUDGING,                // 19. Judge is compare the submit result and the
                          //     standard output
  LAST_JUDGE_STATUS,
  FIRST_REPLY_STATUS = 100,
  READY = 100,            // 100. Judge is ready to judge
  LAST_STATUS,
};

const int COMPILE_TIME_LIMIT = 30;

#endif
