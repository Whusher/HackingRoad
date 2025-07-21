# Need Installation

When we are compiling a program for Win machine from linux we need to install the compiler that is not provided as default

- Run as administrator
-- sudo apt update
-- sudo apt install g++-mingw-w64-i686

Then you can perform compilation process e.g:

```	$ i686-w64-mingw32-g++ hack3.c -o hack3.exe -lws2_32 -s -
	ffunction-sections -fdata-sections -Wno-write-strings -fno-
	exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
	-fpermissive
```
Comments:
- Remember that 32bit is also compatible and available to run in Win systems based in x64 but also you can install a compiler for specific version just in case you needed.

```	sudo apt install g++-mingw-w64-x86-64
```
