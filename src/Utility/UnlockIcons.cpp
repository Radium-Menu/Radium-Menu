#include "UnlockIcons.hpp"

#include <Geode/Geode.hpp>
#include <imgui.h>

using namespace geode::prelude;

namespace Radium::UnlockIcons {

    static bool s_enabled = false;

    bool isEnabled() { return s_enabled; }

    void setEnabled(bool enabled) {
        s_enabled = enabled;
        Mod::get()->setSavedValue("radium.unlock-icons.enabled", enabled);
    }

    void load() {
        s_enabled = Mod::get()->getSavedValue<bool>("radium.unlock-icons.enabled", false);
    }

    void renderUi() {
        ImGui::TextColored(ImVec4(0.49f, 0.77f, 1.f, 1.f), "Unlock icons");
        ImGui::Separator();

        bool enabled = s_enabled;
        if (ImGui::Checkbox("Unlock all icons and colors##unlock", &enabled)) {
            setEnabled(enabled);
        }
    }

}
