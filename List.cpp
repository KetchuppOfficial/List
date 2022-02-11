#include "List.h"
#include "Reading_File.h"
#include "Log_File.h"

const char *GRAPH_FILE = "Graph.dot";

//RELATES TO CONSTRUCTION OF LIST
//******************************************************************************************************************
int List_Constructor (struct List *const list_ptr)
{
    MY_ASSERT (list_ptr, "struct List *list_ptr", NULL_PTR, ERROR);

    list_ptr->data = (list_t *)calloc (MIN_CAPACITY, sizeof (list_t));
    MY_ASSERT (list_ptr->data, "list_ptr->data", NE_MEM, ERROR);

    list_ptr->capacity = MIN_CAPACITY;
    list_ptr->size = 0;

    list_ptr->head = 0;
    list_ptr->tail = 0;
    list_ptr->free = 0;
    list_ptr->linearized = true;

    list_ptr->next = (int *)calloc (MIN_CAPACITY, sizeof (int));
    MY_ASSERT (list_ptr->next, "list_ptr->next", NE_MEM, ERROR);
    for (int i = 1; i < MIN_CAPACITY - 1; i++)
        list_ptr->next[i] = i + 1;
    list_ptr->next[MIN_CAPACITY - 1] = 0;

    list_ptr->prev = (int *)calloc (MIN_CAPACITY, sizeof (int));
    MY_ASSERT (list_ptr->prev, "list_ptr->prev", NE_MEM, ERROR);
    for (int i = 1; i < MIN_CAPACITY; i++)
        list_ptr->prev[i] = FREE_PLACE;

    return NO_ERRORS;
}
//******************************************************************************************************************

//RELATES TO DESTRUCTION OF LIST
//******************************************************************************************************************
int List_Destructor (struct List *const list_ptr)
{
    MY_ASSERT (list_ptr,               "struct List *const list_ptr", NULL_PTR,    ERROR);
    MY_ASSERT (list_ptr->capacity > 0, "list_ptr->capacity",          UNINIT_LIST, ERROR);

    Destruct_Pointer (list_ptr->data);
    Destruct_Pointer (list_ptr->next);
    Destruct_Pointer (list_ptr->prev);

    list_ptr->capacity = POISON;
    list_ptr->size     = POISON;

    list_ptr->tail = POISON;
    list_ptr->head = POISON;
    list_ptr->free = POISON;
    list_ptr->linearized = POISON;

    return NO_ERRORS;
}

void Destruct_Pointer (void *ptr)
{
    free (ptr);
    ptr = (void *)DEAD_PTR;
}
//******************************************************************************************************************

//RELATES TO INSERGTING ELEMENTS INTO LIST
//******************************************************************************************************************
int Insert_ (struct List *const list_ptr, const int position, const list_t elem, const int mode)
{
    MY_ASSERT (list_ptr,                               "struct List *const list_ptr", NULL_PTR,    ERROR);
    MY_ASSERT (position >= 0 &&
               position <= list_ptr->capacity,         "const int position",          INV_POS,     ERROR);
    MY_ASSERT (list_ptr->prev[position] != FREE_PLACE, "list_ptr->prev[position]",    FREE_INSERT, ERROR);

    if (position != list_ptr->tail && mode != AFTER)
        list_ptr->linearized = false;

    if (list_ptr->free == 0 && list_ptr->size > 0)
        List_Resize_Up (list_ptr);

    int *link = NULL, *reverse = NULL;
    int *end_mark = NULL;
    CHOOSE_MODE;

    if (list_ptr->size == 0)
    {
        list_ptr->data[1] = elem;
        list_ptr->free = list_ptr->next[1];
        list_ptr->prev[1] = list_ptr->next[1] = 0;
        list_ptr->head = list_ptr->tail = 1;
        list_ptr->size++;

        return NO_ERRORS;
    }

    int last_free = list_ptr->free;
    list_ptr->free = list_ptr->next[list_ptr->free];

    list_ptr->data[last_free] = elem;

    int neighbour = link[position];
    if (neighbour == 0)
    {
        neighbour = last_free;
        link[last_free] = 0;
        *end_mark = last_free;
    }
    else
        link[last_free] = neighbour;

    reverse[neighbour] = last_free;
    reverse[last_free] = position;

    link[position] = last_free;

    list_ptr->size++;

    return NO_ERRORS;
}

