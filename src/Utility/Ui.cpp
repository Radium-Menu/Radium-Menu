#include "Ui.hpp"
#include "MainLevelBypass.hpp"
#include "Noclip.hpp"
#include "Paths.hpp"
#include "UnlockIcons.hpp"

#include "Assist/Speedhack.hpp"
#include "Physics/TpsBypass.hpp"
#include "RadiumReplay/RadiumReplay.hpp"

#include <Geode/Geode.hpp>
#include <Geode/binding/PlatformToolbox.hpp>
#include <imgui-cocos.hpp>
#include <imgui.h>

#include <algorithm>
#include <filesystem>
#include <vector>

using namespace geode::prelude;

namespace Radium::Ui {

    struct Tab {
        std::string name;
        std::function<void()> draw;
    };

    static std::vector<Tab>& tabs() {
        static std::vector<Tab> s_tabs;
        return s_tabs;
    }

    static bool s_ready = false;

#ifdef GEODE_IS_MOBILE
    static constexpr float kFontSize = 30.f;
    static constexpr float kScale = 1.6f;
#else
    static constexpr float kFontSize = 18.f;
    static constexpr float kScale = 1.f;
#endif

    static std::string toUtf8(std::filesystem::path const& path) {
        auto text = path.u8string();
        return std::string(text.begin(), text.end());
    }

    static void loadFont() {
        auto path = toUtf8(Paths::toStd(Mod::get()->getResourcesDir()) / "OpenSans-Regular.ttf");

        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->AddFontFromFileTTF(path.c_str(), kFontSize);

        if (font) {
            io.FontDefault = font;
        } else {
            log::error("Radium: failed to load font from {}", path);
        }
    }

    static void applyStyle() {
        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 8.f;
        style.FrameRounding = 5.f;
        style.GrabRounding = 5.f;
        style.TabRounding = 5.f;
        style.WindowBorderSize = 1.f;
        style.FrameBorderSize = 0.f;
        style.WindowPadding = ImVec2(14.f, 12.f);
        style.FramePadding = ImVec2(10.f, 6.f);
        style.ItemSpacing = ImVec2(10.f, 8.f);

        ImVec4* c = style.Colors;
        c[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.09f, 0.20f, 0.97f);
        c[ImGuiCol_Border] = ImVec4(0.23f, 0.55f, 1.00f, 0.45f);
        c[ImGuiCol_Separator] = ImVec4(0.23f, 0.55f, 1.00f, 0.35f);
        c[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.16f, 0.42f, 1.00f);
        c[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.21f, 0.55f, 1.00f);
        c[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.16f, 0.42f, 0.80f);
        c[ImGuiCol_FrameBg] = ImVec4(0.09f, 0.16f, 0.36f, 1.00f);
        c[ImGuiCol_FrameBgHovered] = ImVec4(0.13f, 0.25f, 0.55f, 1.00f);
        c[ImGuiCol_FrameBgActive] = ImVec4(0.17f, 0.32f, 0.68f, 1.00f);
        c[ImGuiCol_CheckMark] = ImVec4(0.49f, 0.77f, 1.00f, 1.00f);
        c[ImGuiCol_SliderGrab] = ImVec4(0.23f, 0.55f, 1.00f, 1.00f);
        c[ImGuiCol_SliderGrabActive] = ImVec4(0.49f, 0.77f, 1.00f, 1.00f);
        c[ImGuiCol_Button] = ImVec4(0.12f, 0.30f, 0.70f, 1.00f);
        c[ImGuiCol_ButtonHovered] = ImVec4(0.23f, 0.55f, 1.00f, 1.00f);
        c[ImGuiCol_ButtonActive] = ImVec4(0.18f, 0.45f, 0.90f, 1.00f);
        c[ImGuiCol_Header] = ImVec4(0.12f, 0.30f, 0.70f, 1.00f);
        c[ImGuiCol_HeaderHovered] = ImVec4(0.23f, 0.55f, 1.00f, 1.00f);
        c[ImGuiCol_HeaderActive] = ImVec4(0.18f, 0.45f, 0.90f, 1.00f);
        c[ImGuiCol_Tab] = ImVec4(0.09f, 0.21f, 0.55f, 1.00f);
        c[ImGuiCol_TabHovered] = ImVec4(0.23f, 0.55f, 1.00f, 1.00f);

        style.ScaleAllSizes(kScale);
    }

    static void render() {
#ifdef GEODE_IS_ANDROID
        geode::cocos::setMousePos(ccp(0.f, 0.f));
#endif

        ImVec2 display = ImGui::GetIO().DisplaySize;
        ImVec2 size(
            std::min(560.f * kScale, display.x * 0.92f),
            std::min(380.f * kScale, display.y * 0.90f)
        );
        ImGui::SetNextWindowPos(ImVec2(display.x * 0.04f, display.y * 0.05f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        bool open = true;
        if (ImGui::Begin("Radium", &open, ImGuiWindowFlags_NoCollapse)) {
            if (ImGui::BeginTabBar("radium-tabs")) {
                for (auto& tab : tabs()) {
                    if (ImGui::BeginTabItem(tab.name.c_str())) {
                        tab.draw();
                        ImGui::EndTabItem();
                    }
                }
                ImGui::EndTabBar();
            }
        }
        ImGui::End();

        if (!open) {
            Loader::get()->queueInMainThread([] { toggle(); });
        }
    }

    void addTab(std::string name, std::function<void()> draw) {
        tabs().push_back({ std::move(name), std::move(draw) });
    }

    void init() {
        if (s_ready) return;

        addTab("Global", [] {
            TpsBypass::renderUi();
            ImGui::Spacing();
            Speedhack::renderUi();
        });
        addTab("Player", [] { Noclip::renderUi(); });
        addTab("Unlocks", [] {
            UnlockIcons::renderUi();
            ImGui::Spacing();
            MainLevelBypass::renderUi();
        });
        addTab("Replay", [] { RadiumReplay::renderUi(); });

        ImGuiCocos::get()
            .setup([] {
                loadFont();
                applyStyle();
            })
            .draw([] { render(); });

#ifdef GEODE_IS_MOBILE
        ImGuiCocos::get().setInputMode(ImGuiCocos::InputMode::Blocking);
#endif

        s_ready = true;
    }

    bool isReady() { return s_ready; }

    bool isOpen() { return s_ready && ImGuiCocos::get().isVisible(); }

    bool isTyping() {
        return ImGui::GetCurrentContext() != nullptr && ImGui::GetIO().WantTextInput;
    }

    void toggle() {
        if (!s_ready) return;

        ImGuiCocos::get().toggle();

#ifdef GEODE_IS_DESKTOP
        if (ImGuiCocos::get().isVisible()) {
            PlatformToolbox::showCursor();
            return;
        }

        auto scene = CCDirector::get()->getRunningScene();
        bool paused = scene && scene->getChildByType<PauseLayer>(0) != nullptr;
        if (PlayLayer::get() && !paused) {
            PlatformToolbox::hideCursor();
        }
#endif
    }

}
