The key difference is:

msfvenom creates position-independent shellcode (raw machine code that can run anywhere in memory)
Your HelloWorld.c compiles to a full executable with headers, dependencies, and fixed memory addresses

Here's how you could extract the machine code from your compiled C program:

- Step 1: Compile your C program
```
    gcc -o hello HelloWorld.c
```

- Step 2 Extract the machine code
```
# Extract just the .text section (the actual code)
objcopy -O binary --only-section=.text hello hello.bin

# Convert to C array format like msfvenom
xxd -i hello.bin

```

- Expected Output
```
unsigned char hello_bin[] = {
  0x55, 0x48, 0x89, 0xe5, 0x48, 0x83, 0xec, 0x10, ...
};
```