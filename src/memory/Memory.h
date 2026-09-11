#pragma once
#include <cstdint>
#include <initializer_list>
#include <sys/types.h>

namespace Memory {
    void Init();
    bool IsValid(uintptr_t addr);

    template<typename T>
    T Read(uintptr_t addr) {
        T val{};
        if (!IsValid(addr)) return val;
        pread64(g_mem_fd, &val, sizeof(T), addr);
        return val;
    }

    template<typename T>
    bool Write(uintptr_t addr, T val) {
        if (!IsValid(addr)) return false;
        return pwrite64(g_mem_fd, &val, sizeof(T), addr) == (ssize_t)sizeof(T);
    }

    template<typename T>
    T ReadChain(uintptr_t base, std::initializer_list<uintptr_t> offsets) {
        uintptr_t addr = base;
        for (auto off : offsets) {
            addr = Read<uintptr_t>(addr + off);
            if (!IsValid(addr)) return T{};
        }
        return Read<T>(addr);
    }

    void ReadBytes(uintptr_t addr, void* buf, size_t len);
    bool WriteBytes(uintptr_t addr, const void* buf, size_t len);

    extern int g_mem_fd;
    extern int g_pid;
}
