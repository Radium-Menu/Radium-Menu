#include "Vbot.hpp"

#include <Geode/Geode.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <imgui.h>

using namespace geode::prelude;

namespace Radium::Vbot {

    bool isAvailable() {
        return Loader::get()->isModLoaded(kModId);
    }

    static void send(char const* command) {
        if (!isAvailable()) return;
        DispatchEvent<>(fmt::format("{}/{}", kModId, command)).post();
    }

    void startRecording() { send("start-recording"); }
    void stopRecording() { send("stop-recording"); }
    void startReplay() { send("start-replay"); }
    void stopReplay() { send("stop-replay"); }
    void startRender() { send("start-render"); }
    void stopRender() { send("stop-render"); }

    static void commandRow(char const* label, void (*start)(), void (*stop)()) {
        ImGui::TextUnformatted(label);
        ImGui::SameLine(ImGui::GetFontSize() * 11.f);
        ImGui::PushID(label);
        if (ImGui::Button("Start")) {
            start();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            stop();
        }
        ImGui::PopID();
    }

    void renderUi() {
        ImGui::TextColored(ImVec4(0.49f, 0.77f, 1.f, 1.f), "Vbot");
        ImGui::Separator();

        bool available = isAvailable();
        if (available) {
            ImGui::TextColored(ImVec4(0.45f, 0.90f, 0.55f, 1.f), "Vbot is loaded");
        } else {
            ImGui::TextColored(ImVec4(1.f, 0.55f, 0.45f, 1.f), "Vbot is not loaded");
        }

        ImGui::Spacing();
        ImGui::BeginDisabled(!available);
        commandRow("Macro recording", startRecording, stopRecording);
        commandRow("Macro replay", startReplay, stopReplay);
        commandRow("Renderer", startRender, stopRender);
        ImGui::EndDisabled();
    }

}
