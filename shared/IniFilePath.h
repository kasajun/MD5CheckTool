#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __INIFILEPATH_H_F7E37D0F_8FE9_4c20_B2BA_1CAFA1BF8790__
#define __INIFILEPATH_H_F7E37D0F_8FE9_4c20_B2BA_1CAFA1BF8790__

#include <tchar.h>

#ifndef MAX_PATH_SIZE
#ifdef _UNICODE
#define MAX_PATH_SIZE					(_MAX_PATH * 4)
#define MAX_PATH_SIZE_ALIGNMENT			(272 * 4)
#else
#define MAX_PATH_SIZE					(_MAX_PATH * 2)
#define MAX_PATH_SIZE_ALIGNMENT			(272 * 2)
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

int GetIniFilePath(TCHAR* pInPath, const TCHAR* cpInINIFileName, const TCHAR* cpInAppName);

#ifdef __cplusplus
}
#endif
#endif
