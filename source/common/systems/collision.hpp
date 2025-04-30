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
#include <vector>
namespace our
{

    class CollisionSystem
    {
        // std::vector<Entity *> cars, batteries, planes, packages;
        Entity *player;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> vertices2;
    public:
    void init_collision_wall1(World *world) {
        Entity *wall ;
        for (const auto &entity : world->getEntities())
        {
            if (entity->name == "wall")
            {
                wall = entity;
                vertices = loadVerticesInYRange("assets/models/igloo.obj",wall);
            }
            if (entity->name == "wall2")
            { 
                wall = entity;
                vertices2 = loadVerticesInYRange("assets/models/igloo.obj",wall);
            }
        }
        std::cout << "Wall vertices: " << vertices.size() << ", Wall2 vertices: " << vertices2.size() << "\n";
        
    }
        void setPlayer(Entity *player)
        {
            this->player = player;
        }

    std::vector<glm::vec3> loadVerticesInYRange(const std::string& filename, Entity *wall, float minY = 2.5f, float maxY = 3.5f) {
        std::vector<glm::vec3> vertices;

        std::ifstream file(filename);
        glm::mat4 wallMatrix = wall->getLocalToWorldMatrix();
        if (!file.is_open()) {
            std::cerr << "Failed to open " << filename << std::endl;
            return vertices; // empty
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.substr(0, 2) == "v ") {
                std::istringstream iss(line.substr(2));
                float x, y, z;
                iss >> x >> y >> z;

            glm::vec4 localVertex(x, y, z, 1.0f);
            glm::vec4 transformed = wallMatrix * localVertex;
                
            if (transformed.y >= minY && transformed.y <= maxY) {
                vertices.emplace_back(transformed.x, transformed.y, transformed.z);
            }
            }
        }

        return vertices;
    }

        // bool checkCollision(Entity* objectComponent, Entity* playerComponent) 
        // {
        //     // Get player's world position and forward direction
        //     glm::vec3 playerPos = playerComponent->localTransform.position;
            
        //     glm::vec3 playerForward = glm::normalize(glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 0)));
        
        //     // Get object's world position
        //     glm::vec3 objectPos = objectComponent->localTransform.position;
        //     objectPos.y +=3.0f; // Ignore vertical position
        //     // Vector from player to object
        //     glm::vec3 toObject = objectPos - playerPos;
        
        //     // Project 'toObject' onto the player's forward direction
        //     float forwardDistance = glm::dot(toObject, playerForward);
        
        //     // If the object is behind the player (negative distance), ignore it
        //     if (forwardDistance <= 0) 
        //         return false;
        
        //     // Maximum allowed forward distance for collision
        //     float maxForwardDistance = 1.0f; // Adjust as needed
        //     if (forwardDistance > maxForwardDistance)
        //         return false;
        
        //     // Optional: Check if the object is within a certain angle (for a cone-shaped detection)
        //     glm::vec3 toObjectDir = glm::normalize(toObject);
        //     float angleDot = glm::dot(playerForward, toObjectDir);
        
        //     // Require the object to be mostly in front (adjust threshold as needed)
        //     return angleDot > 0.7f; // ~45-degree cone
        // }
        bool checkCollision(Entity* objectComponent, Entity* playerComponent) 
        {
            // Get positions
            glm::vec3 playerPos = playerComponent->localTransform.position;
            glm::vec3 objectPos = objectComponent->localTransform.position;
            objectPos.y +=3.0f; // Ignore vertical position
            // Calculate distance between player and object
            glm::vec3 toObject = objectPos - playerPos;
            float distance = glm::distance(playerPos, objectPos);
            
            // Define collision threshold (sum of player and object radii)
            float collisionThreshold = 1.5f; // Adjust based on your object sizes
            
            if (distance > collisionThreshold)
                return false;
            
            // For more accurate collision, you might want to:
            // 1. Use bounding boxes/spheres
            // 2. Consider object orientation and size
            
            // If you specifically want to check if player is facing the object:
            glm::vec3 playerForward = glm::normalize(playerComponent->getLocalToWorldMatrix()* glm::vec4(0, 0, -1, 0)); // You need to implement this
            glm::vec3 toObjectDir = glm::normalize(toObject);
            
            float facingDot = glm::dot(playerForward, toObjectDir);
            
            // Only consider it a collision if player is close AND facing the object
            // Adjust threshold (0.7f means ~45 degree cone in front of player)
            return facingDot > 0.5f||facingDot < -0.5f; 
        }

        bool checkWallCollision1(Entity* playerComponent)
        {
            glm::vec3 playerPos = playerComponent->localTransform.position;
            glm::vec3 playerForward = glm::normalize(glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 0)));
        
            float collisionThreshold = 1.5f; // distance threshold
            float angleThreshold = 0.7f;     // dot product threshold (~60 degrees cone)
        
            for (const auto& vertex : vertices)
            {
                glm::vec3 toVertex = vertex - playerPos;
                float distance = glm::length(toVertex);
        
                if (distance <= collisionThreshold)
                {
                    glm::vec3 toVertexDir = glm::normalize(toVertex);
                    float facingDot = glm::dot(playerForward, toVertexDir);
        
                    if (facingDot > angleThreshold )
                    {
                        return true; // Collision: close and in front (or behind, if you want both)
                    }
                }
            }
        
            return false; // No collision
        }
        
        bool checkWallCollision2(Entity* playerComponent)
        {

            glm::vec3 playerPos = playerComponent->localTransform.position;
            glm::vec3 playerForward = glm::normalize(glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 0)));
        
            float collisionThreshold = 1.5f; // distance threshold
            float angleThreshold = 0.7f;     // dot product threshold (~60 degrees cone)
        
            for (const auto& vertex : vertices2)
            {
                glm::vec3 toVertex = vertex - playerPos;
                float distance = glm::length(toVertex);
        
                if (distance <= collisionThreshold)
                {
                    // std::cout << distance << std::endl;
                    glm::vec3 toVertexDir = glm::normalize(toVertex);
                    float facingDot = glm::dot(playerForward, toVertexDir);
        
                    if (facingDot > angleThreshold )
                    {
                        
                        return true; // Collision: close and in front (or behind, if you want both)
                    }
                }
            }
        
            return false; // No collision
        }
        
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
                if ((entity->name == "monster" ) && checkCollision(entity, player))
                {
                    return 1;
                }
                if ((entity->name == "wall" ) && checkWallCollision1(player))
                {
                    return 2;
                }
                if ((entity->name == "wall2" ) && checkWallCollision2(player))
                {
                    return 2;
                }
            }
            return 0;
        }
       
    };

}