#include "the_windows.h"
/*todo
add string search function
use wm_notify later
add better filtering
add write modifications
*/ 
global_window_states gwin;

LRESULT CALLBACK ProcessProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam){
    switch(msg){
        case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

        case WM_COMMAND:
            MessageBox(NULL, "Command received", "Info", MB_OK);//debuging
            //CONTROL CODE IS LOWRD NOTIFICATION CODE IS HIWORD
            if(LOWORD(wparam)==ID_SELECT && HIWORD(wparam)==BN_CLICKED){

                MessageBox(NULL, "Select button clicked", "Info", MB_OK);//debuging
                char pid_buff[50];
                char name_buff[120];
                int pos =ListView_GetNextItem(gwin.hList,-1,LVNI_SELECTED);
                if(pos == -1){
                    MessageBox(hwnd, "No process selected", "Error", MB_OK);
                    break;
                }
                int pid_column = 1;
                int name_colunm =0;

                ListView_GetItemText(gwin.hList,pos,pid_column,pid_buff,sizeof(pid_buff));
                gwin.pid = atoi(pid_buff);
                if(gwin.pid == 0){
                MessageBox(hwnd, "Invalid PID", "Error", MB_OK);
                break;
                }  

                ListView_GetItemText(gwin.hList,pos,name_colunm,name_buff,sizeof(name_buff));

                MessageBox(hwnd, name_buff, "Select process name", MB_OK);//debuging
                MessageBox(hwnd, pid_buff, "Selected PID", MB_OK);//debuging
                snprintf(gwin.info_buff,sizeof(gwin.info_buff),"%s - %d",name_buff,gwin.pid);

               
                char dbg[100];
                snprintf(dbg, sizeof(dbg), "Posting WM_REFRESH to: %p", GetParent(hwnd));//debugging
                MessageBox(NULL, dbg, "DEBUG POST", MB_OK);//debugging
                PostMessage(GetParent(hwnd),WM_REFRESH,0,0);
                DestroyWindow(hwnd);

            }
            break;
        default:
            return DefWindowProc(hwnd,msg,wparam,lparam);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam){
    switch(msg){
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_CREATE:
            //test for now find a way to get the adress from the list
            CREATE_LEFT_SIDE_Table(hwnd,&global_address_info);//
            CREATE_BOTTOM_LIST(hwnd);
            CREATE_SIDE_OPTIONS(hwnd);
            CREATE_GROUP_BOX(hwnd);
            CREATE_SCAN(hwnd);
            NEXT_SCAN(hwnd);
            show_selected_process(hwnd);
            HMENU hmenu,hsub,hsub2;
            hmenu=CreateMenu();


            //create pop up
            hsub=CreatePopupMenu();
            AppendMenu(hsub,MF_STRING,ID_OPEN_PROCESS,"Select process");
            AppendMenu(hsub,MF_STRING,ID_FILE_EXIT,"Exit");
            AppendMenu(hmenu, MF_STRING | MF_POPUP, (UINT_PTR)hsub, "File");

            hsub2=CreatePopupMenu();
            AppendMenu(hsub2,MF_STRING,ID_HELP_ABOUT,"Help");
            AppendMenu(hmenu,MF_STRING|MF_POPUP,(UINT_PTR)hsub2,"Go");

            SetMenu(hwnd,hmenu);
            break;
        case WM_COMMAND:
            switch(LOWORD(wparam)){
                //implement range selection scan
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
                case ID_SCAN_BUTTON:
                    if(HIWORD(wparam)==BN_CLICKED){
                        MessageBeep(MB_ICONINFORMATION);
                        get_target_value();
                        refresh_left_table(hwnd);
                    }
                    break;
                case ID_NEXT_SCAN_BUTTON:
                    if(HIWORD(wparam)==BN_CLICKED){
                        MessageBeep(MB_ICONINFORMATION);
                        char debug_buff[100];
                        snprintf(debug_buff,sizeof(debug_buff),"%d",global_address_info.count);
                        MessageBox(NULL,debug_buff,"debug",MB_OK);
                        compare_changes(gwin.pid,&global_address_info);
                        refresh_left_table(hwnd);
                    }
                    break;
            }
            break;
        case WM_REFRESH:
            //this is not being called after the post message for some reason
            //the issue was due to the process window being an overlappedwindow 
            SetWindowText(gwin.info, gwin.info_buff);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd,msg,wparam,lparam);
    }
    return 0;
}

