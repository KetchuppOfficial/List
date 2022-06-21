#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef double list_t;

struct List;

struct List *List_Constructor (void);
int          List_Destructor  (struct List *const list);

int Insert_After     (struct List *const list, const int position, const list_t elem);
int Insert_Before    (struct List *const list, const int position, const list_t elem);
int Insert_Ending    (struct List *list, const list_t elem);
int Insert_Beginning (struct List *list, const list_t elem);

int Delete           (struct List *list, const int position);
int Delete_Beginning (struct List *list);
int Delete_Ending    (struct List *list);

int List_Linearisation      (struct List *const list);
int Logical_Num_To_Physical (const struct List *list, const int logical_n);
int List_Dump               (const struct List *list);
int Verificator             (const struct List *list);

#endif
