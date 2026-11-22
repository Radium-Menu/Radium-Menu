#include <Geode/Geode.hpp>
#include <Geode/modify/LevelPage.hpp>

#include "Utility/MainLevelBypass.hpp"

using namespace geode::prelude;

class $modify(RadiumLevelPage, LevelPage) {
    void onPlay(CCObject* sender) {
        if (!Radium::MainLevelBypass::isEnabled() || !m_level) {
            LevelPage::onPlay(sender);
            return;
        }

        int requiredCoins = m_level->m_requiredCoins;
        m_level->m_requiredCoins = 0;
        LevelPage::onPlay(sender);
        m_level->m_requiredCoins = requiredCoins;
    }
};
