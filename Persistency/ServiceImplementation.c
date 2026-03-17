#include <windows.h>

// Call the service entry-point

/*
    Entry point is responsible for:
- Initialize any necessary components that were deferred from the main entry point
- The registration of the service control handler (continue, stop, start, etc...)
- The dwControlsAccepted element of the SERVICE STATUS structure is utilized to register them as a bit mask 
- Set service status to SERVICE RUNNING

*/


// ------ Global variables ---------------------------------------------
SERVICE_STATUS      serviceStatus;
SERVICE_STATUS_HANDLE  hStatus;


// ------ Constants ---------------------------------------------
#define SLEEP_TIME 5000

// ----- Forward declarations (so ServiceMain can see them) -----------
void ControlHandler(DWORD request);
int RunMeow();





void ServiceMain(int argc, char ** argv){
    serviceStatus.dwServiceType         = SERVICE_WIN32;
    serviceStatus.dwCurrentState        = SERVICE_START_PENDING;
    serviceStatus.dwControlsAccepted    = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    serviceStatus.dwWin32ExitCode       = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint          = 0;
    serviceStatus.dwWaitHint            = 0;
    
    hStatus = RegisterServiceCtrlHandler("MeowService", (LPHANDLER_FUNCTION)ControlHandler);
    RunMeow();
    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &serviceStatus);
    while (serviceStatus.dwCurrentState == SERVICE_RUNNING){
        Sleep(SLEEP_TIME);
    }
    return;
}


void ControlHandler(DWORD request){
    switch (request){
        case SERVICE_CONTROL_STOP:
        serviceStatus.dwWin32ExitCode = 0;
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus (hStatus, &serviceStatus);
        return;

        case SERVICE_CONTROL_SHUTDOWN:
        serviceStatus.dwWin32ExitCode = 0;
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus (hStatus, &serviceStatus);
        return;

        default:
        break;
    }
    SetServiceStatus(hStatus, &serviceStatus);
    return;
}


// Function with the core logic
int RunMeow(){
    void * lb; // Unkown type like generics
    BOOL rv; 
    HANDLE th;
    char cmd[] = "C:\\Users\\Administrator\\Downloads\\meow.exe"; // .exe path
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    return 0;
}


int main(){
    SERVICE_TABLE_ENTRY ServiceTable[] = {
        {"MeowService", (LPSERVICE_MAIN_FUNCTION) ServiceMain},
        {NULL, NULL}
    };
    StartServiceCtrlDispatcher(ServiceTable);
    return 0;
}