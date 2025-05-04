#pragma once

#include <glm/glm.hpp>

enum class LightType
{
    Directional,
    Point
};

struct LightComponent
{
    LightType type;
    glm::vec3 color = glm::vec3(1.0f); 
    glm::vec3 position = glm::vec3(1.0);
    glm::vec3 direction = glm::vec3(1.0);
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};
