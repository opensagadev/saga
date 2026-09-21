#pragma once

#include "nu2api/nucore/fixed_width.h"

struct EdClass;

struct EdObjectNotifier {
    virtual void NotifyCreateObject(void *, EdClass *, void *, i32, i32, i32) {
    }
    virtual void NotifyDestroyObject(void *, EdClass *, i32, i32) {
    }
    virtual void NotifyDefunctObject(void *, EdClass *, i32) {
    }
    virtual void NotifyReviveObject(void *, EdClass *, i32) {
    }
};
