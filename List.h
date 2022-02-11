#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Inserting macros
//********************************************************************
#define Insert(list_ptr, position, elem, mode)              \
{                                                           \
    if (Insert_ (list_ptr, position, elem, mode) == ERROR)  \
        return ERROR;                                       \
}

#define Insert_Ending(list_ptr, elem)                                \
{                                                                    \
    if (Insert_ (list_ptr, (list_ptr)->tail, elem, AFTER) == ERROR)  \
        return ERROR;                                                \
}

#define Insert_Beginning(list_ptr, elem)                             \
{                                                                    \
    if (Insert_ (list_ptr, (list_ptr)->head, elem, BEFORE) == ERROR) \
        return ERROR;                                                \
}
//********************************************************************

//Deleting macros
//********************************************************************
#define Delete(list_ptr, position)              \
{                                               \
    if (Delete_ (list_ptr, position) == ERROR)  \
        return ERROR;                           \
}

#define Delete_Ending(list)                      \
{                                                \
    if (Delete_ (list, (list)->tail) == ERROR)   \
        return ERROR;                            \
}

#define Delete_Beginning(list)                   \
{                                                \
    if (Delete_ (list, (list)->head) == ERROR)   \
        return ERROR;                            \
}
//********************************************************************

#define Print_Node(colour)                                                                                              \
{                                                                                                                       \
    fprintf (graph_file, "\tnode%d                                                                                      \
                          \t[shape = record, style = filled, fillcolor = "#colour",                                     \
                          \tlabel = \"<name%d> NODE %d | value: %g | <prev%d> prev: %d | <next%d> next: %d\"];\n\n",    \
                          i, i, i, list.data[i], i, list.prev[i], i, list.next[i]);                                     \
}

#define CHOOSE_MODE                                         \
{                                                           \
    switch (mode)                                           \
    {                                                       \
        case AFTER:                                         \
            link = list_ptr->next;                          \
            reverse = list_ptr->prev;                       \
            end_mark = &(list_ptr->tail);                   \
            break;                                          \
        case BEFORE:                                        \
            link = list_ptr->prev;                          \
            reverse = list_ptr->next;                       \
            end_mark = &(list_ptr->head);                   \
            break;                                          \
        default:                                            \
            MY_ASSERT (false, "None", UNDEF_BEH, ERROR);    \
    }                                                       \
}

typedef double list_t;

struct List
{
    list_t *data;
    int    *next;
    int    *prev;
    int    head;
    int    tail;
    int    free;
    int    capacity;
    int    size;
    bool   linearized;
};

const int MIN_CAPACITY =  8;
const int MULTIPLIER   =  2;
const int FREE_PLACE   = -1;
const int AFTER        = -2;
const int BEFORE       = -3;

const int POISON = 2004;
const void *const DEAD_PTR = NULL;

int  List_Constructor  (struct List *const list_ptr);
int  List_Destructor   (struct List *const list_ptr);
void Destruct_Pointer  (void *ptr);

int Insert_        (struct List *const list_ptr, const int position, const list_t elem, const int mode);
int List_Resize_Up (struct List *const list_ptr);

int Delete_ (struct List *const list_ptr, const int position);
//int List_Resize_Down (struct List *const list_ptr);

int  List_Linearisation      (struct List *const list_ptr);
int  Logical_Num_To_Physical (const struct List list, const int logical_n);
int  Verificator             (const struct List list);
int  List_Dump               (const struct List list);
void Print_Dump              (void);

#endif // LIST_H_INCLUDED
