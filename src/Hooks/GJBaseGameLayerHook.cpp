#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

#include <chrono>

#include "Physics/TpsBypass.hpp"
#include "RadiumReplay/RadiumReplay.hpp"

using namespace geode::prelude;

class $modify(RadiumTpsGameLayer, GJBaseGameLayer) {
    struct Fields {
        double overflow = 0.0;
        double tick = 0.0;
        double cost = 0.0;
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

        bool exact = !Radium::TpsBypass::isRealTime()
            || Radium::RadiumReplay::mode() != Radium::RadiumReplay::Mode::Idle;

        auto plan = exact
            ? Radium::TpsBypass::planTicks(dt, m_fields->overflow, tick)
            : Radium::TpsBypass::planRealTime(dt, m_fields->overflow, tick, m_fields->cost);

        m_fields->overflow = plan.overflow;
        m_fields->tick = plan.tick;

        for (int i = 0; i < plan.count; ++i) {
            auto start = std::chrono::steady_clock::now();
            GJBaseGameLayer::update(static_cast<float>(plan.tick));
            double spent = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
            m_fields->cost = m_fields->cost > 0.0 ? m_fields->cost * 0.9 + spent * 0.1 : spent;
        }

        m_fields->tick = 0.0;
    }
};
