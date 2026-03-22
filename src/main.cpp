#include <Geode/Geode.hpp>
#include <Geode/modify/EffectGameObject.hpp>
#include <Geode/modify/EditButtonBar.hpp>

constexpr int D_BLOCK = 1755;
constexpr int J_BLOCK = 1813;
constexpr int S_BLOCK = 1829;
constexpr int H_BLOCK = 1859;
constexpr int F_BLOCK = 2866;

using namespace geode::prelude;

class $modify(MyEffectGameObject, EffectGameObject) {

    void customSetup() {
        EffectGameObject::customSetup();

        static const std::unordered_map<int, std::string> icons = {
            {D_BLOCK, "d_block.png"_spr},
            {J_BLOCK, "j_block.png"_spr},
            {S_BLOCK, "s_block.png"_spr},
            {H_BLOCK, "h_block.png"_spr},
            {F_BLOCK, "f_block.png"_spr}
        };

        auto it = icons.find(m_objectID);
        if (it != icons.end()) {
            setIcon(it->second);
        }
    }

    void setIcon(const std::string& texture) {
        this->setCascadeColorEnabled(true);
        this->setCascadeOpacityEnabled(true);

        if (!Mod::get()->getSettingValue<bool>("solid-border")) {
            auto newSpr = CCSprite::createWithSpriteFrameName("edit_eCollisionBlock01_001.png");
            this->setTexture(newSpr->getTexture());
            this->setTextureRect(newSpr->getTextureRect());
        }

        auto spr = CCSprite::create(texture.c_str());
        spr->setScale(0.9f);
        this->addChildAtPosition(spr, Anchor::Center);

        this->runAction(CallFuncExt::create([this] {
            if (auto label = this->getChildByType<CCLabelBMFont>(0)) {
                label->setVisible(Mod::get()->getSettingValue<bool>("show-letter"));
                if (label->isVisible()) {
                    label->setPosition({2.f, this->getContentHeight()});
                    label->setScale(0.3f);
                    label->setAnchorPoint({0.f, 1.f});
                }
            }
        }));
    }
};

class $modify(MyEditButtonBar, EditButtonBar) {

    void loadFromItems(CCArray* items, int c, int r, bool unkBool) {
        if (m_tabIndex != 5) {
            EditButtonBar::loadFromItems(items, c, r, unkBool);
            return;
        }

        if (Mod::get()->getSettingValue<bool>("move-f-block")) {
            int hIndex = -1;
            CCNode* fBlock = nullptr;

            for (int i = 0; i < items->count(); ++i) {
                if (auto menuItem = typeinfo_cast<CreateMenuItem*>(items->objectAtIndex(i))) {
                    if (menuItem->m_objectID == F_BLOCK) fBlock = menuItem;
                    if (menuItem->m_objectID == H_BLOCK) hIndex = i;
                    if (fBlock && hIndex != -1) break;
                }
            }

            if (fBlock && hIndex != -1) {
                items->removeObject(fBlock);
                items->insertObject(fBlock, hIndex + 1);
            }
        }

        EditButtonBar::loadFromItems(items, c, r, unkBool);
    }
};
