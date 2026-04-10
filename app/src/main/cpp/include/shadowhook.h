#ifndef SHADOWHOOK_H
#define SHADOWHOOK_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SHADOWHOOK_MODE_UNIQUE = 0,
    SHADOWHOOK_MODE_SHARED = 1,
    SHADOWHOOK_MODE_AUTO = 2,
} shadowhook_mode_t;

const char* shadowhook_get_version(void);

int shadowhook_init(shadowhook_mode_t default_mode, bool debuggable);
int shadowhook_get_init_errno(void);

void* shadowhook_hook_func_addr(void* func_addr, void* new_addr, void** orig_addr);
void* shadowhook_hook_sym_addr(void* sym_addr, void* new_addr, void** orig_addr);
void* shadowhook_hook_sym_name(const char* lib_name, const char* sym_name, void* new_addr, void** orig_addr);

int shadowhook_unhook(void* ctx);

void* shadowhook_dlopen(const char* lib_name);
void shadowhook_dlclose(void* handle);
void* shadowhook_dlsym(void* handle, const char* sym_name);

#ifdef __cplusplus
}
#endif

#endif // SHADOWHOOK_H
