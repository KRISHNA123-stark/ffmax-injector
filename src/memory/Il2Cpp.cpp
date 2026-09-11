#include "Il2Cpp.h"
#include "Memory.h"
#include <dlfcn.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <android/log.h>

namespace Il2Cpp {

    typedef void* (*fn_domain_get)();
    typedef void* (*fn_asm_open)(void*, const char*);
    typedef void* (*fn_asm_image)(void*);
    typedef void* (*fn_class_from_name)(void*, const char*, const char*);
    typedef void* (*fn_method_from_name)(void*, const char*, int);
    typedef void* (*fn_field_from_name)(void*, const char*);

    static fn_domain_get        p_domain_get;
    static fn_asm_open          p_asm_open;
    static fn_asm_image         p_asm_image;
    static fn_class_from_name   p_class_from_name;
    static fn_method_from_name  p_method_from_name;
    static fn_field_from_name   p_field_from_name;
    static void* g_image = nullptr;

    void Init() {
        void* h = dlopen("libil2cpp.so", RTLD_NOW);
        if (!h) { __android_log_print(ANDROID_LOG_ERROR,"Il2Cpp","libil2cpp.so not found"); return; }

        p_domain_get       = (fn_domain_get)       dlsym(h, "il2cpp_domain_get");
        p_asm_open         = (fn_asm_open)         dlsym(h, "il2cpp_domain_assembly_open");
        p_asm_image        = (fn_asm_image)        dlsym(h, "il2cpp_assembly_get_image");
        p_class_from_name  = (fn_class_from_name)  dlsym(h, "il2cpp_class_from_name");
        p_method_from_name = (fn_method_from_name) dlsym(h, "il2cpp_class_get_method_from_name");
        p_field_from_name  = (fn_field_from_name)  dlsym(h, "il2cpp_class_get_field_from_name");

        if (!p_domain_get || !p_asm_open || !p_asm_image) return;

        void* domain = p_domain_get();
        void* asm_   = p_asm_open(domain, "Assembly-CSharp.dll");
        if (!asm_) asm_ = p_asm_open(domain, "Assembly-CSharp");
        if (asm_) g_image = p_asm_image(asm_);
        __android_log_print(ANDROID_LOG_INFO,"Il2Cpp","domain=%p image=%p",domain,g_image);
    }

    uintptr_t GetBase(const char* lib) {
        FILE* f = fopen("/proc/self/maps","r");
        if (!f) return 0;
        char line[512];
        uintptr_t base = 0;
        while (fgets(line,sizeof(line),f)) {
            if (strstr(line, lib) && strstr(line, "r-xp")) {
                base = strtoull(line, nullptr, 16);
                break;
            }
        }
        fclose(f);
        return base;
    }

    uintptr_t ResolveClass(const char* ns, const char* name) {
        if (!g_image || !p_class_from_name) return 0;
        return (uintptr_t)p_class_from_name(g_image, ns, name);
    }
    uintptr_t ResolveMethod(uintptr_t klass, const char* name, int argc) {
        if (!klass || !p_method_from_name) return 0;
        return (uintptr_t)p_method_from_name((void*)klass, name, argc);
    }
    uintptr_t ResolveField(uintptr_t klass, const char* name) {
        if (!klass || !p_field_from_name) return 0;
        return (uintptr_t)p_field_from_name((void*)klass, name);
    }
}
