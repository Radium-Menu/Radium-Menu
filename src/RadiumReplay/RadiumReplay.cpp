#include "RadiumReplay.hpp"

#include <Geode/Geode.hpp>
#include <imgui.h>

#include <algorithm>
#include <cctype>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <utility>

#include "Physics/TpsBypass.hpp"
#include "Utility/Paths.hpp"

using namespace geode::prelude;

namespace Radium::RadiumReplay {

    namespace {

        Mode s_mode = Mode::Idle;
        std::uint32_t s_frame = 0;
        std::size_t s_index = 0;
        bool s_injecting = false;
        Macro s_macro;
        std::string s_status = "Idle";
        std::vector<std::string> s_files;
        bool s_filesLoaded = false;

        std::filesystem::path replayDirectory() {
            return Paths::toStd(Mod::get()->getSaveDir()) / "replays";
        }

        std::filesystem::path replayPath(std::string const& name) {
            return replayDirectory() / (name + kExtension);
        }

        bool isPlayLayer(GJBaseGameLayer* layer) {
            return layer != nullptr && layer == static_cast<GJBaseGameLayer*>(PlayLayer::get());
        }

        float currentTps() {
            return TpsBypass::isEnabled() ? TpsBypass::getTps() : TpsBypass::kVanillaTps;
        }

        std::string makeFileName(std::string const& levelName) {
            std::string safe;
            for (char c : levelName) {
                safe.push_back(std::isalnum(static_cast<unsigned char>(c)) ? c : '_');
            }
            if (safe.empty()) {
                safe = "level";
            }

            char stamp[32] = "0";
            std::time_t now = std::time(nullptr);
            if (auto* local = std::localtime(&now)) {
                std::strftime(stamp, sizeof(stamp), "%Y%m%d-%H%M%S", local);
            }
            return safe + "-" + stamp;
        }

        void refreshFiles() {
            s_files.clear();

            std::error_code error;
            for (auto const& entry : std::filesystem::directory_iterator(replayDirectory(), error)) {
                if (entry.is_regular_file() && entry.path().extension() == kExtension) {
                    s_files.push_back(entry.path().stem().string());
                }
            }

            std::sort(s_files.begin(), s_files.end());
            s_filesLoaded = true;
        }

