#include "DlOpen.h"


namespace UniSplice::Hook {
    void* (*DlOpen::_original)(const char* filename, int flags) = nullptr;

    bool DlOpen::Initialize() {
        void* stub = shadowhook_hook_sym_name(
                "libdl.so",
                "dlopen",
                (void*)mHook,
                (void**)&_original
        );

        if (!stub) {
            LOGE("Failed to hook dlopen!");
            return false;
        }

        LOGI("dlopen hook registered!");
        return true;
    }

    void* DlOpen::mHook(const char* filename, int flags) {
        void* handle = _original(filename, flags);

        if (filename) {
            LOGI("dlopen called: %s", filename);

            if (strstr(filename, "libmonobdwgc") || strstr(filename, "libmono")) {
                LOGI("Mono library detected: %s", filename);
                UniSplice::Main::mono_handle = handle;
                UniSplice::Main::OnMonoLoaded();
            }
        }

        return handle;
    }
}