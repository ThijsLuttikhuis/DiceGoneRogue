//
// Created by thijs on 01-06-22.
//

#include <iostream>
#include "Face.h"
#include "Dice.h"

namespace DGR {


const std::map<int, const glm::vec2> Face::faceDeltaPos = std::map<int, const glm::vec2>{
      {0, glm::vec2(2, 17)},
      {1, glm::vec2(17, 2)},
      {2, glm::vec2(17, 17)},
      {3, glm::vec2(17, 32)},
      {4, glm::vec2(32, 17)},
      {5, glm::vec2(47, 17)}};

const std::map<int, const glm::vec2> Face::tickValueDeltaPos = std::map<int, const glm::vec2>{
      {0, glm::vec2(11, 12)},
      {1, glm::vec2(11, 10)},
      {2, glm::vec2(11, 8)},
      {3, glm::vec2(11, 6)},
      {4, glm::vec2(11, 4)},
      {5, glm::vec2(11, 2)}};

bool checkBit(unsigned int value, unsigned int pos) {
    return value & (1u << (pos));
}

void Face::setName(const std::string &name_) {
    name = name_;
}

FaceModifier::modifier FaceModifier::stringToModifier(const std::string& modifierStr) {
    if (modifierStr == "ranged") {
        return FaceModifier::modifier::ranged;
    }
    if (modifierStr == "sweeping_edge") {
        return FaceModifier::modifier::sweeping_edge;
    }
    if (modifierStr == "single_use") {
        return FaceModifier::modifier::single_use;
    }
    if (modifierStr == "poison") {
        return FaceModifier::modifier::poison;
    }
    if (modifierStr == "cleanse") {
        return FaceModifier::modifier::cleanse;
    }
    if (modifierStr == "first_blood") {
        return FaceModifier::modifier::first_blood;
    }

    std::cerr << "FaceModifier::stringToModifier: error: string is unknown: " << modifierStr << std::endl;
    return FaceModifier::modifier::none;
}

bool FaceModifier::hasModifier(FaceModifier::modifier modifier) {
    return modifiers & static_cast<unsigned int>(modifier);
}

unsigned int FaceModifier::getModifiers() {
    return modifiers;
}

void FaceModifier::addModifier(const std::string &modifierStr) {
    FaceModifier::modifier modifier_ = stringToModifier(modifierStr);
    if (!hasModifier(modifier_)) {
        modifiers += static_cast<unsigned int>(modifier_);
    }
}

void FaceModifier::addModifier(FaceModifier::modifier modifier_) {
    if (!hasModifier(modifier_)) {
        modifiers += static_cast<unsigned int>(modifier_);
    }
}

Face::Face(std::string name, Dice* dice, int face_, int value, faceType type, FaceModifier modifiers)
      : name(std::move(name)), dice(dice), face_(face_), value(value), type(type), modifiers(modifiers) {
}

glm::vec3 FaceModifier::toColor() {
    if (modifiers == 0) {
        return glm::vec3(1.0f);
    }

    if (modifiers & static_cast<unsigned int>(FaceModifier::modifier::ranged)) {
        return glm::vec3(1.0f, 0.4f, 0.4f);
    }
    if (modifiers & static_cast<unsigned int>(FaceModifier::modifier::sweeping_edge)) {
        return glm::vec3(4.0f, 4.5f, 1.0f);
    }
    if (modifiers & static_cast<unsigned int>(FaceModifier::modifier::single_use)) {
        return glm::vec3(0.4f, 0.1f, 0.7f);
    }
    if (modifiers & static_cast<unsigned int>(FaceModifier::modifier::poison)) {
        return glm::vec3(0.1f, 0.5f, 0.1f);
    }
    if (modifiers & static_cast<unsigned int>(FaceModifier::modifier::cleanse)) {
        return glm::vec3(0.8f, 1.0f, 0.7f);
    }
    if (modifiers & static_cast<unsigned int>(FaceModifier::modifier::first_blood)) {
        return glm::vec3(0.0f, 0.9f, 0.7f);
    }

    return glm::vec3(1.0f);
}


std::string Face::faceTypeToString(faceType type) {
    switch (type) {
        case damage:
            return "damage";
        case mana:
            return "mana";
        case heal:
            return "damage";
        case shield:
            return "shield";
        case dodge:
            return "damage";
        case undying:
            return "damage";
        case heal_and_shield:
            return "damage";
        case heal_and_mana:
            return "damage";
        case shield_and_mana:
            return "damage";
        case damage_and_mana:
            return "damage";
        case damage_and_self_shield:
            return "damage";
        case empty:
            return "damage";
        default:
            std::cerr << "Face::faceTypeToString: error, type string unknown: " << type << std::endl;
            return "error";
    }
}

void Face::setType(faceType type_) {
    type = type_;
}

void Face::drawFace(SpriteRenderer* spriteRenderer) {
    auto position = getPosition();

    int value_ = value < 0 ? 0 : value > 40 ? 40 : value;

    int tens = value_ / 10;
    int fives = (value_ - tens * 10) / 5;
    int ones = (value_ - tens * 10 - fives * 5);

    int h = 0;
    while (h < 12) {
        glm::vec2 tickValueSize;
        glm::vec2 tickValuePos;
        if (tens > 0) {
            tickValueSize = glm::vec2(3, 3);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 2);
            spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{1.0f, 0.5f, 0.0f});
            tens--;
            h += 4;
            continue;
        }
        if (fives > 0) {
            tickValueSize = glm::vec2(3, 2);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 1);
            spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{0.9f, 0.8f, 0.0f});
            fives--;
            h += 3;
            continue;
        }
        if (ones == 4 || ones == 9) {
            tickValueSize = glm::vec2(3, 1);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h);
            spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{0.9f, 0.9f, 0.9f});
            h += 2;

            if (ones == 4) {
                tickValueSize = glm::vec2(3, 2);
                tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 1);
                spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                           0.0f, glm::vec3{0.9f, 0.8f, 0.0f});
                h += 3;

            } else {
                tickValueSize = glm::vec2(3, 3);
                tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h - 2);
                spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                           0.0f, glm::vec3{1.0f, 0.5f, 0.0f});
                h += 4;
            }
            ones = 0;
            continue;
        }
        if (ones > 0) {
            tickValueSize = glm::vec2(3, 1);
            tickValuePos = position + tickValueDeltaPos.at(0) + glm::vec2(0, -h);
            spriteRenderer->drawSprite("dice_face_on", 0.1f, tickValuePos, tickValueSize,
                                       0.0f, glm::vec3{0.9f, 0.9f, 0.9f});
            ones--;
            h += 2;
            continue;
        }
        break;
    }

    //TODO: create sprites for every hero type
    std::string textureName = "knight_" + faceTypeToString(type);
    glm::vec3 textureColor = modifiers.toColor();
    spriteRenderer->drawSprite(textureName, 0.1f,
                               position, glm::vec2(11, 14), 0, textureColor);
}

