#include "../List.h"
#include "My_Lib.h"

int main (void)
{
    Open_Log_File ("List");

    struct List *list = List_Constructor ();

    Insert_Beginning (list, 88);
    #if 1
    Insert_Beginning (list, 81);
    Insert_Beginning (list, 15);
    Insert_Beginning (list, 77);
    Insert_After (list, 2, 83);
    Insert_Ending (list, -13);
    Insert_Before (list, 4, 83);
    Insert_Ending (list, 38);
    Delete (list, 4);
    #endif

    if (Verificator (list) == ERROR)
    {
        List_Dump (list);
        return ERROR;
    }

    //List_Linearisation (list);

    List_Dump (list);

    List_Destructor (list);

    return 0;
}
