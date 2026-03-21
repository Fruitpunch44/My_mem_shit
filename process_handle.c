#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<strings.h>
#define BUFF_SIZE 206


void read_memory(HANDLE proc,unsigned long long addr){
    //you technically can read out all
    SIZE_T number_of_bytes_read;
    BYTE *buff = malloc(sizeof(BYTE)* BUFF_SIZE);
    if(!buff){
        fprintf(stderr,"error in allocaring memory for buffer\n");
        return;
    }
    if(!ReadProcessMemory(proc,(LPCVOID)addr,buff,BUFF_SIZE,&number_of_bytes_read)){
        fprintf(stderr,"error in reading from 0%p\n",(LPCVOID)addr);
        free(buff);
        return;

    }
     for(int i = 0; i < number_of_bytes_read; i++){
        printf("%02X ",buff[i]);
        if((i+1)%16==0)
            printf("\n");
}
}

void get_process_id(DWORD proc_id){
    HANDLE proc;
    MEMORY_BASIC_INFORMATION mbi ={0};
    unsigned long long base_addr = 0;
    proc= OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,FALSE,proc_id);
    if(proc== NULL){
        fprintf(stderr,"unable to open process \n pls pass in a valid pid %s",strerror(GetLastError()));
        return;
    }
    //the whole idea of virtual query is to the get the memmory info of process and check it's various states
    //then u can now use read process memory to read the data of the process 

    while(VirtualQueryEx(proc,(LPCVOID)base_addr,&mbi,sizeof(mbi))==sizeof(mbi)){
        if(mbi.State == MEM_COMMIT && !(mbi.Protect & PAGE_NOACCESS) &&
            !(mbi.Protect & PAGE_GUARD)){
                fprintf(stdout,"Memory region at 0x%p is committed and accessible.\n", (void*)mbi.BaseAddress);
            }
        read_memory(proc,(unsigned long long)mbi.BaseAddress);
        base_addr += mbi.RegionSize;

    }
    //add read memory function
    CloseHandle(proc);
}

int main(int argc,char *argv[]){
    for(int i = 0;i<argc;i++){
        fprintf(stdout,"arg :%d :%s\n",i,argv[i]);
    }
    if(argc<2){
        fprintf(stderr,"lol not enough args");
        exit(EXIT_FAILURE);
    }
    DWORD proc_id =(DWORD)atoi(argv[1]);
    get_process_id(proc_id);
    return 0;
}