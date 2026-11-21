#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/GameStatsManager.hpp>

#include "../Radium/UnlockIcons.hpp"

using namespace geode::prelude;

class $modify(RadiumUnlockGameManager, GameManager) {
    bool isIconUnlocked(int id, IconType type) {
        if (Radium::UnlockIcons::isEnabled()) {
            return true;
        }
        return GameManager::isIconUnlocked(id, type);
    }

    bool isColorUnlocked(int id, UnlockType type) {
        if (Radium::UnlockIcons::isEnabled()) {
            return true;
        }
        return GameManager::isColorUnlocked(id, type);
    }
};

class $modify(RadiumUnlockStatsManager, GameStatsManager) {
    bool isItemUnlocked(UnlockType type, int id) {
        if (Radium::UnlockIcons::isEnabled()) {
            return true;
        }
        return GameStatsManager::isItemUnlocked(type, id);
    }
};
