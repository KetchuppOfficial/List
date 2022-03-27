#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

//Inserting macros
//********************************************************************
#define Insert(list_ptr, elem, position, mode)              \
do                                                          \
{                                                           \
    if (Insert_ (list_ptr, elem, position, mode) == ERROR)  \
        return ERROR;                                       \
}                                                           \
while (0)

#define Insert_Ending(list_ptr, elem)                                   \
do                                                                      \
{                                                                       \
    if (Insert_ (list_ptr, elem, (list_ptr)->tail, AFTER) == ERROR)     \
        return ERROR;                                                   \
}                                                                       \
while (0)

#define Insert_Beginning(list_ptr, elem)                                \
do                                                                      \
{                                                                       \
    if (Insert_ (list_ptr, elem, (list_ptr)->head, BEFORE) == ERROR)    \
        return ERROR;                                                   \
}                                                                       \
while (0)
//********************************************************************

//Deleting macros
//********************************************************************
#define Delete(list_ptr, position)              \
do                                              \
{                                               \
    if (Delete_ (list_ptr, position) == ERROR)  \
        return ERROR;                           \
}                                               \
while (0)

#define Delete_Ending(list)                         \
do                                                  \
{                                                   \
    if (Delete_ (list, (list)->tail) == ERROR)      \
        return ERROR;                               \
}                                                   \
while (0)

#define Delete_Beginning(list)                      \
do                                                  \
{                                                   \
    if (Delete_ (list, (list)->head) == ERROR)      \
        return ERROR;                               \
}                                                   \
while (0)
//********************************************************************

#define Print_Node(colour)                                                                                                  \
do                                                                                                                          \
{                                                                                                                           \
    fprintf (graph_file, "\tnode%zd                                                                                         \
                          \t[shape = record, style = filled, fillcolor = "#colour",                                         \
                          \tlabel = \"<name%zd> NODE %zd | value: %g | <prev%zd> prev: %zd | <next%zd> next: %zd\"];\n\n",  \
                          i, i, i, list.data[i], i, list.prev[i], i, list.next[i]);                                         \
}                                                                                                                           \
while (0)   

typedef double list_t;

struct List
{
    list_t  *data;
    ssize_t *next;
    ssize_t *prev;
    ssize_t head;
    ssize_t tail;
    ssize_t free;
    ssize_t  capacity;
    ssize_t  size;
    bool    linearized;
};

enum Location
{
    AFTER,
    BEFORE
};

int  List_Constructor  (struct List *const list_ptr);
int  List_Destructor   (struct List *const list_ptr);

int Insert_        (struct List *const list_ptr, const list_t elem, const ssize_t position, enum Location mode);
int List_Resize_Up (struct List *const list_ptr);

int Delete_ (struct List *const list_ptr, const int position);

int  List_Linearisation      (struct List *const list_ptr);
int  Logical_Num_To_Physical (const struct List list, const int logical_n);
int  Verificator             (const struct List list);
int  List_Dump               (const struct List list);
void Print_Dump              (void);

#endif // LIST_H_INCLUDED
