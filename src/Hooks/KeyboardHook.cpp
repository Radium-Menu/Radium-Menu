#include <Geode/Geode.hpp>

#ifdef GEODE_IS_DESKTOP

#include <Geode/utils/Keyboard.hpp>

#include "Utility/Ui.hpp"

using namespace geode::prelude;

$execute {
    KeyboardInputEvent().listen([](KeyboardInputData& data) -> bool {
        if (data.key != KEY_Tab || data.action != KeyboardInputData::Action::Press) return false;
        if (!Radium::Ui::isReady() || Radium::Ui::isTyping()) return false;

        Radium::Ui::toggle();
        return true;
    }, Priority::Early).leak();
}

#endif
