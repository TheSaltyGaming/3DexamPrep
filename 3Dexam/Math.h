﻿#pragma once
#include "Mesh.h"

class Math
{
public:

    Math();
    
    void moveObject(Mesh* mesh, Mesh* target, float speed);

    int pointIndex = 0;
    
    void moveObject(Mesh* mesh, std::vector<glm::vec3> pointList, float speed);

    glm::vec3 barycentricCoordinates(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P);

    float calculateHeightUsingBarycentric2(const glm::vec3& A, const glm::vec3& B, const glm::vec3& C, const glm::vec3& P);


    
    
};
