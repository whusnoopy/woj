// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _FLOOD_JUDGE_CLIENT_COMPILE_H__
#define _FLOOD_JUDGE_CLIENT_COMPILE_H__

#include <string>

using namespace std;

int doCompile(int communicate_socket,
              const string& source_filename);

#endif