        bool saveMacro(Macro const& macro, std::string& name) {
            std::error_code error;
            std::filesystem::create_directories(replayDirectory(), error);

            name = makeFileName(macro.levelName);
            auto bytes = serialize(macro);

            std::ofstream file(replayPath(name), std::ios::binary);
            if (!file) {
                return false;
            }
            file.write(reinterpret_cast<char const*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
            return static_cast<bool>(file);
        }

        std::optional<Macro> loadMacro(std::string const& name, std::string& error) {
            std::ifstream file(replayPath(name), std::ios::binary);
            if (!file) {
                error = "Could not open the file";
                return std::nullopt;
            }

            std::vector<std::uint8_t> bytes(
                (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
            );
            return parse(bytes, &error);
        }

        void restartLevel() {
            Loader::get()->queueInMainThread([] {
                if (auto layer = PlayLayer::get()) {
                    layer->resetLevel();
                }
            });
        }

        void finishRecording() {
            s_macro.frames = s_frame;
            s_mode = Mode::Idle;

            if (s_macro.actions.empty()) {
                s_status = "Nothing was recorded";
                return;
            }

            std::string name;
            if (saveMacro(s_macro, name)) {
                s_status = "Saved " + name + kExtension;
                refreshFiles();
            } else {
                s_status = "Could not save the replay";
            }
        }

        void startRecording() {
            auto layer = PlayLayer::get();
            if (!layer) {
                s_status = "Enter a level first";
                return;
            }

            s_macro = Macro{};
            s_macro.tps = currentTps();
            s_macro.levelName = std::string(layer->m_level->m_levelName);

            s_frame = 0;
            s_index = 0;
            s_mode = Mode::Recording;
            s_status = "Recording";
            restartLevel();
        }

        void startPlayback(std::string const& name) {
            if (!PlayLayer::get()) {
                s_status = "Enter a level first";
                return;
            }

            std::string error;
            auto macro = loadMacro(name, error);
            if (!macro) {
                s_status = "Could not load: " + error;
                return;
            }

            if (macro->tps < TpsBypass::kMinTps || macro->tps > TpsBypass::kMaxTps) {
                s_status = "This replay's TPS is outside the supported range";
                return;
            }

            s_macro = std::move(*macro);
            TpsBypass::setTps(s_macro.tps);
            TpsBypass::setEnabled(s_macro.tps != TpsBypass::kVanillaTps);

            s_frame = 0;
            s_index = 0;
            s_mode = Mode::Playing;
            s_status = "Playing " + name;
            restartLevel();
        }

        void stop() {
            if (s_mode == Mode::Recording) {
                finishRecording();
                return;
            }
            s_mode = Mode::Idle;
            s_status = "Stopped";
        }

        void deleteReplay(std::string const& name) {
            std::error_code error;
            std::filesystem::remove(replayPath(name), error);
            refreshFiles();
        }

    }

    Mode mode() { return s_mode; }

    std::uint32_t currentFrame() { return s_frame; }

    void beginStep(GJBaseGameLayer* layer) {
        if (s_mode != Mode::Playing || !isPlayLayer(layer)) return;

        s_injecting = true;
        while (s_index < s_macro.actions.size() && s_macro.actions[s_index].frame <= s_frame) {
            auto const& action = s_macro.actions[s_index++];
            layer->handleButton(action.down, action.button, !action.player2);
        }
        s_injecting = false;
    }

    void endStep(GJBaseGameLayer* layer) {
        if (s_mode == Mode::Idle || !isPlayLayer(layer)) return;
        ++s_frame;
    }

    bool interceptButton(bool down, int button, bool isPlayer1) {
        if (s_injecting || s_mode == Mode::Idle || !PlayLayer::get()) {
            return false;
        }

        if (s_mode == Mode::Playing) {
            return true;
        }

        Action action;
        action.frame = s_frame;
        action.button = static_cast<std::uint8_t>(button);
        action.down = down;
        action.player2 = !isPlayer1;
        s_macro.actions.push_back(action);
        return false;
    }

    void onLevelReset() {
        s_frame = 0;
        s_index = 0;
        if (s_mode == Mode::Recording) {
            s_macro.actions.clear();
        }
    }

    void onLevelComplete() {
        if (s_mode == Mode::Recording) {
            finishRecording();
        } else if (s_mode == Mode::Playing) {
            s_mode = Mode::Idle;
            s_status = "Finished";
        }
    }

    void onLevelQuit() {
        if (s_mode == Mode::Recording) {
            s_mode = Mode::Idle;
            s_status = "Recording discarded";
        } else if (s_mode == Mode::Playing) {
            s_mode = Mode::Idle;
            s_status = "Stopped";
        }
    }

    void renderUi() {
        ImGui::TextColored(ImVec4(0.49f, 0.77f, 1.f, 1.f), "Radium Replay");
        ImGui::Separator();

        bool inLevel = PlayLayer::get() != nullptr;
        ImGui::Text("Status: %s", s_status.c_str());

        if (s_mode != Mode::Idle) {
            ImGui::Text("Frame: %u", s_frame);
        }
        if (s_mode == Mode::Recording) {
            ImGui::Text("Inputs recorded: %zu", s_macro.actions.size());
        }

        ImGui::Spacing();

        ImGui::BeginDisabled(!inLevel || s_mode != Mode::Idle);
        if (ImGui::Button("Record")) {
            startRecording();
        }
        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(s_mode == Mode::Idle);
        if (ImGui::Button("Stop")) {
            stop();
        }
        ImGui::EndDisabled();

        if (!inLevel) {
            ImGui::TextDisabled("Open a level to record or play a replay");
        }

        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.49f, 0.77f, 1.f, 1.f), "Saved replays");
        ImGui::Separator();

        if (!s_filesLoaded) {
            refreshFiles();
        }

        if (ImGui::Button("Refresh")) {
            refreshFiles();
        }

        if (s_files.empty()) {
            ImGui::TextDisabled("No replays yet");
        }

        std::string toDelete;
        std::string toPlay;

        for (auto const& name : s_files) {
            ImGui::PushID(name.c_str());

            ImGui::BeginDisabled(!inLevel || s_mode != Mode::Idle);
            if (ImGui::Button("Play")) {
                toPlay = name;
            }
            ImGui::EndDisabled();

            ImGui::SameLine();

            ImGui::BeginDisabled(s_mode != Mode::Idle);
            if (ImGui::Button("Delete")) {
                toDelete = name;
            }
            ImGui::EndDisabled();

            ImGui::SameLine();
            ImGui::TextUnformatted(name.c_str());

            ImGui::PopID();
        }

        if (!toPlay.empty()) {
            startPlayback(toPlay);
        }
        if (!toDelete.empty()) {
            deleteReplay(toDelete);
        }
    }

}
