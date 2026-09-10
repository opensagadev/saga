#include "nusound_weakptr.hpp"

NuCriticalSection NuSoundWeakPtrListNode::sPtrListLock("NuSoundCriticalSection::mCriticalSection");
NuCriticalSection NuSoundWeakPtrListNode::sPtrAccessLock("NuSoundCriticalSection::mCriticalSection");

template <typename T> NuSoundWeakPtrObj<T>::~NuSoundWeakPtrObj() {
    NuSoundWeakPtrListNode::sPtrListLock.Lock();

    if (this->weak_count != 0) {
        // The two list sentinels are biased NuSoundWeakPtrListNode pointers.
        // The target identifies the end sentinel by its null next link rather
        // than comparing it with the cached end pointer.
        NuSoundWeakPtrListNode *node = this->head->next;
        while (node != NULL && node->prev != NULL && node->next != NULL) {
            NuSoundWeakPtrListNode *next = node->next;
            node->Clear();
            node = next;
        }

        // Clear() only releases the weak reference. The target then removes
        // every entry from the intrusive list while still holding the global
        // list lock.
        while (this->weak_count != 0) {
            node = this->head->next;
            NuSoundWeakPtrListNode *previous = node->prev;
            NuSoundWeakPtrListNode *next = node->next;
            if (previous != NULL) {
                previous->next = next;
            }
            if (next != NULL) {
                next->prev = previous;
            }
            node->prev = NULL;
            node->next = NULL;
            this->weak_count--;
        }
    }

    NuSoundWeakPtrListNode::sPtrListLock.Unlock();
}

class NuSoundBufferCallback;
template class NuSoundWeakPtrObj<NuSoundBufferCallback>;
template class NuSoundWeakPtr<NuSoundBufferCallback>;
