#pragma once
#include <vector>
#include <glm/vec3.hpp>

struct Vertex;

class Surface
{
public:
    Surface();
    Surface(int size, glm::vec3 color);

    void Setup();
    void Draw(unsigned int shaderProgram);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    int size = 10;

    unsigned int VBO, VAO, EBO;

    glm::vec3 globalPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    
    
};
