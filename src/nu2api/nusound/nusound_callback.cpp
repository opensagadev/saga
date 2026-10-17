#include "decomp.h"
#include "nu2api/nucore/nuvuvec.hpp"
#include "nusound_weakptr.hpp"

// Weak-pointer list locks (retail nusound_callback.cpp).

NuSoundCriticalSection NuSoundWeakPtrListNode::sPtrListLock;
NuSoundCriticalSection NuSoundWeakPtrListNode::sPtrAccessLock;