void handle_address_range(){
    char start_address_buff[124];
    char end_address_buff[124];
    char pid_dgb[50];
    snprintf(pid_dgb,sizeof(pid_dgb),"PID: %d",gwin.pid);
    GetDlgItem(gwin.group_box,ID_GROUP_BOX);
    GetDlgItemText(gwin.group_box,ID_START_EDIT,start_address_buff,sizeof(start_address_buff));
    GetDlgItemText(gwin.group_box,ID_STOP_EDIT,end_address_buff,sizeof(end_address_buff));
    unsigned long long start_address =strtoull(start_address_buff,NULL,16);
    unsigned long long end_address =strtoull(end_address_buff,NULL,16);
    //scan_memory((DWORD)gwin.pid,start_address,end_address);

}
HWND CREATE_SCAN(HWND Parent){
    HWND scan_button;
    scan_button=CreateWindowEx(WS_EX_WINDOWEDGE,
    "BUTTON",
    "FIRST SCAN",
    BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD,
    520,30,100,20,
    Parent,(HMENU)ID_SCAN_BUTTON,
    GetModuleHandle(NULL),
    NULL);
    return scan_button;
    }
HWND NEXT_SCAN(HWND Parent){
    HWND next_scan_button;
    next_scan_button=CreateWindowEx(WS_EX_WINDOWEDGE,
    "BUTTON",
    "NEXT SCAN",
    BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD,
    640,30,100,20,
    Parent,(HMENU)ID_NEXT_SCAN_BUTTON,
    GetModuleHandle(NULL),
    NULL);

    return next_scan_button;
    }

HWND CREATE_GROUP_BOX(HWND Parent){
    int x_padding = 20;
    int y_padding = 40;
    int gap_y = 20;
    
    gwin.group_box = CreateWindowEx(0,
    "BUTTON",
    "Mem scan options",
    BS_GROUPBOX |WS_VISIBLE|WS_CHILD,
    520,130,270,210,
    Parent,
    NULL,
    GetModuleHandle(NULL),
    NULL);
    CreateWindowEx(
        0,
        "STATIC",          // this is the label
        "Start:",          // text to display
        WS_VISIBLE | WS_CHILD,
        x_padding,y_padding,100,25,
        gwin.group_box,
        (HMENU)ID_GROUP_BOX,
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
        gwin.group_box,
        (HMENU)ID_START_EDIT,
        GetModuleHandle(NULL),
        NULL);
        y_padding +=gap_y;


    CreateWindowEx(
        0,
        "STATIC",          //this is the label
        "Stop:",          //text to display
        WS_VISIBLE | WS_CHILD,
        x_padding,y_padding,100,25,
        gwin.group_box,
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
        gwin.group_box,
        (HMENU)ID_STOP_EDIT,
        GetModuleHandle(NULL),
        NULL);
        y_padding+=gap_y;

    CreateWindowEx(WS_EX_CLIENTEDGE,
    "BUTTON",
    "WRITEABLE",
    WS_CHILD|BS_CHECKBOX|WS_VISIBLE,
    x_padding,y_padding,100,20,
    gwin.group_box,
    (HMENU)ID_WRITEABLE_CHECK,
    GetModuleHandle(NULL),
    NULL
    );
    return gwin.group_box;
}
HWND CREATE_LEFT_SIDE_Table(HWND PARENT,address_arr *addr_arr){
    gwin.hlist_left_table = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            WC_LISTVIEW,
            "",
            WS_CHILD | WS_VISIBLE | LVS_REPORT,
            20,40,400,300,
            PARENT,
            NULL,
            GetModuleHandle(NULL),
            NULL
            );
            
    LVCOLUMN col;
    col.mask = LVCF_TEXT | LVCF_WIDTH;
    col.cx = 200;
    col.pszText = "Address";
    ListView_InsertColumn(gwin.hlist_left_table,0,&col);

    LVCOLUMN col2;
    col2.mask = LVCF_TEXT | LVCF_WIDTH; 
    col2.cx = 200;
    col2.pszText = "Value";
    ListView_InsertColumn(gwin.hlist_left_table,1,&col2);

    LVCOLUMN col3;
    col3.mask = LVCF_TEXT | LVCF_WIDTH; 
    col3.cx = 200;
    col3.pszText = "Previous";
    ListView_InsertColumn(gwin.hlist_left_table,2,&col3);

     for(size_t i =0 ; i<addr_arr->count;i++){
            LVITEM item = {0};
            item.mask = LVIF_TEXT;
            item.iItem = (int)i;
            item.iSubItem = 0;
            static char buff[43];
            sprintf(buff,"0x%llx",addr_arr->info[i].addr);
            item.pszText = buff;
            ListView_InsertItem(gwin.hlist_left_table,&item);

            /*char value[64];
            snprintf(value,sizeof(value),"%d",addr_arr->info[i].values);
            ListView_SetItemText(gwin.hlist_left_table,i,1,value);*/  
    }
    return gwin.hlist_left_table;
}
        
