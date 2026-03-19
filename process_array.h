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
    process_info *entries;
    size_t capacity;
    size_t count;
}process_arr;

process_arr init_array();
void add_array(process_arr *array_list,process_info *entries);

#endif


