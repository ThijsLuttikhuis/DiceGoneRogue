//
// Created by thijs on 03-06-22.
//

#include <iostream>
#include <algorithm>

#include "utilities/Constants.h"
#include "YamlReader.h"

namespace DGR {


YamlReader::YamlReader() {

    keyToFunc["heroes"] = new YamlHandleHeroes();

    keyToFunc["knight"] = new YamlHandleHero("knight");
    keyToFunc["rogue"] = new YamlHandleHero("rogue");
    keyToFunc["paladin"] = new YamlHandleHero("paladin");
    keyToFunc["druid"] = new YamlHandleHero("druid");
    keyToFunc["wizard"] = new YamlHandleHero("wizard");

    keyToFunc["dice"] = new YamlHandleDice();
    keyToFunc["face0"] = new YamlHandleFace(0);
    keyToFunc["face1"] = new YamlHandleFace(1);
    keyToFunc["face2"] = new YamlHandleFace(2);
    keyToFunc["face3"] = new YamlHandleFace(3);
    keyToFunc["face4"] = new YamlHandleFace(4);
    keyToFunc["face5"] = new YamlHandleFace(5);

    keyToFunc["hp"] = new YamlHandleInt(stringCode::hp);
    keyToFunc["mod"] = new YamlHandleString(stringCode::mod);
    keyToFunc["mod_1"] = new YamlHandleString(stringCode::mod);
    keyToFunc["damage"] = new YamlHandleInt(stringCode::damage);
    keyToFunc["shield"] = new YamlHandleInt(stringCode::shield);
    keyToFunc["heal"] = new YamlHandleInt(stringCode::shield);
    keyToFunc["damage_and_self_shield"] = new YamlHandleInt(stringCode::damage_and_self_shield);
    keyToFunc["mana"] = new YamlHandleInt(stringCode::mana);
    keyToFunc["shield_and_mana"] = new YamlHandleInt(stringCode::shield_and_mana);
    keyToFunc["heal_and_shield"] = new YamlHandleInt(stringCode::heal_and_shield);

    keyToFunc["empty"] = new YamlHandleInt(stringCode::empty);
}

void YamlReader::readFile(const std::string &name) {
    /// read file and put all data into a string

    std::string fileName = "../src/iofilemanager/" + name + ".dgr";
    std::ifstream file;
    file.open(fileName);
    if (!file) { // file couldn't be opened
        std::cerr << "[YamlReader::readfile] file could not be opened" << std::endl;
        exit(1);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string worldStr = buffer.str();
    worldStr.erase(std::remove_if(worldStr.begin(), worldStr.end(), isspace), worldStr.end());


    std::string word;
    size_t i = 0;
    int colonCounter = 0;
    int colonCounterMax = 8;
    auto worldStrLength = worldStr.size();

    std::vector<YamlHandle*> objectHandle(colonCounterMax, nullptr);
    while (i < worldStrLength) {
        size_t posColon = worldStr.find(':', i);
        size_t posSemi = worldStr.find(';', i);

        if (posSemi < posColon) {
            word = worldStr.substr(i, posSemi - i);
#if DEBUG
            std::cout << "\t" << posSemi << " ; " << word << std::endl;
#endif
            i = posSemi + 1;

            if (!word.empty()) {
                objectHandle[colonCounter] = keyToFunc.find(word) != keyToFunc.end() ? keyToFunc[word] : nullptr;
            }

            if (objectHandle[colonCounter]) {
                objectHandle[colonCounter]->handle();

                objectHandle[colonCounter - 1]->handle(objectHandle[colonCounter]);

            } else {
                objectHandle[colonCounter - 1]->handle(word);
            }

            if (objectHandle[colonCounter]) {
                objectHandle[colonCounter]->reset();
                objectHandle[colonCounter] = nullptr;
            }
            colonCounter--;
        } else {
            word = worldStr.substr(i, posColon - i);
#if DEBUG
            std::cout << "\t" << posColon << " : " << word << std::endl;
#endif
            i = posColon + 1;

            if (objectHandle[colonCounter]) {
                objectHandle[colonCounter - 1]->handle(objectHandle[colonCounter]);
            }
            objectHandle[colonCounter] = keyToFunc.find(word) != keyToFunc.end() ? keyToFunc[word] : nullptr;
            colonCounter++;
        }
    }

    if (!objectHandle[0]) {
        std::cerr << "[YamlReader::readFile] no object handle remaining" << std::endl;
    }

    handle = objectHandle[0];
}

}
