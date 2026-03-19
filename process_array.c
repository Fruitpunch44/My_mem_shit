#include "process_array.h"



process_arr init_array(){
    process_arr arr_ptr;
    arr_ptr.capacity=5;
    arr_ptr.count=0;
    arr_ptr.entries=malloc(arr_ptr.capacity *sizeof(process_info));
    return arr_ptr;
}

void add_array(process_arr *arr,process_info *info){
    if(arr->count == arr->capacity){
        size_t new_cap = arr->capacity*=2;
        process_info *temp;
        temp=realloc(arr->entries,arr->capacity *sizeof(process_info));
        if(!temp){
            fprintf(stderr,"realloc failed");
            return;
        }
        arr->entries=temp;
        arr->capacity =new_cap;
    }
    arr->entries[arr->count++]=*info;
}