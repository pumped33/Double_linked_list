#include <cstring>
#include <direct.h>
#include <cstdio>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG_MODE

#ifdef DEBUG_MODE
    #define ON_DBG(...) __VA_ARGS__
#else
    #define ON_DBG(...) 
#endif
 
#define GRAPHVIZ_MODE

#ifdef DEBUG_MODE
    #define ON_GRPV(...) __VA_ARGS__
#else
    #define ON_GRPV(...) 
#endif

#define BLUE   "\033[34m"
#define YELLOW "\033[33m"
#define PURPLE "\033[35m"
#define RESET  "\033[37m"

#define LIST_VERIFY                                                              \
do {                                                                              \
    if (list_verify(double_linked_list_info) != LIST_ERRORS_SUCCESS_EXIT) {        \
        double_linked_list_info -> str_code_failure = __LINE__;                     \
        double_linked_list_info -> func_failure = __func__;                          \
        list_dump_in_console(double_linked_list_info);                                \
                                                                                       \
        return LIST_ERRORS_ERROR;                                                       \
    }                                                                                    \
} while(0);     


#define CHECK_FREE_IN_NEXT                                                               \
    ON_DBG(if (double_linked_list_info -> free == -1) {                                   \
        double_linked_list_info -> list_error = LIST_ERRORS_TRY_INSERT_IN_FULL_LIST;       \
        })                                                                                  \

const int POISON                       = 67;
const int MAXLEN_EXTENSION             = 4; // like .png  .htm
const int MAXLEN_GRAPHVIZ_FOLDER_NAME  = 75;
const int MAXLEN_SYSTEM_STR            = MAXLEN_GRAPHVIZ_FOLDER_NAME +  50;
const int MAXLEN_STR_HTML_IMAGE_INSERT = 75;
const int FILE_PNG_GRAPHVIZ_NAME       = 75;
const int MAXLEN_HTML_DUMP_FILE_NAME   = 75;
const int MAXLEN_COLOR                 = 10;

enum list_errors_t {

    LIST_ERRORS_SUCCESS_EXIT               = 0, 
    LIST_ERRORS_NULL_PTR_ON_DATA           = 1, 
    LIST_ERRORS_NULL_PTR_ON_NEXT           = 2,
    LIST_ERRORS_NULL_PTR_ON_PREF           = 3,
    LIST_ERRORS_CAPACITY                   = 4,
    LIST_ERRORS_NULL_PTR_ON_LIST_STRUCT    = 5,
    LIST_ERRORS_FREE_OUT_OF_RANGE          = 6,
    LIST_ERRORS_FREE_VALUE_ZERO            = 7, 
    LIST_ERRORS_TRY_INSERT_IN_FULL_LIST    = 8,

    LIST_ERRORS_ERROR                      = -1,
};


struct double_linked_list_t {
    int* data    = NULL;
    int* next    = NULL;
    int* pref    = NULL;
    
    int free     = 1;
    int capacity = 0;

    int head = 0;
    int tail = 1;

    ON_DBG( list_errors_t list_error        = LIST_ERRORS_SUCCESS_EXIT; \
            int           str_code_failure  = 0;                         \
            const char*   func_failure      = "ALL GOOD BRO";             \
    ;)
};

list_errors_t   list_init                               (double_linked_list_t* double_linked_list_info, int capacity); 
void            list_destroy                            (double_linked_list_t* double_linked_list_info);
        
list_errors_t   list_insert                             (double_linked_list_t* double_linked_list_info, int index_to_insert, int element);
list_errors_t   list_insert_begin                       (double_linked_list_t* double_linked_list_info, int element);
list_errors_t   list_insert_end                         (double_linked_list_t* double_linked_list_info, int element);
int             change_links_insert_element             (double_linked_list_t* double_linked_list_info, int index_insert, int element);

list_errors_t   list_pop                                (double_linked_list_t* double_linked_list_info, int index_to_pop);
list_errors_t   list_pop_begin                          (double_linked_list_t* double_linked_list_info);
list_errors_t   list_pop_end                            (double_linked_list_t* double_linked_list_info); 
void            change_links_pop_element                (double_linked_list_t* double_linked_list_info, int index_pop);
    