HWND CREATE_SIDE_OPTIONS(HWND Parent){
    int y_padding =65;
    int gap_y =25;
        CreateWindowEx(
        0,
        "STATIC",          //this is the label
        "Value:",          // text to display
        WS_VISIBLE | WS_CHILD,
        520,y_padding,220,25,
        Parent,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );
    y_padding+=gap_y;

    gwin.h_options =CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_BORDER|WS_CHILD|WS_VISIBLE,
        520,y_padding,300,20,
        Parent,
        (HMENU)ID_EDIT_VALUE,
        GetModuleHandle(NULL),
        NULL);
    return gwin.h_options;
}

void get_target_value(){
    char target_value[50];
    char dbg[120];
    char dbg_2[120];
    GetWindowText(gwin.h_options, target_value, sizeof(target_value));
    unsigned int value = atoi(target_value);
    snprintf(dbg,sizeof(dbg),"%d",value);
    MessageBox(NULL,dbg,"DEBUG",MB_OK);
    snprintf(dbg_2,sizeof(dbg_2),"%d",gwin.pid);
    MessageBox(NULL,dbg_2,"DEBUG",MB_OK);
    scan_memory(gwin.pid,value);
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
            20,390,800,200,
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
        //when posting to a parent window avoid using WS_OVERLAPPEDWINDOW
        HWND hlist_main;
        HWND button;
        //create main window for list
        hlist_main = CreateWindowEx(WS_EX_CLIENTEDGE,
            "ProcessListWindow",
            "test",
            WS_VISIBLE|WS_POPUPWINDOW|WS_CAPTION|WS_SYSMENU,
            20,20,500,600,
            PARENT,
            NULL,
            GetModuleHandle(NULL),
            NULL);
            
        //add button to select process
        button=CreateWindow(
        "BUTTON",
        "SELECT",
        WS_VISIBLE|WS_CHILD|WS_TABSTOP|BS_PUSHBUTTON|BS_CENTER,
        380,470,80,20,
        hlist_main,
        (HMENU)ID_SELECT,
        (HINSTANCE)GetWindowLongPtr(hlist_main,GWLP_HINSTANCE),
        NULL);
        
        
        // Create ListView
        gwin.hList = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            WC_LISTVIEW,
            "Processes",
            WS_VISIBLE |WS_CHILD| LVS_REPORT |LVS_AUTOARRANGE|LVS_SINGLESEL,
            20,20,350,500,
            hlist_main,
            NULL,
            GetModuleHandle(NULL),
            NULL
            );

            // Add column
            LVCOLUMN col;
            col.mask = LVCF_TEXT | LVCF_WIDTH;
            col.cx = 200;
            col.pszText = "Process Name";
            ListView_InsertColumn(gwin.hList,0,&col);

            LVCOLUMN col2;
            col2.mask = LVCF_TEXT | LVCF_WIDTH; 
            col2.cx = 200;
            col2.pszText = "PID";
            ListView_InsertColumn(gwin.hList,1,&col2);

            LVCOLUMN col3;
            col3.mask = LVCF_TEXT | LVCF_WIDTH; 
            col3.cx = 200;
            col3.pszText = "Thread";
            ListView_InsertColumn(gwin.hList,2,&col3);


            // Add row
            for(size_t i =0 ; i<array->count;i++){
            LVITEM item = {0};
            item.mask = LVIF_TEXT;
            item.iItem = (int)i;
            item.iSubItem = 0;
            item.pszText = array->entries[i].NAME;
            ListView_InsertItem(gwin.hList,&item);

            char pid[32];
            snprintf(pid,sizeof(pid),"%d",array->entries[i].PID);
            ListView_SetItemText(gwin.hList,i,1,pid);

            char thread[32];
            snprintf(thread,sizeof(thread),"%d",array->entries[i].Thread);
            ListView_SetItemText(gwin.hList,i,2,thread);
            }


    return gwin.hList;
}
HWND show_selected_process(HWND Parent){
        int x_padding = 20;
        int y_padding = 10;
        gwin.info= CreateWindowEx(WS_EX_CLIENTEDGE,
        "STATIC",
        gwin.info_buff,
        WS_CHILD|WS_VISIBLE,
        x_padding,y_padding,280,20,
        Parent,
        NULL,
        GetModuleHandle(NULL),
        NULL);
        return gwin.info;
}

