<?php

$IMGDIR = '192.168.5.182/files/'; //image folder
$ADMINSERVER = '192.168.5.182';

$LANGUAGE = array('GCC','G++','JAVA','PASCAL');

$JUDGE_STATUS = array(
  'UNKNOW',
  'ACCEPT',           // 1. Correct answer
  'WRONG_ANSWER',           // 2. Wrong answer
  'PRESENTATION_ERROR',     // 3. Presentation error
  'COMPILE_ERROR',          // 4. Compile error
  'TIME_LIMIT_EXCEEDED',    // 5. Time limit exceed
  'MEMORY_LIMIT_EXCEEDED',  // 6. Memory limit exceed
  'OUTPUT_LIMIT_EXCEEDED',  // 7. Output limit exceed
  'RUNTIME_ERROR_SIGSEGV',  // 8. Runtime-error when buffer overflow, stack
                          //    overflow or illegal file access
  'RUNTIME_ERROR_SIGFPE',   // 9. Runtime-error when divided by 0
  'RUNTIME_ERROR_SIGBUS',   // 10. Runtime-error when hardware error occurred
  'RUNTIME_ERROR_SIGABRT',  // 11. Runtime-error when programme aborted before it
                          //     should be finished
  'RUNTIME_ERROR_JAVA',     // 12. Runtime-error of JAVA when there is outputable
                          //     info
  'RESTRICTED_FUNCTION',    // 13. Restricted functions, like system call
  'SYSTEM_ERROR',           // 14. Unknown system errors
  'PENDING',                // 15. Pending for Judge
  'COMPILING',              // 16. Judge is compiling the source file
  'RUNNING',                // 17. Judge is running the submit executable file
  'JUDGING',                // 18. Judge is compare the submit result and the
  )                       //     standard output

?>