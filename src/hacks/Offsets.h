#pragma once
#include <cstdint>

namespace Offsets {
    constexpr const char* PLAYER_NS      = "";
    constexpr const char* PLAYER_CLASS   = "Player";
    constexpr const char* CAMERA_NS      = "";
    constexpr const char* CAMERA_CLASS   = "Camera";
    constexpr const char* PLAYERMGR_NS   = "";
    constexpr const char* PLAYERMGR_CLASS= "PlayerManager";

    constexpr uintptr_t OFF_PLAYERMGR_INSTANCE = 0x000000;
    constexpr uintptr_t OFF_PLAYER_LIST        = 0x000000;
    constexpr uintptr_t OFF_LIST_ITEMS         = 0x000000;
    constexpr uintptr_t OFF_LIST_SIZE          = 0x000000;

    constexpr uintptr_t OFF_PLAYER_POSITION    = 0x000000;
    constexpr uintptr_t OFF_PLAYER_HEAD        = 0x000000;
    constexpr uintptr_t OFF_PLAYER_HEALTH      = 0x000000;
    constexpr uintptr_t OFF_PLAYER_TEAM        = 0x000000;
    constexpr uintptr_t OFF_PLAYER_ISDEAD      = 0x000000;
    constexpr uintptr_t OFF_PLAYER_ISLOCAL     = 0x000000;
    constexpr uintptr_t OFF_PLAYER_ROTATION    = 0x000000;

    constexpr uintptr_t OFF_CAMERA_VIEWMATRIX  = 0x000000;
    constexpr uintptr_t OFF_TRANSFORM_POS      = 0x000000;

    constexpr float     AIM_FOV_DEFAULT        = 90.0f;
    constexpr float     AIM_SMOOTH_DEFAULT     = 0.35f;
}