void refresh_left_table(HWND hwnd){
        MessageBox(NULL, "WM_REFRESH received", "DEBUG", MB_OK);
        
            ListView_DeleteAllItems(gwin.hlist_left_table);
            if(global_address_info.count == 0 ){
                MessageBox(hwnd,"no readable memory found","info",MB_OK);
                return;
            }
            else if(gwin.pid ==0)
            {
                MessageBox(hwnd,"invalid pid for process","info",MB_OK);
                return;
            }
            else{
                 MessageBox(hwnd,"refreshing list","info",MB_OK);
            }
            for(size_t i =0 ; i<global_address_info.count;i++){
                LVITEM item;
                item.mask = LVIF_TEXT;
                item.iItem = i;
                item.iSubItem = 0;
                static char addr_buff[43];
                sprintf(addr_buff,"0x%llx",global_address_info.info[i].addr);
                item.pszText = addr_buff;
                ListView_InsertItem(gwin.hlist_left_table,&item);

                static char value[64];
                snprintf(value,sizeof(value),"%u",global_address_info.info[i].value);
                ListView_SetItemText(gwin.hlist_left_table,i,1,value);

                static char prev[64];
                snprintf(prev,sizeof(prev),"%u",global_address_info.info[i].previous);
                ListView_SetItemText(gwin.hlist_left_table,i,2,prev);
        }
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
    WNDCLASSEX wc = {0};
    WNDCLASSEX wc2 = {0};
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
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
    wc.lpszClassName = "myWindowClass";
    wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

    //process list window class
    wc2.cbSize=sizeof(WNDCLASSEX);
    wc2.lpfnWndProc =ProcessProc;
    wc2.hInstance=GetModuleHandle(NULL);
    wc2.lpszClassName="ProcessListWindow";
    wc2.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc2.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);

    if(!RegisterClassEx(&wc)){
        MessageBox(NULL,"Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }
    if(!RegisterClassEx(&wc2)){
        char err_mess[50];
        DWORD err= GetLastError();
        MessageBox(NULL,"Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
        snprintf(err_mess,sizeof(err_mess),"%d",err);
        MessageBox(NULL,err_mess,"error code",MB_ICONEXCLAMATION|MB_OK); 
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