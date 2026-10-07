#include "nu2api/nucore/nulst.h"

#include <stddef.h>

#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nuthread.h"

NULNKHDR *NuLstAllocFree(NULSTHDR *list) {
    u32 current_thread;
    NULNKHDR *node;
    if (list->free != NULL) {
        current_thread = nu_current_thread_id;
        if (list->safe_thread != current_thread)
            (*NuThreadDisableThreadSwap)();
        node = list->free;
        list->free = list->free->next;
        if (list->free == NULL)
            list->free_tail = NULL;
        else
            list->free->prev = NULL;
        node->prev = NULL;
        node->is_used = true;
        ++list->used_count;
        if (list->safe_thread != current_thread)
            (*NuThreadEnableThreadSwap)();
        return node + 1;
    }
    return NULL;
}

NULNKHDR *NuLstAllocBefore(NULNKHDR *anchor) {
    NULSTHDR *list;
    u32 current_thread;
    NULNKHDR *node;
    --anchor;
    list = anchor->owner;
    current_thread = nu_current_thread_id;
    if (list->safe_thread != current_thread)
        (*NuThreadDisableThreadSwap)();
    if (list->free != NULL) {
        node = list->free;
        list->free = list->free->next;
        if (list->free == NULL)
            list->free_tail = NULL;
        else
            list->free->prev = NULL;
        node->next = anchor;
        node->prev = anchor->prev;
        anchor->prev = node;
        if (node->prev != NULL)
            node->prev->next = node;
        else
            list->head = node;
        node->is_used = true;
        ++list->used_count;
        if (list->safe_thread != current_thread)
            (*NuThreadEnableThreadSwap)();
        return node + 1;
    }
    if (list->safe_thread != current_thread)
        (*NuThreadEnableThreadSwap)();
    return NULL;
}

NULNKHDR *NuLstAllocAfter(NULNKHDR *anchor) {
    NULSTHDR *list;
    u32 current_thread;
    NULNKHDR *node;
    --anchor;
    list = anchor->owner;
    current_thread = nu_current_thread_id;
    if (list->safe_thread != current_thread)
        (*NuThreadDisableThreadSwap)();
    if (list->free != NULL) {
        node = list->free;
        list->free = list->free->next;
        if (list->free == NULL)
            list->free_tail = NULL;
        else
            list->free->prev = NULL;
        node->prev = anchor;
        node->next = anchor->next;
        // The original omits the corresponding write to anchor->next.
        if (node->next != NULL)
            node->next->prev = node;
        else
            list->tail = node;
        node->is_used = true;
        ++list->used_count;
        if (list->safe_thread != current_thread)
            (*NuThreadEnableThreadSwap)();
        return node + 1;
    }
    if (list->safe_thread != current_thread)
        (*NuThreadEnableThreadSwap)();
    return NULL;
}

void NuLstAtachHead(NULSTHDR *list, NULNKHDR *node) {
    u32 current_thread = nu_current_thread_id;
    if (list->safe_thread != current_thread)
        (*NuThreadDisableThreadSwap)();
    node->next = list->head;
    node->prev = NULL;
    if (list->head != NULL)
        list->head->prev = node;
    else
        list->tail = node;
    list->head = node;
    if (list->safe_thread != current_thread)
        (*NuThreadEnableThreadSwap)();
}

void NuLstAttachTail(NULSTHDR *list, NULNKHDR *node) {
    u32 current_thread = nu_current_thread_id;
    if (list->safe_thread != current_thread)
        (*NuThreadDisableThreadSwap)();
    node->prev = list->tail;
    node->next = NULL;
    if (list->tail != NULL)
        list->tail->next = node;
    else
        list->head = node;
    list->tail = node;
    if (list->safe_thread != current_thread)
        (*NuThreadEnableThreadSwap)();
}

NULNKHDR *NuLstGetPrev(NULSTHDR *list, NULNKHDR *node) {
    if (node != NULL) {
        --node;
        if (node->prev != NULL)
            return node->prev + 1;
    } else if (list->tail != NULL) {
        return list->tail + 1;
    }
    return NULL;
}

