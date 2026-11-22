#include <Geode/Geode.hpp>

#ifdef GEODE_IS_MOBILE

#include <Geode/modify/CCDirector.hpp>

#include "Utility/FloatingButton.hpp"

using namespace geode::prelude;

class $modify(RadiumDirector, CCDirector) {
    void willSwitchToScene(CCScene* scene) {
        CCDirector::willSwitchToScene(scene);
        Radium::FloatingButton::attach(scene);
    }
};

#endif
