//
// Created by thijs on 20-06-22.
//

#ifndef DICEGONEROGUE_BATTLEVICTORYSCENE_H
#define DICEGONEROGUE_BATTLEVICTORYSCENE_H


#include "Scene.h"

namespace DGR {

class BattleVictoryScene : public Scene {
public:
    enum victoryGameState {
        get_item,
        get_xp,
        get_xp_done,
        level_up_select,
        level_up_prompt
    };
private:
    victoryGameState state = get_item;
    double xpPercent = 0;
    int heroToLevelUp = -1;
    int faceToLevelUp = -1;

    void pressButton(std::shared_ptr<Button> button);

public:
    explicit BattleVictoryScene(std::weak_ptr<GameStateManager> gameState);

    void handleMousePosition(double xPos, double yPos) override;

    void handleMouseButton(double xPos, double yPos) override;

    void render(const std::shared_ptr<SpriteRenderer> &spriteRenderer,
                const std::shared_ptr<TextRenderer> &textRenderer) const override;

    void onPushToStack() override;

    void update(double dt) override;
};

}


#endif //DICEGONEROGUE_BATTLEVICTORYSCENE_H
