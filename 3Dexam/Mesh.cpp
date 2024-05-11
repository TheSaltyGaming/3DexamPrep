#include "Mesh.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh()
{
}

Mesh::Mesh(MeshType type, float radius, glm::vec3 color)
{
    mType = type;
    switch (type)
    {
    case Cube:
        CreateCube(radius, color);
        break;
    case Triangle:
        CreateTriangle(radius, color);
        break;
    case Square:
        CreateSquare(radius, color);
        break;
    case Pyramid:
        CreatePyramid(radius, color);
        break;
    case Sphere:
        std::cout << "Error, please specify number of segments" << std::endl;
        break;
        
    }
}

Mesh::Mesh(MeshType type, float radius, int segments, glm::vec3 color)
{
    mType = type;
    switch (type)
    {
    case Sphere:
        CreateSphere(radius, segments, color);
        break;
    default:
        std::cout << "Only sphere accepts int segments" << std::endl;
        break;
    }
}


void Mesh::CreateCube(float radius, glm::vec3 color)
{
    // Define cube vertices
    vertices = {
        // Positions           // Normals           // Color
        { -radius, -radius, -radius, glm::vec3(-1.0f, -1.0f, -1.0f), color },
        {  radius, -radius, -radius, glm::vec3( 1.0f, -1.0f, -1.0f), color },
        {  radius,  radius, -radius, glm::vec3( 1.0f,  1.0f, -1.0f), color },
        { -radius,  radius, -radius, glm::vec3(-1.0f,  1.0f, -1.0f), color },
        { -radius, -radius,  radius, glm::vec3(-1.0f, -1.0f,  1.0f), color },
        {  radius, -radius,  radius, glm::vec3( 1.0f, -1.0f,  1.0f), color },
        {  radius,  radius,  radius, glm::vec3( 1.0f,  1.0f,  1.0f), color },
        { -radius,  radius,  radius, glm::vec3(-1.0f,  1.0f,  1.0f), color }
    };

    // Define triangles
    indices = {
        0, 1, 2, 2, 3, 0,   // front
        1, 5, 6, 6, 2, 1,   // right
        7, 6, 5, 5, 4, 7,   // back
        4, 0, 3, 3, 7, 4,   // left
        4, 5, 1, 1, 0, 4,   // bottom
        3, 2, 6, 6, 7, 3    // top
    };

    Setup();
}

void Mesh::CreateTriangle(float radius, glm::vec3 color)
{
    // Define triangle vertices
    vertices = {
        // Positions           // Normals           // Color
        { -radius, -radius, 0.0f, glm::vec3(0.0f, 0.0f, -1.0f), color },
        {  radius, -radius, 0.0f, glm::vec3(0.0f, 0.0f, -1.0f), color },
        {  0.0f,   radius, 0.0f, glm::vec3(0.0f, 0.0f, -1.0f), color }
    };

    // Define triangles
    indices = {
        0, 1, 2    // front
    };

    Setup();
}

void Mesh::CreateSquare(float radius, glm::vec3 color)
{
    // Define square vertices
    vertices = {
        // Positions           // Normals           // Color
        { -radius, -radius, 0.0f, glm::vec3(0.0f, 0.0f, -1.0f), color },
        {  radius, -radius, 0.0f, glm::vec3(0.0f, 0.0f, -1.0f), color },
        {  radius,  radius, 0.0f, glm::vec3(0.0f, 0.0f, -1.0f), color },
        { -radius,  radius, 0.0f, glm::vec3(0.0f, 0.0f, -1.0f), color }
    };

    // Define triangles
    indices = {
        0, 1, 2,    // first triangle
        2, 3, 0     // second triangle
    };

    Setup();
}

void Mesh::CreatePyramid(float radius, glm::vec3 color)
{
    // Vertices
    vertices = {
        // Positions           // Normals           // Color
        { -radius, -radius, -radius, glm::vec3(-1.0f, -1.0f, -1.0f), color }, // 0
        {  radius, -radius, -radius, glm::vec3( 1.0f, -1.0f, -1.0f), color }, // 1
        {  radius, -radius,  radius, glm::vec3( 1.0f, -1.0f,  1.0f), color }, // 2
        { -radius, -radius,  radius, glm::vec3(-1.0f, -1.0f,  1.0f), color }, // 3
        {  0.0f,   radius,  0.0f,   glm::vec3( 0.0f,  1.0f,  0.0f), color }  // 4
    };

    // Define triangles
    indices = {
        0, 1, 4,    // front
        1, 2, 4,    // right
        2, 3, 4,    // back
        3, 0, 4,    // left
        3, 2, 1, 1, 0, 3     // base
    };

    Setup();
} 

void Mesh::CreateSphere(float radius, int segments, glm::vec3 color)
{

    for (int i = 0; i <= segments; ++i) {
        for (int j = 0; j <= segments; ++j) {
            float y = radius * cos(glm::radians(180.0f - i * 180.0f / segments));
            float x = radius * cos(glm::radians(j * 360.0f / segments)) * sin(glm::radians(180.0f - i * 180.0f / segments));
            float z = radius * sin(glm::radians(j * 360.0f / segments)) * sin(glm::radians(180.0f - i * 180.0f / segments));

            vertices.push_back({x,y,z, glm::normalize(glm::vec3(x, y, z)), color});
        }
    }

    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
            indices.push_back(i * (segments + 1) + j);
            indices.push_back((i + 1) * (segments + 1) + j);
            indices.push_back((i + 1) * (segments + 1) + j + 1);

            indices.push_back(i * (segments + 1) + j);
            indices.push_back((i + 1) * (segments + 1) + j + 1);
            indices.push_back(i * (segments + 1) + j + 1);
        }
    }

    Setup();
}

void Mesh::Setup()
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

void Mesh::Draw(unsigned shaderProgram)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, globalPosition);
    
    model = glm::rotate(model, glm::radians(globalRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(globalRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(globalRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    glBindVertexArray(VAO);

    switch (mType)
    {
    case Cube:
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        break;
    case Square:
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        break;
    case Triangle:
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        break;
    case Pyramid:
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        break;
    case Sphere:
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        break;
    }

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