void            print_list                              (double_linked_list_t* double_linked_list_info);

void            list_dump_in_console                    (double_linked_list_t* double_linked_list_info);
void            list_graph_dump                         (double_linked_list_t* double_linked_list_info, char* name_file_out, char* folder_name, int number_of_dumps);
void            list_dump_html                          (double_linked_list_t* double_linked_list_info);
list_errors_t   list_verify                             (double_linked_list_t* double_linked_list_info);
        
int             list_get_data                           (double_linked_list_t* double_linked_list_info, int index);
int             list_set_data                           (double_linked_list_t* double_linked_list_info, int index, int elem);
int             list_get_next                           (double_linked_list_t* double_linked_list_info, int index);
int             list_set_next                           (double_linked_list_t* double_linked_list_info, int index, int elem);
int             list_get_pref                           (double_linked_list_t* double_linked_list_info, int index);
int             list_set_pref                           (double_linked_list_t* double_linked_list_info, int index, int elem);


list_errors_t list_init(double_linked_list_t* double_linked_list_info, int capacity) {

    double_linked_list_info -> capacity = capacity;
    double_linked_list_info -> data = (int*)calloc(capacity + 1, sizeof(int));
    double_linked_list_info -> next = (int*)calloc(capacity + 1, sizeof(int));
    double_linked_list_info -> pref = (int*)calloc(capacity + 1, sizeof(int));

    ON_DBG(LIST_VERIFY)

    ON_DBG(for (int i = 0; i < capacity + 1; i++) {
        list_set_data(double_linked_list_info, i, POISON);
    })

    ON_DBG(for (int i = 1; i < capacity; i++) { 
       list_set_pref(double_linked_list_info, i, -1); 
    })

    for (int i = 1; i < capacity; i++) { 
       list_set_next(double_linked_list_info, i, i + 1);; 
    }
    list_set_next(double_linked_list_info, capacity, -1);
    
    ON_GRPV(list_dump_html(double_linked_list_info);) 
    ON_DBG(LIST_VERIFY)

    return LIST_ERRORS_SUCCESS_EXIT;
}

list_errors_t list_insert(double_linked_list_t* double_linked_list_info, int index_to_insert, int element) {

    CHECK_FREE_IN_NEXT
    ON_DBG(LIST_VERIFY)

    if      (index_to_insert == double_linked_list_info -> tail)      return list_insert_end(double_linked_list_info, element);
    else if (index_to_insert == double_linked_list_info -> head)      return list_insert_begin(double_linked_list_info, element);
    else                                                              change_links_insert_element(double_linked_list_info, index_to_insert, element); 

    ON_GRPV(list_dump_html(double_linked_list_info);) 
    ON_DBG(LIST_VERIFY)  //работа  с переменной ошибка 
    return LIST_ERRORS_SUCCESS_EXIT;
}

list_errors_t list_insert_end(double_linked_list_t* double_linked_list_info, int element) {

    CHECK_FREE_IN_NEXT
    ON_DBG(LIST_VERIFY)

    int current_free = change_links_insert_element(double_linked_list_info, double_linked_list_info -> tail, element);
    double_linked_list_info -> tail = current_free;

    ON_GRPV(list_dump_html(double_linked_list_info);) 
    ON_DBG(LIST_VERIFY)
    return LIST_ERRORS_SUCCESS_EXIT;
}

list_errors_t list_insert_begin(double_linked_list_t* double_linked_list_info, int element) {

    CHECK_FREE_IN_NEXT
    ON_DBG(LIST_VERIFY)

    int current_free = change_links_insert_element(double_linked_list_info, 0, element); // 0
    double_linked_list_info -> head = current_free;

    ON_GRPV(list_dump_html(double_linked_list_info);) 
    ON_DBG(LIST_VERIFY)  
    return LIST_ERRORS_SUCCESS_EXIT;
}

