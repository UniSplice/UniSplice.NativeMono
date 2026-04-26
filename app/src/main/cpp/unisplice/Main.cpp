#include "Main.h"
#include <cstdlib>

namespace UniSplice {
    bool Main::_initialized = false;
    void* Main::_shadowhook_handle = nullptr;
    void* Main::mono_handle = nullptr;
    bool Main::injected = false;
    bool Main::is_old_mono = false;
    bool Main::shadow_hooks = false;

    bool Main::Initialize() {
        if(_initialized) return true; // Already Initialized

        shadow_hooks = (getenv("UNISPLICE_SHADOW_HOOKS") != nullptr);
        if (shadow_hooks) {
            LOGI("Shadow hooks enabled!");
        }

        LOGI("Loading ShadowHook...");
        if(!mLoadShadowHook()){
            LOGE("Failed to load ShadowHook!");
            return false;
        }

        if (shadowhook_init(SHADOWHOOK_MODE_UNIQUE, true) != 0) {
            LOGE("ShadowHook init failed!");
            return false;
        }

        if(injected) {
            LOGI("Not Registering DlOpen hook (Injector)");
            LOGI("UniSplice Bootstrapper Partial load finished!");
        }else{
            if(!HookDlOpen()) return false;
        }

        return true;
    }

    bool Main::HookDlOpen() {
        LOGI("Registering DlOpen hook...");
        if (!UniSplice::Hook::DlOpen::Initialize()) return false; // Failed Hook
        _initialized = true;

        LOGI("UniSplice Bootstrapper Finished Loading!");
        return true;
    }

    bool Main::mLoadShadowHook() {
        _shadowhook_handle = dlopen("libshadowhook.so", RTLD_NOW);
        if (!_shadowhook_handle) {
            LOGE("Failed to load libshadowhook.so: %s", dlerror());
            return false;
        }
        LOGI("Loaded libshadowhook.so");
        return true;
    }

    void Main::OnMonoLoaded() {
        LOGI("Mono loaded, hooking mono_runtime_invoke...");

        UniSplice::Hook::Mono::Initialize();

    }
}