<?php

$IMGDIR = '127.0.0.1/files/'; //image folder
$ADMINSERVER = '127.0.0.1';
$DOCUMENT_ROOT = '/var/www/flood/';

$LANGUAGE = array('GCC','G++','JAVA','PASCAL');

$STATUS_COLOR = array(
    '#0000dd', //0 Waiting
    '#dd0000', //1 AC
    '#858530', //2 WA
    '#00dd00', //3 PE
    '#000000', //4 CE
    '#66bb66', //5 TLE
    '#66bb66', //6 MLE
    '#66bb66', //7 OLE
    '#770077', //8 RE SIGSIGV
    '#770077', //9 RE
    '#770077', //10 RE
    '#770077', //11 RE
    '#770077', //12 RE
    '#770077', //13 RE
    '#770077', //14 RE
    '#0000ff', //15 RE
    '#0000ff', //16 RE
    '#0000ff', //17 RE
    '#0000ff', //18 RE
);

$JUDGE_STATUS = array(
  'Waiting...',
  'Accepted',           // 1. Correct answer
  'Wront Answer',           // 2. Wrong answer
  'Presentation Error',     // 3. Presentation error
  'Compile Error',          // 4. Compile error
  'Time Limit Exceeded',    // 5. Time limit exceed
  'Memory Limit Exceeded',  // 6. Memory limit exceed
  'Output Limit Exceeded',  // 7. Output limit exceed
  'Runtime Error(SIGSEGV)',  // 8. Runtime-error when buffer overflow, stack
                          //    overflow or illegal file access
  'Runtime Error(SIGFPE)',   // 9. Runtime-error when divided by 0
  'Runtime Error(SIGBUS)',   // 10. Runtime-error when hardware error occurred
  'Runtime Error(SIGABRT)',  // 11. Runtime-error when programme aborted before it
                          //     should be finished
  'Runtime Error(JAVA)',     // 12. Runtime-error of JAVA when there is outputable
                          //     info
  'Restricted Function',    // 13. Restricted functions, like system call
  'System Error',           // 14. Unknown system errors
  'Pending',                // 15. Pending for Judge
  'Compiling',              // 16. Judge is compiling the source file
  'Running',                // 17. Judge is running the submit executable file
  'Judging',                // 18. Judge is compare the submit result and the
  )                       //     standard output

?>
