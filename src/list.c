#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

void initialize(linked_list* list) {
    *list = (linked_list) calloc(1, sizeof(struct linked_list));

    (*list)->first = NULL;
    (*list)->last = NULL;
    (*list)->length = 0;
}

status create(generic_t value, list_node* node) {
    *node = (list_node) calloc(1, sizeof(struct node));

    if (!(*node)) {
        return OUT_OF_MEMORY;
    }

    (*node)->value = value;

    (*node)->next = NULL;

    return OK;
}

status insert_first(generic_t value, linked_list list) {
    status s = create(value, &(list->first));

    if (s != OK) {
        return s;
    }

    list->last = list->first;

    return OK;
}

status append(generic_t value, linked_list list) {
    if (list->length == 0) {
        status s = insert_first(value, list);

        if (s != OK) {
            return s;
        }
    } else {
        status s = create(value, &(list->last->next));

        if (s != OK) {
            return s;
        }

        list->last = list->last->next;
    }

    list->length++;

    return OK;
}

status prepend(generic_t value, linked_list list) {
    if (list->length == 0) {
        status s = insert_first(value, list);

        if (s != OK) {
            return s;
        }
    } else {
        list_node new_node;

        status s = create(value, &new_node);

        if (s != OK) {
            return s;
        }

        new_node->next = list->first;
        list->first = new_node;
    }

    list->length++;

    return OK;
}

generic_t nth(linked_list_size_t position, linked_list list) {
    if (position == 1) {
        return list->first->value;
    }

    if (position >= list->length) {
        return list->last->value;
    }

    linked_list_size_t index = 2;
    list_node current = list->first->next;

    while (index != position) {
        current = current->next;
        index++;
    }

    return current->value;
}

linked_list_size_t count(linked_list list) {
    return list->length;
}

void delete_at(linked_list_size_t position, linked_list list) {
    linked_list_size_t length = count(list);

    if (length == 0 || length < position) {
        return;
    }

    list_node to_del;

    if (position == 1) {
        to_del = list->first;

        list->first = to_del->next;

        free(to_del);

        if (position == length) {
            list->last = list->first;
        }
    } else {
        linked_list_size_t index = 2;
        list_node previous = list->first;

        while (index != position) {
            previous = previous->next;
            index++;
        }

        to_del = previous->next;

        previous->next = to_del->next;

        if (position == length) {
            list->last = previous;
        }
    }

    free(to_del);

    list->length--;
}

void iterate(bool (*f)(linked_list_size_t, generic_t), linked_list list) {
    list_node current = list->first;
    linked_list_size_t index = 1;

    while (current) {
        bool go_ahead = (*f)(index, current->value);

        if (!go_ahead) {
            return;
        }

        current = current->next;
        index++;
    }
}

void reduce(void (*f)(generic_t, generic_t), generic_t accumulator, linked_list list) {
    if (count(list) == 0) {
        return;
    }

    list_node current = list->first;

    while (current) {
        (*f)(accumulator, current->value);
        current = current->next;
    }
}

linked_list_size_t find(bool (*f)(linked_list_size_t, generic_t), linked_list list) {
    list_node current = list->first;
    linked_list_size_t index = 1;

    while (current) {
        bool found = (*f)(index, current->value);

        if (found) {
            return index;
        }

        index++;
        current = current->next;
    }

    return 0;
}

void split(linked_list_size_t length_first, linked_list list, linked_list first, linked_list second) {
    list_node current = list->first;
    linked_list_size_t index = 1;

    while (current) {
        if (index <= length_first) {
            append(current->value, first);
        } else {
            append(current->value, second);
        }

        index++;
        current = current->next;
    }
}

void filter(bool (*f)(generic_t), linked_list list) {
    list_node current = list->first;
    linked_list_size_t index = 1;

    while (current) {
        bool keep = (*f)(current->value);

        current = current->next;

        if (keep) {
            index++;
        } else {
            delete_at(index, list);
        }
    }
}

linked_list join(linked_list list1, linked_list list2) {
    linked_list new_list;
    initialize(&new_list);

    if (count(list1) == 0) {
        release(list1);

        if (count(list2) > 0) {
            new_list->first = list2->first;
            new_list->last = list2->last;

            free(list2);
        } else {
            release(list2);
        }

        return new_list;
    }

    list1->last->next = list2->first;

    new_list->first = list1->first;
    new_list->last = list2->last;

    new_list->length = list1->length + list2->length;

    free(list1);
    free(list2);

    return new_list;
}

void release(linked_list list) {
    if (count(list) == 0) {
        free(list);

        return;
    }

    list_node current;
    list_node next = list->first;

    while (next) {
        current = next;
        next = next->next;

        free(current->value);
        free(current);
    }

    free(list);
}
