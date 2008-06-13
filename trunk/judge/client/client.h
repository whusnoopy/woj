#ifndef _JUDGE_CLIENT_CLIENT_H__
#define _JUDGE_CLIENT_CLIENT_H__

// Max path length
const int MAX_PATH_LENGTH = 256;

// Time limit for compiling, 30s
const int COMPILE_TIME_LIMIT = 30 * 1000;

// Max time limit for any submit, 60s
const int MAX_TIME_LIMIT = 60 * 1000;

// Max case timie limit for any submit, 60s
const int MAX_CASE_TIME_LIMIT = 60 * 1000;

// Max memory limit for any submit, 1G
const int MAX_MEMORY_LIMIT = 1 * 1024 * 1024;

// Output limit for any process, 64M
const int MAX_OUTPUT_LIMIT = 64 * 1024;

// Max test case number, 60
const int MAX_TEST_CASE = 60;

// Java time limit blow up multiple times, *5
const int JAVA_TIME_LIMIT_MULTIPLE = 5;

// Java time limit blow up remainder, 2s
const int JAVA_TIME_LIMIT_REMAINDER = 2000;

#endif

