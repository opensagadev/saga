#include "nu2api/nucore/nulst.h"

#include <stddef.h>

#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"

NULSTHDR *NuLstCreate(i32 element_count, i32 element_size) {
    NULSTHDR *list;
    i32 element_size_total;
    i32 i;
    NULNKHDR *first_free;
    union {
        char *char_ptr;
        NULNKHDR *node_ptr;
    } next;
    union {
        char *char_ptr;
        NULNKHDR *node_ptr;
    } curr;

    element_size_total = element_size + sizeof(NULNKHDR);
    list =
        (NULSTHDR *)NU_ALLOC(element_count * element_size_total + sizeof(nulsthdr_s), 4, 1, "", NUMEMORY_CATEGORY_NONE);

    if (list != NULL) {
        list->free = (NULNKHDR *)(list + 1);
        list->head = NULL;
        list->tail = NULL;

        list->element_count = element_count;
        list->element_size = element_size;
        list->element_size_total = element_size_total;
        list->used_count = 0;

        first_free = list->free;
        curr.node_ptr = first_free;
        next.char_ptr = curr.char_ptr + element_size_total;
        for (i = 1; i < element_count; i++) {
            curr.node_ptr->next = next.node_ptr;
            curr.node_ptr->id = i - 1;
            curr.node_ptr->owner = list;
            curr.node_ptr = next.node_ptr;

            next.char_ptr = next.char_ptr + element_size_total;
        }

        curr.node_ptr->next = NULL;
        list->free_tail = curr.node_ptr;
        curr.node_ptr->id = i - 1;

        curr.node_ptr->owner = list;
        list->safe_thread = nu_current_thread_id;
    }

    return list;
}

void NuLstDestroy(NULSTHDR *list) {
    NuMemoryGet()->GetThreadMem()->BlockFree(list, 0);
}

NULSTHDR *NuLstCreateBuff(i32 count, i32 size, VARIPTR *buffer, VARIPTR end, i32 alignment) {
    NULSTHDR *list = NULL;
    buffer->addr = (buffer->addr + alignment - 1) & -static_cast<usize>(alignment);
    u32 stride = (alignment + size + sizeof(NULNKHDR) - 1) & -alignment;
    usize bytes = count * stride + sizeof(NULSTHDR);
    if (bytes < end.addr - buffer->addr) {
        list = static_cast<NULSTHDR *>(buffer->void_ptr);
        buffer->addr += bytes;
        list->free = reinterpret_cast<NULNKHDR *>(list + 1);
        list->head = NULL;
        list->tail = NULL;
        list->element_count = count;
        list->element_size = size;
        list->element_size_total = stride;
        list->used_count = 0;
        NULNKHDR *node = list->free;
        i32 i;
        for (i = 1; i < count; ++i) {
            NULNKHDR *next = reinterpret_cast<NULNKHDR *>(reinterpret_cast<u8 *>(node) + stride);
            node->next = next;
            node->id = i - 1;
            node->owner = list;
            node = next;
        }
        node->next = NULL;
        list->free_tail = node;
        node->id = i - 1;
        node->owner = list;
        list->safe_thread = nu_current_thread_id;
    }
    return list;
}

NULNKHDR *NuLstGetByIdx(NULSTHDR *list, i32 index) {
    NULNKHDR *node = reinterpret_cast<NULNKHDR *>(reinterpret_cast<u8 *>(list + 1) +
                                                  static_cast<i16>(list->element_size_total) * index);
    return node->is_used ? node + 1 : NULL;
}

NULNKHDR *NuLstAlloc(NULSTHDR *list) {
    return NuLstAllocHead(list);
}

NULNKHDR *NuLstAllocHead(NULSTHDR *list) {
    u32 current_thread;
    NULNKHDR *node;

    if (list->free != NULL) {
        current_thread = nu_current_thread_id;

        if (list->safe_thread != current_thread) {
            (*NuThreadDisableThreadSwap)();
        }

        node = list->free;
        list->free = list->free->next;

        if (list->free == NULL) {
            list->free_tail = NULL;
        } else {
            list->free->prev = NULL;
        }

        node->next = list->head;

        if (list->head != NULL) {
            list->head->prev = node;
        } else {
            list->tail = node;
        }

        node->prev = NULL;

        list->head = node;
        list->used_count++;

        node->is_used = true;

        if (list->safe_thread != current_thread) {
            (*NuThreadEnableThreadSwap)();
        }

        return node + 1;
    }

    return NULL;
}

NULNKHDR *NuLstAllocTail(NULSTHDR *list) {
    u32 current_thread;
    NULNKHDR *node;

    if (list->free != NULL) {
        current_thread = nu_current_thread_id;

        if (list->safe_thread != current_thread) {
            (*NuThreadDisableThreadSwap)();
        }

        node = list->free;
        list->free = list->free->next;

        if (list->free == NULL) {
            list->free_tail = NULL;
        } else {
            list->free->prev = NULL;
        }

        node->prev = list->tail;

        if (list->tail != NULL) {
            list->tail->next = node;
        } else {
            list->head = node;
        }

        node->next = NULL;

        list->tail = node;

        node->is_used = true;
        list->used_count++;

        if (list->safe_thread != current_thread) {
            (*NuThreadEnableThreadSwap)();
        }

        return node + 1;
    }

    return NULL;
}

void NuLstFree(NULNKHDR *node) {
    u32 current_thread;
    NULSTHDR *list;

    node--;
    list = node->owner;

    current_thread = nu_current_thread_id;

    if (list->safe_thread != current_thread) {
        (*NuThreadDisableThreadSwap)();
    }

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

    node->prev = list->free_tail;
    node->next = NULL;

    if (list->free_tail != NULL) {
        list->free_tail->next = node;
    } else {
        list->free_tail = list->free = node;
    }

    list->free_tail = node;

    list->used_count--;

    node->is_used = false;

    if (list->safe_thread != current_thread) {
        (*NuThreadEnableThreadSwap)();
    }
}

NULNKHDR *NuLstGetNext(NULSTHDR *list, NULNKHDR *node) {
    if (node != NULL) {
        node--;

        if (node->next != NULL) {
            return node->next + 1;
        }
    } else if (list->head != NULL) {
        return list->head + 1;
    }

    return NULL;
}
