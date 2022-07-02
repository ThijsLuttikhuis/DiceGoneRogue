//
// Created by thijs on 01-06-22.
//

#ifndef DICEGONEROGUE_FACE_H
#define DICEGONEROGUE_FACE_H

#include <memory>
#include <string>
#include <map>
#include <utility>
#include <glm/vec2.hpp>

#include "shaders/SpriteRenderer.h"
#include "shaders/TextRenderer.h"
#include "FaceModifier.h"
#include "FaceType.h"
#include "Dice.h"

namespace DGR {

class Dice;

class Face {
private:
    std::string name;
     std::shared_ptr<Dice> dice = nullptr;

    int face_{};
    int value{};
    FaceType type{};
    FaceModifier modifiers{};

    bool hover = false;

    static const std::vector<glm::vec2> faceDeltaPos;
    static const std::vector<glm::vec2> tickValueDeltaPos;

    void drawFace( std::shared_ptr<SpriteRenderer> spriteRenderer, Dice::dicePos dicePos);

    void drawFaceToolTip( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textRenderer, Dice::dicePos dicePos);
public:
    Face() = default;

    Face(int face_, int value, FaceType type, FaceModifier modifiers = {})
          : face_(face_), value(value), type(type), modifiers(modifiers) {};

    Face(std::string name,  std::shared_ptr<Dice> dice, int face_,
         int value, FaceType type, FaceModifier modifiers);

    /// getters
    [[nodiscard]] glm::vec2 getPosition(Dice::dicePos dicePos = Dice::diceLayoutPos) const;

    [[nodiscard]] glm::vec2 getSize() const;

    [[nodiscard]] bool isMouseHovering(double xPos, double yPos) const;

    [[nodiscard]] int getFace_() const;

    [[nodiscard]] int getValue() const;

    [[nodiscard]] FaceType getFaceType() const;

    [[nodiscard]] FaceModifier getModifiers() const;

    [[nodiscard]]  std::shared_ptr<Face> makeCopy() const;

    [[nodiscard]]  std::shared_ptr<Dice> getDice() const;

    /// setters
    void setName(const std::string &name_);

    void setValue(int value_);

    void setHover(bool hover_);

    void setType(FaceType type);

    void setType(FaceType::faceType type_);

    void addModifier(FaceModifier::modifier modifier);

    void addModifier(const std::string &modifierStr);

    void removeModifier(FaceModifier::modifier modifier);

    void setModifiers(unsigned int modifiers_);

    void setDice( std::shared_ptr<Dice> dice);

    /// render
    void drawHover( std::shared_ptr<SpriteRenderer> spriteRenderer,  std::shared_ptr<TextRenderer> textureRenderer,
                   Dice::dicePos dicePos = Dice::diceLayoutPos);

    void draw( std::shared_ptr<SpriteRenderer> spriteRenderer);

};

}


#endif //DICEGONEROGUE_FACE_H
