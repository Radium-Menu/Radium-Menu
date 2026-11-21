#include "MainLevelBypass.hpp"

#include <Geode/Geode.hpp>
#include <imgui.h>

using namespace geode::prelude;

namespace Radium::MainLevelBypass {

    static bool s_enabled = false;

    bool isEnabled() { return s_enabled; }

    void setEnabled(bool enabled) {
        s_enabled = enabled;
        Mod::get()->setSavedValue("radium.main-level-bypass.enabled", enabled);
    }

    void load() {
        s_enabled = Mod::get()->getSavedValue<bool>("radium.main-level-bypass.enabled", false);
    }

    void renderUi() {
        ImGui::TextColored(ImVec4(0.49f, 0.77f, 1.f, 1.f), "Main level bypass");
        ImGui::Separator();

        bool enabled = s_enabled;
        if (ImGui::Checkbox("Play locked main levels##mainlevel", &enabled)) {
            setEnabled(enabled);
        }
    }

}