list_errors_t list_pop(double_linked_list_t* double_linked_list_info, int index_to_pop) {
    
    ON_DBG(LIST_VERIFY)

    change_links_pop_element(double_linked_list_info, index_to_pop);

    ON_GRPV(list_dump_html(double_linked_list_info);) 
    ON_DBG(LIST_VERIFY)
    return LIST_ERRORS_SUCCESS_EXIT;
}
 
list_errors_t list_pop_begin(double_linked_list_t* double_linked_list_info) {
    
    ON_DBG(LIST_VERIFY)

    change_links_pop_element(double_linked_list_info, double_linked_list_info -> head);
    double_linked_list_info -> head = list_get_next(double_linked_list_info, double_linked_list_info -> head);

    ON_GRPV(list_dump_html(double_linked_list_info);) 
    ON_DBG(LIST_VERIFY)
    return LIST_ERRORS_SUCCESS_EXIT;
}

list_errors_t list_pop_end(double_linked_list_t* double_linked_list_info) {
    
    ON_DBG(LIST_VERIFY)

    change_links_pop_element(double_linked_list_info, double_linked_list_info -> tail);
    double_linked_list_info -> tail = list_get_next(double_linked_list_info, double_linked_list_info -> tail);

    ON_GRPV(list_dump_html(double_linked_list_info);) 
    ON_DBG(LIST_VERIFY)
    return LIST_ERRORS_SUCCESS_EXIT;
}

int change_links_insert_element(double_linked_list_t* double_linked_list_info, int index_insert, int element) {

    int current_free = double_linked_list_info -> free;

    double_linked_list_info -> free = list_get_next(double_linked_list_info, double_linked_list_info -> free); 

    list_set_data(double_linked_list_info, current_free, element);

    int temp = list_get_next(double_linked_list_info, index_insert); 
    list_set_next(double_linked_list_info, index_insert, current_free);
    list_set_next(double_linked_list_info, current_free, temp);

    list_set_pref(double_linked_list_info, current_free, index_insert);
    list_set_pref(double_linked_list_info, double_linked_list_info -> next[current_free], current_free);

    return current_free;
}
 
void change_links_pop_element(double_linked_list_t* double_linked_list_info, int index_pop) {

    list_set_next(double_linked_list_info, list_get_pref(double_linked_list_info, index_pop), list_get_next(double_linked_list_info, index_pop));
    list_set_pref(double_linked_list_info, list_get_next(double_linked_list_info, index_pop), list_get_pref(double_linked_list_info, index_pop));
    list_set_next(double_linked_list_info, index_pop, double_linked_list_info -> free);
    double_linked_list_info -> free = index_pop;
}

int list_get_data(double_linked_list_t* double_linked_list_info, int index) {
    return  double_linked_list_info -> data[index];
}
int list_set_data(double_linked_list_t* double_linked_list_info, int index, int elem) {
    return  double_linked_list_info -> data[index] = elem;
}

int list_get_next(double_linked_list_t* double_linked_list_info, int index) {
    return double_linked_list_info -> next[index];
}
int list_set_next(double_linked_list_t* double_linked_list_info, int index, int elem) {
    return  double_linked_list_info -> next[index] = elem;
}

int list_get_pref(double_linked_list_t* double_linked_list_info, int index) {
    return  double_linked_list_info -> pref[index];
}
int list_set_pref(double_linked_list_t* double_linked_list_info, int index, int elem) {
    return  double_linked_list_info -> pref[index] = elem;
}

void list_destroy(double_linked_list_t* double_linked_list_info) {

    free(double_linked_list_info -> data);
    free(double_linked_list_info -> next);
    free(double_linked_list_info -> pref);

    double_linked_list_info -> data     = NULL;
    double_linked_list_info -> next     = NULL;
    double_linked_list_info -> pref     = NULL;
    
    double_linked_list_info -> free     = 1;
    double_linked_list_info -> capacity = 0;

    double_linked_list_info -> head     = 1;
    double_linked_list_info -> tail     = 1;

    ON_DBG(double_linked_list_info -> list_error        = LIST_ERRORS_SUCCESS_EXIT; 
           double_linked_list_info -> str_code_failure  = 0;                         
           double_linked_list_info -> func_failure      = "ALL GOOD BRO";             
    ;) 
}


