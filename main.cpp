#include "List.h"
#include "Log_File.h"

int main (void)
{
    OPEN_LOG_FILE;

    struct List list = {};
    List_Constructor (&list);

    Insert_Beginning (&list, 88);
    Insert_Beginning (&list, 89);
    Insert_Beginning (&list, 81);
    Insert (&list, 2, 83, AFTER);
    Insert_Beginning (&list, 86);
    Insert (&list, 2, 74, BEFORE);

    if (Verificator (list) == ERROR)
    {
        List_Dump (list);
        return ERROR;
    }

    int n = 0;
    printf ("logical: %d, physical: %d\n", n, Logical_Num_To_Physical (list, n));

    //List_Linearisation (&list);

    List_Dump (list);

    List_Destructor (&list);

    return 0;
}
