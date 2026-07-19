/*
Malware Development 

Evil application for windows persistence via 
hijacking uninstall app

*/

#include <windows.h>
#pragma comment (lib,"user32.lib")


int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow

){
    MessageBoxA(NULL,"Hello there!", "=^..^=", MB_OK);
    return 0;
}