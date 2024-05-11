#include "Math.h"

Math::Math()
{
    
}

/// \brief Moves a mesh towards a target mesh
/// \param mesh the mesh that will be moved
/// \param target destination mesh
/// \param speed movement speed
void Math::moveObject(Mesh* mesh, Mesh* target, float speed)
{
    
    // Get the current target point
    glm::vec3 readposition = mesh->globalPosition;


    glm::vec3 targetPoint = target->globalPosition;

    // Check if the NPC is close enough to the target point
    float distanceToTarget = glm::distance(mesh->globalPosition, targetPoint);
    if (distanceToTarget < 0.1f) {
        
        return;
    }


    // Calculate the direction vector from the NPC to the target point
    glm::vec3 direction = glm::normalize(targetPoint - readposition);

    // Move NPC towards target
    glm::vec3 tempposition = mesh->globalPosition;
    tempposition += direction * speed;
    mesh->globalPosition = tempposition;
    
    }

void Math::moveObject(Mesh* mesh, std::vector<glm::vec3> pointList, float speed)
{
    // Get the current target point
    glm::vec3 readposition = mesh->globalPosition;


    glm::vec3 targetPoint = pointList[pointIndex+1];


    // Calculate the direction vector from the NPC to the target point
    glm::vec3 direction = glm::normalize(targetPoint - readposition);

    // Move NPC towards target
    glm::vec3 tempposition = mesh->globalPosition;
    tempposition += direction * speed;
    mesh->globalPosition = tempposition;

    // Check if the NPC is close enough to the target point
    float distanceToTarget = glm::distance(mesh->globalPosition, targetPoint);
    if (distanceToTarget < 0.1f) { // Threshold value
        // Move to the next point
        pointIndex++;

        // Loop back to start
        if (pointIndex >= pointList.size()-1) {
            pointIndex = 0;
        }
    }
}
