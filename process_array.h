#ifndef PROC_ARR_H
#define PROC_ARR_H


#include<stdio.h>
#include<stdlib.h>
#include<strings.h>
#include<windows.h>


typedef struct{
    CHAR *NAME;
    DWORD PID;
}process_info;


typedef struct{
    unsigned long long addr;
}address_info;

typedef struct{
    process_info *entries;
    size_t capacity;
    size_t count;
}process_arr;

typedef struct {
    address_info *info;
    size_t capacity;
    size_t count;
}address_arr;

extern address_arr global_address_info;
extern process_arr global_process;
process_arr init_array();
void add_array(process_arr *array_list,process_info *entries);
address_arr init_addr_array();
void add_array_address_info(address_arr *arr,address_info *info);
#endif


