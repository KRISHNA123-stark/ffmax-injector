#include "Menu.h"
#include "ESP.h"
#include "Aimbot.h"
#include "../memory/Memory.h"
#include "../memory/Il2Cpp.h"
#include "imgui.h"
#include <unistd.h>

namespace Menu {
    static int tab = 0;
    static bool  cfg_aim_on     = false;
    static int   cfg_aim_mode   = 2;
    static float cfg_aim_fov    = 90.f;
    static float cfg_aim_smooth = 0.35f;
    static int   cfg_aim_bone   = 0;

    void Render() {
        ImGui::SetNextWindowSize(ImVec2(560, 520), ImGuiCond_FirstUseEver);
        ImGui::Begin("FFMax | Potato Injector");

        if (ImGui::BeginTabBar("##tabs")) {
            if (ImGui::BeginTabItem("HOME"))   { tab = 0; ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("AIMBOT")) { tab = 1; ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("ESP"))    { tab = 2; ImGui::EndTabItem(); }
            if (ImGui::BeginTabItem("SETTINGS")){ tab = 3; ImGui::EndTabItem(); }
            ImGui::EndTabBar();
        }
        ImGui::Separator();

        if (tab == 0) {
            ImGui::Text("PID     : %d", Memory::g_pid);
            ImGui::Text("MEM FD  : %d", Memory::g_mem_fd);
            ImGui::Text("libbase : 0x%lx", Il2Cpp::GetBase("libil2cpp.so"));
        } else if (tab == 1) {
            ImGui::Checkbox("Aimbot Enabled", &cfg_aim_on);
            ImGui::Combo("Mode", &cfg_aim_mode, "Silent\0Snap\0Smooth\0Trigger\0");
            ImGui::SliderFloat("FOV", &cfg_aim_fov, 1.f, 360.f);
            ImGui::SliderFloat("Smoothness", &cfg_aim_smooth, 0.05f, 1.f);
            ImGui::Combo("Bone", &cfg_aim_bone, "Head\0Chest\0Nearest\0");
        } else if (tab == 2) {
            ImGui::Checkbox("ESP Enabled", &ESP::cfg.enabled);
            ImGui::Checkbox("Box",         &ESP::cfg.box);
            ImGui::Checkbox("Skeleton",    &ESP::cfg.skeleton);
            ImGui::Checkbox("Health",      &ESP::cfg.health);
            ImGui::Checkbox("Distance",    &ESP::cfg.distance);
            ImGui::Checkbox("Snapline",    &ESP::cfg.snapline);
            ImGui::SliderFloat("Max Dist", &ESP::cfg.maxDistance, 10.f, 800.f);
        } else if (tab == 3) {
            static float opacity = 1.f;
            ImGui::SliderFloat("UI Opacity", &opacity, 0.1f, 1.f);
            ImGui::GetStyle().Alpha = opacity;
            if (ImGui::Button("Exit")) _exit(0);
        }
        ImGui::End();
    }
}
