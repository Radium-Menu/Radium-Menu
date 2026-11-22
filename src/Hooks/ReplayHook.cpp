#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "RadiumReplay/RadiumReplay.hpp"

using namespace geode::prelude;

class $modify(RadiumReplayGameLayer, GJBaseGameLayer) {
    void processCommands(float dt, bool isHalfTick, bool isLastTick) {
        Radium::RadiumReplay::beginStep(this);
        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
        Radium::RadiumReplay::endStep(this);
    }

    void handleButton(bool down, int button, bool isPlayer1) {
        if (Radium::RadiumReplay::interceptButton(down, button, isPlayer1)) {
            return;
        }
        GJBaseGameLayer::handleButton(down, button, isPlayer1);
    }
};

class $modify(RadiumReplayPlayLayer, PlayLayer) {
    void resetLevel() {
        PlayLayer::resetLevel();
        Radium::RadiumReplay::onLevelReset();
    }

    void levelComplete() {
        Radium::RadiumReplay::onLevelComplete();
        PlayLayer::levelComplete();
    }

    void onQuit() {
        Radium::RadiumReplay::onLevelQuit();
        PlayLayer::onQuit();
    }
};
