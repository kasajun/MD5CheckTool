#if _MSC_VER > 1000
#pragma once
#endif

/*
 * getopt - POSIX like getopt for Windows console Application
 *
 * win-c - Windows Console Library
 * Copyright (c) 2015 Koji Takami
 * Released under the MIT license
 * https://github.com/takamin/win-c/blob/master/LICENSE
 */

#ifndef __GETOPT_H_B67FA15E_0E68_4885_A402_F7DF12FE675E__
#define __GETOPT_H_B67FA15E_0E68_4885_A402_F7DF12FE675E__

#include <tchar.h>

#ifdef __cplusplus
extern "C" {
#endif

int getopt(int argc, TCHAR* const argv[], const TCHAR* optstring);

extern TCHAR* optarg;
extern int optind, opterr, optopt;

#define no_argument			0
#define required_argument	1
#define optional_argument	2

struct option {
	const TCHAR* name;
	int has_arg;
	int* flag;
	int val;
};

int getopt_long(int argc, TCHAR* const argv[], const TCHAR* optstring, const struct option* longopts, int* longindex);

#ifdef __cplusplus
}
#endif
#endif
