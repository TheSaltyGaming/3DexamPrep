#include "Mesh.h"
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
    }

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
