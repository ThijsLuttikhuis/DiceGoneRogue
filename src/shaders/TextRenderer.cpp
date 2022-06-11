//
// Created by thijs on 06-06-22.
//

#include "TextRenderer.h"
#include "utilities/Constants.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <filesystem>

namespace DGR {

const std::vector<int> TextRenderer::letterWidths = {
      3, 1, 3, 5, 3, 5, 4, 1,
      2, 2, 3, 3, 1, 4, 1, 4,
      3, 1, 3, 3, 3, 3, 3, 3,
      3, 3, 1, 1, 4, 4, 4, 4,
      4, 4, 4, 4, 4, 4, 4, 4,
      4, 3, 3, 4, 3, 5, 4, 4,
      4, 4, 4, 4, 5, 4, 4, 5,
      5, 5, 4, 2, 4, 2, 4, 4,
      3, 4, 4, 4, 4, 4, 4, 4,
      4, 2, 2, 3, 3, 5, 4, 4,
      4, 4, 3, 3, 3, 4, 4, 5,
      3, 4, 3, 0, 0, 0, 0, 0
};


TextRenderer::TextRenderer(Shader* shader, glm::mat4 projection)
      : shader(shader) {


    std::string dir = "../src/textures/textrenderer/";
    auto dirIt = std::filesystem::directory_iterator(dir);
    for (const auto &entry : dirIt) {
#if DEBUG
        std::cout << entry.path() << std::endl;
#endif
        if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg") {
            texture = new Texture2D(entry.path().string());
        }
    }

    shader->use();
    shader->setInteger("sprite", 0);
    shader->setMatrix4("projection", projection);

    unsigned int VBO;

    const int imageWidth = 480;
    const int letterWidth = 5;
    const float uvWidth = (float) letterWidth / imageWidth;
    float vertices[24 * nLetters];

    float verticesStart[] = {
          // pos      // tex
          0.0f, 1.0f, 0.0f, 1.0f,
          1.0f, 0.0f, uvWidth, 0.0f,
          0.0f, 0.0f, 0.0f, 0.0f,

          0.0f, 1.0f, 0.0f, 1.0f,
          1.0f, 1.0f, uvWidth, 1.0f,
          1.0f, 0.0f, uvWidth, 0.0f
    };
    float verticesAdd[] = {
          // pos      // tex
          0.0f, 0.0f, uvWidth, 0.0f,
          0.0f, 0.0f, uvWidth, 0.0f,
          0.0f, 0.0f, uvWidth, 0.0f,

          0.0f, 0.0f, uvWidth, 0.0f,
          0.0f, 0.0f, uvWidth, 0.0f,
          0.0f, 0.0f, uvWidth, 0.0f
    };


    for (int i = 0; i < nLetters; i++) {
        int letterStart = i * 24;

        for (int j = 0; j < 24; j++) {
            float value = verticesStart[j] + verticesAdd[j] * (float) i;
            vertices[letterStart + j] = value;
        }
    }

    for (int i = 0; i < nLetters; i++) {
        glGenVertexArrays(1, &quadVAO[i]);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesStart), &*&vertices[i * 24], GL_STATIC_DRAW);

        glBindVertexArray(quadVAO[i]);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

TextRenderer::~TextRenderer() {
    for (unsigned int &i : quadVAO) {
        glDeleteVertexArrays(1, &i);
    }
}

void TextRenderer::drawText(const std::string &text, float zIndex, glm::vec2 position, glm::vec2 size,
                            glm::vec3 color, float alpha) {

    glm::vec3 white = glm::vec3(1.0f);
    glm::vec3 textColor = white;
    const glm::vec2 textStart = position;

    glm::vec2 currentTextPos = textStart + glm::vec2(1, 1);
    glm::vec2 letterSize(5, 7);

    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    for (char i : text) {
        if (i == '^') {
            textColor = textColor == white ? color : white;
            continue;
        }
        int c = i - (int) ' ';


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(currentTextPos, 0.0f));
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = glm::scale(model, glm::vec3(letterSize, 1.0f));

        currentTextPos += glm::vec2(letterWidths[c] + 1, 0);

        if (currentTextPos.x + letterSize.x > textStart.x + size.x) {
            currentTextPos = glm::vec2(textStart.x + 1, currentTextPos.y + letterSize.y + 1);
        }

        shader->use();
        shader->setMatrix4("model", model);
        shader->setVector3f("spriteColor", textColor);
        shader->setFloat("spriteAlpha", alpha);
        shader->setFloat("zIndex", zIndex);

        glBindVertexArray(this->quadVAO[c]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
}

}