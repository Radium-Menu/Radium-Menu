#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "Utility/Noclip.hpp"

using namespace geode::prelude;

class $modify(RadiumNoclipPlayLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* object) {
        if (Radium::Noclip::isEnabled() && object != m_anticheatSpike) {
            Radium::Noclip::countBlockedDeath();
            return;
        }
        PlayLayer::destroyPlayer(player, object);
    }
};
