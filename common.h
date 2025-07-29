#include <stdio.h>
#include <stdlib.h>

// ERROR ==========================================================================================
#define ERROR(msg) \
    printf(msg);   \
    exit(1);       

// ================================================================================================

// LIST ===========================================================================================
// must have allocated memory > 0 for the items or else many of these macros will segfault
// this happens because in some macros I dereference items in the list (usually to extract the type, so that the user doesn't constantly need to pass the type in functions 
// figured this is ok since it doesn't really make sense to have a list that can't contain anything

#define def_list(type)\
    typedef struct {  \
        type* items;  \
        int count;    \
        int cap;      \
    } type##_list

#define list(type) type##_list

#define lcreate(type, size)                            \
    {                                                  \
        .items = (type*) malloc(size * sizeof(type)),  \
        .count = 0,                                    \
        .cap = size                                    \
    } 

#define lexpand(list, size) \
    (list).cap = (size);                                                                 \
    (list).items = (typeof((list).items)) realloc((list).items, (size) * sizeof(typeof((list).items[0])))

#define ldestroy(list) free((list).items)

#define lpush(list, item)                       \
    do {                                        \
        if((list).count >= (list).cap){             \
            lexpand(list, (list).count * 1.5);    \
        }                                       \
        (list).items[(list).count++] = (item);      \
    } while(0) 

#define lget(list, index, var_name)  \
    do{ \
        if(index >= (list).count){ \
            ERROR("Out of range at index "); \
        } \
        else {  \
            var_name = (list).items[index]; \
        } \
    } while(0) \

#define lset(list, index, item) \
    do{ \
        if(index >= (list).count){ \
            ERROR("Out of range at index "); \
        } \
        else {  \
            (list).items[index] = item; \
        } \
    } while(0) \


#define ITEM_TO_STR_BUF_LEN 20
#define itostr(buf, i) snprintf(buf, sizeof(char) * ITEM_TO_STR_BUF_LEN, "%d", i)
#define ftostr(buf, f) snprintf(buf, sizeof(char) * ITEM_TO_STR_BUF_LEN, "%f", f)
#define ctostr(buf, c) snprintf(buf, sizeof(char) * ITEM_TO_STR_BUF_LEN, "%c", c)

#define lprint(list, item_to_str)                               \
    do {                                                        \
        printf("[ ");                                           \
        char buf[ITEM_TO_STR_BUF_LEN];                          \
        for(int i = 0; i < (list).count - 1; i++){                \
            item_to_str(buf, (list).items[i]);                    \
            printf("%s, ", buf);                                \
        }                                                       \
        item_to_str(buf, (list).items[list.count - 1]);           \
        printf("%s ]\n", buf);                                  \
    } while(0)


#define foreach(item, list, code)                           \
    do {                                                    \
        for(int _iter = 0; _iter < (list).count; _iter++){    \
            typeof(*(list.items)) item = (list).items[_iter]; \
            code                                            \
        }                                                   \
    } while(0)

// ==================================================================================================


// String ===========================================================================================
#define def_string def_list(char)
#define string list(char)
#define screate(size) lcreate(char, size)
#define sdestroy(str) ldestroy(str)

#define sappend(str, newstr) \
    do {                     \
        for(int i = 0; newstr[i] != '\0'; i++){ \
            lpush(str, newstr[i]); \
        }   \
    } while(0)

#define sprint(str)                         \
    do {                                    \
        if((str).count < (str).cap) {         \
            (str).items[(str).count] = '\0';  \
        }                                   \
        else{                               \
            lexpand(str, (str).count + 1);  \
            (str).items[(str).count] = '\0';            \
        }                                   \
        printf("%s\n", (str).items);         \
    } while(0)

// ==================================================================================================










