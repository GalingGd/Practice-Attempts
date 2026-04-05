#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/LevelCompleteLayer.hpp>

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    void resetLevel() {
        PlayLayer::resetLevel();

        if (this->m_isPracticeMode) {
            auto level = this->m_level;
            std::string levelKey = "practice_attempts_" + std::to_string(level->m_levelID.value());
            
            int currentAttempts = Mod::get()->getSavedValue<int>(levelKey, 0);
            Mod::get()->setSavedValue<int>(levelKey, currentAttempts + 1);
        }
    }
};

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
    void onStats(CCObject* sender) {
        LevelInfoLayer::onStats(sender);

        if (auto alert = CCScene::get()->getChildByType<FLAlertLayer>(0)) {
            auto layer = alert->m_mainLayer;
            
            for (auto child : CCArrayExt<CCNode*>(layer->getChildren())) {
                if (auto label = typeinfo_cast<CCLabelBMFont*>(child)) {
                    std::string content = label->getString();
                    
                    if (content.find("Practice:") != std::string::npos) {
                        std::string levelKey = "practice_attempts_" + std::to_string(this->m_level->m_levelID.value());
                        int totalPractice = Mod::get()->getSavedValue<int>(levelKey, 0);
                        
                        std::string newText = content + "\nTotal Practice: " + std::to_string(totalPractice);
                        label->setString(newText.c_str());
                        label->setScale(0.5f);
                        //This part makes the font to be blue change it to an another colour if you want!
                        label->setColor({ 0, 150, 255 }); 
                    }
                }
            }
        }
    }
};

class $modify(MyLevelCompleteLayer, LevelCompleteLayer) {
    bool init(PlayLayer* pl) {
        if (!LevelCompleteLayer::init(pl)) return false;

        auto mainLayer = this->getChildByType<CCLayer*>(0);
        if (mainLayer) {
            std::string levelKey = "practice_attempts_" + std::to_string(pl->m_level->m_levelID.value());
            int totalPractice = Mod::get()->getSavedValue<int>(levelKey, 0);

            auto label = CCLabelBMFont::create(
                (std::string("Total Practice: ") + std::to_string(totalPractice)).c_str(),
                "goldFont.fnt"
            );
            label->setScale(0.6f);
            label->setPosition({CCDirector::get()->getWinSize().width / 2, 45});
            //This part makes the font to be blue change it to an another colour if you want!
            label->setColor({ 0, 150, 255 });
            mainLayer->addChild(label);
        }

        return true;
    }
};

