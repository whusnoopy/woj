// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _FLOOD_JUDGE_CLIENT_JUDGE_H__
#define _FLOOD_JUDGE_CLIENT_JUDGE_H__

#include <string>

using namespace std;

int doJudge(int communicate_socket,
            const string& standard_input_file_name,
            const string& standard_output_file_name,
            const string& users_output_file_name,
            const string& special_judge_executable_file_name);

#endif
