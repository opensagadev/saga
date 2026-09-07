#include "nu2api_nufile_types.h"
#include "nu2api/nucore/nustring.h"

NuFileDevice *NuFileDevice::sm_Devices[16];
i32 NuFileDevice::sm_NumDevices;
NuFileDevice *NuFileDevice::sm_DefaultDevice;
NuFileDevice *NuFileDevice::sm_HostDevice;
i32 NuFileDevice::sm_NumRules;
NuFileDevice::DirectoryHandle NuFileDevice::sm_DirectoryHandles[16];

void NuFileDevice::AddDevice(NuFileDevice *device) {
    device->device_id = sm_NumDevices;
    sm_Devices[sm_NumDevices++] = device;
}

void NuFileDevice::AddPathRule(NuFileDeviceType, char const *) {
}

void NuFileDevice::AllocDirectoryHandle(char const *) {
}

void NuFileDevice::ClearPathRules() {
    sm_NumRules = 0;
}

NuFileBase *NuFileDevice::FileOpen(char const *path, NuFile::OpenMode::T mode) const {
    char formatted[1024];
    NuFileBase *file = NULL;
    if (path[0] && FormatName(formatted, sizeof(formatted), path))
        file = CreateNuFile(formatted, mode);
    return file;
}

i64 NuFileDevice::FileSize(char const *path) const {
    if (!path || !path[0]) return -1;
    NuFileBase *file = FileOpen(path, NuFile::OpenMode::READ);
    if (!file) return -1;
    file->Seek(0, NuFile::SeekOrigin::END);
    i64 size = file->GetPos();
    file->Close();
    return size;
}

i32 NuFileDevice::FormatName(char *output, i32 size, char const *path) const {
    char relative[512] = "";
    output[0] = 0;
    char *normalized;
    if (((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z')) &&
        path[1] == ':' && (path[2] == '/' || path[2] == '\\')) {
        NuStrCat(output, path);
        normalized = output + 3;
    } else {
        const char *colon = path;
        for (i32 i = 0; *colon != ':' && *colon && i < 8; ++i) ++colon;
        if (*colon == ':') path = colon + 1;
        if (*mount_name > 0) {
            NuStrCat(output, mount_name);
            char *last = output + NuStrLen(mount_name) - 1;
            if (*last == '/' || *last == '\\') *last = 0;
        }
        i32 length = NuStrLen(output);
        normalized = output + length;
        if ((((path[0] >= 'a' && path[0] <= 'z') || (path[0] >= 'A' && path[0] <= 'Z')) &&
             path[1] == ':' && (path[2] == '/' || path[2] == '\\')) ||
            path[0] == '/' || path[0] == '\\') {
            NuStrCat(relative, path);
        } else {
            if (length > 0 && current_dir[0] != '\\' && current_dir[0] != '/')
                NuStrCat(relative, separator);
            NuStrCat(relative, current_dir);
            NuStrCat(relative, path);
        }
    }
    if (flags & 2) NuStrUpr(relative, relative);
    if (*relative) NuFileNormalise(normalized, size, relative);
    char *source = normalized;
    while (*source) {
        if (*source == '/' || *source == '\\') {
            *normalized++ = *separator;
            do { ++source; } while (*source == '/' || *source == '\\');
        } else {
            *normalized++ = *source++;
        }
    }
    *normalized = 0;
    return 1;
}

void NuFileDevice::FreeDirectoryHandle(i32) {
}

void NuFileDevice::GetDeviceByType(NuFileDeviceType) {
}

NuFileDevice *NuFileDevice::GetDeviceFromDirectoryHandle(i32 handle) {
    return sm_DirectoryHandles[handle].device;
}

void NuFileDevice::GetDeviceFromPath(char const *) {
}

void NuFileDevice::Interrogate() {
    status = 1;
}

NuFileDevice::NuFileDevice() {
    device_type = NUFILE_DEVICE_UNKNOWN;
    flags = 0;
    status = 0;
    separator = "\\";
    label = "";
    current_dir[0] = 0;
    device_id = -1;
}

i32 NuFileDevice::QueryInstallProgress() {
    return 100;
}

void NuFileDevice::SetCurrentDir(char const *path) {
    char suffix[2] = "\\";
    if (!path || !*path) {
        current_dir[0] = 0;
        return;
    }
    i32 length = NuStrCpy(current_dir, path);
    if (length && path[length - 1] != '/' && path[length - 1] != '\\') {
        suffix[0] = *separator;
        NuStrCat(current_dir, suffix);
    }
}

void NuFileDevice::SetDefaultDevice(NuFileDeviceType type) {
    for (i32 i = 0; i < sm_NumDevices; ++i) {
        NuFileDevice *device = sm_Devices[i];
        if (device != NULL && device->device_type == type) {
            sm_DefaultDevice = device;
            return;
        }
    }
}

void NuFileDevice::SetLabel(char *name) {
    label = name;
}

void NuFileDevice::SetMountName(char *name) {
    mount_name = name;
}

NuFileDevice::~NuFileDevice() {
    if (device_id >= 0) sm_Devices[device_id] = NULL;
    if (this == sm_DefaultDevice) sm_DefaultDevice = NULL;
    if (this == sm_HostDevice) sm_HostDevice = NULL;
}
