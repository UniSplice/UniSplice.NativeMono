# UniSplice - IL2CPP Mod Loader for Android

A native C++ mod loader for Mono Unity games on Android, using ShadowHook for function hooking.

## Architecture

```
app/src/main/cpp/
├── CMakeLists.txt           # Main CMake build file
├── main.cpp                 # JNI entry point
├── UniSplice/                # Core UniSplice library
│   ├── UniSplice.h          # Main header
│   ├── core/               # Core functionality
│   │   ├── UniSplice_core.h
│   │   └── UniSplice_core.cpp
│   ├── hooks/              # Hook management
│   │   ├── hook_manager.h
│   │   ├── hook_manager.cpp
│   │   ├── init_hook.h
│   │   └── init_hook.cpp
│   └── icalls/             # Internal calls / Interop
│       ├── bootstrap_interop.h
│       └── bootstrap_interop.cpp
├── melon_loader/            # MelonLoader C# components
│   └── README.md
└── include/                 # External headers
```

## Components

### UniSplice/core/
- `UniSplice_core` - Main singleton for managing UniSplice state

### UniSplice/hooks/
- `hook_manager` - Wrapper around ShadowHook for managing hooks
- `init_hook` - Hook for `il2cpp_init` to bootstrap the loader

### UniSplice/icalls/
- `bootstrap_interop` - Internal calls exposed to C# for hooking and logging

## Dependencies

- **ShadowHook** (v1.0.10) - Android inline hook library by ByteDance
  - Implemented via Maven: `com.bytedance.android:shadowhook:1.0.10`

## Building

```bash
./gradlew assembleDebug
```

Output: `app/build/outputs/aar/app-debug.aar`