NULNKHDR *NuLstGetFree(NULSTHDR *list) {
    return list->free + 1;
}

i32 NuLstMoveNext(NULSTHDR *list, NULNKHDR *node) {
    NULNKHDR *next;
    --node;
    if (node->next != NULL) {
        if (node->prev != NULL)
            node->prev->next = node->next;
        else
            list->head = node->next;
        node->next->prev = node->prev;
        if (node->next->next != NULL)
            node->next->next->prev = node;
        else
            list->tail = node;
        node->prev = node->next;
        next = node->next;
        node->next = node->next->next;
        next->next = node;
        return 1;
    }
    return 0;
}

i32 NuLstMovePrev(NULSTHDR *list, NULNKHDR *node) {
    NULNKHDR *prev;
    --node;
    if (node->prev != NULL) {
        if (node->next != NULL)
            node->next->prev = node->prev;
        else
            list->tail = node->prev;
        node->prev->next = node->next;
        if (node->prev->prev != NULL)
            node->prev->prev->next = node;
        else
            list->head = node;
        prev = node->prev;
        node->prev = node->prev->prev;
        node->next = prev;
        prev->prev = node;
        return 1;
    }
    return 0;
}

NULSTHDR *NuLstCreate(i32 element_count, i32 element_size) {
    NULSTHDR *list;
    i32 element_size_total;
    i32 i;
    NULNKHDR *first_free;
    VARIPTR next;
    VARIPTR curr;

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
        curr.void_ptr = first_free;
        next.char_ptr = curr.char_ptr + element_size_total;
        for (i = 1; i < element_count; i++) {
            static_cast<NULNKHDR *>(curr.void_ptr)->next = static_cast<NULNKHDR *>(next.void_ptr);
            static_cast<NULNKHDR *>(curr.void_ptr)->id = i - 1;
            static_cast<NULNKHDR *>(curr.void_ptr)->owner = list;
            curr.void_ptr = next.void_ptr;

            next.char_ptr = next.char_ptr + element_size_total;
        }

        static_cast<NULNKHDR *>(curr.void_ptr)->next = NULL;
        list->free_tail = static_cast<NULNKHDR *>(curr.void_ptr);
        static_cast<NULNKHDR *>(curr.void_ptr)->id = i - 1;

        static_cast<NULNKHDR *>(curr.void_ptr)->owner = list;
        list->safe_thread = nu_current_thread_id;
    }

    return list;
}

void NuLstDestroy(NULSTHDR *list) {
    NuMemoryGet()->GetThreadMem()->BlockFree(list, 0);
}

NULSTHDR *NuLstCreateBuff(i32 count, i32 size, VARIPTR *buffer, VARIPTR end, i32 alignment) {
    NULSTHDR *list = NULL;
    i32 i;
    u32 stride;
    usize bytes;
    NULNKHDR *first_free;
    VARIPTR next;
    VARIPTR node;
    buffer->addr = (buffer->addr + alignment - 1) & -static_cast<usize>(alignment);
    stride = (size + sizeof(NULNKHDR) + alignment - 1) & -alignment;
    bytes = count * stride + sizeof(NULSTHDR);
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
        first_free = list->free;
        node.void_ptr = first_free;
        next.char_ptr = node.char_ptr + stride;
        for (i = 1; i < count; ++i) {
            static_cast<NULNKHDR *>(node.void_ptr)->next = static_cast<NULNKHDR *>(next.void_ptr);
            static_cast<NULNKHDR *>(node.void_ptr)->id = i - 1;
            static_cast<NULNKHDR *>(node.void_ptr)->owner = list;
            node.void_ptr = next.void_ptr;
            next.char_ptr = next.char_ptr + stride;
        }
        static_cast<NULNKHDR *>(node.void_ptr)->next = NULL;
        list->free_tail = static_cast<NULNKHDR *>(node.void_ptr);
        static_cast<NULNKHDR *>(node.void_ptr)->id = i - 1;
        static_cast<NULNKHDR *>(node.void_ptr)->owner = list;
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
