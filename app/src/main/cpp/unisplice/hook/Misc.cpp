//
// Created by fox on 26/04/2026.
//

#include "Misc.h"
#include <dlfcn.h>

namespace UniSplice::Hook {

    bool Misc::Initialize() {
        if (g_mkdir_hooks_installed) return true;
        if (!UniSplice::Main::shadow_hooks) return true;
/*
        const char* target_pkg = getenv("UNISPLICE_TARGET_PKG");
        const char* our_pkg = getenv("UNISPLICE_OUR_PKG");

        LOGI("Misc init: shadow_hooks=%d, target_pkg=%s, our_pkg=%s",
            UniSplice::Main::shadow_hooks ? 1 : 0,
            target_pkg ? target_pkg : "(null)",
            our_pkg ? our_pkg : "(null)");

        if (!target_pkg || !our_pkg) return true;

        LOGI("Installing shadow hooks for file redirection...");

        // Hook mkdir
        void* stub = shadowhook_hook_func_addr((void*)mkdir, (void*)Hook_mkdir, (void**)&original_mkdir);
        if (stub) {
            LOGI("Hooked mkdir!");
        }

        // Hook mkdirs and mkdirat via dlsym
        void* handle = dlopen("libc.so", RTLD_NOW);
        if (handle) {
            void* mkdirs_sym = dlsym(handle, "mkdirs");
            if (mkdirs_sym) {
                void* stub2 = shadowhook_hook_func_addr(mkdirs_sym, (void*)Hook_mkdirs, (void**)&original_mkdirs);
                if (stub2) {
                    LOGI("Hooked mkdirs!");
                }
            }

            // Hook mkdirat
            typedef int (*mkdirat_t)(int, const char*, mode_t);
            void* mkdirat_sym = dlsym(handle, "mkdirat");
            if (mkdirat_sym) {
                // Just use Hook_mkdirat - it won't properly redirect but at least logs
                LOGI("Hooking mkdirat...");
            }
        }

        if (original_mkdir || original_mkdirs) {
            g_mkdir_hooks_installed = true;
            LOGI("Shadow hooks installed successfully!");
        } else {
            LOGE("Failed to install shadow hooks!");
        }
        return true;*/
    }
}