# Understanding Registry KEYS

Windows systems by default generates the following run keys:

```
HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunOnce
HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\RunOnce

```

As you can see there are keys separated for USER and LOCAL_MACHINE with two methods *Run* and *RunOnce*



Compile the persistency script

```

x86_64-w64-mingw32-g++ -O2 PersistencyInjector.c -o persistency.exe -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc -fpermissive

```




- Cleanup command when Registry Key was added to Shell

```
reg add "HKEY_LOCAL_MACHINE\Software\Microsoft\Windows NT\CurrentVersion\Winlogon" /v "Shell" /t REG_SZ /d "explorer.exe"
```




## Windows services

Create a service
```
 sc create MeowService binpath= "Z:\PATH_TO_YOUR_EXE\meowsrv.exe" start= auto
```


Start a service
```
    sc start MeowService
```

Stop a service 
```
    sc stop MeowService
```


Delete a service (Even if you delete the service it could be still running if wasn't stopped)
```
    sc delete MeowService
```