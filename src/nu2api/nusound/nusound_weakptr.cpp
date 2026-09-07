#include "nusound_weakptr.hpp"

NuCriticalSection NuSoundWeakPtrListNode::sPtrListLock("NuSoundCriticalSection::mCriticalSection");
NuCriticalSection NuSoundWeakPtrListNode::sPtrAccessLock("NuSoundCriticalSection::mCriticalSection");

class NuSoundBufferCallback;

// The original callback/OGG destructors call this shared template body.
template <typename T> NuSoundWeakPtrObj<T>::~NuSoundWeakPtrObj() {
    NuSoundWeakPtrListNode::sPtrListLock.Lock();

    NuSoundWeakPtrListNode *node = GetLinks(head)->next;
    while (node != tail) {
        node->Clear();
        node = node->next;
    }

    while (weak_count != 0) {
        node = GetLinks(head)->next;
        if (node->prev != NULL)
            GetLinks(node->prev)->next = node->next;
        if (node->next != NULL)
            GetLinks(node->next)->prev = node->prev;
        node->next = NULL;
        node->prev = NULL;
        --weak_count;
    }

    NuSoundWeakPtrListNode::sPtrListLock.Unlock();

    // The embedded list's destructor follows the owner's locked detach pass.
    while (weak_count != 0) {
        node = GetLinks(head)->next;
        if (node->prev != NULL)
            GetLinks(node->prev)->next = node->next;
        if (node->next != NULL)
            GetLinks(node->next)->prev = node->prev;
        node->next = NULL;
        node->prev = NULL;
        --weak_count;
        delete node;
    }
}

template <typename T> void NuSoundWeakPtr<T>::Set(T *ptr) {
    NuSoundWeakPtrListNode::sPtrListLock.Lock();
    if (this->obj != (void *)ptr) {
        if (this->obj != NULL)
            this->obj->Unlink(this);
        if (ptr != NULL)
            ((NuSoundWeakPtrObj<T> *)ptr)->Link(this);
        this->obj = (NuSoundWeakPtrObj<T> *)ptr;
    }
    NuSoundWeakPtrListNode::sPtrListLock.Unlock();
}

template class NuSoundWeakPtrObj<NuSoundBufferCallback>;
template class NuSoundWeakPtr<NuSoundBufferCallback>;
class NuSoundVoice;
template void NuSoundWeakPtr<NuSoundVoice>::Set(NuSoundVoice *);
