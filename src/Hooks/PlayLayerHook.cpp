#include <Geode/Geode.hpp>

#ifdef GEODE_IS_MOBILE

#include <Geode/modify/PauseLayer.hpp>

#include "Utility/FloatingButton.hpp"

using namespace geode::prelude;

class $modify(RadiumPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();
        Radium::FloatingButton::attach(this);
    }
};

#endif
