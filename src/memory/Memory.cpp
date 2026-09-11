#include "Memory.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <android/log.h>

namespace Memory {
    int g_mem_fd = -1;
    int g_pid    = -1;

    static int FindPid(const char* pkg) {
        DIR* d = opendir("/proc");
        if (!d) return -1;
        dirent* e;
        char path[64], cmd[256];
        while ((e = readdir(d))) {
            int id = atoi(e->d_name);
            if (id <= 0) continue;
            snprintf(path, sizeof(path), "/proc/%d/cmdline", id);
            int fd = open(path, O_RDONLY);
            if (fd < 0) continue;
            ssize_t n = read(fd, cmd, sizeof(cmd)-1);
            close(fd);
            if (n <= 0) continue;
            cmd[n] = 0;
            if (strcmp(cmd, pkg) == 0) { closedir(d); return id; }
        }
        closedir(d);
        return -1;
    }

    void Init() {
        g_pid = FindPid("com.dts.freefiremax");
        if (g_pid <= 0) g_pid = getpid();
        char p[64];
        snprintf(p, sizeof(p), "/proc/%d/mem", g_pid);
        g_mem_fd = open(p, O_RDWR);
        __android_log_print(ANDROID_LOG_INFO,"Memory","pid=%d fd=%d",g_pid,g_mem_fd);
    }

    bool IsValid(uintptr_t addr) {
        return addr > 0x1000 && addr < 0x7fffffffffffULL && g_mem_fd >= 0;
    }

    void ReadBytes(uintptr_t addr, void* buf, size_t len) {
        if (!IsValid(addr)) { memset(buf,0,len); return; }
        pread64(g_mem_fd, buf, len, addr);
    }

    bool WriteBytes(uintptr_t addr, const void* buf, size_t len) {
        if (!IsValid(addr)) return false;
        return pwrite64(g_mem_fd, buf, len, addr) == (ssize_t)len;
    }
}
