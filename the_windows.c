#include "the_windows.h"
#include"process_array.h"
#include"proclist.h"

HWND CREATE_SIDE_OPTIONS(HWND Parent);
//add pid serach
//add memory addition
HWND CREATE_BOTTOM_LIST(HWND PARENT){
    HWND hlist_bottom;
    INITCOMMONCONTROLSEX icex ={0};
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    hlist_bottom = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            WC_LISTVIEW,
            "",
            WS_CHILD | WS_VISIBLE | LVS_REPORT,
            20,350,800,200,
            PARENT,
            NULL,
            GetModuleHandle(NULL),
            NULL
            );
      // Add column
    LVCOLUMN col;
    col.mask = LVCF_TEXT | LVCF_WIDTH;
    col.cx = 200;
    col.pszText = "ACTIVE";
    ListView_InsertColumn(hlist_bottom,0,&col);

    LVCOLUMN col2;
    col2.mask = LVCF_TEXT | LVCF_WIDTH; 
    col2.cx = 200;
    col2.pszText = "DESCRIPTION";
    ListView_InsertColumn(hlist_bottom,1,&col2);

    LVCOLUMN col3;
    col3.mask = LVCF_TEXT | LVCF_WIDTH; 
    col3.cx = 200;
    col3.pszText = "TYPE";
    ListView_InsertColumn(hlist_bottom,2,&col3);

    LVCOLUMN col4;
    col4.mask = LVCF_TEXT | LVCF_WIDTH; 
    col4.cx = 200;
    col4.pszText = "VALUE";
    ListView_InsertColumn(hlist_bottom,3,&col4);

    return hlist_bottom;

}
HWND CREATE_LIST(HWND PARENT,process_arr *array){

        HWND hList;
        INITCOMMONCONTROLSEX icex = {0};
        icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icex.dwICC = ICC_LISTVIEW_CLASSES;
        InitCommonControlsEx(&icex);
        // Create ListView
        hList = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            WC_LISTVIEW,
            "",
            WS_CHILD | WS_VISIBLE | LVS_REPORT,
            20,20,400,300,
            PARENT,
            NULL,
            GetModuleHandle(NULL),
            NULL
            );

            // Add column
            LVCOLUMN col;
            col.mask = LVCF_TEXT | LVCF_WIDTH;
            col.cx = 200;
            col.pszText = "Process Name";
            ListView_InsertColumn(hList,0,&col);

            LVCOLUMN col2;
            col2.mask = LVCF_TEXT | LVCF_WIDTH; 
            col2.cx = 200;
            col2.pszText = "PID";
            ListView_InsertColumn(hList,1,&col2);

            LVCOLUMN col3;
            col3.mask = LVCF_TEXT | LVCF_WIDTH; 
            col3.cx = 200;
            col3.pszText = "Thread";
            ListView_InsertColumn(hList,2,&col3);


            // Add row
            for(size_t i =0 ; i<array->count;i++){
            LVITEM item;
            item.mask = LVIF_TEXT;
            item.iItem = (int)i;
            item.iSubItem = 0;
            item.pszText = array->entries[i].NAME;
            ListView_InsertItem(hList,&item);

            char pid[32];
            snprintf(pid,sizeof(pid),"%d",array->entries[i].PID);
            ListView_SetItemText(hList,i,1,pid);
            }

    return hList;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam){
    switch(msg){
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_CREATE:
            HMENU hmenu,hsub,hsub2;
            hmenu=CreateMenu();

            //create pop up
            hsub=CreatePopupMenu();
            AppendMenu(hsub,MF_STRING,ID_FILE_EXIT,"Exit");
            AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT_PTR)hsub, "File");

            hsub2=CreatePopupMenu();
            AppendMenu(hsub2,MF_STRING,ID_HELP_ABOUT,"Help");
            AppendMenu(hmenu,MF_STRING|MF_POPUP,(UINT_PTR)hsub2,"Go");

            SetMenu(hwnd,hmenu);
            break;
        case WM_COMMAND:
            switch(LOWORD(wparam)){
                case ID_FILE_EXIT:
                    PostMessage(hwnd,WM_CLOSE,0,0);
                    break;
                case ID_HELP_ABOUT:
                    MessageBox(NULL,"LOL","LOL 2.O",MB_OK|MB_ICONINFORMATION);
                    break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_LBUTTONDOWN:
            MessageBox(hwnd,"You clicked the left mouse button!","Mouse Click",MB_OK);
            break;
        default:
            return DefWindowProc(hwnd,msg,wparam,lparam);
    }
    return 0;
}



int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
    WNDCLASSEX wc = {0};
    HWND hwnd;
    MSG msg;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
    wc.lpszClassName = "myWindowClass";
    wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

    if(!RegisterClassEx(&wc)){
        MessageBox(NULL,"Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"myWindowClass","The title of my window",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,860,680,NULL,NULL,hInstance,NULL);

    if(hwnd == NULL){
        MessageBox(NULL,"Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    ShowWindow(hwnd,nCmdShow);
    UpdateWindow(hwnd);

    getproclist();
    CREATE_LIST(hwnd,&global_process);
    CREATE_BOTTOM_LIST(hwnd);
    while(GetMessage(&msg,NULL,0,0) > 0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}