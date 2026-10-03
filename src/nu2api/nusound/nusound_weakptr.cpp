#include "nusound_weakptr.hpp"

NuSoundCriticalSection NuSoundWeakPtrListNode::sPtrListLock;
NuSoundCriticalSection NuSoundWeakPtrListNode::sPtrAccessLock;

template <typename T> NuSoundWeakPtrObj<T>::~NuSoundWeakPtrObj() {
    NuSoundWeakPtrListNode::sPtrListLock.Lock();

    if (this->weak_count != 0) {
        // The list sentinels are biased NuSoundWeakPtrListNode pointers; the
        // end sentinel is recognised by its null next link.
        for (NuSoundWeakPtrListNode *node = this->head->next; node != NULL && node->next != NULL && node->prev != NULL;
             node = node->next) {
            node->Clear();
        }
        while (this->weak_count != 0) {
            PopFront();
        }
    }

    NuSoundWeakPtrListNode::sPtrListLock.Unlock();

    while (this->weak_count != 0) {
        delete PopFront();
    }
}

class NuSoundBufferCallback;
template class NuSoundWeakPtrObj<NuSoundBufferCallback>;
template class NuSoundWeakPtr<NuSoundBufferCallback>;
