// source/components/physics.hpp
#pragma once

#include <glm/glm.hpp>

struct PhysicsComponent {
    float mass = 1.0f;
    glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
    glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};
    bool isAffectedByGravity = true;
    bool isStatic = false;
};