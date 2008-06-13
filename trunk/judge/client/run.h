// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _JUDGE_CLIENT_RUN_H__
#define _JUDGE_CLIENT_RUN_H__

#include <string>

using namespace std;

int doRun(int communicate_socket,
          const string& working_dir,
          const string& program_name,
          const string& source_file_type,
          const string& input_filename,
          const string& program_output_filename,
          int time_limit,
          int memory_limit,
          int output_limit);

#endif

