//
// Created by fox on 26/04/2026.
//

#include "Misc.h"

namespace UniSplice::Hook {

    bool Misc::Initialize() {
        if (g_mkdir_hooks_installed) return true;
        if (!UniSplice::Main::shadow_hooks) return true;

        const char* target_pkg = getenv("UNISPLICE_TARGET_PKG");
        const char* our_pkg = getenv("UNISPLICE_OUR_PKG");

        LOGI("Misc init: shadow_hooks=%d, target_pkg=%s, our_pkg=%s",
            UniSplice::Main::shadow_hooks ? 1 : 0,
            target_pkg ? target_pkg : "(null)",
            our_pkg ? our_pkg : "(null)");

        if (!target_pkg || !our_pkg) return true;

        LOGI("Installing shadow hooks for file redirection...");
        void* stub = shadowhook_hook_func_addr((void*)mkdir, (void*)Hook_mkdir, (void**)&original_mkdir);
        if (stub) {
            g_mkdir_hooks_installed = true;
            LOGI("Shadow hooks installed successfully!");
        } else {
            LOGE("Failed to install shadow hooks!");
        }
        return true;
    }
}