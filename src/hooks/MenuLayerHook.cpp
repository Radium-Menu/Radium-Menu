#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "../Radium/FloatingButton.hpp"

using namespace geode::prelude;

class $modify(RadiumMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }
        Radium::FloatingButton::install();
        return true;
    }
};
