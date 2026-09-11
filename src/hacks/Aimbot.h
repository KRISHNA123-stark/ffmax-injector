#pragma once
#include "Structs.h"
#include <vector>

namespace Aimbot {
    enum Mode { SILENT=0, SNAP=1, SMOOTH=2, TRIGGER=3 };
    struct Config {
        bool  enabled = false;
        int   mode = SMOOTH;
        int   bone = 0;
        float fov = 90.f;
        float smoothness = 0.35f;
        bool  visibleOnly = true;
        bool  ignoreTeam = true;
    };
    extern Config cfg;
    Entity* GetBestTarget(std::vector<Entity>& list, const Vector2& screenCenter);
    void    Apply(uintptr_t localPlayer, const Entity& target);
}
