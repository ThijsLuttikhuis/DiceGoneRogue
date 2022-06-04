//
// Created by thijs on 30-05-22.
//

#ifndef DICEGONEROGUE_SPRITERENDERER_H
#define DICEGONEROGUE_SPRITERENDERER_H

#include <map>
#include "shaders/Shader.h"
#include "shaders/Texture2D.h"

namespace DGR {

class SpriteRenderer {
private:
    Shader* shader;
    unsigned int quadVAO{};

    std::map<std::string, Texture2D*> textures;
public:
    SpriteRenderer(Shader* shader, glm::mat4 projection);

    ~SpriteRenderer();

    void addTexture(const std::string &name);

    void addAllTexturesInDir(const std::string &dirName);

    void addTexture(const std::string &fileDir, const std::string &name);

    void drawSprite(const std::string &texture, float zIndex, glm::vec2 position, glm::vec2 size, float rotate = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f), float alpha = 1.0f);
};

}

#endif //DICEGONEROGUE_SPRITERENDERER_H