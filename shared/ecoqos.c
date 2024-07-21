// ecoqos.c

#include <Windows.h>
#include <tchar.h>
#include "charlib.h"
#include "charlib_sysdll.h"


#if _MSC_VER < 1700
#ifndef MEMORY_PRIORITY_LOWEST
#define MEMORY_PRIORITY_LOWEST           0
#define MEMORY_PRIORITY_VERY_LOW         1
#define MEMORY_PRIORITY_LOW              2
#define MEMORY_PRIORITY_MEDIUM           3
#define MEMORY_PRIORITY_BELOW_NORMAL     4
#define MEMORY_PRIORITY_NORMAL           5
#endif

#ifndef PROCESS_POWER_THROTTLING_CURRENT_VERSION
typedef struct _MEMORY_PRIORITY_INFORMATION {
	ULONG MemoryPriority;
} MEMORY_PRIORITY_INFORMATION, * PMEMORY_PRIORITY_INFORMATION;

typedef enum _PROCESS_INFORMATION_CLASS {
	ProcessMemoryPriority,          // MEMORY_PRIORITY_INFORMATION
	ProcessMemoryExhaustionInfo,    // PROCESS_MEMORY_EXHAUSTION_INFO
	ProcessAppMemoryInfo,           // APP_MEMORY_INFORMATION
	ProcessInPrivateInfo,           // BOOLEAN
	ProcessPowerThrottling,         // PROCESS_POWER_THROTTLING_STATE
	ProcessReservedValue1,          // Used to be for ProcessActivityThrottlePolicyInfo
	ProcessTelemetryCoverageInfo,   // TELEMETRY_COVERAGE_POINT
	ProcessProtectionLevelInfo,     // PROCESS_PROTECTION_LEVEL_INFORMATION
	ProcessLeapSecondInfo,          // PROCESS_LEAP_SECOND_INFO
	ProcessMachineTypeInfo,         // PROCESS_MACHINE_INFORMATION
	ProcessInformationClassMax
} PROCESS_INFORMATION_CLASS;

#define PROCESS_POWER_THROTTLING_CURRENT_VERSION 1

#define PROCESS_POWER_THROTTLING_EXECUTION_SPEED 0x1
#define PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION 0x4

#define PROCESS_POWER_THROTTLING_VALID_FLAGS ((PROCESS_POWER_THROTTLING_EXECUTION_SPEED | \
											   PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION))

typedef struct _PROCESS_POWER_THROTTLING_STATE {
	ULONG Version;
	ULONG ControlMask;
	ULONG StateMask;
} PROCESS_POWER_THROTTLING_STATE, * PPROCESS_POWER_THROTTLING_STATE;
#endif
#endif

int SetEcoQos(int nIsEcoQos)
{
	HANDLE hProcess = GetCurrentProcess();
	PROCESS_POWER_THROTTLING_STATE PowerThrottling = { 0 };
	MEMORY_PRIORITY_INFORMATION MemPrio = { 0 };
#if _MSC_VER < 1700
	HMODULE hDll = NULL;
#endif
	DWORD dwPriorityClass = 0;
	int nRet = FALSE;

#if _MSC_VER < 1700
	hDll = LOADSYSLIB(_T("KERNEL32.DLL"));
	if (hDll != NULL)
	{
		BOOL(WINAPI * SetProcessInformation)(HANDLE hProcess, PROCESS_INFORMATION_CLASS ProcessInformationClass, LPVOID ProcessInformation, DWORD ProcessInformationSize);
		const char* szFunc = "SetProcessInformation";

		SetProcessInformation = (BOOL(WINAPI*)(HANDLE, PROCESS_INFORMATION_CLASS, LPVOID, DWORD))(GetProcAddress(hDll, szFunc));
		if (SetProcessInformation != NULL)
		{
#endif
			PowerThrottling.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;

			if (nIsEcoQos)
			{
				dwPriorityClass = IDLE_PRIORITY_CLASS;
				MemPrio.MemoryPriority = MEMORY_PRIORITY_LOW;
				PowerThrottling.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
				PowerThrottling.StateMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
			}
			else
			{
				dwPriorityClass = NORMAL_PRIORITY_CLASS;
				MemPrio.MemoryPriority = MEMORY_PRIORITY_NORMAL;
				PowerThrottling.ControlMask = 0;
				PowerThrottling.StateMask = 0;
			}

			nRet = SetPriorityClass(hProcess, dwPriorityClass);
			if (nRet)
			{
				SetProcessInformation(hProcess, ProcessMemoryPriority, &MemPrio, sizeof(MemPrio));
				SetProcessInformation(hProcess, ProcessPowerThrottling, &PowerThrottling, sizeof(PowerThrottling));
				nRet = TRUE;
			}
#if _MSC_VER < 1700
		}
		FreeLibrary(hDll);
	}
#endif
	return nRet;
}
