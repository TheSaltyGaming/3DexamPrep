#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "Vertex.h"

enum MeshType {Cube, Triangle, Square  };

class Mesh
{
public:

    Mesh();
    Mesh(MeshType type, float radius);
    

    void CreateCube(float radius);
    void CreateTriangle();
    void CreateSquare();


    void Setup();
    void Draw(unsigned int shaderProgram);
    
    MeshType mType;

    glm::mat4 model = glm::mat4(1.0f);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
