#include <windows.h>
#include <iostream>
#include "Bastard.hpp"

using namespace std;
bool SetPrivilege(LPCWSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES priv = { 0,0,0,0 };
	HANDLE hToken = NULL;
	LUID luid = { 0,0 };
	BOOL Status = true;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		Status = false;
		goto EXIT;
	}
	if (!LookupPrivilegeValueW(0, lpszPrivilege, &luid))
	{
		Status = false;
		goto EXIT;
	}
	priv.PrivilegeCount = 1;
	priv.Privileges[0].Luid = luid;
	priv.Privileges[0].Attributes = bEnablePrivilege ? SE_PRIVILEGE_ENABLED : SE_PRIVILEGE_REMOVED;
	if (!AdjustTokenPrivileges(hToken, false, &priv, 0, 0, 0))
	{
		Status = false;
		goto EXIT;
	}
EXIT:
	if (hToken)
		CloseHandle(hToken);
	return Status;
}
int main(int argc, char* argv[]) {
	SetPrivilege(SE_DEBUG_NAME, true);
	LPCWSTR GameFile = L"TslGame.exe";
	HANDLE hGame = GetBastardHandle(GameFile, PARENT_LSASS, TRUE);
	cout << "Handle to target: 0x" << hex << hGame << endl;
	system("pause");
	return EXIT_SUCCESS;
}