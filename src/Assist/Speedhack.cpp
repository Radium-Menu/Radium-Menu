#include "Speedhack.hpp"

#include <Geode/Geode.hpp>
#include <imgui.h>

using namespace geode::prelude;

namespace Radium::Speedhack {

    static bool s_enabled = false;
    static float s_speed = 1.f;

    bool isEnabled() { return s_enabled; }

    void setEnabled(bool enabled) {
        s_enabled = enabled;
        Mod::get()->setSavedValue("radium.speedhack.enabled", enabled);
    }

    float getSpeed() { return s_speed; }

    void setSpeed(float speed) {
        s_speed = clampSpeed(speed);
        Mod::get()->setSavedValue("radium.speedhack.value", s_speed);
    }

    void load() {
        s_enabled = Mod::get()->getSavedValue<bool>("radium.speedhack.enabled", false);
        s_speed = clampSpeed(Mod::get()->getSavedValue<float>("radium.speedhack.value", 1.f));
    }

    void renderUi() {
        ImGui::TextColored(ImVec4(0.49f, 0.77f, 1.f, 1.f), "Speedhack");
        ImGui::Separator();

        bool enabled = s_enabled;
        if (ImGui::Checkbox("Enabled##speed", &enabled)) {
            setEnabled(enabled);
        }

        ImGui::BeginDisabled(!s_enabled);

        float speed = s_speed;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 14.f);
        if (ImGui::SliderFloat("Speed##speed", &speed, kMinSpeed, kMaxSpeed, "%.2fx", ImGuiSliderFlags_Logarithmic)) {
            setSpeed(speed);
        }

        if (ImGui::Button("Reset to 1x##speed")) {
            setSpeed(1.f);
        }

        ImGui::EndDisabled();
    }

}
