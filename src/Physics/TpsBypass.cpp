#include "TpsBypass.hpp"

#include <Geode/Geode.hpp>
#include <imgui.h>

using namespace geode::prelude;

namespace Radium::TpsBypass {

    static bool s_enabled = false;
    static float s_tps = kVanillaTps;
    static bool s_realTime = false;

    bool isEnabled() { return s_enabled; }

    void setEnabled(bool enabled) {
        s_enabled = enabled;
        Mod::get()->setSavedValue("radium.tps.enabled", enabled);
    }

    bool isRealTime() { return s_realTime; }

    void setRealTime(bool realTime) {
        s_realTime = realTime;
        Mod::get()->setSavedValue("radium.tps.realtime", realTime);
    }

    float getTps() { return s_tps; }

    void setTps(float tps) {
        s_tps = std::clamp(tps, kMinTps, kMaxTps);
        Mod::get()->setSavedValue("radium.tps.value", s_tps);
    }

    void load() {
        s_realTime = Mod::get()->getSavedValue<bool>("radium.tps.realtime", false);
        s_enabled = Mod::get()->getSavedValue<bool>("radium.tps.enabled", false);
        s_tps = std::clamp(
            Mod::get()->getSavedValue<float>("radium.tps.value", kVanillaTps), kMinTps, kMaxTps
        );
    }

    void renderUi() {
        ImGui::TextColored(ImVec4(0.49f, 0.77f, 1.f, 1.f), "TPS bypass");
        ImGui::Separator();

        bool enabled = s_enabled;
        if (ImGui::Checkbox("Enabled##tps", &enabled)) {
            setEnabled(enabled);
        }

        ImGui::BeginDisabled(!s_enabled);

        float tps = s_tps;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 14.f);
        if (ImGui::SliderFloat("TPS##tps-slider", &tps, kMinTps, kMaxTps, "%.0f", ImGuiSliderFlags_Logarithmic)) {
            setTps(tps);
        }

        tps = s_tps;
        ImGui::SetNextItemWidth(ImGui::GetFontSize() * 9.f);
        if (ImGui::InputFloat("Exact##tps-input", &tps, 1.f, 60.f, "%.0f")) {
            setTps(tps);
        }

        ImGui::TextUnformatted("Presets");
        for (float preset : { 240.f, 480.f, 1000.f, 4000.f, 10000.f }) {
            ImGui::SameLine();
            ImGui::PushID(static_cast<int>(preset));
            if (ImGui::Button(fmt::format("{}", static_cast<int>(preset)).c_str())) {
                setTps(preset);
            }
            ImGui::PopID();
        }

        bool realTime = s_realTime;
        if (ImGui::Checkbox("Real Time##tps-realtime", &realTime)) {
            setRealTime(realTime);
        }

        ImGui::PushTextWrapPos(0.f);
        ImGui::TextDisabled(
            "On: the game keeps its normal speed and lowers the TPS if your device cannot keep up. "
            "Off: every tick is computed and the game slows down instead. Replays always use Off."
        );
        ImGui::PopTextWrapPos();

        ImGui::EndDisabled();
    }

}
