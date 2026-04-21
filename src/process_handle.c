#include "process_handle.h"
#include "process_array.h"

// void write_memory(HANDLE process, BYTE *address,BYTE value);
//add range scan
address_arr global_address_info;

//if void can handle various types i don't know;
/*
void *write_memomry(HANDLE proc,UINT value,unsigned long long addr){
    size_t number_of_bytes_to_read;
    HMODULE ntdll = GetModuleHandle("ntdll.dll");
    pZwriteVirtualMemory ZwReadVirtualMemory = (pZwriteVirtualMemory)GetProcAddress(ntdll,"ZwReadVirtualMemory");
    if(!ZwReadVirtualMemory){
        MessageBox(NULL,"unable to get address of ZwReadVirtualMemory","error",MB_OK);
        return 0;
    }

    NTSTATUS status = ZwReadVirtualMemory(proc,(LPCVOID)addr,)
}
    implement later
    */  
unsigned int read_memory(HANDLE proc,unsigned long long addr){
    //you technically can read out all
    SIZE_T number_of_bytes_read;

    //try to fix this 
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
    NTSTATUS status =ZwReadVirtualMemory(proc,(LPCVOID)addr,buff,sizeof(buff),&number_of_bytes_read);
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
    free(buff);
    return *buff;
}

void scan_memory(DWORD proc_id,DWORD target){
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

    while(NT_SUCCESS(status)){
        // memory filter 
        if(mbi.State == MEM_COMMIT && 
            ((mbi.Protect & PAGE_READONLY)|| (mbi.Protect & PAGE_READWRITE) ||(mbi.Protect & PAGE_EXECUTE_READ) 
            ||(mbi.Protect & PAGE_EXECUTE_READWRITE)) &&
            !(mbi.Protect & PAGE_NOACCESS) &&
            !(mbi.Protect & PAGE_GUARD)){
                unsigned long long start = (unsigned long long)(void*)mbi.BaseAddress;
                unsigned long long end = start + mbi.RegionSize;
                fprintf(stdout,"Memory region at 0x%p is committed and accessible.\n", (void*)mbi.BaseAddress);
                
                while(start < end){
                    unsigned int value = read_memory(proc, start);
                    if(value == target){
                        address_info *info = malloc(sizeof(address_info));
                        if(!info){
                            fprintf(stderr,"error in mem alloc");
                            CloseHandle(proc);
                            return;
                        }
                        info->addr = start;
                        info->value = value;
                        info->previous = value;
                        add_array_address_info(&global_address_info, info);
                        free(info);
                    }
                    start += sizeof(int); //increment by 4 bytes to read the next value
                }
            }
            base_addr += mbi.RegionSize;//move to the next region
            status = NtQueryVirtualMemory(proc, (LPVOID)base_addr, MemoryBasicInformation, &mbi, sizeof(mbi), NULL);
        }
        CloseHandle(proc);
}


void compare_changes(DWORD proc_id,address_arr *arr){
    HANDLE proc;
    proc= OpenProcess(PROCESS_QUERY_INFORMATION |PROCESS_VM_READ,FALSE,proc_id);

    if(proc == NULL){
        MessageBox(NULL,"ERROR IN GETING PROCESS INFO","ERROR",MB_ICONERROR);
        return;
    }

    for(int i = 0 ;i< arr->count;i++){
        unsigned int re_read_value = read_memory(proc,arr->info[i].addr);
        if(re_read_value != arr->info[i].value){
            arr->info[i].previous = arr->info[i].value;
            arr->info[i].value = re_read_value;
        }
    }
    CloseHandle(proc);
}
/*there is something to consider reagding this and how can i effectively filter out the
process after a 2nd scan i do not know how to go about that for now what happens is that you initially 
search for value and it pops out the various address associated with that value that's all fine and good 
but the issue now is how can i use next scan to now filter out the values so only that said 
values will be shown that is the main issue rn and i have to try to figure it out 
do i need to scan again maybe i will figure it out but that might cause issues*/
//scan from 0 to max
/*
LEAVE THIS OUT FOR NOW 
void get_process_id(DWORD proc_id,unsigned long long start){
    unsigned long long end =0xffffffffffffffff;
    scan_memory(proc_id,start,end,);
}
    */