#include "process_handle.h"
#include "process_array.h"

// void write_memory(HANDLE process, BYTE *address,BYTE value);

address_arr global_address_info;
void read_memory(HANDLE proc,unsigned long long addr){
    //you technically can read out all
    SIZE_T number_of_bytes_read;
    BYTE *buff = malloc(sizeof(BYTE)* BUFF_SIZE);
    HMODULE ntdll = GetModuleHandle("ntdll.dll");
    pZwReadVirtualMemory ZwReadVirtualMemory = (pZwReadVirtualMemory)GetProcAddress(ntdll,"ZwReadVirtualMemory");
    if(!buff){
        fprintf(stderr,"error in allocaring memory for buffer\n");
        return;
    }
    NTSTATUS status =ZwReadVirtualMemory(proc,(LPCVOID)addr,buff,BUFF_SIZE,&number_of_bytes_read);
    if(NT_ERROR(status)){
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
    global_address_info = init_addr_array();
    HANDLE proc;
    MEMORY_BASIC_INFORMATION mbi ={0};
    unsigned long long base_addr = 0;
    proc= OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,FALSE,proc_id);

    //defining undocumented win calls
    HMODULE ntdll = GetModuleHandle("ntdll.dll");
    pNtQueryVirtualMemory NtQueryVirtualMemory = (pNtQueryVirtualMemory)GetProcAddress(ntdll,"NtQueryVirtualMemory");
    NTSTATUS status = NtQueryVirtualMemory(proc,(LPVOID)base_addr,MemoryBasicInformation,&mbi,sizeof(mbi),0);

    
    if(proc== NULL){
        fprintf(stderr,"unable to open process \n pls pass in a valid pid %s",strerror(GetLastError()));
        return;
    }
    //the whole idea of virtual query is to the get the memmory info of process and check it's various states
    //then u can now use read process memory to read the data of the process 
    //not that ntstatus only returns a code to signifiy it's success with the api call
    while(NT_SUCCESS(status)){
        address_info *info = malloc(sizeof(address_info));
        if(!info){
            fprintf(stderr,"error in mem alloc");
            return ;
        }

        if(mbi.State == MEM_COMMIT && !(mbi.Protect & PAGE_NOACCESS) &&
            !(mbi.Protect & PAGE_GUARD)){
                fprintf(stdout,"Memory region at 0x%p is committed and accessible.\n", (void*)mbi.BaseAddress);
                read_memory(proc,(unsigned long long)mbi.BaseAddress);
            }
        base_addr += mbi.RegionSize;
        status = NtQueryVirtualMemory(proc, (LPVOID)base_addr, MemoryBasicInformation, &mbi, sizeof(mbi), NULL);
        info->addr =base_addr;
        add_array_address_info(&global_address_info,info);
    }
    //add read memory function
    CloseHandle(proc);
}
