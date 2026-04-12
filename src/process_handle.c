#include "process_handle.h"
#include "process_array.h"

// void write_memory(HANDLE process, BYTE *address,BYTE value);
//add range scan
address_arr global_address_info;
unsigned int read_memory(HANDLE proc,unsigned long long addr){
    //you technically can read out all
    SIZE_T number_of_bytes_read;

    unsigned int *buff = malloc(sizeof(int)* BUFF_SIZE);
    HMODULE ntdll = GetModuleHandle("ntdll.dll");
    pZwReadVirtualMemory ZwReadVirtualMemory = (pZwReadVirtualMemory)GetProcAddress(ntdll,"ZwReadVirtualMemory");
    if(!ZwReadVirtualMemory){
        MessageBox(NULL,"unable to get address of ZwReadVirtualMemory","error",MB_OK);
        free(buff);
        return 0;
    }

    if(!buff){
        MessageBox(NULL,"unable to allocate memory for buffer","error",MB_OK);  
        return 0;
    }
    NTSTATUS status =ZwReadVirtualMemory(proc,(LPCVOID)addr,buff,BUFF_SIZE,&number_of_bytes_read);
    if(NT_ERROR(status)){
        char err_mess[50];
        char err_code[20];
        DWORD err= GetLastError();
        snprintf(err_mess,sizeof(err_mess),"error in reading from 0%p", (LPCVOID)addr);
        snprintf(err_code,sizeof(err_code),"%d",err);
        MessageBox(NULL,err_code,"error code",MB_OK);
        MessageBox(NULL,err_mess,"error",MB_OK);
        free(buff);
        return 0;
    }
    return *buff;
}

void get_range_address(HANDLE proc,unsigned long long start_addr,unsigned long long end_address);
void get_process_id(DWORD proc_id){
    global_address_info = init_addr_array();
    HANDLE proc;
    MEMORY_BASIC_INFORMATION mbi ={0};
    unsigned long long base_addr = 0;
    proc= OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,FALSE,proc_id);

    if(proc== NULL){
        fprintf(stderr,"unable to open process \n pls pass in a valid pid %s",strerror(GetLastError()));
        return;
    }
    
    //defining undocumented win calls
    HMODULE ntdll = GetModuleHandle("ntdll.dll");
    pNtQueryVirtualMemory NtQueryVirtualMemory = (pNtQueryVirtualMemory)GetProcAddress(ntdll,"NtQueryVirtualMemory");
    if(!NtQueryVirtualMemory){
        MessageBox(NULL,"unable to get address of NtQueryVirtualMemory","error",MB_OK);
        CloseHandle(proc);
        return;
    }
    NTSTATUS status = NtQueryVirtualMemory(proc,(LPVOID)base_addr,MemoryBasicInformation,&mbi,sizeof(mbi),0);

    
    //the whole idea of virtual query is to the get the memmory info of process and check it's various states
    //then u can now use read process memory to read the data of the process 
    //note that ntstatus only returns a code to signifiy it's success with the api call
    while(NT_SUCCESS(status)){
        address_info *info = malloc(sizeof(address_info));
        if(!info){
            fprintf(stderr,"error in mem alloc");
            return ;
        }

        // meomry filter 
        if(mbi.State == MEM_COMMIT && 
            ((mbi.Protect & PAGE_READONLY)|| (mbi.Protect & PAGE_READWRITE))  &&
            !(mbi.Protect & PAGE_NOACCESS) &&
            !(mbi.Protect & PAGE_GUARD)){
                fprintf(stdout,"Memory region at 0x%p is committed and accessible.\n", (void*)mbi.BaseAddress);
                info->addr =(unsigned long long)mbi.BaseAddress;
                info->value = read_memory(proc, (unsigned long long)mbi.BaseAddress);
                info->previous = info->value;
                add_array_address_info(&global_address_info,info);
                free(info);
            }
            base_addr += mbi.RegionSize;
            status = NtQueryVirtualMemory(proc, (LPVOID)base_addr, MemoryBasicInformation, &mbi, sizeof(mbi), NULL);
            //note to self do not call outside of a loop to prevent crashes 
        }
    CloseHandle(proc);
}