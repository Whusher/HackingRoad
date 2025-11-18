#include <windows.h>
#include <stdio.h>

int main() {
    printf("Loading DLL...\n");
    
    // Load the DLL
    // HMODULE hDLL = LoadLibrary(TEXT("mydll.dll"));

    // I prefer to be explicit indicating the DLL location
    HMODULE hDLL = LoadLibrary(TEXT(".\\mydll.dll"));
    // As a comment you can use relative or full paths to specify the DLL
    // MUST EXISTS IN THE DIRECTORY OF THE EXECUTABLE .exe
    char currentDir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDir);
    printf("Current Working directory: %s\n", currentDir);


    if (hDLL == NULL) {
        printf("Failed to load DLL. Error: %lu\n", GetLastError());
        return 1;
    }
    
    printf("DLL loaded successfully!\n");
    printf("Press Enter to unload DLL...\n");
    getchar();
    
    // Unload the DLL
    if (FreeLibrary(hDLL)) {
        printf("DLL unloaded successfully!\n");
    } else {
        printf("Failed to unload DLL.\n");
    }
    
    return 0;
}