#pragma once

#include "nu2api/nucore/common.h"

#include "nu2api/nusound/nusound_system.hpp"

class __attribute__((packed, aligned(4))) NuSoundBuffer {
  public:
    struct __attribute__((packed, aligned(4))) Context {
        u64 read_size;
        u64 size2;
        u64 size3;
        i32 field5_0x18;
        u8 flags;
        u8 reserved_0x1d[3];
        i32 field5_0x20;
    };

    static_assert(sizeof(Context) == 0x24, "sound buffer context size");
    static_assert(__builtin_offsetof(Context, flags) == 0x1c, "sound buffer flag byte");
    static_assert(__builtin_offsetof(Context, field5_0x20) == 0x20, "sound buffer context tail");

  private:
    u64 size;
    void *address;
    NuSoundMemoryBuffer *memory_buffer;
    i32 lock_count;
    i32 allocated;
    NuSoundSystem::MemoryDiscipline memory_discipline;
    Context context;

  public:
    NuSoundBuffer();
    NuSoundBuffer(char *name, u64 size);
    ~NuSoundBuffer();

    void Free();

    i32 Provide(char *address, u64 size);

    i32 Allocate(u64 size, NuSoundSystem::MemoryDiscipline disc);

    void Lock();
    void Unlock();

    void SetCurrentContext(Context &context);

    Context &GetCurrentContext();

    void *GetAddress() const;

    bool IsAllocated() const;

    u64 GetBufferSize() const;

    bool IsLocked() const;

    void GetSegmentAddress(unsigned int, unsigned int, unsigned int) const;
    void GetSegmentSize(unsigned int, unsigned int) const;
};
