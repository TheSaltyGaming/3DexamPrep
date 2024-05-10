#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(MeshType type, float radius)
{
    mType = type;
    switch (type)
    {
    case Cube:
        CreateCube(radius);
            break;
    case Triangle:
        CreateTriangle();
            break;
    case Square:
        CreateSquare();
            break;
        
    }
}

void Mesh::CreateCube(float radius)
{
    
}

void Mesh::CreateTriangle()
{
    
}

void Mesh::CreateSquare()
{
    
}

void Mesh::Setup()
{
}

void Mesh::Draw(unsigned shaderProgram)
{
}
