#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED
#include <stdbool.h>

enum status {
    OK,
    OUT_OF_MEMORY
};

struct node {
    struct node* next;
    void* value;
};

struct linked_list {
    struct node* first;
    struct node* last;
    unsigned int length;
};

typedef enum status status;

typedef unsigned int linked_list_size_t;

typedef void* generic_t;

typedef struct node* list_node;

typedef struct linked_list* linked_list;

/*
    Initializes a list.
    Each list should be initialized right after declaration. Every function here
    expect an initialized list.
    Returns OUT_OF_MEMORY if could not allocate memory; OK otherwise.
*/
void initialize(struct linked_list**);

/*
    Allocates memory for a node.
    This function should only be used by the library.
    Returns OUT_OF_MEMORY if could not allocate memory; OK otherwise.
*/
enum status create(void*, struct node**);

/*
    Inserts given element in an empty list.
    This function should only be used by the library.
    Returns OUT_OF_MEMORY if could not allocate memory; OK otherwise.
*/
enum status insert_first(void*, struct linked_list*);

/*
    Puts given element in the back of the list.
    Returns OUT_OF_MEMORY if could not allocate memory; OK otherwise.
*/
enum status append(void*, struct linked_list*);

/*
    Puts given element in the front of the list.
    Returns OUT_OF_MEMORY if could not allocate memory; OK otherwise.
*/
enum status prepend(void*, struct linked_list*);

/*
    Returns the element located at the given position.
    Index starts in 1.
*/
void* nth(unsigned int, struct linked_list*);

/*
    Returns the number of elements currently in the list.
*/
unsigned int count(struct linked_list*);

/*
    Removes an element at given position.
    Index starts in 1.
*/
void delete_at(unsigned int, struct linked_list*);

/*
    Calls the given function for each element in the list, passing the element's
    index and element itself.
    The given function should return a bool indicating if the iteration should
    continue. It is guarantee the iteration stops after the last element.
    It is ok modify elements content, but the list should not be modified (i.e.:
    append or delete elements).
    This function can be used for mapping (see https://en.wikipedia.org/wiki/Map_(higher-order_function))
*/
void iterate(bool (*f)(unsigned int, void*), struct linked_list*);

/*

*/
void reduce(void (*f)(void*, void*), void*r, struct linked_list*);

/*
    Returns the element's index for the first time the given function returns
    true.
    The given function should receive the element's index and the element
    itself, and return a bool indicating if it is the desired element.
    If the given function never return true, 0 is returned.
*/
unsigned int find(bool (*f)(unsigned int, void*), struct linked_list*);

/*
    Splits a list into two lists.
    The first parameter indicates the size of the first new list.
*/
void split(unsigned int, struct linked_list*, struct linked_list*, struct linked_list*);

/*
    Removes all elements for which the given function returns false.
    The given function receives an element and returns a bool indicating if the
    element should stay in the list.
*/
void filter(bool (*f)(void*), struct linked_list*);

/*
    Returns a new list with the content of the two given lists concatenated.
    The two given lists are freed and should not be used anymore.
*/
struct linked_list* join(struct linked_list*, struct linked_list*);

/*
    Frees the elements of a list and the list itself.
*/
void release(struct linked_list*);

#endif // LIST_H_INCLUDED
