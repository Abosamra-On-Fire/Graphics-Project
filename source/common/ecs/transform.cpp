#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our
{

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const
    {
        // Create transformation matrices for each component
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);

        // Combine transformations in the correct order: Scaling -> Rotation -> Translation
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json &data)
    {
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale = data.value("scale", scale);
    }

}