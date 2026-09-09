#include "nu2api/nucore/nulist.h"

#include <stddef.h>

int NuLinkedListCheck(NULISTHDR *list) {
    NULISTLNK *node;
    int count;
    NULISTLNK *previous = NULL;
    count = 0;
    node = list->head;
    while (node != NULL) {
        count++;
        previous = node;
        node = node->next;
    }
    return count;
}

void NuLinkedListAppend(NULISTHDR *list, NULISTLNK *node) {
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail != NULL) {
        list->tail->next = node;
    }

    list->tail = node;

    if (list->head == NULL) {
        list->head = node;
    }
}

void NuLinkedListInsert(NULISTHDR *list, NULISTLNK *node) {
    node->next = list->head;
    node->prev = NULL;

    if (list->head != NULL) {
        list->head->prev = node;
    }

    if (list->tail == NULL) {
        list->tail = node;
    }

    list->head = node;
}

void NuLinkedListInsertBefore(NULISTHDR *list, NULISTLNK *position, NULISTLNK *node) {
    if (position != NULL) {
        node->next = position;
        node->prev = position->prev;
        if (position->prev != NULL) {
            position->prev->next = node;
        } else {
            list->head = node;
        }
        position->prev = node;
    } else {
        NuLinkedListInsert(list, node);
    }
}

void NuLinkedListInsertAfter(NULISTHDR *list, NULISTLNK *position, NULISTLNK *node) {
    if (position != NULL) {
        node->prev = position;
        node->next = position->next;
        if (position->next != NULL) {
            position->next->prev = node;
        } else {
            list->tail = node;
        }
        position->next = node;
    } else {
        NuLinkedListAppend(list, node);
    }
}

void NuLinkedListRemove(NULISTHDR *list, NULISTLNK *node) {
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
}

NULISTLNK *NuLinkedListGetHead(NULISTHDR *list) {
    return list->head;
}

NULISTLNK *NuLinkedListGetTail(NULISTHDR *list) {
    return list->tail;
}

NULISTLNK *NuLinkedListGetPrev(NULISTHDR *list, NULISTLNK *node) {
    if (node != NULL) {
        return node->prev;
    } else {
        return list->tail;
    }
}

NULISTLNK *NuLinkedListGetNext(NULISTHDR *list, NULISTLNK *node) {
    if (node != NULL) {
        return node->next;
    } else {
        return list->head;
    }
}
