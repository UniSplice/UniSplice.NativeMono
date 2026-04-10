#include "Main.h"

namespace UniSplice {
    bool Main::_initialized = false;
    void* Main::_shadowhook_handle = nullptr;
    void* Main::mono_handle = nullptr;

    bool Main::Initialize() {
        if(_initialized) return true; // Already Initialized

        LOGI("Loading ShadowHook...");
        if(!mLoadShadowHook()){
            LOGE("Failed to load ShadowHook!");
            return false;
        }

        LOGI("Registering DlOpen hook...");
        if(!UniSplice::Hook::DlOpen::Initialize()) return false; // Failed Hook

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