void print_list(double_linked_list_t* double_linked_list_info) {

    int i = list_get_next(double_linked_list_info, 0);

    while (i != 0) {
        printf("%3d ", list_get_data(double_linked_list_info, i));
        i = list_get_next(double_linked_list_info, i);
    }
    printf("\n");
}

ON_DBG(
list_errors_t list_verify(double_linked_list_t* double_linked_list_info) {

    if (double_linked_list_info -> list_error != LIST_ERRORS_SUCCESS_EXIT) {
        return double_linked_list_info -> list_error;
    }

    if (double_linked_list_info == NULL)            return double_linked_list_info -> list_error =
                                                    LIST_ERRORS_NULL_PTR_ON_LIST_STRUCT;

    if (double_linked_list_info -> data == NULL)    return double_linked_list_info -> list_error = 
                                                    LIST_ERRORS_NULL_PTR_ON_DATA;

    if (double_linked_list_info -> next == NULL)    return double_linked_list_info -> list_error = 
                                                    LIST_ERRORS_NULL_PTR_ON_NEXT;

    if (double_linked_list_info -> pref == NULL)    return double_linked_list_info -> list_error = 
                                                    LIST_ERRORS_NULL_PTR_ON_PREF;

    if (double_linked_list_info -> capacity < 1)    return double_linked_list_info -> list_error = 
                                                    LIST_ERRORS_CAPACITY;

    if (double_linked_list_info -> free > 
        double_linked_list_info -> capacity ||
        double_linked_list_info -> free < -1)       return double_linked_list_info -> list_error = 
                                                    LIST_ERRORS_FREE_OUT_OF_RANGE;

    if (double_linked_list_info -> free == 0)       return double_linked_list_info -> list_error = 
                                                    LIST_ERRORS_FREE_VALUE_ZERO;

    return LIST_ERRORS_SUCCESS_EXIT;
})


void list_dump_html(double_linked_list_t* double_linked_list_info) {

    static int number_of_dumps = 1;

  
    
    static FILE* file = NULL;
    static char folder_name[MAXLEN_GRAPHVIZ_FOLDER_NAME] = "";

    if (number_of_dumps == 1) {

        time_t now = time(NULL);
        tm* t      = localtime(&now);
        strftime(folder_name, MAXLEN_GRAPHVIZ_FOLDER_NAME, "logs_images_%m-%d_%H-%M-%S", t);
        _mkdir(folder_name);

        char html_file_dump_name[MAXLEN_HTML_DUMP_FILE_NAME]  = "";
        snprintf(html_file_dump_name, MAXLEN_HTML_DUMP_FILE_NAME, "%s/general_dump.html", folder_name);
        file = fopen(html_file_dump_name, "w");
        fprintf(file,
        "<!DOCTYPE html>\n"
                "<html>\n\n"
                "\t<meta charset=\"UTF-8\">\n"
                "\t<title>List dump</title>\n"
                "</head>\n"
                "<body>\n"
                "<div style=\"display: flex; flex-direction: column; gap: 30px;\">\n");
        
    }

    // Generate PNG
    char name_graphviz_file[FILE_PNG_GRAPHVIZ_NAME];
    list_graph_dump(double_linked_list_info, name_graphviz_file, folder_name, number_of_dumps);


    // Two blocks: dump and image
fprintf(file,
    "<div style=\"display: flex; flex-direction: row; "
    "gap: 40px; align-items: flex-start; margin-bottom: 30px;\">\n"
);

    // TEXT DUMP
    fprintf(file, "<pre>\n");

    fprintf(file, "<span style=\"color: #0c32f0;\">"
                                 "ERROR NUMBER:  %d\n",
            double_linked_list_info -> list_error);

    fprintf(file, "FUNCTION NAME: %s\n",
            double_linked_list_info -> func_failure);

    fprintf(file, "LINE ERROR:    %d\n",
            double_linked_list_info -> str_code_failure);

    fprintf(file, "CAPACITY:      %d\n",
            double_linked_list_info -> capacity);

    fprintf(file, "\n");
    fprintf(file, "HEAD: %d\n", double_linked_list_info -> head);
    fprintf(file, "TAIL: %d\n", double_linked_list_info -> tail);
    fprintf(file, "FREE: %d\n", double_linked_list_info -> free);
    fprintf(file, "\n");
    fprintf(file, "</span>\n");

    // INDEX
    fprintf(file, "<span style=\"color: #f00caf;\">INDEX:\t</span>");

    for (int i = 0; i < double_linked_list_info -> capacity + 1; i++) {
        fprintf(file, "%4d ", i);
    }

    // DATA
    fprintf(file, "\n<span style=\"color: #f0e90c;\">DATA:\t</span>");

    for (int i = 0; i < double_linked_list_info -> capacity + 1; i++) {
        fprintf(file, "%4d ", list_get_data(double_linked_list_info, i));
    }

    // NEXT
    fprintf(file, "\n<span style=\"color: #f0e90c;\">NEXT:\t</span>");

    for (int i = 0; i < double_linked_list_info -> capacity + 1; i++) {
        fprintf(file, "%4d ", list_get_next(double_linked_list_info, i));
    }

    // PREF
    fprintf(file, "\n<span style=\"color: #f0e90c;\">PREF:\t</span>");

    for (int i = 0; i < double_linked_list_info -> capacity + 1; i++) {
        fprintf(file, "%4d ", list_get_pref(double_linked_list_info, i));
    }

    fprintf(file, "\n</pre>\n");

    // IMAGE
    char image_html_str[MAXLEN_STR_HTML_IMAGE_INSERT] = "";
    char* image_name = strrchr(name_graphviz_file, '/') + 1;
    snprintf(image_html_str, MAXLEN_STR_HTML_IMAGE_INSERT, "<img src=\"%s\" width=\"1250\">\n", image_name);
    fprintf(file, "%s", image_html_str);
    // HTML ending
    fprintf(file,
        "</div>\n"
    );
    fflush(file);
    

    number_of_dumps++; 
}



