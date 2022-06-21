//
// Created by thijs on 21-06-22.
//

#ifndef DICEGONEROGUE_CHARACTERSELECTSCENE_H
#define DICEGONEROGUE_CHARACTERSELECTSCENE_H


#include <gameobject/Character.h>
#include "Scene.h"

namespace DGR {

class CharacterSelectScene : public Scene {
private:
    int maxSelect = 3;
    std::vector<Character*> allHeroes;
    std::vector<Character*> selectedHeroes;

    void alignCharacterPositions(double dt);
public:
    explicit CharacterSelectScene(GameStateManager* gameState);

    /// functions
    void handleMouseButton(double xPos, double yPos) override;

    void handleMousePosition(double xPos, double yPos) override;

    void reset() override;

    void update(double dt) override;

    void render(SpriteRenderer* spriteRenderer, TextRenderer* textRenderer) override;

    void handleMousePosition(Character* character, double xPos, double yPos);

    void pressButton(Button* button);
};

}


#endif //DICEGONEROGUE_CHARACTERSELECTSCENE_H