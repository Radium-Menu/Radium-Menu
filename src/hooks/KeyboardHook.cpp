#include <Geode/Geode.hpp>

#ifdef GEODE_IS_DESKTOP

#include <Geode/modify/CCKeyboardDispatcher.hpp>

#include "../Radium/Ui.hpp"

using namespace geode::prelude;

class $modify(RadiumKeyboardDispatcher, CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool repeat, double timestamp) {
        if (down && !repeat && key == KEY_Tab && Radium::Ui::isReady() && !Radium::Ui::isTyping()) {
            Radium::Ui::toggle();
            return true;
        }
        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat, timestamp);
    }
};

#endif
