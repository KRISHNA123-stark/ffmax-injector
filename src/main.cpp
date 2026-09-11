#include <jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>
#include <thread>
#include <atomic>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_android.h"

#include "hacks/Menu.h"
#include "hacks/ESP.h"
#include "hacks/Aimbot.h"
#include "memory/Memory.h"
#include "memory/Il2Cpp.h"

#define LOG_TAG "FFMaxInjector"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay, EGLSurface) = nullptr;
static bool g_imgui_ready = false;
static int g_width  = 0;
static int g_height = 0;

static EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surf) {
    if (!g_imgui_ready) {
        eglQuerySurface(dpy, surf, EGL_WIDTH,  &g_width);
        eglQuerySurface(dpy, surf, EGL_HEIGHT, &g_height);
        if (g_width > 0 && g_height > 0) {
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2((float)g_width, (float)g_height);
            ImGui::StyleColorsDark();
            ImGui_ImplOpenGL3_Init("#version 300 es");
            ImGui_ImplAndroid_Init(nullptr);
            g_imgui_ready = true;
            LOGI("ImGui initialized %dx%d", g_width, g_height);
        }
    }

    if (g_imgui_ready) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplAndroid_NewFrame(g_width, g_height);
        ImGui::NewFrame();
        Menu::Render();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    return orig_eglSwapBuffers(dpy, surf);
}

__attribute__((constructor))
static void on_load() {
    LOGI("FFMaxInjector loaded");
    Memory::Init();
    Il2Cpp::Init();

    void* handle = dlopen("libEGL.so", RTLD_NOW);
    if (!handle) { LOGE("libEGL.so not found"); return; }
    void* sym = dlsym(handle, "eglSwapBuffers");
    if (!sym)  { LOGE("eglSwapBuffers not found"); return; }

    orig_eglSwapBuffers = (EGLBoolean(*)(EGLDisplay, EGLSurface))sym;

    // NOTE: Dobby hook removed - ImGui renders through original swap
    // To add hook later, re-add Dobby library and DobbyHook call here
    LOGI("eglSwapBuffers found at %p", sym);
}
