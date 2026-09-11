#pragma once
#include "Structs.h"
#include <vector>
#include "Offsets.h"

namespace EntityUpdater {
    void Update(std::vector<Entity>& out, uintptr_t localPlayer, const Matrix4x4& view, int sw, int sh);
    uintptr_t GetLocalPlayer();
}
