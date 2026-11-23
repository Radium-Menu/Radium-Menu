#include "FloatingButton.hpp"

#include <Geode/Geode.hpp>

#ifdef GEODE_IS_MOBILE

#include "Ui.hpp"

#include <algorithm>
#include <cmath>

using namespace geode::prelude;

namespace Radium::FloatingButton {

    static constexpr float kSize = 44.f;
    static constexpr float kDragThreshold = 10.f;
    static constexpr int kTouchPriority = -600;

    class RadiumButton : public CCLayer {
    public:
        static RadiumButton* create() {
            auto ret = new RadiumButton();
            if (ret && ret->init()) {
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }

        bool init() override {
            if (!CCLayer::init()) return false;

            m_sprite = CCSprite::create("radium-button.png"_spr);
            if (!m_sprite) return false;

            m_baseScale = kSize / m_sprite->getContentSize().height;
            m_sprite->setScale(m_baseScale);
            m_sprite->setPosition(ccp(kSize / 2.f, kSize / 2.f));

            this->ignoreAnchorPointForPosition(false);
            this->setAnchorPoint(ccp(0.5f, 0.5f));
            this->setContentSize(CCSize(kSize, kSize));
            this->addChild(m_sprite);
            this->setTouchEnabled(true);

            auto winSize = CCDirector::get()->getWinSize();
            auto mod = Mod::get();
            float x = mod->getSavedValue<float>("button-x", 36.f);
            float y = mod->getSavedValue<float>("button-y", winSize.height * 0.62f);
            this->setPosition(clamp(ccp(x, y)));

            return true;
        }

        void onExit() override {
            CCLayer::onExit();
            if (Radium::Ui::isOpen()) Radium::Ui::toggle();
        }

        void registerWithTouchDispatcher() override {
            CCTouchDispatcher::get()->addTargetedDelegate(this, kTouchPriority, true);
        }

        bool ccTouchBegan(CCTouch* touch, CCEvent*) override {
            if (!this->getParent() || !this->isVisible()) return false;

            CCPoint local = this->convertTouchToNodeSpace(touch);
            CCRect bounds(0.f, 0.f, kSize, kSize);
            if (!bounds.containsPoint(local)) return false;

            m_dragging = false;
            m_start = touch->getLocation();
            m_offset = this->getPosition() - this->getParent()->convertToNodeSpace(m_start);
            m_sprite->setScale(m_baseScale * 0.9f);
            return true;
        }

        void ccTouchMoved(CCTouch* touch, CCEvent*) override {
            CCPoint location = touch->getLocation();

            if (!m_dragging) {
                float dx = location.x - m_start.x;
                float dy = location.y - m_start.y;
                if (std::sqrt(dx * dx + dy * dy) < kDragThreshold) return;
                m_dragging = true;
            }

            CCPoint target = this->getParent()->convertToNodeSpace(location) + m_offset;
            this->setPosition(clamp(target));
        }

        void ccTouchEnded(CCTouch*, CCEvent*) override {
            m_sprite->setScale(m_baseScale);

            if (m_dragging) {
                m_dragging = false;
                auto mod = Mod::get();
                mod->setSavedValue("button-x", this->getPositionX());
                mod->setSavedValue("button-y", this->getPositionY());
                return;
            }

            Radium::Ui::toggle();
        }

        void ccTouchCancelled(CCTouch*, CCEvent*) override {
            m_dragging = false;
            m_sprite->setScale(m_baseScale);
        }

    private:
        static CCPoint clamp(CCPoint p) {
            auto winSize = CCDirector::get()->getWinSize();
            float half = kSize / 2.f;
            p.x = std::clamp(p.x, half, winSize.width - half);
            p.y = std::clamp(p.y, half, winSize.height - half);
            return p;
        }

        CCSprite* m_sprite = nullptr;
        CCPoint m_start;
        CCPoint m_offset;
        float m_baseScale = 1.f;
        bool m_dragging = false;
    };

    void attach(CCNode* parent) {
        if (!parent) return;

        auto id = "radium-button"_spr;
        if (parent->getChildByID(id)) return;

        auto button = RadiumButton::create();
        if (!button) return;

        button->setID(id);
        parent->addChild(button, 100);
    }

}

#else

namespace Radium::FloatingButton {

    void attach(cocos2d::CCNode*) {}

}

#endif
