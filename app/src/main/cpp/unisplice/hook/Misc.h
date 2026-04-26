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
    int (*original_mkdirs)(const char*, mode_t) = nullptr;

    int Hook_mkdir(const char* pathname, mode_t mode) {
        LOGI("MKDIR called: %s", pathname);
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

    int Hook_mkdirs(const char* pathname, mode_t mode) {
        LOGI("MKDIRS called: %s", pathname);
        const char* target_pkg = getenv("UNISPLICE_TARGET_PKG");
        const char* our_pkg = getenv("UNISPLICE_OUR_PKG");
        if (!target_pkg || !our_pkg || !original_mkdirs) return original_mkdirs(pathname, mode);

        std::string path(pathname);
        std::string prefix = "/storage/emulated/0/Android/data/";
        prefix += target_pkg;

        if (path.rfind(prefix, 0) == 0) {
            size_t pos = path.find(target_pkg, prefix.length());
            if (pos != std::string::npos) {
                std::string rewritten = path;
                rewritten.replace(pos, strlen(target_pkg), std::string(our_pkg) + "/shadow/" + target_pkg);
                LOGI("Redirected mkdirs: %s -> %s", pathname, rewritten.c_str());
                return original_mkdirs(rewritten.c_str(), mode);
            }
        }
        return original_mkdirs(pathname, mode);
    }

    int Hook_mkdirat(int dirfd, const char* pathname, mode_t mode) {
        LOGI("MKDIRAT called: %s", pathname);
        const char* target_pkg = getenv("UNISPLICE_TARGET_PKG");
        const char* our_pkg = getenv("UNISPLICE_OUR_PKG");
        if (!target_pkg || !our_pkg) return -1;

        std::string path(pathname);
        std::string prefix = "/storage/emulated/0/Android/data/";
        prefix += target_pkg;

        if (path.rfind(prefix, 0) == 0) {
            size_t pos = path.find(target_pkg, prefix.length());
            if (pos != std::string::npos) {
                std::string rewritten = path;
                rewritten.replace(pos, strlen(target_pkg), std::string(our_pkg) + "/shadow/" + target_pkg);
                LOGI("Redirected mkdirat: %s -> %s", pathname, rewritten.c_str());
                // For mkdirat, we need to get the actual function
                // This is tricky - we'd need to call the original via address
            }
        }
        return -1; // Let it fail or try original
    }
}

namespace UniSplice::Hook {
    class Misc {
    public:
        static bool Initialize();
    };
}