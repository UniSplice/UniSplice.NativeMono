#include "Mono.h"

namespace UniSplice::Hook {
    void* (*Mono::_orig_mono_runtime_invoke)(void*, void*, void**, void**) = nullptr;

    void Mono::Initialize() {
        void* stub = shadowhook_hook_sym_name(
                "libmonobdwgc-2.0.so",
                "mono_runtime_invoke",
                (void*) mHookMonoRuntimeInvoke,
                (void**)&_orig_mono_runtime_invoke
        );

        if (!stub) {
            LOGE("Failed to hook mono_runtime_invoke!");
        } else {
            LOGI("mono_runtime_invoke hook installed!");
        }
    }


    void* Mono::ResolveSymbol(const char* name) {
        void* sym = nullptr;

        // Try mono directly
        if (UniSplice::Main::mono_handle) {
            sym = dlsym(UniSplice::Main::mono_handle, name);
            if (sym) return sym;
        }

        // Try global scope
        sym = dlsym(RTLD_DEFAULT, name);
        if (sym) return sym;

        // Try libunity (VERY IMPORTANT for Unity builds)
        void* unity = dlopen("libunity.so", RTLD_NOW);
        if (unity) {
            sym = dlsym(unity, name);
            if (sym) return sym;
        }

        return nullptr;
    }

    void* Mono::mHookMonoRuntimeInvoke(void* method, void* obj, void** params, void** exc) {
        static bool ran = false;

        if (ran) return _orig_mono_runtime_invoke(method, obj, params, exc);
        ran = true;

        LOGI("mono_runtime_invoke hit -> Mono fully ready");

        // Resolve Mono functions NOW (safe timing)
        auto mono_get_root_domain = (MonoDomain*(*)())ResolveSymbol("mono_get_root_domain");

        auto mono_thread_attach = (void(*)(MonoDomain*))ResolveSymbol("mono_thread_attach");

        auto mono_image_open_from_data =(MonoImage*(*)(char*, uint32_t, int, MonoImageOpenStatus*, int))
                        ResolveSymbol("mono_image_open_from_data");

        auto mono_assembly_load_from_full = (MonoAssembly*(*)(MonoImage*, const char*, MonoImageOpenStatus*, int))
                        ResolveSymbol("mono_assembly_load_from_full");

        auto mono_assembly_get_image =(MonoImage*(*)(MonoAssembly*)) ResolveSymbol("mono_assembly_get_image");

        auto mono_class_from_name = (void*(*)(MonoImage*, const char*, const char*)) ResolveSymbol("mono_class_from_name");

        auto mono_class_get_method_from_name = (void*(*)(void*, const char*, int))
                        ResolveSymbol("mono_class_get_method_from_name");

        auto mono_runtime_invoke_fn = (void*(*)(void*, void*, void**, void**)) ResolveSymbol("mono_runtime_invoke");

        if (!mono_get_root_domain || !mono_thread_attach ||
            !mono_image_open_from_data || !mono_assembly_load_from_full ||
            !mono_assembly_get_image || !mono_class_from_name ||
            !mono_class_get_method_from_name || !mono_runtime_invoke_fn) {

            LOGE("Failed to resolve Mono symbols!");
        } else {
            MonoDomain* domain = mono_get_root_domain();
            mono_thread_attach(domain);
            FILE* preloader = fopen("/sdcard/UniSplice/core/preload/UniSplice.Preloader.dll", "rb");
            if (preloader) {
                // Read File
                fseek(preloader, 0, SEEK_END);
                size_t size = ftell(preloader);
                rewind(preloader);

                char* data = (char*)malloc(size);
                fread(data, 1, size, preloader);
                fclose(preloader);

                // Create Image Assembly
                MonoImageOpenStatus status;
                MonoImage* image = mono_image_open_from_data(data, size, 1, &status, 0);

                if (image) {
                    // Load Assembly
                    MonoAssembly* assembly =
                            mono_assembly_load_from_full(image, "UniSplice.Preloader.dll", &status, 0);

                    if (assembly) {
                        LOGI("Preloader loaded!");

                        // Invoke Entry.Initialize()
                        MonoImage* img = mono_assembly_get_image(assembly);
                        void* klass = mono_class_from_name(img, "UniSplice.Preloader", "Entry");
                        void* method_init = mono_class_get_method_from_name(klass, "Initialize", 0);

                        if (method_init) {
                            mono_runtime_invoke_fn(method_init, nullptr, nullptr, nullptr);
                            LOGI("Entry.Initialize() invoked!");
                        }
                    }
                }

                free(data);
            } else {
                LOGE("Failed to open Preloader DLL!");
            }
        }

        return _orig_mono_runtime_invoke(method, obj, params, exc);
    }
}