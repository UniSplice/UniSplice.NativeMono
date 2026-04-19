#pragma once
#include "shadowhook.h"
#include "unisplice.h"
#include "unisplice/Main.h"
#include <string>

namespace UniSplice::Hook {

    class DlOpen {
    private:
        static void* (*_original)(const char* filename, int flags);
        static bool _hookEnabled;
        static void* mHook(const char* filename, int flags);
    public:
        static bool Initialize();
    };

}
