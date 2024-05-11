#pragma once
#include "Mesh.h"

class Math
{
public:

    Math();
    
    void moveObject(Mesh* mesh, Mesh* target, float speed);

    int pointIndex = 0;
    
    void moveObject(Mesh* mesh, std::vector<glm::vec3> pointList, float speed);

    
    
};
