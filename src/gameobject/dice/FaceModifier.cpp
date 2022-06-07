//
// Created by thijs on 04-06-22.
//

#include <iostream>
#include "FaceModifier.h"
#include <vector>

namespace DGR {

BiDirectionalMap<std::string, FaceModifier::modifier> FaceModifier::stringsAndModifiers =
      BiDirectionalMap(std::vector<std::pair<std::string, FaceModifier::modifier>>{
            {"none",          modifier::none},
            {"ranged",        modifier::ranged},
            {"sweeping edge", modifier::sweeping_edge},
            {"single use",    modifier::single_use},
            {"poison",        modifier::poison},
            {"cleanse",       modifier::cleanse},
            {"first blood",   modifier::first_blood},
            {"growth",        modifier::growth},
            {"decay",         modifier::decay}}
      );

FaceModifier::modifier FaceModifier::stringToModifier(const std::string &modifierStr) {
    modifier &mod = stringsAndModifiers.at(modifierStr);
    return mod;
}

std::string FaceModifier::toString() {
    std::string modString;
    unsigned int maxMod = modifiers;
    for (unsigned int i = 0; i < maxMod; i++) {
        unsigned int m = 1u << i;
        if (m > modifiers) {
            break;
        }
        auto mod = (modifier)m;
        if (hasModifier(mod)) {
            if (!modString.empty()) {
                modString += ", ";
            }
            modString += stringsAndModifiers.at(mod);
        }
    }
    return modString;
}

bool FaceModifier::hasModifier(modifier modifier) {
    return modifiers & static_cast<unsigned int>(modifier);
}

unsigned int FaceModifier::getModifiers() {
    return modifiers;
}

void FaceModifier::addModifier(const std::string &modifierStr) {
    modifier modifier_ = stringToModifier(modifierStr);
    if (!hasModifier(modifier_)) {
        modifiers += static_cast<unsigned int>(modifier_);
    }
}

void FaceModifier::addModifier(modifier modifier_) {
    if (!hasModifier(modifier_)) {
        modifiers += static_cast<unsigned int>(modifier_);
    }
}

glm::vec3 FaceModifier::toColor() {
    if (modifiers == 0) {
        return glm::vec3(1.0f);
    }

    if (modifiers & static_cast<unsigned int>(modifier::ranged)) {
        return glm::vec3(1.0f, 0.4f, 0.4f);
    }
    if (modifiers & static_cast<unsigned int>(modifier::sweeping_edge)) {
        return glm::vec3(4.0f, 4.5f, 1.0f);
    }
    if (modifiers & static_cast<unsigned int>(modifier::single_use)) {
        return glm::vec3(0.4f, 0.1f, 0.7f);
    }
    if (modifiers & static_cast<unsigned int>(modifier::poison)) {
        return glm::vec3(0.1f, 0.5f, 0.1f);
    }
    if (modifiers & static_cast<unsigned int>(modifier::cleanse)) {
        return glm::vec3(0.8f, 1.0f, 0.7f);
    }
    if (modifiers & static_cast<unsigned int>(modifier::first_blood)) {
        return glm::vec3(0.0f, 0.9f, 0.7f);
    }

    return glm::vec3(1.0f);
}


}
