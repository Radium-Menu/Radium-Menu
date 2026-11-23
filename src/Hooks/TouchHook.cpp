#include <Geode/Geode.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>

#include <imgui-cocos.hpp>
#include <imgui.h>

#include "Utility/Ui.hpp"

using namespace geode::prelude;

#ifdef GEODE_IS_MOBILE

class $modify(RadiumTouchDispatcher, CCTouchDispatcher) {
    void touches(CCSet* touches, CCEvent* event, unsigned int type) {
        if (!touches || !Radium::Ui::isOpen()) {
            return CCTouchDispatcher::touches(touches, event, type);
        }

        auto* touch = static_cast<CCTouch*>(touches->anyObject());
        if (!touch) {
            return CCTouchDispatcher::touches(touches, event, type);
        }

        ImGuiIO& io = ImGui::GetIO();
        ImVec2 pos = ImGuiCocos::cocosToFrame(touch->getLocation());

        io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
        io.AddMousePosEvent(pos.x, pos.y);

        if (type == CCTOUCHBEGAN) {
            io.AddMouseButtonEvent(0, true);
            log::info("Radium touch began at {}, {} (display {}x{}, capture {})",
                pos.x, pos.y, io.DisplaySize.x, io.DisplaySize.y, io.WantCaptureMouse);
        } else if (type == CCTOUCHENDED || type == CCTOUCHCANCELLED) {
            io.AddMouseButtonEvent(0, false);
            log::info("Radium touch ended at {}, {}", pos.x, pos.y);
        }
    }
};

#endif
