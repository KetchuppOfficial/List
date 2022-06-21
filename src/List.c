#include "../List.h"
#include "My_Lib.h"

static int  List_Resize_Up (struct List *const list);
static void Print_Dump     (void);

const char *GRAPH_FILE = "./dump/dump.dot";

const int MIN_CAPACITY =  8;
const int MULTIPLIER   =  2;
const int FREE_PLACE   = -1;

struct List
{
    list_t *data;
    int    *next;
    int    *prev;

    int     head;
    int     tail;
    int     free;

    int     capacity;
    int     size;

    bool    linearized;
};

//RELATES TO CONSTRUCTION OF LIST
//******************************************************************************************************************
struct List *List_Constructor (void)
{
    struct List *list = (struct List *)calloc (1, sizeof (struct List));
    MY_ASSERT (list, "struct List *list", NE_MEM, NULL);

    list->data = (list_t *)calloc (MIN_CAPACITY, sizeof (list_t));
    MY_ASSERT (list->data, "list->data", NE_MEM, NULL);

    list->capacity = MIN_CAPACITY;
    list->size = 0;

    list->head = 0;
    list->tail = 0;
    list->free = 0;
    list->linearized = true;

    list->next = (int *)calloc (MIN_CAPACITY, sizeof (int));
    MY_ASSERT (list->next, "list->next", NE_MEM, NULL);

    for (int i = 1; i < MIN_CAPACITY - 1; i++)
        list->next[i] = i + 1;
    list->next[MIN_CAPACITY - 1] = 0;

    list->prev = (int *)calloc (MIN_CAPACITY, sizeof (int));
    MY_ASSERT (list->prev, "list->prev", NE_MEM, NULL);

    for (int i = 1; i < MIN_CAPACITY; i++)
        list->prev[i] = FREE_PLACE;

    return list;
}
//******************************************************************************************************************

//RELATES TO DESTRUCTION OF LIST
//******************************************************************************************************************
int List_Destructor (struct List *const list)
{
    MY_ASSERT (list,               "struct List *const list", NULL_PTR, ERROR);
    MY_ASSERT (list->capacity > 0, "list->capacity",          POS_VAL,  ERROR);

    list->capacity = 0;
    list->size     = 0;

    list->tail = 0;
    list->head = 0;
    list->free = 0;
    list->linearized = 0;

    free (list->data);
    free (list->next);
    free (list->prev);

    free (list);

    return NO_ERRORS;
}
//******************************************************************************************************************

//RELATES TO INSERGTING ELEMENTS INTO LIST
//******************************************************************************************************************
int Insert_After (struct List *const list, const int position, const list_t elem)
{
    MY_ASSERT (list,                               "struct List *const list", NULL_PTR,  ERROR);
    MY_ASSERT (position >= 0 &&
               position <= list->capacity,         "const int position",      UNEXP_VAL, ERROR);
    MY_ASSERT (list->prev[position] != FREE_PLACE, "list->prev[position]",    UNEXP_VAL, ERROR);

    if (position != list->tail)
        list->linearized = false;

    if (list->free == 0 && list->size > 0)
        List_Resize_Up (list);

    if (list->size == 0)
    {
        list->data[1] = elem;
        list->prev[1] = 0;
        list->next[1] = 0;
        list->free = 2;
        list->head = 1;
        list->tail = 1;
    }
    else
    {
        int new_elem_pos = list->free;
        list->free = list->next[new_elem_pos];

        int last_next = list->next[position];

        list->data[new_elem_pos] = elem;

        list->prev[new_elem_pos] = position;
        list->next[new_elem_pos] = last_next;

        list->next[position] = new_elem_pos;
        
        if (position == list->tail)
            list->tail = new_elem_pos;
        else
            list->prev[last_next] = new_elem_pos;
    }

    list->size++;

    return NO_ERRORS;
}

