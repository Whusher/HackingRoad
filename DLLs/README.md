# Complete Guide to Compiling C/C++ DLLs

## Table of Contents
1. [Introduction](#introduction)
2. [Prerequisites](#prerequisites)
3. [Basic DLL Structure](#basic-dll-structure)
4. [Compilation Methods](#compilation-methods)
5. [Export Functions](#export-functions)
6. [Advanced Examples](#advanced-examples)
7. [Troubleshooting](#troubleshooting)

---

## Introduction

Dynamic Link Libraries (DLLs) are shared libraries in Windows that contain code and data that can be used by multiple programs simultaneously. This guide covers cross-compilation from Linux and native Windows compilation.

---

## Prerequisites

### Linux (Cross-Compilation)
Install MinGW-w64:
```bash
# Debian/Ubuntu
sudo apt-get install mingw-w64

# Arch Linux
sudo pacman -S mingw-w64-gcc

# Fedora
sudo dnf install mingw64-gcc mingw32-gcc
```

### Windows (Native)
- Install MinGW-w64 or Visual Studio
- Install GCC via MSYS2 or use Microsoft's MSVC

---

## Basic DLL Structure

### DllMain Entry Point

Every DLL has a `DllMain` function that serves as the entry point:

```c
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE moduleHandle, DWORD actionReason, LPVOID reservedPointer) {
    switch(actionReason) {
        case DLL_PROCESS_ATTACH:
            // DLL is being loaded into a process
            MessageBox(NULL, "DLL Loaded!", "Info", MB_OK);
            break;
            
        case DLL_PROCESS_DETACH:
            // DLL is being unloaded from a process
            MessageBox(NULL, "DLL Unloaded!", "Info", MB_OK);
            break;
            
        case DLL_THREAD_ATTACH:
            // A new thread is being created
            break;
            
        case DLL_THREAD_DETACH:
            // A thread is exiting cleanly
            break;
    }
    return TRUE;
}
```

---

## Compilation Methods

### Method 1: Basic Compilation (Linux Cross-Compile)

```bash
# 64-bit DLL
x86_64-w64-mingw32-gcc -shared -o MyLibrary.dll MyLibrary.c

# 32-bit DLL
i686-w64-mingw32-gcc -shared -o MyLibrary.dll MyLibrary.c
```

### Method 2: C++ Compilation

```bash
# 64-bit C++ DLL
x86_64-w64-mingw32-g++ -shared -o MyLibrary.dll MyLibrary.cpp -fpermissive

# 32-bit C++ DLL
i686-w64-mingw32-g++ -shared -o MyLibrary.dll MyLibrary.cpp -fpermissive
```

### Method 3: With Additional Flags

```bash
x86_64-w64-mingw32-gcc -shared \
    -o MyLibrary.dll \
    MyLibrary.c \
    -static-libgcc \
    -static-libstdc++ \
    -Wl,--subsystem,windows \
    -luser32 -lkernel32
```

**Flag Explanations:**
- `-shared`: Create a shared library (DLL)
- `-static-libgcc`: Statically link GCC runtime
- `-static-libstdc++`: Statically link C++ standard library
- `-Wl,--subsystem,windows`: Link for Windows subsystem
- `-luser32`: Link User32.dll (for MessageBox, etc.)
- `-lkernel32`: Link Kernel32.dll (for core Windows APIs)
- `-fpermissive`: Allow some non-standard C++ code

### Method 4: Windows Native Compilation

```bash
# Using MinGW on Windows
gcc -shared -o MyLibrary.dll MyLibrary.c -Wl,--out-implib,libMyLibrary.a

# Using MSVC (Visual Studio)
cl /LD MyLibrary.c
```

---

## Export Functions

### Using __declspec(dllexport)

```c
#include <windows.h>

// Export function using __declspec
__declspec(dllexport) int Add(int a, int b) {
    return a + b;
}

__declspec(dllexport) void ShowMessage(const char* message) {
    MessageBox(NULL, message, "Exported Function", MB_OK);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    return TRUE;
}
```

**Compile:**
```bash
x86_64-w64-mingw32-gcc -shared -o MathLib.dll MathLib.c -luser32
```

### Using .def File (Definition File)

**MyLibrary.c:**
```c
#include <windows.h>

int Add(int a, int b) {
    return a + b;
}

int Subtract(int a, int b) {
    return a - b;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    return TRUE;
}
```

**MyLibrary.def:**
```
LIBRARY MyLibrary
EXPORTS
    Add
    Subtract
```

**Compile:**
```bash
x86_64-w64-mingw32-gcc -shared -o MyLibrary.dll MyLibrary.c MyLibrary.def
```

---

## Advanced Examples

### Example 1: Simple Calculator DLL

**Calculator.c:**
```c
#include <windows.h>

__declspec(dllexport) int Add(int a, int b) {
    return a + b;
}

__declspec(dllexport) int Multiply(int a, int b) {
    return a * b;
}

__declspec(dllexport) double Divide(double a, double b) {
    if (b == 0) return 0;
    return a / b;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    switch(reason) {
        case DLL_PROCESS_ATTACH:
            // Initialization code
            break;
        case DLL_PROCESS_DETACH:
            // Cleanup code
            break;
    }
    return TRUE;
}
```

**Compile:**
```bash
x86_64-w64-mingw32-gcc -shared -o Calculator.dll Calculator.c
```

### Example 2: Message Box DLL

**MessageDLL.c:**
```c
#include <windows.h>
#include <stdio.h>

__declspec(dllexport) void ShowInfo(const char* title, const char* message) {
    MessageBox(NULL, message, title, MB_OK | MB_ICONINFORMATION);
}

__declspec(dllexport) int ShowYesNo(const char* title, const char* question) {
    int result = MessageBox(NULL, question, title, MB_YESNO | MB_ICONQUESTION);
    return (result == IDYES) ? 1 : 0;
}

__declspec(dllexport) void ShowError(const char* errorMsg) {
    MessageBox(NULL, errorMsg, "Error", MB_OK | MB_ICONERROR);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    return TRUE;
}
```

**Compile:**
```bash
x86_64-w64-mingw32-gcc -shared -o MessageDLL.dll MessageDLL.c -luser32
```

### Example 3: File Operations DLL

**FileOps.c:**
```c
#include <windows.h>
#include <stdio.h>

__declspec(dllexport) int WriteToFile(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) return 0;
    
    fprintf(file, "%s", content);
    fclose(file);
    return 1;
}

__declspec(dllexport) int FileExists(const char* filename) {
    DWORD attrib = GetFileAttributes(filename);
    return (attrib != INVALID_FILE_ATTRIBUTES && 
            !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

__declspec(dllexport) long GetFileSize(const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 
                              NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return -1;
    
    DWORD fileSize = GetFileSize(hFile, NULL);
    CloseHandle(hFile);
    return fileSize;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
    return TRUE;
}
```

**Compile:**
```bash
x86_64-w64-mingw32-gcc -shared -o FileOps.dll FileOps.c -lkernel32
```

### Example 4: Stealth/Injection-Ready DLL

**StealthDLL.c:**
```c
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE moduleHandle, DWORD actionReason, LPVOID reservedPointer) {
    switch(actionReason) {
        case DLL_PROCESS_ATTACH:
            // Disable thread notifications for performance
            DisableThreadLibraryCalls(moduleHandle);
            
            // Your payload here
            MessageBox(NULL, "Hello =^..^=", "Injected!", MB_OK);
            
            // Optional: Create a new thread to avoid blocking
            // CreateThread(NULL, 0, PayloadThread, NULL, 0, NULL);
            break;
            
        case DLL_PROCESS_DETACH:
            // Cleanup
            break;
    }
    return TRUE;
}

// Optional: Exported function for manual invocation
__declspec(dllexport) void Execute() {
    MessageBox(NULL, "Manually executed!", "Info", MB_OK);
}
```

**Compile:**
```bash
x86_64-w64-mingw32-gcc -shared -o StealthDLL.dll StealthDLL.c -luser32
```

---

## Troubleshooting

### Common Errors and Solutions

**Error: "undefined reference to `__imp_MessageBoxA`"**
```bash
# Solution: Link user32 library
x86_64-w64-mingw32-gcc -shared -o MyDLL.dll MyDLL.c -luser32
```

**Error: "undefined reference to `WinMain`"**
```bash
# Solution: Use -shared flag
x86_64-w64-mingw32-gcc -shared -o MyDLL.dll MyDLL.c
```

**Error: Architecture mismatch (32-bit vs 64-bit)**
```bash
# Use correct compiler:
# For 32-bit: i686-w64-mingw32-gcc
# For 64-bit: x86_64-w64-mingw32-gcc
```

### Verification Commands

**Check DLL exports (Linux):**
```bash
x86_64-w64-mingw32-objdump -p MyLibrary.dll | grep "Export"
```

**Check DLL architecture:**
```bash
file MyLibrary.dll
```

**Check DLL dependencies:**
```bash
x86_64-w64-mingw32-objdump -p MyLibrary.dll | grep "DLL Name"
```

---

## Testing Your DLL

### Simple Test Program (TestDLL.c)

```c
#include <windows.h>
#include <stdio.h>

// Define function pointer types
typedef int (*AddFunc)(int, int);

int main() {
    // Load the DLL
    HMODULE hDll = LoadLibrary("Calculator.dll");
    if (hDll == NULL) {
        printf("Failed to load DLL\n");
        return 1;
    }
    
    // Get function address
    AddFunc Add = (AddFunc)GetProcAddress(hDll, "Add");
    if (Add == NULL) {
        printf("Failed to get function address\n");
        FreeLibrary(hDll);
        return 1;
    }
    
    // Call the function
    int result = Add(5, 3);
    printf("5 + 3 = %d\n", result);
    
    // Unload the DLL
    FreeLibrary(hDll);
    return 0;
}
```

**Compile test program:**
```bash
x86_64-w64-mingw32-gcc -o TestDLL.exe TestDLL.c
```

---

## Quick Reference

### Common Compiler Flags

| Flag | Description |
|------|-------------|
| `-shared` | Create shared library (DLL) |
| `-o <file>` | Specify output filename |
| `-luser32` | Link User32.dll |
| `-lkernel32` | Link Kernel32.dll |
| `-lgdi32` | Link GDI32.dll |
| `-lws2_32` | Link Winsock2 library |
| `-static-libgcc` | Statically link GCC runtime |
| `-fpermissive` | Allow non-standard C++ |
| `-O2` | Optimization level 2 |
| `-s` | Strip symbols (smaller file) |

### Template Makefile

```makefile
CC = x86_64-w64-mingw32-gcc
CFLAGS = -shared -O2
LDFLAGS = -luser32 -lkernel32

all: MyLibrary.dll

MyLibrary.dll: MyLibrary.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.dll

.PHONY: all clean
```

**Usage:**
```bash
make          # Compile
make clean    # Remove compiled files
```

---

## Additional Resources

- MinGW-w64 Documentation: https://www.mingw-w64.org/
- Windows API Reference: https://docs.microsoft.com/en-us/windows/win32/
- DLL Injection Techniques: Research on process injection methods
- PE Format: Understanding Portable Executable structure

---

*Happy DLL compiling! 🛠️*