#pragma once
#include "Structs.h"
#include <vector>

namespace ESP {
    struct Config {
        bool enabled = true;
        bool box = true;
        bool skeleton = true;
        bool health = true;
        bool name = true;
        bool distance = true;
        bool snapline = false;
        float maxDistance = 300.f;
        unsigned int colorEnemy = 0xFFFF3030;
        unsigned int colorTeam  = 0xFF30FF30;
        unsigned int colorBox   = 0xFFFFFFFF;
    };
    extern Config cfg;
    void Render(const std::vector<Entity>& entities, int screenW, int screenH);
}
