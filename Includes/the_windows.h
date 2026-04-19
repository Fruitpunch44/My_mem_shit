#include<windows.h>
#include<stdlib.h>
#include<commctrl.h>
#include <tlhelp32.h>
#pragma comment(lib, "comctl32.lib")



#include"process_array.h"
#include"process_handle.h"
#include"proclist.h"

HWND CREATE_LIST(HWND PARENT,process_arr *array);
HWND CREATE_BOTTOM_LIST(HWND PARENT);
HWND CREATE_SIDE_OPTIONS(HWND Parent);
HWND CREATE_GROUP_BOX(HWND Parent);
HWND CREATE_LEFT_SIDE_Table(HWND Parent,address_arr *addr_arr);
HWND CREATE_SCAN(HWND Parent);
void handle_address_range();
void refresh_left_table(HWND Parent);
void get_target_value();
HWND show_selected_process(HWND Parent);
HWND NEXT_SCAN(HWND Parent);

typedef struct{
    HWND hList;//global handle for the process list to be able to
    HWND hlist_left_table;//global handle for list in main window
    HWND group_box;//global handle for group box
    int pid;//make pid global
    char info_buff[200];//
    HWND info;
    HWND h_options;
}global_window_states;

extern global_window_states gwin;


//objects within the window
#define IDR_MYMENU 101
#define WM_REFRESH (WM_USER+1) // handle this for refreshing the list after a scan
#define ID_FILE_EXIT 60001
#define ID_HELP_ABOUT 60002
#define ID_OPEN_PROCESS 60003
#define ID_SELECT 60004 //BUTTON
#define ID_EDIT_VALUE 60005 //EDIT FIELS    
#define ID_START_EDIT 60006 //START FIELD
#define ID_STOP_EDIT 60007 //STOP FIELD
#define ID_WRITEABLE_CHECK 60008 //CHECKBOX
#define ID_GROUP_BOX 60009 
#define ID_SCAN_BUTTON 60010
#define ID_NEXT_SCAN_BUTTON 60011