void Face::drawFaceToolTip(SpriteRenderer* spriteRenderer) {
    auto position = getPosition();

    glm::vec2 backgroundSize = glm::vec2(32, 16);
    spriteRenderer->drawSprite("dice_face_template_background", 0.0f,
                               position + glm::vec2{5, -5}, backgroundSize);

    std::string textureName = "knight_" + faceTypeToString(type);
    glm::vec3 textureColor = modifiers.toColor();
    spriteRenderer->drawSprite(textureName, 0.1f,
                               position, glm::vec2(11, 14), 0, textureColor);
}

void Face::draw(SpriteRenderer* spriteRenderer) {

    drawFace(spriteRenderer);

    if (hover) {
        drawFaceToolTip(spriteRenderer);
    }
}

void Face::setValue(int value_) {
    value = value_;
}

glm::vec2 Face::getPosition() const {
    glm::vec2 dicePosition = dice->getPosition(false);

    return dicePosition + faceDeltaPos.at(face_);
}

glm::vec2 Face::getSize() const {
    return glm::vec2(15, 15);
}

void Face::setHover(bool hover_) {
    hover = hover_;
}

bool Face::isMouseHovering(double xPos, double yPos) const {
    auto position = getPosition();
    auto size = getSize();
    return (xPos > position.x && xPos < position.x + size.x)
           && (yPos > position.y && yPos < position.y + size.y);
}

void Face::setDice(Dice* dice_) {
    dice = dice_;
}

void Face::addModifier(FaceModifier::modifier modifier) {
    modifiers.addModifier(modifier);
}

void Face::addModifier(const std::string &modifierStr) {
    modifiers.addModifier(modifierStr);
}

int Face::getFace_() {
    return face_;
}

}