// source/systems/physics.cpp
#include "physics.hpp"
#include "../components/physics.hpp"
#include "../components/transform.hpp"

PhysicsSystem::PhysicsSystem() {
    requireComponent<TransformComponent>();
    requireComponent<PhysicsComponent>();
}

void PhysicsSystem::update(float deltaTime) {
    for (auto entity : getEntities()) {
        auto& transform = entity.getComponent<TransformComponent>();
        auto& physics = entity.getComponent<PhysicsComponent>();
        
        if (!physics.isStatic) {
            // Apply gravity if needed
            if (physics.isAffectedByGravity) {
                physics.acceleration += gravity;
            }
            
            // Update velocity
            physics.velocity += physics.acceleration * deltaTime;
            
            // Update position
            transform.position += physics.velocity * deltaTime;
            
            // Reset acceleration
            physics.acceleration = {0.0f, 0.0f, 0.0f};
        }
    }
}