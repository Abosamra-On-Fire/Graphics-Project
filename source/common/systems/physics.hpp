// source/systems/physics.hpp
#pragma once

#include "../ecs/system.hpp"

class PhysicsSystem : public System {
public:
    PhysicsSystem();
    void update(float deltaTime) override;
    
private:
    const glm::vec3 gravity = {0.0f, -9.81f, 0.0f}; // Earth gravity
};