void list_dump_in_console(double_linked_list_t* double_linked_list_info) {

    printf(BLUE "ERROR NUMBER:  %d\n", double_linked_list_info -> list_error);
    printf(     "FUNCTION NAME: %s\n", double_linked_list_info -> func_failure);
    printf(     "LINE ERROR =   %d\n", double_linked_list_info -> str_code_failure);
    printf(     "CAPACITY =     %d\n", double_linked_list_info -> capacity); 

    printf("\n");
    printf(     "HEAD =         %d\n", double_linked_list_info -> head);
    printf(     "TAIL =         %d\n", double_linked_list_info -> tail);
    printf(     "FREE =         %d\n", double_linked_list_info -> free);
    printf("\n");
     
    
    printf(PURPLE "\nINDEX:\t");
    for (int i = 0; i < double_linked_list_info -> capacity + 1; i++) {
        printf("%4d ", i);
    }
    printf(YELLOW "\nDATA: \t" RESET);
    for (int i = 0; i < double_linked_list_info -> capacity + 1; i++) {
        printf("%4d ", list_get_data(double_linked_list_info, i));
    }

    printf(YELLOW "\nNEXT: \t" RESET);
    for (int i = 0; i < double_linked_list_info -> capacity + 1; i++) {
        printf("%4d ", list_get_next(double_linked_list_info, i));
    }
    
    printf(YELLOW "\nPREF: \t" RESET);
    for (int i = 0; i < double_linked_list_info -> capacity + 1; i++) {
        printf("%4d ", list_get_pref(double_linked_list_info, i));
    }
    printf("\n--------------------------------------------------------------\n" RESET);
    getchar();
}

