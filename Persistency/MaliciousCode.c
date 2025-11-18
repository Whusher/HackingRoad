/**
Simple Message Box for windows using C

*/

#include <windows.h>

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow

){
    MessageBoxA(NULL,"Hello there!", "=^..^=", MB_OK);
    return 0;
}