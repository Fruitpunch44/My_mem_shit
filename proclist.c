#include"proclist.h"

BOOL getproclist(){
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

        fprintf(stdout,"process id      =0x %08x\n",entry32.th32ProcessID);
        fprintf(stdout,"thread count      = %d\n",entry32.cntThreads);
        listprocmodules(entry32.th32ProcessID);

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
    while(Module32Next(MODULE,&me32)){
        fprintf(stdout,"MOUDLE NAME :%s\n",me32.szModule);

    }
    CloseHandle(MODULE);
    return TRUE;
}

int main(int argc,  char **argv[]){
    getproclist();

    return 0;
}