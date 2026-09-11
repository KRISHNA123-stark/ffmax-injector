#include "Aimbot.h"
#include "Offsets.h"
#include "../memory/Memory.h"
#include <cmath>
#include <limits>

namespace Aimbot {
    Config cfg;

    static float Dist2D(const Vector2& a, const Vector2& b) {
        float dx = a.x-b.x, dy = a.y-b.y;
        return sqrtf(dx*dx + dy*dy);
    }

    Entity* GetBestTarget(std::vector<Entity>& list, const Vector2& screenCenter) {
        Entity* best = nullptr;
        float bestScore = std::numeric_limits<float>::max();
        for (auto& e : list) {
            if (!e.valid || e.isLocal || e.isDead) continue;
            if (cfg.ignoreTeam && e.team == 0) continue;
            if (!e.onScreen) continue;
            float d = Dist2D(e.screenPos, screenCenter);
            if (d > cfg.fov) continue;
            if (d < bestScore) { bestScore = d; best = &e; }
        }
        return best;
    }

    void Apply(uintptr_t localPlayer, const Entity& target) {
        if (!localPlayer || !target.base) return;
        Vector3 aim = (cfg.bone == 0) ? target.headPos : target.position;
        if (cfg.mode == SILENT || cfg.mode == SNAP) {
            Memory::Write<Vector3>(localPlayer + Offsets::OFF_PLAYER_ROTATION, aim);
        } else if (cfg.mode == SMOOTH) {
            Vector3 cur = Memory::Read<Vector3>(localPlayer + Offsets::OFF_PLAYER_ROTATION);
            Vector3 next{
                cur.x + (aim.x - cur.x) * cfg.smoothness,
                cur.y + (aim.y - cur.y) * cfg.smoothness,
                cur.z + (aim.z - cur.z) * cfg.smoothness
            };
            Memory::Write<Vector3>(localPlayer + Offsets::OFF_PLAYER_ROTATION, next);
        }
    }
}
