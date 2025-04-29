#pragma once
#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/mesh-renderer.hpp"
#include "../application.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <string>

namespace our
{

    class CollisionSystem
    {
        // std::vector<Entity *> cars, batteries, planes, packages;
        Entity *player;

    public:
        void setPlayer(Entity *player)
        {
            this->player = player;
        }
        
        bool checkCollision(Entity* objectComponent, Entity* playerComponent) 
        {
            // Get player's world position and forward direction
            glm::vec3 playerPos = playerComponent->localTransform.position;
            glm::vec3 playerForward = glm::normalize(glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 0)));
        
            // Get object's world position
            glm::vec3 objectPos = objectComponent->localTransform.position;
        
            // Vector from player to object
            glm::vec3 toObject = objectPos - playerPos;
        
            // Project 'toObject' onto the player's forward direction
            float forwardDistance = glm::dot(toObject, playerForward);
        
            // If the object is behind the player (negative distance), ignore it
            if (forwardDistance <= 0) 
                return false;
        
            // Maximum allowed forward distance for collision
            float maxForwardDistance = 1.0f; // Adjust as needed
            if (forwardDistance > maxForwardDistance)
                return false;
        
            // Optional: Check if the object is within a certain angle (for a cone-shaped detection)
            glm::vec3 toObjectDir = glm::normalize(toObject);
            float angleDot = glm::dot(playerForward, toObjectDir);
        
            // Require the object to be mostly in front (adjust threshold as needed)
            return angleDot > 0.7f; // ~45-degree cone
        }
        // bool checkCollision(Entity* objectComponent, Entity* playerComponent) 
        // {
        //     // Get positions
        //     glm::vec3 playerPos = playerComponent->localTransform.position;
        //     glm::vec3 objectPos = objectComponent->localTransform.position;
            
        //     // Calculate distance between player and object
        //     glm::vec3 toObject = objectPos - playerPos;
        //     float distance = glm::distance(playerPos, objectPos);
            
        //     // Define collision threshold (sum of player and object radii)
        //     float collisionThreshold = 3.0f; // Adjust based on your object sizes
            
        //     if (distance > collisionThreshold)
        //         return false;
            
        //     // For more accurate collision, you might want to:
        //     // 1. Use bounding boxes/spheres
        //     // 2. Consider object orientation and size
            
        //     // If you specifically want to check if player is facing the object:
        //     glm::vec3 playerForward = glm::normalize(playerComponent->getLocalToWorldMatrix()* glm::vec4(0, 0, -1, 0)); // You need to implement this
        //     glm::vec3 toObjectDir = glm::normalize(toObject);
            
        //     float facingDot = glm::dot(playerForward, toObjectDir);
            
        //     // Only consider it a collision if player is close AND facing the object
        //     // Adjust threshold (0.7f means ~45 degree cone in front of player)
        //     return facingDot > 0.5f||facingDot < -0.5f; 
        // }
        int update(World *world, float deltaTime)
        {
            Entity *player ;
            for (const auto &entity : world->getEntities())
            {
                if (entity->name == "camera")
                {
                    player = entity;
                    break;
                }
            }
            for(const auto &entity : world->getEntities())
            {
                if ((entity->name == "cube" || entity->name == "monkey" || entity->name == "moon") && checkCollision(entity, player))
                {
                    return 1;
                }
            }
            return 0;
        }
       
    };

}