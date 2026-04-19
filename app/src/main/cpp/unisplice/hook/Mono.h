#pragma once
#include "unisplice/Main.h"


namespace UniSplice::Hook {

    class Mono {
    private:
        typedef struct _MonoDomain MonoDomain;
        typedef struct _MonoAssembly MonoAssembly;
        typedef struct _MonoImage MonoImage;
        typedef int MonoImageOpenStatus;

        static void* (*_orig_mono_runtime_invoke)(void*, void*, void**, void**);
        static void* (*_orig_mono_assembly_load)(void*, void*, void*);
        static void* (*_orig_mono_assembly_load_from_full)(void*, void*, void*, void*);

        static void* mHookMonoRuntimeInvoke(void* method, void* obj, void** params, void** exc);
        static void* mHookMonoAssemblyLoad(void* aname, void* dependent_assm, void* status);
        static void* mHookMonoAssemblyLoadFromFull(void* image, void* name, void* status, void* refonly);
    public:
        static void Initialize();
        static void* ResolveSymbol(const char* name);

    };

}
