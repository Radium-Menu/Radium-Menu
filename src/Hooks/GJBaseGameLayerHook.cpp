#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include "Physics/TpsBypass.hpp"

using namespace geode::prelude;

class $modify(RadiumTpsGameLayer, GJBaseGameLayer) {
    struct Fields {
        double overflow = 0.0;
        double tick = 0.0;
    };

    double getModifiedDelta(float dt) {
        if (m_fields->tick > 0.0) {
            return m_fields->tick;
        }
        return GJBaseGameLayer::getModifiedDelta(dt);
    }

    void update(float dt) {
        if (!Radium::TpsBypass::isEnabled() || !typeinfo_cast<PlayLayer*>(this)) {
            m_fields->overflow = 0.0;
            GJBaseGameLayer::update(dt);
            return;
        }

        double tick = Radium::TpsBypass::tickLength(
            Radium::TpsBypass::getTps(),
            m_gameState.m_timeWarp
        );

        auto plan = Radium::TpsBypass::planTicks(dt, m_fields->overflow, tick);
        m_fields->overflow = plan.overflow;

        m_fields->tick = tick;
        for (int i = 0; i < plan.count; ++i) {
            GJBaseGameLayer::update(static_cast<float>(tick));
        }
        m_fields->tick = 0.0;
    }
};
