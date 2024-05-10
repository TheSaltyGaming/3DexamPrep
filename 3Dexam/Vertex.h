#pragma once
#include <glm/vec3.hpp>

struct Vertex
{

    float x, y, z;
    Vertex() = default;

    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec3 Color;
};