int Insert_Before (struct List *const list, const int position, const list_t elem)
{
    MY_ASSERT (list,                               "struct List *const list", NULL_PTR,  ERROR);
    MY_ASSERT (position >= 0 &&
               position <= list->capacity,         "const int position",      UNEXP_VAL, ERROR);
    MY_ASSERT (list->prev[position] != FREE_PLACE, "list->prev[position]",    UNEXP_VAL, ERROR);

    list->linearized = false;

    if (list->free == 0 && list->size > 0)
        List_Resize_Up (list);

    if (list->size == 0)
    {
        list->data[1] = elem;
        list->prev[1] = 0;
        list->next[1] = 0;
        list->free = 2;
        list->head = 1;
        list->tail = 1;
    }
    else
    {
        int new_elem_pos = list->free;
        list->free = list->next[new_elem_pos];

        int last_prev = list->prev[position];

        list->data[new_elem_pos] = elem;

        list->prev[new_elem_pos] = last_prev;
        list->next[new_elem_pos] = position;

        list->prev[position] = new_elem_pos;
        
        if (position == list->head)
            list->head = new_elem_pos;
        else
            list->next[last_prev] = new_elem_pos;
    }

    list->size++;

    return NO_ERRORS;
}

int Insert_Beginning (struct List *list, const list_t elem)
{
    MY_ASSERT (Insert_Before (list, list->head, elem) != ERROR, "Insert ()", FUNC_ERROR, ERROR);

    return NO_ERRORS;
}

int Insert_Ending (struct List *list, const list_t elem)
{
    MY_ASSERT (Insert_After (list, list->tail, elem) != ERROR, "Insert ()", FUNC_ERROR, ERROR);

    return NO_ERRORS;
}

