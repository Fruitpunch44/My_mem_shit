#include<windows.h>
#include<stdlib.h>
#include<commctrl.h>
#include <tlhelp32.h>
#include"process_array.h"

#pragma comment(lib, "comctl32.lib")
#define IDR_MYMENU 101

HWND CREATE_LIST(HWND PARENT,process_arr *array);
#define ID_FILE_EXIT 60001
#define ID_HELP_ABOUT 60002

