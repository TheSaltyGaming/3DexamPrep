#include "Surface.h"
#include "Vertex.h"
#include <glm/gtc/noise.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>


Surface::Surface()
{
    
}

Surface::Surface(int sizeint, glm::vec3 color) {
    int detail = 10;
    size = sizeint * detail;
    glm::vec3 defaultColor = color;

    // Generate vertices for the grid
    for (int i = 0; i <= size; ++i) {
        for (int j = 0; j <= size; ++j) {
            glm::vec3 position((float)i / detail*2, glm::perlin(glm::vec2((float)i / detail, (float)j / detail)), (float)j / detail*2);

            glm::vec3 vertexColor = defaultColor;

            // If the color is (420, 420, 420), use rainbow colors
            if (color == glm::vec3(420, 420, 420)) {
                // Calculate color based on the position in the grid
                float colorFactor = (float)(i * (size + 1) + j) / (size * size);
                vertexColor = glm::vec3(
                    glm::sin(colorFactor * 2.0f * glm::pi<float>()) * 0.5f + 0.5f,
                    glm::sin(colorFactor * 2.0f * glm::pi<float>() + 2.0f * glm::pi<float>() / 3.0f) * 0.5f + 0.5f,
                    glm::sin(colorFactor * 2.0f * glm::pi<float>() + 4.0f * glm::pi<float>() / 3.0f) * 0.5f + 0.5f
                );
            }

            vertices.push_back(Vertex(position, glm::vec3(0.0f), vertexColor));
        }
    }

    // Generate indices and calculate normals
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int topLeft = i * (size + 1) + j;
            int bottomLeft = (i + 1) * (size + 1) + j;

            // First triangle
            indices.push_back(bottomLeft);
            indices.push_back(bottomLeft + 1);
            indices.push_back(topLeft + 1);

            glm::vec3 normal1 = glm::normalize(glm::cross(vertices[bottomLeft + 1].Position - vertices[bottomLeft].Position, vertices[topLeft + 1].Position - vertices[bottomLeft].Position));
            vertices[bottomLeft].Normal += normal1;
            vertices[bottomLeft + 1].Normal += normal1;
            vertices[topLeft + 1].Normal += normal1;

            // Add the triangle to the triangles vector
            triangles.push_back(TriangleStruct(vertices[bottomLeft].Position, vertices[bottomLeft + 1].Position, vertices[topLeft + 1].Position, normal1));

            // Second triangle
            indices.push_back(topLeft+1);
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);

            glm::vec3 normal2 = glm::normalize(glm::cross(vertices[topLeft].Position - vertices[topLeft + 1].Position, vertices[bottomLeft].Position - vertices[topLeft + 1].Position));
            vertices[topLeft + 1].Normal += normal2;
            vertices[topLeft].Normal += normal2;
            vertices[bottomLeft].Normal += normal2;

            // Add the triangle to the triangles vector
            triangles.push_back(TriangleStruct(vertices[topLeft + 1].Position, vertices[topLeft].Position, vertices[bottomLeft].Position, normal2));
        }
    }

    // Normalize the normals
    for (auto& vertex : vertices) {
        vertex.Normal = glm::normalize(vertex.Normal);
    }

    Setup();
}

void Surface::Setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);
    // Color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color)); // Add this line
    glEnableVertexAttribArray(2);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Surface::Draw(unsigned shaderProgram)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, globalPosition);

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind VAO
}
