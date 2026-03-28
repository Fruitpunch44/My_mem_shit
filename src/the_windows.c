#include "the_windows.h"



/*todo
add string search function
create a singular group box for the options*/ 
HWND CREATE_GROUP_BOX(HWND Parent){
    HWND group_box;
    int x_padding = 20;
    int y_padding = 40;
    int gap_y = 20;
    group_box = CreateWindowEx(0,
    "BUTTON",
    "Mem scan options",
    BS_GROUPBOX |WS_VISIBLE | WS_CHILD,
    520,130,270,210,
    Parent,
    NULL,
    GetModuleHandle(NULL),
    NULL);
    CreateWindowEx(
        0,
        "STATIC",          // <-- this is the label
        "Start:",          // <-- text to display
        WS_VISIBLE | WS_CHILD,
        x_padding,y_padding,100,25,
        group_box,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );
    y_padding +=gap_y;
    CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_BORDER|WS_CHILD|WS_VISIBLE,
        x_padding,y_padding,100,20,
        group_box,
        NULL,
        GetModuleHandle(NULL),
        NULL);
        y_padding +=gap_y;


    CreateWindowEx(
        0,
        "STATIC",          // <-- this is the label
        "Stop:",          // <-- text to display
        WS_VISIBLE | WS_CHILD,
        x_padding,y_padding,100,25,
        group_box,
        NULL,
        GetModuleHandle(NULL),
        NULL);
        y_padding +=gap_y;

    CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_BORDER|WS_CHILD|WS_VISIBLE,
        x_padding,y_padding,100,20,
        group_box,
        NULL,
        GetModuleHandle(NULL),
        NULL);

    return group_box;
}
HWND CREATE_LEFT_SIDE_Table(HWND PARENT,address_arr *addr_arr){
    HWND hlist_left_table;
    hlist_left_table = CreateWindowEx(
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
    LVCOLUMN col;
    col.mask = LVCF_TEXT | LVCF_WIDTH;
    col.cx = 200;
    col.pszText = "Address";
    ListView_InsertColumn(hlist_left_table,0,&col);

    LVCOLUMN col2;
    col2.mask = LVCF_TEXT | LVCF_WIDTH; 
    col2.cx = 200;
    col2.pszText = "Value";
    ListView_InsertColumn(hlist_left_table,1,&col2);

    LVCOLUMN col3;
    col3.mask = LVCF_TEXT | LVCF_WIDTH; 
    col3.cx = 200;
    col3.pszText = "Previous";
    ListView_InsertColumn(hlist_left_table,2,&col3);

     for(size_t i =0 ; i<addr_arr->count;i++){
            LVITEM item;
            item.mask = LVIF_TEXT;
            item.iItem = (int)i;
            item.iSubItem = 0;
            char buff[43];
            sprintf(buff,"0x%llx",addr_arr->info[i].addr);
            item.pszText = buff;
            ListView_InsertItem(hlist_left_table,&item);

    }
}
        
HWND CREATE_SIDE_OPTIONS(HWND Parent){
    HWND h_options;
        CreateWindowEx(
        0,
        "STATIC",          // <-- this is the label
        "Value:",          // <-- text to display
        WS_VISIBLE | WS_CHILD,
        520,85,220,25,
        Parent,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    h_options =CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_BORDER|WS_CHILD|WS_VISIBLE,
        520,100,300,20,
        Parent,
        NULL,
        GetModuleHandle(NULL),
        NULL);
    return h_options;
}
//add pid serach
//add memory addition
HWND CREATE_BOTTOM_LIST(HWND PARENT){
    HWND hlist_bottom;
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
        // Create ListView
        hList = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            WC_LISTVIEW,
            "",
            WS_VISIBLE | LVS_REPORT |LVS_AUTOARRANGE,
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
            DWORD id= 21384;
            get_process_id(id);
            CREATE_LEFT_SIDE_Table(hwnd,&global_address_info);
            CREATE_BOTTOM_LIST(hwnd);
            CREATE_SIDE_OPTIONS(hwnd);
            CREATE_GROUP_BOX(hwnd);
            HMENU hmenu,hsub,hsub2;
            hmenu=CreateMenu();

            //create pop up
            hsub=CreatePopupMenu();
            AppendMenu(hsub,MF_STRING,ID_OPEN_PROCESS,"Select process");
            AppendMenu(hsub,MF_STRING,ID_FILE_EXIT,"exit");
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
                    MessageBox(NULL,"A work in progress memory hacker","LOL 2.O",MB_OK|MB_ICONINFORMATION);
                    break;
                case ID_OPEN_PROCESS:
                    getproclist();
                    CREATE_LIST(hwnd,&global_process);
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
    INITCOMMONCONTROLSEX icex = {0};
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

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

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"myWindowClass","Sucky scan",WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,860,680,NULL,NULL,hInstance,NULL);

    if(hwnd == NULL){
        MessageBox(NULL,"Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }
  
    ShowWindow(hwnd,nCmdShow);
    UpdateWindow(hwnd);
    while(GetMessage(&msg,NULL,0,0) > 0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}