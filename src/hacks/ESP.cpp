#include "ESP.h"
#include "imgui.h"
#include <cstdio>

namespace ESP {
    Config cfg;

    static void DrawBox2D(ImDrawList* dl, const ImVec2& tl, const ImVec2& br, ImU32 col, float th=1.5f) {
        dl->AddRect(tl, br, col, 0.f, 0, th);
    }
    static void DrawHealthBar(ImDrawList* dl, const ImVec2& tl, const ImVec2& br, float hp, float maxhp) {
        float h = br.y - tl.y;
        float pct = hp / maxhp;
        if (pct < 0.f) pct = 0.f; if (pct > 1.f) pct = 1.f;
        ImVec2 barTL(tl.x - 6.f, tl.y);
        ImVec2 barBR(tl.x - 2.f, br.y);
        dl->AddRectFilled(barTL, barBR, IM_COL32(0,0,0,180));
        ImVec2 fillTL(barTL.x, barBR.y - h * pct);
        ImU32 col = pct > 0.6f ? IM_COL32(0,255,0,255) :
                    pct > 0.3f ? IM_COL32(255,255,0,255) :
                                 IM_COL32(255,0,0,255);
        dl->AddRectFilled(fillTL, barBR, col);
    }
    static void DrawSkeleton(ImDrawList* dl, const Vector2& h, const Vector2& c, const Vector2& p, ImU32 col) {
        dl->AddLine(ImVec2(h.x,h.y), ImVec2(c.x,c.y), col, 1.5f);
        dl->AddLine(ImVec2(c.x,c.y), ImVec2(p.x,p.y), col, 1.5f);
    }

    void Render(const std::vector<Entity>& entities, int screenW, int screenH) {
        if (!cfg.enabled) return;
        ImDrawList* dl = ImGui::GetBackgroundDrawList();
        for (const auto& e : entities) {
            if (!e.valid || e.isLocal || e.isDead) continue;
            if (e.distance > cfg.maxDistance) continue;
            if (!e.onScreen) continue;
            ImU32 col = (e.team == 0) ? cfg.colorEnemy : cfg.colorTeam;
            float boxH = 120.f / (e.distance * 0.02f + 1.f);
            float boxW = boxH * 0.5f;
            if (boxH < 15.f) boxH = 15.f;
            if (boxH > 400.f) boxH = 400.f;
            ImVec2 tl(e.screenPos.x - boxW*0.5f, e.screenPos.y - boxH);
            ImVec2 br(e.screenPos.x + boxW*0.5f, e.screenPos.y);
            if (cfg.box) DrawBox2D(dl, tl, br, cfg.colorBox);
            if (cfg.health) DrawHealthBar(dl, tl, br, e.health, 200.f);
            if (cfg.skeleton) DrawSkeleton(dl, e.headScreen, e.screenPos, {e.screenPos.x, e.screenPos.y+boxH*0.3f}, col);
            if (cfg.snapline) dl->AddLine(ImVec2(screenW*0.5f, (float)screenH), ImVec2(e.screenPos.x, e.screenPos.y), col, 1.f);
            if (cfg.distance) {
                char buf[32]; snprintf(buf, sizeof(buf), "%.0fm", e.distance);
                dl->AddText(ImVec2(tl.x, br.y + 2.f), col, buf);
            }
        }
    }
}
