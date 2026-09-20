#pragma once

#include "decomp.h"

// Bridge-editor state and lifecycle shared across its reconstructed owners.
extern "C" {
    extern i32 edbri_page_used[8];

    void edbriStartPage(i32 page);
    void edbriStartAllPages(void);
}
