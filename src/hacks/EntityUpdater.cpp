#include "EntityUpdater.h"
#include "../memory/Memory.h"
#include "../memory/Il2Cpp.h"
#include <cmath>

namespace EntityUpdater {

    static uintptr_t GetPlayerManager() {
        uintptr_t klass = Il2Cpp::ResolveClass(Offsets::PLAYERMGR_NS, Offsets::PLAYERMGR_CLASS);
        if (!klass) return 0;
        return *(uintptr_t*)(klass + Offsets::OFF_PLAYERMGR_INSTANCE);
    }

    uintptr_t GetLocalPlayer() {
        uintptr_t pm = GetPlayerManager();
        if (!pm) return 0;
        uintptr_t list = Memory::Read<uintptr_t>(pm + Offsets::OFF_PLAYER_LIST);
        if (!list) return 0;
        uintptr_t items = Memory::Read<uintptr_t>(list + Offsets::OFF_LIST_ITEMS);
        int size = Memory::Read<int>(list + Offsets::OFF_LIST_SIZE);
        for (int i = 0; i < size; i++) {
            uintptr_t p = Memory::Read<uintptr_t>(items + i*8);
            if (!p) continue;
            if (Memory::Read<bool>(p + Offsets::OFF_PLAYER_ISLOCAL)) return p;
        }
        return 0;
    }

    void Update(std::vector<Entity>& out, uintptr_t localPlayer, const Matrix4x4& view, int sw, int sh) {
        out.clear();
        uintptr_t pm = GetPlayerManager();
        if (!pm) return;
        uintptr_t list = Memory::Read<uintptr_t>(pm + Offsets::OFF_PLAYER_LIST);
        if (!list) return;
        uintptr_t items = Memory::Read<uintptr_t>(list + Offsets::OFF_LIST_ITEMS);
        int size = Memory::Read<int>(list + Offsets::OFF_LIST_SIZE);
        if (size <= 0 || size > 200) return;

        Vector3 camPos{0,0,0};
        for (int i = 0; i < size; i++) {
            uintptr_t p = Memory::Read<uintptr_t>(items + i*8);
            if (!p) continue;
            Entity e;
            e.base     = p;
            e.health   = Memory::Read<float>(p + Offsets::OFF_PLAYER_HEALTH);
            e.team     = Memory::Read<int>(p + Offsets::OFF_PLAYER_TEAM);
            e.isDead   = Memory::Read<bool>(p + Offsets::OFF_PLAYER_ISDEAD);
            e.isLocal  = (p == localPlayer);
            e.position = Memory::Read<Vector3>(p + Offsets::OFF_PLAYER_POSITION);

            uintptr_t headTr = Memory::Read<uintptr_t>(p + Offsets::OFF_PLAYER_HEAD);
            if (headTr) e.headPos = Memory::Read<Vector3>(headTr + Offsets::OFF_TRANSFORM_POS);
            else        e.headPos = {e.position.x, e.position.y+1.7f, e.position.z};

            e.distance = (e.position - camPos).Length();
            if (view.WorldToScreen(e.position, e.screenPos, sw, sh) &&
                view.WorldToScreen(e.headPos,  e.headScreen, sw, sh)) {
                e.onScreen = true;
            }
            e.valid = true;
            out.push_back(e);
        }
    }
}
