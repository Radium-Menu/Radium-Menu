#include "FloatingButton.hpp"

#include <Geode/Geode.hpp>

#ifdef GEODE_IS_MOBILE

#include "Ui.hpp"

using namespace geode::prelude;

namespace Radium::FloatingButton {

    void attach(CCScene* scene) {
        if (!scene) return;

        auto id = "radium-button"_spr;
        if (scene->getChildByID(id)) return;

        auto winSize = CCDirector::get()->getWinSize();

        auto sprite = ButtonSprite::create("Ra");
        sprite->setScale(0.7f);

        auto button = CCMenuItemExt::createSpriteExtra(sprite, [](CCMenuItemSpriteExtra*) {
            Radium::Ui::toggle();
        });

        auto menu = CCMenu::create();
        menu->setID(id);
        menu->setPosition(ccp(36.f, winSize.height * 0.62f));
        menu->addChild(button);

        scene->addChild(menu, 10000);
    }

}

#else

namespace Radium::FloatingButton {

    void attach(cocos2d::CCScene*) {}

}

#endif
