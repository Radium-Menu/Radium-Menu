#include "FloatingButton.hpp"

#include <Geode/Geode.hpp>

#ifdef GEODE_IS_MOBILE

#include "Ui.hpp"

using namespace geode::prelude;

namespace Radium::FloatingButton {

    void install() {
        static bool s_installed = false;
        if (s_installed) return;
        s_installed = true;

        auto winSize = CCDirector::get()->getWinSize();

        auto sprite = ButtonSprite::create("Ra");
        sprite->setScale(0.7f);

        auto button = CCMenuItemExt::createSpriteExtra(sprite, [](CCMenuItemSpriteExtra*) {
            Radium::Ui::toggle();
        });

        auto menu = CCMenu::create();
        menu->setID("radium-button"_spr);
        menu->setPosition(ccp(36.f, winSize.height * 0.62f));
        menu->setZOrder(10000);
        menu->addChild(button);

        SceneManager::get()->keepAcrossScenes(menu);
    }

}

#else

namespace Radium::FloatingButton {

    void install() {}

}

#endif
