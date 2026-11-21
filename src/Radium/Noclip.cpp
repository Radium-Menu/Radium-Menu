#include "Noclip.hpp"

#include <Geode/Geode.hpp>
#include <imgui.h>

using namespace geode::prelude;

namespace Radium::Noclip {

    static bool s_enabled = false;
    static int s_blocked = 0;

    bool isEnabled() { return s_enabled; }

    void setEnabled(bool enabled) {
        s_enabled = enabled;
        Mod::get()->setSavedValue("radium.noclip.enabled", enabled);
    }

    int blockedDeaths() { return s_blocked; }
    void countBlockedDeath() { ++s_blocked; }
    void resetBlockedDeaths() { s_blocked = 0; }

    void load() {
        s_enabled = Mod::get()->getSavedValue<bool>("radium.noclip.enabled", false);
    }

    void renderUi() {
        ImGui::TextColored(ImVec4(0.49f, 0.77f, 1.f, 1.f), "Noclip");
        ImGui::Separator();

        bool enabled = s_enabled;
        if (ImGui::Checkbox("Enabled##noclip", &enabled)) {
            setEnabled(enabled);
        }

        ImGui::Text("Deaths blocked: %d", s_blocked);
        ImGui::SameLine();
        if (ImGui::Button("Reset##noclip")) {
            resetBlockedDeaths();
        }
    }

}
