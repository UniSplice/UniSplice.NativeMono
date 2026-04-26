#include "DlOpen.h"
#include "Misc.h"


namespace UniSplice::Hook {
    void* (*DlOpen::_original)(const char* filename, int flags) = nullptr;
    bool DlOpen::_hookEnabled = false;
    static bool _miscInitialized = false;

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

        _hookEnabled = true;
        LOGI("dlopen hook registered!");
        return true;
    }

    void* DlOpen::mHook(const char* filename, int flags) {
        // Don't process if hook isn't fully enabled yet (recursive init guard)
        if (!_hookEnabled || !_original) {
            // Use dlsym to get the real dlopen
            static void* (*real_dlopen)(const char*, int) = nullptr;
            if (!real_dlopen) {
                real_dlopen = (void*(*)(const char*, int))dlsym(RTLD_NEXT, "dlopen");
            }
            return real_dlopen ? real_dlopen(filename, flags) : nullptr;
        }

        void* handle = _original(filename, flags);

        // Install misc hooks on first dlopen (not after mono!)
        if (!_miscInitialized && UniSplice::Main::shadow_hooks) {
            _miscInitialized = true;
            Misc::Initialize();
        }

        if (filename) {
            LOGI("dlopen called: %s", filename);

            bool old_mono = true;
            // Check for libmono separately, in case this game is older
            if(strstr(filename, "libmonobdwgc")) old_mono = false;

            if (strstr(filename, "libmonobdwgc") || strstr(filename, "libmono")) {
                LOGI("Mono library detected: %s", filename);
                UniSplice::Main::mono_handle = handle;
                UniSplice::Main::is_old_mono = old_mono;
                UniSplice::Main::OnMonoLoaded();
            }
        }

        return handle;
    }
}