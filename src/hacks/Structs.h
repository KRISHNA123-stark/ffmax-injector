#pragma once
#include <cmath>

struct Vector2 { float x, y; };
struct Vector3 {
    float x, y, z;
    Vector3 operator-(const Vector3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    float Length() const { return sqrtf(x*x + y*y + z*z); }
};

struct Matrix4x4 {
    float m[16];
    Vector3 MultiplyPoint3x4(const Vector3& p) const {
        Vector3 r;
        r.x = m[0]*p.x + m[4]*p.y + m[8] *p.z + m[12];
        r.y = m[1]*p.x + m[5]*p.y + m[9] *p.z + m[13];
        r.z = m[2]*p.x + m[6]*p.y + m[10]*p.z + m[14];
        return r;
    }
    bool WorldToScreen(const Vector3& world, Vector2& out, int w, int h) const {
        Vector3 c = MultiplyPoint3x4(world);
        if (c.z < 0.01f) return false;
        out.x = (w * 0.5f) + (c.x / c.z) * (w * 0.5f);
        out.y = (h * 0.5f) - (c.y / c.z) * (h * 0.5f);
        return true;
    }
};

struct Entity {
    uintptr_t base = 0;
    Vector3   position{};
    Vector3   headPos{};
    Vector2   screenPos{};
    Vector2   headScreen{};
    float     health = 0.f;
    float     distance = 0.f;
    int       team = 0;
    bool      isDead = false;
    bool      isLocal = false;
    bool      valid = false;
    bool      onScreen = false;
};
