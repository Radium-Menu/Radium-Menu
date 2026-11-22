#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>

#include "Assist/Speedhack.hpp"

using namespace geode::prelude;

class $modify(RadiumSpeedScheduler, CCScheduler) {
    void update(float dt) {
        if (Radium::Speedhack::isEnabled()) {
            dt *= Radium::Speedhack::getSpeed();
        }
        CCScheduler::update(dt);
    }
};
