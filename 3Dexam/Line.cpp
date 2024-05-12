//
// Created by Ander on 08.04.2024.
//

#include "Line.h"

#include <glad/glad.h>

Line::Line() {
}

Line::Line(std::vector<Vertex> vertices)
{
    lineVertices = vertices;
    Setup();
}


void Line::Setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(Vertex), lineVertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Line::Draw(unsigned int shaderProgram) {

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, globalPosition);
    
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, lineVertices.size());
    glBindVertexArray(0);
}
