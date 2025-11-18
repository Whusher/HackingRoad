/**
 * Author: Angel Anaya
 * Whusher PK
 * 
 * Simple DLL (Dynamic Linked Libraries)
 *  
 */

#include<windows.h>
#pragma comment (lib, "user32.lib")
// Read the notes in README.md to catch up what's going on here
BOOL APIENTRY DllMain( HMODULE moduleHandle, DWORD actionReason, LPVOID reservedPointer){
    switch(actionReason){
        case DLL_PROCESS_ATTACH:
            MessageBox(
                NULL,
                "Hello from my first evil.dll",
                "=^..^=", // This is a cat haha!
                MB_OK
            );
        break;
        case DLL_PROCESS_DETACH:
        break;
        case DLL_THREAD_ATTACH:
        break;
        case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}