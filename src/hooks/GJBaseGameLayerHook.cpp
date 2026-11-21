#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include "../Radium/TpsBypass.hpp"

using namespace geode::prelude;

class $modify(RadiumTpsGameLayer, GJBaseGameLayer) {
    float getModifiedDelta(float dt) {
        if (!Radium::TpsBypass::isEnabled()) {
            return GJBaseGameLayer::getModifiedDelta(dt);
        }

        auto result = Radium::TpsBypass::computeStep(
            dt,
            m_extraDelta,
            m_gameState.m_timeWarp,
            Radium::TpsBypass::getTps()
        );

        m_extraDelta = static_cast<float>(result.extra);
        return static_cast<float>(result.delta);
    }
};
