//
// Created by Ander on 08.04.2024.
//

#ifndef LINE_H
#define LINE_H
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.h"


class Line {

public:
        unsigned VAO, VBO, EBO;
        Line();

        Line(std::vector<Vertex> vertices);

        void Setup();

        void Draw(unsigned int shaderProgram);

        std::vector<Vertex> lineVertices;
        

        glm::vec3 globalPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 globalRotation = glm::vec3(0.0f, 0.0f, 0.0f);
        
};



#endif //LINE_H
