#pragma once

#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <string>
#include <android/log.h>
#include <shadowhook.h>
#include "unisplice/Main.h"
#include "unisplice.h"


namespace {
    bool g_mkdir_hooks_installed = false;
    int (*original_mkdir)(const char*, mode_t) = nullptr;

    int Hook_mkdir(const char* pathname, mode_t mode) {
        const char* target_pkg = getenv("UNISPLICE_TARGET_PKG");
        const char* our_pkg = getenv("UNISPLICE_OUR_PKG");
        if (!target_pkg || !our_pkg || !original_mkdir) return original_mkdir(pathname, mode);

        std::string path(pathname);
        std::string prefix = "/storage/emulated/0/Android/data/";
        prefix += target_pkg;

        if (path.rfind(prefix, 0) == 0) {
            size_t pos = path.find(target_pkg, prefix.length());
            if (pos != std::string::npos) {
                std::string rewritten = path;
                rewritten.replace(pos, strlen(target_pkg), std::string(our_pkg) + "/shadow/" + target_pkg);
                LOGI("Redirected mkdir: %s -> %s", pathname, rewritten.c_str());
                return original_mkdir(rewritten.c_str(), mode);
            }
        }
        return original_mkdir(pathname, mode);
    }
}

namespace UniSplice::Hook {
    class Misc {
    public:
        static bool Initialize();
    };
}