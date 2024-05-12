#pragma once
#include <glm/vec3.hpp>

struct Vertex
{
    
    Vertex() = default;
    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color) : Position(position), Normal(normal), Color(color) { };

    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Color;
};
