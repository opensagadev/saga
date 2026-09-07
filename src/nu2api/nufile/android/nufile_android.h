#pragma once
#include "decomp_assert.h"

#include "nu2api/nufile/nufile.h"

class NuFileDeviceAndroidAPK;
struct AAsset;

extern NuFileDeviceAndroidAPK *g_apkFileDevice;

class NuFileAndroidAPK : public NuFileBase {
  public:
    NuFileAndroidAPK(const char *filepath, NuFile::OpenMode::T mode);
    static NuFileAndroidAPK *ms_fileId[0x400];
    static void Init();
    static NuFileAndroidAPK *Open(const char *filepath, NuFile::OpenMode::T mode);
    static void ResetId(i32 id);
    static i32 SetFileId(NuFileAndroidAPK *file);
    static NuFileAndroidAPK *GetFile(i32 id) { return ms_fileId[id - 0x2000]; }

    virtual i64 Seek(i64 offset, NuFile::SeekOrigin::T) override;
    virtual isize Read(void *buf, usize size) override;
    virtual isize Write(const void *buf, usize size) override;
    virtual void Close() override;

    static NUFILE OpenFile(const char *filepath, NuFile::OpenMode::T mode) {
        NuFileAndroidAPK *file = Open(filepath, mode);
        i32 id = SetFileId(file);
        return id;
    }
    static void CloseFile(NUFILE file) {
        NuFileAndroidAPK *object = GetFile(file);
        ResetId(file);
        object->Close();
        delete object;
    }

    static i64 SeekFile(NUFILE file, i64 offset, NuFile::SeekOrigin::T mode) {
        return GetFile(file)->Seek(offset, mode);
    }
    static i32 ReadFile(NUFILE file, void *buf, u32 size) {
        return GetFile(file)->Read(buf, size);
    }

    static i64 GetFilePos(NUFILE file) { return GetFile(file)->GetPos(); }
    static i64 GetFileSize(NUFILE file) { return GetFile(file)->GetSize(); }

  protected:
    virtual ~NuFileAndroidAPK() override;

  private:
    char asset_path[256];
    i32 file_size;
    u32 chunk_size;
    i32 position;
    AAsset *asset;
    u32 asset_index;
};

DECOMP_ASSERT(sizeof(NuFileAndroidAPK) == 0x224, "NuFileAndroidAPK target layout");
