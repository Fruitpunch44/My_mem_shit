#ifndef PROC_H
#define PROC_H
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<winternl.h>
#define BUFF_SIZE 206

typedef enum _MEMORY_INFORMATION_CLASS
{
    MemoryBasicInformation
}MEMORY_INFORMATION_CLASS;

typedef NTSTATUS(NTAPI *pZwReadVirtualMemory)(
	  HANDLE  hProcess,
	  LPCVOID lpBaseAddress,
	  LPVOID  lpBuffer,
	  SIZE_T  nSize,
	  SIZE_T *lpNumberOfBytesRead
);

typedef NTSTATUS(NTAPI *pZwriteVirtualMemory)( 
    HANDLE hProcess,
    LPCVOID BaseAddress,
    LPVOID Buffer,
    SIZE_T NumberOfBytesToWrite,
    SIZE_T *NumberOfBytesWritten
);

typedef NTSTATUS(NTAPI *pNtQueryVirtualMemory)(
    HANDLE ProcessHandle,
    PVOID BaseAddress,
    MEMORY_INFORMATION_CLASS MemoryInformationClass,
    PVOID MemoryInformation,
    SIZE_T MemoryInformationLength,
    PSIZE_T ReturnLength
);

//void string_search(char *string_to_find);


void read_memory(HANDLE proc,unsigned long long addr);
void get_process_id(DWORD proc_id);



#endif

/*so this is going over working with undocumented api functions opposed to the normal api call 
do note that they do both the same thing differnce being that the nt version isn't a wraper and you have to declaring it */