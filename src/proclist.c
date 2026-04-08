#include"proclist.h"
#include"process_array.h"


process_arr global_process;
BOOL getproclist(){
    global_process =init_array();
    HANDLE hprocsnap;
    HANDLE process;
    PROCESSENTRY32 entry32;
    DWORD priorityclass;

    //get snap
    hprocsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if(hprocsnap == INVALID_HANDLE_VALUE){
        fprintf(stderr,"an error %s occured\n",strerror(GetLastError()));
        return FALSE;
    }

    //SET THE SIZE OF THE PROC STRUCTURE
    entry32.dwSize = sizeof(PROCESSENTRY32);

    if(!Process32First(hprocsnap,&entry32)){
        fprintf(stderr,"an error %s occured\n",strerror(GetLastError()));
        CloseHandle(hprocsnap);
        return FALSE;
    }
    while(Process32Next(hprocsnap,&entry32)){
        fprintf(stdout,"PROCESS NAME : %s\n",entry32.szExeFile);
        priorityclass = 0;
        process= OpenProcess(PROCESS_ALL_ACCESS,FALSE,entry32.th32ProcessID);
        if(process == NULL){
            fprintf(stderr,"an error %s occured\n",strerror(GetLastError()));
        }
        priorityclass = GetPriorityClass(process);
        if(!priorityclass){
            fprintf(stderr,"an error %s occured\n",strerror(GetLastError()));
            CloseHandle(process);
        }
        process_info *info= malloc(sizeof(process_info));
        if(!info){
            fprintf(stderr,"error in mem alloc");
            return FALSE;
        }
        fprintf(stdout,"----PROCESS INFO----\n");
        fprintf(stdout,"process id      = %d\n",entry32.th32ProcessID);
        fprintf(stdout,"thread count    = %d\n",entry32.cntThreads);
        fprintf(stdout,"priority class  = %d\n",priorityclass);
        fprintf(stdout,"\n");
        info->NAME=_strdup(entry32.szExeFile);
        info->PID=entry32.th32ProcessID;
        info->Thread=entry32.cntThreads;
        add_array(&global_process,info);
        listprocmodules(entry32.th32ProcessID);
        free(info);

    }
    CloseHandle(hprocsnap);
    return TRUE;
}
BOOL listprocmodules(DWORD dwpid){
    HANDLE MODULE;
    MODULEENTRY32 me32;

    MODULE=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwpid);
      if(MODULE == INVALID_HANDLE_VALUE){
        fprintf(stderr,"an error %s occured\n",strerror(GetLastError()));
        return FALSE;
    }

    me32.dwSize = sizeof(MODULEENTRY32);
    if(!Module32First(MODULE,&me32)){
         fprintf(stderr,"an error %s occured",strerror(GetLastError()));
         CloseHandle(MODULE);
         return FALSE;
    }
    fprintf(stdout,"----MODULES----\n");
    while(Module32Next(MODULE,&me32)){
        fprintf(stdout,"MOUDLE NAME :%s\n",me32.szModule);
        fprintf(stdout,"Exe Path = %s\n",me32.szExePath);
        fprintf(stdout,"Base address = 0x%08x\n",me32.modBaseAddr);
        fprintf(stdout,"Base Size = %d\n",me32.modBaseSize);

        fprintf(stdout,"\n");
        
    }
    CloseHandle(MODULE);
    return TRUE;
}