#define Resize_Array_Up(ptr, type, value)                                       \
{                                                                               \
    ptr = (type *)realloc (ptr, list_ptr->capacity * sizeof (type));            \
    MY_ASSERT (ptr, #ptr, NE_MEM, ERROR);                                       \
                                                                                \
    for (int i = list_ptr->capacity / MULTIPLIER; i < list_ptr->capacity; i++)  \
        ptr[i] = value;                                                         \
}
int List_Resize_Up (struct List *const list_ptr)
{
    MY_ASSERT (list_ptr,               "struct List *const list_ptr", NULL_PTR,    ERROR);
    MY_ASSERT (list_ptr->capacity > 0, "list_ptr->capacity",          UNINIT_LIST, ERROR);

    list_ptr->capacity *=  MULTIPLIER;

    Resize_Array_Up (list_ptr->data, list_t, 0);
    Resize_Array_Up (list_ptr->next, int, i + 1);
    list_ptr->next[list_ptr->capacity - 1] = 0;
    Resize_Array_Up (list_ptr->prev, int, FREE_PLACE);

    list_ptr->free = list_ptr->size + 1;

    return NO_ERRORS;
}
//******************************************************************************************************************

//SPECIFIC LIST FUNCTIONS
//******************************************************************************************************************
int List_Linearisation (struct List *const list_ptr)
{
    MY_ASSERT (list_ptr,               "struct List *const list_ptr", NULL_PTR,    ERROR);
    MY_ASSERT (list_ptr->capacity > 0, "list_ptr->capacity",          UNINIT_LIST, ERROR);

    if (list_ptr->size == 0)
        return NO_ERRORS;

    list_t *lin_data = (list_t *)calloc (list_ptr->capacity, sizeof (list_t));
    MY_ASSERT (lin_data, "list_t *lin_data", NE_MEM, ERROR);

    //Linearization of list.data
    int logic_i = list_ptr->head;
    int position = 0;
    for (position = 1; logic_i != list_ptr->tail; position++)
    {
        lin_data[position] = list_ptr->data[logic_i];
        logic_i = list_ptr->next[logic_i];
    }
    lin_data[position] = list_ptr->data[list_ptr->tail];

    free (list_ptr->data);
    list_ptr->data = lin_data;

    //Linearization of list.next
    list_ptr->next[0] = 0;
    for (int i = 1; i < list_ptr->size; i++)
        list_ptr->next[i] = i + 1;
    list_ptr->next[list_ptr->size] = 0;

    for (int i = list_ptr->size + 1; i < list_ptr->capacity; i++)
        list_ptr->next[i] = i + 1;

    //Linearization of list.prev
    list_ptr->prev[0] = list_ptr->prev[1] = 0;
    for (int i = 2; i < list_ptr->size + 1; i++)
        list_ptr->prev[i] = i - 1;

    for (int i = list_ptr->size + 1; i < list_ptr->capacity; i++)
        list_ptr->prev[i] = FREE_PLACE;

    list_ptr->head = 1;
    list_ptr->tail = list_ptr->size;
    list_ptr->linearized = true;

    return NO_ERRORS;
}

int Logical_Num_To_Physical (const struct List list, const int logical_n)
{
    MY_ASSERT (list.capacity > 0,                        "list_ptr->capacity",  UNINIT_LIST, ERROR);
    MY_ASSERT (logical_n >= 0 && logical_n <= list.size, "const int logical_n", NO_NUM,      ERROR);

    int last_elem = 0;
    int logical_i = list.head;

    if (list.linearized == true)
        return logical_i;

    while (logical_i != logical_n)
    {
        last_elem = logical_i;
        logical_i = list.next[logical_i];
    }

    int physical_n = list.next[last_elem];

    return physical_n;
}

int Verificator (const struct List list)
{
    MY_ASSERT (list.data,                 "list.data",       NULL_PTR,    ERROR);
    MY_ASSERT (list.next,                 "list.next",       NULL_PTR,    ERROR);
    MY_ASSERT (list.prev,                 "list.prev",       NULL_PTR,    ERROR);
    MY_ASSERT (list.capacity > 0,         "list.capacity",   UNINIT_LIST, ERROR);
    MY_ASSERT (list.size < list.capacity, "list.size",       LIST_OF,     ERROR);
    MY_ASSERT (list.head >= 0,            "list.head",       POS_VAL,     ERROR);
    MY_ASSERT (list.tail >= 0,            "list.head",       POS_VAL,     ERROR);
    MY_ASSERT (list.free > 0,             "list.head",       POS_VAL,     ERROR);
    MY_ASSERT (list.linearized == false ||
               list.linearized == true,   "list.linearized", UNEXP_VAL,   ERROR);

    int i = 0;
    for (i = list.head; i != list.tail; i = list.next[i])
    {
        if (i == list.head)
            {MY_ASSERT (list.prev[i] == 0, "list.head", PREV_HEAD, ERROR);}
        else if (i != list.next[list.prev[i]])
            {MY_ASSERT (false, "struct List list", LIST_DAM, ERROR);}

        if (i != list.prev[list.next[i]])
            MY_ASSERT (false, "struct List list", LIST_DAM, ERROR);
    }
    if (i == list.tail)
        MY_ASSERT (list.next[i] == 0, "list.tail", NEXT_TAIL, ERROR);

    for (i = list.free; i != list.capacity - 1; i = list.next[i])
    {
        if (list.prev[i] != FREE_PLACE && list.data[i] != 0)
            MY_ASSERT (false, "list.free", FREE_DAM, ERROR);
    }

    return NO_ERRORS;
}
//******************************************************************************************************************

//RELATES TO DELETING ELEMENTS FROM LIST
//******************************************************************************************************************
int Delete_ (struct List *const list_ptr, const int position)
{
    MY_ASSERT (list_ptr,                               "struct List *const list_ptr", NULL_PTR,    ERROR);
    MY_ASSERT (list_ptr->capacity > 0,                 "list_ptr->capacity",          UNINIT_LIST, ERROR);
    MY_ASSERT (list_ptr->size > 0,                     "list_ptr->size",              LIST_UF,     ERROR);
    MY_ASSERT (position >= 0 &&
               position < list_ptr->capacity,          "const int after",             INV_DEL,     ERROR);
    MY_ASSERT (list_ptr->prev[position] != FREE_PLACE, "list_ptr->prev[position]",    FREE_DEL,    ERROR);

    if (position != list_ptr->head && position != list_ptr->tail)
        list_ptr->linearized = false;

    list_ptr->data[position] = 0;

    int next = list_ptr->next[position];
    if (position == list_ptr->head)
        list_ptr->head = list_ptr->next[position];

    int prev = list_ptr->prev[position];
    if (position == list_ptr->tail)
        list_ptr->tail = list_ptr->prev[position];

    list_ptr->next[position] = list_ptr->free;
    list_ptr->free = position;

    if (prev != 0)
        list_ptr->next[prev] = next;

    if (next != 0)
        list_ptr->prev[next] = prev;

    list_ptr->prev[position] = FREE_PLACE;

    list_ptr->size--;

    return NO_ERRORS;
}

//DUMP functions
//******************************************************************************************************************
int List_Dump (const struct List list)
{
    FILE *graph_file = Open_File (GRAPH_FILE, "wb");

    fprintf (graph_file, "digraph List\n"
                         "{\n"
                         "\trankdir=LR;\n"
                         "\tnode [style=rounded];\n\n"
                         "\thead [shape = record, label = \"HEAD | %d\"];\n\n"
                         "\ttail [shape = record, label = \"TAIL | %d\"];\n\n"
                         "\tfree [shape = record, label = \"FREE | %d\"];\n\n"
                         "\tcapacity [shape = record, label = \"CAPACITY | %d\"];\n\n"
                         "\tsize [shape = record, label = \"SIZE | %d\"];\n\n"
                         "\tlinearized [shape = record, label = \"LINEARIZED | %s\"];\n\n"
                         "\tedge [color = white]\n"
                         "{\n"
                         "head -> tail\n"
                         "tail -> free\n"
                         "free -> capacity\n"
                         "capacity -> size\n"
                         "size -> linearized\n"
                         "}\n",
                         list.head, list.tail, list.free, list.capacity, list.size, (list.linearized == true) ? "true" : "false");

    int i = 0;

    //Nodes
    Print_Node(yellow);
    for (i = 1; i < list.capacity; i++)
    {
        if (list.prev[i] == FREE_PLACE)
            {Print_Node(gray);}
        else
            {Print_Node(green);}
    }

    int last_free = 0;
    for (i = list.free; i != list.capacity - 1; i = list.next[i])
    {
        if (last_free > 0)
            fprintf (graph_file, "\tnode%d -> node%d [color = \"gray\", constraint = false];\n", last_free, i);
        last_free = i;
    }
    fprintf (graph_file, "\tnode%d -> node%d [color = \"gray\", constraint = false];\n", last_free, i);

    //Supporting ages
    fprintf (graph_file, "{\n\tedge [color = white]\n");
    for (i = 0; i < list.capacity - 1; i++)
        fprintf (graph_file, "\tnode%d -> node%d\n", i, i + 1);
    fprintf (graph_file, "}\n");

    //Next arrows
    for (i = 0; i < list.capacity; i++)
    {
        if (list.next[i] > 0 && list.prev[i] != FREE_PLACE)
            fprintf (graph_file, "\tnode%d: <next%d> -> node%d: <name%d> [color = \"blue\", constraint = false];\n",
                     i, i, list.next[i], list.next[i]);
    }

    fprintf (graph_file, "\n");

    //Prev arrows
    for (i = 0; i < list.capacity; i++)
    {
        int node_i = list.capacity - i - 1;
        if (list.prev[node_i] > 0)
            fprintf (graph_file, "\tnode%d: <prev%d> -> node%d: <name%d> [color = \"red\", constraint = false];\n",
                     node_i, node_i, list.prev[node_i], list.prev[node_i]);
    }

    fprintf (graph_file, "}\n");

    Close_File (graph_file, GRAPH_FILE);

    Print_Dump ();

    return NO_ERRORS;
}

void Print_Dump (void)
{
    char *print_dump = (char *)calloc (1024, sizeof (char));
    sprintf (print_dump, "dot -Tpng ./%s -o Dump.png", GRAPH_FILE);
    system (print_dump);
    free (print_dump);
}
//******************************************************************************************************************