#define Resize_Array_Up(ptr, type, value)                               \
do                                                                      \
{                                                                       \
    ptr = (type *)realloc (ptr, list->capacity * sizeof (type));        \
    MY_ASSERT (ptr, #ptr, NE_MEM, ERROR);                               \
                                                                        \
    for (int i = list->capacity / MULTIPLIER; i < list->capacity; i++)  \
        ptr[i] = value;                                                 \
}                                                                       \
while (0)

static int List_Resize_Up (struct List *const list)
{
    MY_ASSERT (list,               "struct List *const list", NULL_PTR, ERROR);
    MY_ASSERT (list->capacity > 0, "list->capacity",          POS_VAL,  ERROR);

    list->capacity *=  MULTIPLIER;

    Resize_Array_Up (list->data, list_t, 0);
    Resize_Array_Up (list->next, int, i + 1);

    list->next[list->capacity - 1] = 0;

    Resize_Array_Up (list->prev, int, FREE_PLACE);

    list->free = list->size + 1;

    return NO_ERRORS;
}
//******************************************************************************************************************

//SPECIFIC LIST FUNCTIONS
//******************************************************************************************************************
int List_Linearisation (struct List *const list)
{
    MY_ASSERT (list,               "struct List *const list", NULL_PTR, ERROR);
    MY_ASSERT (list->capacity > 0, "list->capacity",          POS_VAL , ERROR);

    if (list->size == 0)
        return NO_ERRORS;

    list_t *lin_data = (list_t *)calloc (list->capacity, sizeof (list_t));
    MY_ASSERT (lin_data, "list_t *lin_data", NE_MEM, ERROR);

    //Linearization of list.data
    int logic_i = list->head;
    int position = 0;
    for (position = 1; logic_i != list->tail; position++)
    {
        lin_data[position] = list->data[logic_i];
        logic_i = list->next[logic_i];
    }
    lin_data[position] = list->data[list->tail];

    free (list->data);
    list->data = lin_data;

    //Linearization of list.next
    list->next[0] = 0;
    for (int i = 1; i < list->size; i++)
        list->next[i] = i + 1;
    list->next[list->size] = 0;

    for (int i = list->size + 1; i < list->capacity; i++)
        list->next[i] = i + 1;

    //Linearization of list.prev
    list->prev[0] = list->prev[1] = 0;
    for (int i = 2; i < list->size + 1; i++)
        list->prev[i] = i - 1;

    for (int i = list->size + 1; i < list->capacity; i++)
        list->prev[i] = FREE_PLACE;

    list->head = 1;
    list->tail = list->size;
    list->linearized = true;

    return NO_ERRORS;
}

int Logical_Num_To_Physical (const struct List *list, const int logical_n)
{
    MY_ASSERT (list->capacity > 0,                        "list->capacity",  POS_VAL,   ERROR);
    MY_ASSERT (logical_n >= 0 && logical_n <= list->size, "const int logical_n", UNEXP_VAL, ERROR);

    int last_elem = 0;
    int logical_i = list->head;

    if (list->linearized == true)
        return logical_i;

    while (logical_i != logical_n)
    {
        last_elem = logical_i;
        logical_i = list->next[logical_i];
    }

    int physical_n = list->next[last_elem];

    return physical_n;
}

int Verificator (const struct List *list)
{
    MY_ASSERT (list, "const struct List *list", NULL_PTR, ERROR);
    
    MY_ASSERT (list->data,                  "list->data",       NULL_PTR,  ERROR);
    MY_ASSERT (list->next,                  "list->next",       NULL_PTR,  ERROR);
    MY_ASSERT (list->prev,                  "list->prev",       NULL_PTR,  ERROR);
    MY_ASSERT (list->capacity > 0,          "list->capacity",   POS_VAL,   ERROR);
    MY_ASSERT (list->size < list->capacity, "list->size",       UNEXP_VAL, ERROR);
    MY_ASSERT (list->head >= 0,             "list->head",       POS_VAL,   ERROR);
    MY_ASSERT (list->tail >= 0,             "list->head",       POS_VAL,   ERROR);
    MY_ASSERT (list->free > 0,              "list->head",       POS_VAL,   ERROR);
    MY_ASSERT (list->linearized == false ||
               list->linearized == true,    "list->linearized", UNEXP_VAL, ERROR);

    int i = 0;

    for (i = list->head; i != list->tail; i = list->next[i])
    {
        if (i == list->head)
            MY_ASSERT (list->prev[i] == 0, "list->head", UNEXP_VAL, ERROR);
        else if (i != list->next[list->prev[i]])
            MY_ASSERT (false, "struct List list", UNEXP_VAL, ERROR);

        if (i != list->prev[list->next[i]])
            MY_ASSERT (false, "struct List list", UNEXP_VAL, ERROR);
    }

    if (i == list->tail)
        MY_ASSERT (list->next[i] == 0, "list->tail", UNEXP_VAL, ERROR);

    for (i = list->free; i != list->capacity - 1; i = list->next[i])
    {
        if (list->prev[i] != FREE_PLACE && fabs (list->data[i]) > 1E-6)
            MY_ASSERT (false, "list->free", UNEXP_VAL, ERROR);
    }

    return NO_ERRORS;
}
//******************************************************************************************************************

//RELATES TO DELETING ELEMENTS FROM LIST
//******************************************************************************************************************
int Delete (struct List *list, const int position)
{
    MY_ASSERT (list,                               "struct List *const list", NULL_PTR,  ERROR);
    MY_ASSERT (list->capacity > 0,                 "list->capacity",          POS_VAL,   ERROR);
    MY_ASSERT (list->size > 0,                     "list->size",              POS_VAL,   ERROR);
    MY_ASSERT (position >= 0 &&
               position < list->capacity,          "const int after",         UNEXP_VAL, ERROR);
    MY_ASSERT (list->prev[position] != FREE_PLACE, "list->prev[position]",    UNEXP_VAL, ERROR);

    if (position != list->head && position != list->tail)
        list->linearized = false;


    int last_next = list->next[position];
    int last_prev = list->prev[position];

    list->data[position] = 0;
    
    list->next[position] = list->free;
    list->free = position;

    list->prev[position] = FREE_PLACE;

    if (position == list->tail)
        list->tail = list->prev[position];
    else
        list->prev[last_next] = last_prev;

    if (position == list->tail)
        list->head = list->next[position];
    else
        list->prev[last_next] = last_prev;

    list->size--;

    return NO_ERRORS;
}

int Delete_Beginning (struct List *list)
{
    MY_ASSERT (Delete (list, list->head) != ERROR, "Delete ()", FUNC_ERROR, ERROR);

    return NO_ERRORS;
}

int Delete_Ending (struct List *list)
{
    MY_ASSERT (Delete (list, list->tail) != ERROR, "Delete ()", FUNC_ERROR, ERROR);

    return NO_ERRORS;
}

//DUMP functions
//******************************************************************************************************************

#define Print_Node(colour)                                                                                              \
do                                                                                                                      \
{                                                                                                                       \
    fprintf (graph_file, "\tnode%d                                                                                      \
                          \t[shape = record, style = filled, fillcolor = "#colour",                                     \
                          \tlabel = \"<name%d> NODE %d | value: %g | <prev%d> prev: %d | <next%d> next: %d\"];\n\n",    \
                          i, i, i, list->data[i], i, list->prev[i], i, list->next[i]);                                  \
}                                                                                                                       \
while (0)   

int List_Dump (const struct List *list)
{
    system ("mkdir -p dump");

    FILE *graph_file = Open_File (GRAPH_FILE, "wb");
    MY_ASSERT (graph_file, "Open_File ()", EMPTY_FILE, ERROR);

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
                         list->head, list->tail, list->free, list->capacity, list->size, (list->linearized == true) ? "true" : "false");

    int i = 0;

    //Nodes
    Print_Node(yellow);
    for (i = 1; i < list->capacity; i++)
    {
        if (list->prev[i] == FREE_PLACE)
            Print_Node(gray);
        else
            Print_Node(green);
    }

    int last_free = 0;
    for (i = list->free; list->next[i] != 0; i = list->next[i])
    {
        if (last_free != 0)
            fprintf (graph_file, "\tnode%d -> node%d [color = \"gray\", constraint = false];\n", last_free, i);

        last_free = i;
    }

    //Supporting ages
    fprintf (graph_file, "{\n\tedge [color = white]\n");
    for (i = 0; i < list->capacity - 1; i++)
        fprintf (graph_file, "\tnode%d -> node%d\n", i, i + 1);
    fprintf (graph_file, "}\n");

    //Next arrows
    for (i = 0; i < list->capacity; i++)
    {
        if (list->next[i] > 0 && list->prev[i] != FREE_PLACE)
            fprintf (graph_file, "\tnode%d: <next%d> -> node%d: <name%d> [color = \"blue\", constraint = false];\n",
                     i, i, list->next[i], list->next[i]);
    }
    fprintf (graph_file, "\n");

    //Prev arrows
    for (i = 0; i < list->capacity; i++)
    {
        int node_i = list->capacity - i - 1;
        if (list->prev[node_i] > 0)
            fprintf (graph_file, "\tnode%d: <prev%d> -> node%d: <name%d> [color = \"red\", constraint = false];\n",
                     node_i, node_i, list->prev[node_i], list->prev[node_i]);
    }

    fprintf (graph_file, "}\n");

    Close_File (graph_file, GRAPH_FILE);

    Print_Dump ();

    return NO_ERRORS;
}

static void Print_Dump (void)
{
    char *print_dump = (char *)calloc (1024, sizeof (char));
    sprintf (print_dump, "dot -Tpng ./%s -o ./dump/dump.png", GRAPH_FILE);
    system (print_dump);
    free (print_dump);
}
//******************************************************************************************************************