void list_graph_dump(double_linked_list_t* double_linked_list_info, char* name_graphviz_file_out, char* folder_name, int number_of_dumps) { 

    snprintf(name_graphviz_file_out, FILE_PNG_GRAPHVIZ_NAME, "%s/log_%d.dot", folder_name, number_of_dumps);
    FILE* debug_out = fopen(name_graphviz_file_out, "w");

    //general settings
    fprintf(debug_out, "digraph List {\n"
                        "rankdir = TB;   \n"
                        "ranksep = 1   \n"
                        "nodesep = 0.7;    \n"
                        "splines = ortho;\n");
         
    //zero element
    fprintf(debug_out, "elem0 [shape=Mrecord, style=filled, fillcolor=\"#f06dfc\", label = \"{ {index = %d } | {data = %d} | {next = %d} | {prev = %d}}\"];\n", 
                                       0, list_get_data(double_linked_list_info, 0), list_get_next(double_linked_list_info, 0),list_get_pref(double_linked_list_info, 0)); 


    const char** colors = (const char**)calloc(double_linked_list_info -> capacity + 1, sizeof(char*));

    int next_cell = list_get_next(double_linked_list_info, 0);
    while (next_cell != 0) {
        colors[next_cell] = "#6db2fc";
        next_cell = list_get_next(double_linked_list_info, next_cell);
    }

    int free_cell = double_linked_list_info -> free;
    while (free_cell != -1) { 
        colors[free_cell] = "#6dfc9a";
        free_cell = list_get_next(double_linked_list_info, free_cell);
    }

    for(int j = 1; j < double_linked_list_info -> capacity + 1; j++) {
        fprintf(debug_out,  "elem%d [shape=Mrecord, style=filled, fillcolor=\"%s\", label = \"{ {index = %d } | {data = %d} | {next = %d} | {prev = %d}}\"];\n",
                j, colors[j], j, list_get_data(double_linked_list_info, j), list_get_next(double_linked_list_info, j), list_get_pref(double_linked_list_info, j));
    }

    //special blocks
    fprintf(debug_out, "HEAD [shape=rectangle, color=\"#ff6200\", style=filled, fillcolor=\"#ffd085\"];\n");
    fprintf(debug_out, "TAIL [shape=rectangle, color=\"#ff6200\", style=filled, fillcolor=\"#ffd085\"];\n");
    fprintf(debug_out, "FREE [shape=rectangle, color=\"#ff6200\", style=filled, fillcolor=\"#cbf708\"];\n");

    fprintf(debug_out, "{rank = same; HEAD; FREE; TAIL};\n");

    // one str elems
    fprintf(debug_out, "{rank = same;\n");
    for (int j = 0; j < double_linked_list_info -> capacity + 1; j++) {
        fprintf(debug_out, "\telem%d [group=\"g%d\"];\n", j, j);
    }

    //invisible chain of elements
    for (int j = 0; j < double_linked_list_info -> capacity; j++) {
        fprintf(debug_out, "\telem%d -> elem%d [style=invis, weight=100];\n", j, j + 1);
    }
    fprintf(debug_out, "};\n");

    fprintf(debug_out, "edge[dir=\"forward\", weight=0, arrowsize=0.5];\n");

    // connect special blocks to elems
    fprintf(debug_out, "HEAD -> elem%d\n",  double_linked_list_info -> head);
    fprintf(debug_out, "TAIL -> elem%d\n",  double_linked_list_info -> tail);
    fprintf(debug_out, "FREE -> elem%d\n",  double_linked_list_info -> free);


    next_cell = 0;
    do {
        fprintf(debug_out, "elem%d -> elem%d;\n", next_cell, list_get_next(double_linked_list_info, next_cell));
        next_cell = list_get_next(double_linked_list_info, next_cell);
    } while (next_cell != 0);
    
    fprintf(debug_out, "}\n");

    fclose(debug_out);
    free(colors);

    char system_str[MAXLEN_GRAPHVIZ_FOLDER_NAME + 50] = "";
    snprintf(system_str, MAXLEN_SYSTEM_STR,  "dot -Tpng \"%s\" -o \"%.*s.png\"", name_graphviz_file_out, 
                                                                                                  (int)strlen(name_graphviz_file_out) - MAXLEN_EXTENSION, name_graphviz_file_out);
    system(system_str);

    char* extension = strchr(name_graphviz_file_out, '.');
    strcpy(extension, ".png");
}

