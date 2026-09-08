#include "nu2api_nufile_types.h"

NuFileBase *NuFileDeviceAndroidAPK::CreateNuFile(char const *path, NuFile::OpenMode::T mode) const {
    return NuFileAndroidAPK::Open(path, mode);
}

NuFileDeviceAndroidAPK::NuFileDeviceAndroidAPK(char const *name, NuFile::InitData const &) {
    device_type = NUFILE_DEVICE_ANDROID_APK;
    separator = "/";
    label = name;
    flags = 9;
    mount_name = "";
}

NuFileDeviceAndroidAPK::~NuFileDeviceAndroidAPK() {
}
