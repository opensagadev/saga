

#include <stdio.h>

static char host_documents_path[256] = "res/";

void HostSetDocumentsPath(const char *path) {
    snprintf(host_documents_path, sizeof(host_documents_path), "%s", path);
}

char *NuIOS_GetDocumentsPath(void) {
    // slotfolder appends SavedGames directly, so the path needs a trailing slash.
    return host_documents_path;
}
