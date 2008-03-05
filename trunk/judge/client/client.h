// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _FLOOD_JUDGE_CLIENT_CLIENT_H__
#define _FLOOD_JUDGE_CLIENT_CLIENT_H__

enum COMMUNICATION_STATUS {
  FIRST_STATUS,
  ACCEPTED = 0,           // 0. Correct answer
  PRESENTATION_ERROR,     // 1. Presentation error
  TIME_LIMIT_EXCEED,      // 2. Time limit exceed
  MEMORY_LIMIT_EXCEED,    // 3. Memory limit exceed
  WRONG_ANSWER,           // 4. Wrong answer
  OUTPUT_LIMIT_EXCEED,    // 5. Output limit exceed
  COMPILE_ERROR,          // 6. Compile error
  RUNTIME_ERROR_SIGSEGV,  // 7. Runtime-error when buffer overflow, stack
                          //    overflow or illegal file access
  RUNTIME_ERROR_SIGFPE,   // 8. Runtime-error when divided by 0
  RUNTIME_ERROR_SIGBUG,   // 9. Runtime-error when hardware error occurred
  RUNTIME_ERROR_SIGABRT,  // 10. Runtime-error when programme aborted before it
                          //     should be finished
  RUNTIME_ERROR_JAVA,     // 11. Runtime-error of JAVA when there is outputable
                          //     info
  RUNTIME_ERROR_PAS,      // 12. Runtime-error of PASCAL when there is
                          //     outputable info
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
  LAST_STATUS,
};

#endif
