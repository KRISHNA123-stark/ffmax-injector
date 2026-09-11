#pragma once
#include <cstdint>

namespace Il2Cpp {
    void Init();
    uintptr_t GetBase(const char* lib);
    uintptr_t ResolveClass(const char* ns, const char* name);
    uintptr_t ResolveMethod(uintptr_t klass, const char* name, int argc);
    uintptr_t ResolveField(uintptr_t klass, const char* name);
}
