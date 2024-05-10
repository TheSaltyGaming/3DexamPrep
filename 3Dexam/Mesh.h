#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Vertex.h"

enum MeshType {Cube, Triangle, Square  };

class Mesh
{
public:

    Mesh();
    Mesh(MeshType type, float radius, glm::vec3 color);
    

    void CreateCube(float radius, glm::vec3 color);
    void CreateTriangle(float radius, glm::vec3 color);
    void CreateSquare(float radius, glm::vec3 color);


    void Setup();
    void Draw(unsigned int shaderProgram);
    
    MeshType mType;

    glm::mat4 model = glm::mat4(1.0f);
    unsigned int VBO, VAO, EBO;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
