#ifndef PROC_H
#define PROC_H

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<tlhelp32.h>

BOOL getproclist();
BOOL listprocmodules(DWORD dwpid);




#endif