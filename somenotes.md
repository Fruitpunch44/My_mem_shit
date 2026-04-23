when u are working with threading her are some thing  u should consider espeicallly when working with multiple pareameters

DWORD WINAPI my_thread(LPVOID lparam ) is the deafult way of creating a thread object 
if u want to use multiple function pareameters create a struct to store them

typedef struct{
    parameters go here//
}my_thread;

then in your thread func create a pointer to the struct and cast it to lparam

my_thread *param = (my_threads*)lparam
func_u_want_threaded(param->x,param->y,and so on);
free(param);
return 0;

the in u wndproc
create a another struct pointer and malloc it based of the the size of the thread_struct params
my_thread *param = malloc(sizeof(my_thread));
pass the values to the params
call create thread
