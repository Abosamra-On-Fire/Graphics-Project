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
        // Entity *player;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> vertices2;
        std::vector<glm::vec3> vertices3;
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
                vertices2 = loadVerticesInYRange("assets/models/ring.obj",wall);
            }
            if(entity->name == "hallway")
            {
                wall = entity;
                vertices3 = loadVerticesInYRange("assets/models/truss.obj",wall);
            }
        }
        std::cout << "Wall vertices: " << vertices.size() << ", Wall2 vertices: " << vertices2.size() << ", Hallway vertices: " << vertices3.size() << "\n";
        
    }
        // void setPlayer(Entity *player)
        // {
        //     this->player = player;
        // }

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
        std::vector<int> checkCollision(Entity* objectComponent, Entity* playerComponent) 
        {
            // std::cout<<"Collision check\n";
            glm::vec3 playerPos = playerComponent->localTransform.position;
            glm::vec3 objectPos = objectComponent->localTransform.position;
            objectPos.y += 3.0f; // Ignore vertical component
        
            glm::vec3 toObject = objectPos - playerPos;
            float distance = glm::distance(playerPos, objectPos);
        
            float collisionThreshold = 3.0f;
            std::vector<int> collisionType;
            if (distance > collisionThreshold)
                return collisionType;
            
            toObject.y = 0.0f;
            glm::vec3 toObjectDir = glm::normalize(toObject);
        
            // Get direction vectors
            glm::vec3 playerForward3D = glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 0));
            glm::vec3 playerRight3D   = glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(1, 0, 0, 0));
            
            // Project onto XZ plane (ignore Y)
            playerForward3D.y = 0.0f;
            playerRight3D.y = 0.0f;
            
            glm::vec3 playerForward = glm::normalize(playerForward3D);
            glm::vec3 playerRight   = glm::normalize(playerRight3D);
            
            float dotForward = glm::dot(playerForward, toObjectDir);
            float dotRight = glm::dot(playerRight, toObjectDir);
        
            // Forward
            if (dotForward > 0.7f) collisionType.push_back(2);
            // Backward
            else if (dotForward < -0.7f) collisionType.push_back(3);
            // Right
            else if (dotRight > 0.7f) collisionType.push_back(4);
            // Left
            else if (dotRight < -0.7f)  collisionType.push_back(5);
        
            return collisionType;
        }
        

        std::vector<int> checkWallCollision1(Entity* playerComponent)
        {
            glm::vec3 playerPos = playerComponent->localTransform.position;
            glm::vec3 playerForward3D = glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 0));
            glm::vec3 playerRight3D   = glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(1, 0, 0, 0));
            
            // Project onto XZ plane (ignore Y)
            playerForward3D.y = 0.0f;
            playerRight3D.y = 0.0f;
            std::vector<int>collisionType;
            glm::vec3 playerForward = glm::normalize(playerForward3D);
            glm::vec3 playerRight   = glm::normalize(playerRight3D);
            
            float collisionThreshold = 1.5f; // distance threshold
            float angleThreshold = 0.7f;     // dot product threshold (~60 degrees cone)
        
            for (const auto& vertex : vertices)
            {
                glm::vec3 toVertex = vertex - playerPos;
                float distance = glm::length(toVertex);
        
                if (distance <= collisionThreshold)
                {
                    toVertex.y = 0.0f;
                    glm::vec3 toVertexDir = glm::normalize(toVertex);
                    float facingDot = glm::dot(playerForward, toVertexDir);
                    float dotRight = glm::dot(playerRight, toVertexDir);
                    if (facingDot > angleThreshold )
                    {
                        std::cout<<"Collision front\n";
                        collisionType.push_back(2); 
                    }
                    else if(facingDot< -angleThreshold)
                    {
                        std::cout<<"Collision back\n";
                        collisionType.push_back(3);
                    }
                    else if(dotRight > angleThreshold)
                    {
                        std::cout<<"Collision right\n";
                        collisionType.push_back(4);
                    }
                    else if(dotRight < -angleThreshold)
                    {
                        std::cout<<"Collision left\n";
                        collisionType.push_back(5);
                    }

                }
            }
        
            return collisionType;
        }
        
        std::vector<int> checkWallCollision2(Entity* playerComponent)
        {

            glm::vec3 playerPos = playerComponent->localTransform.position;
            glm::vec3 playerForward3D = glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 0));
            glm::vec3 playerRight3D   = glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(1, 0, 0, 0));

            // Project onto XZ plane (ignore Y)
            playerForward3D.y = 0.0f;
            playerRight3D.y = 0.0f;
            std::vector<int>collisionType;
            glm::vec3 playerForward = glm::normalize(playerForward3D);
            glm::vec3 playerRight   = glm::normalize(playerRight3D);

            float collisionThreshold = 1.5f; // distance threshold
            float angleThreshold = 0.7f;     
        
            for (const auto& vertex : vertices2)
            {
                glm::vec3 toVertex = vertex - playerPos;
                toVertex.y = 0.0f;
                float distance = glm::length(toVertex);
        
                if (distance <= collisionThreshold)
                {
                    // std::cout << distance << std::endl;
                    glm::vec3 toVertexDir = glm::normalize(toVertex);
                    float facingDot = glm::dot(playerForward, toVertexDir);
                    float dotRight = glm::dot(playerRight, toVertexDir);
                    if (facingDot > angleThreshold )
                    {
                        
                        collisionType.push_back(2);
                    }
                    else if(facingDot< -angleThreshold)
                    {
                        collisionType.push_back(3);
                    }
                    else if(dotRight > angleThreshold)
                    {
                        collisionType.push_back(4);
                    }
                    else if(dotRight < -angleThreshold)
                    {
                        collisionType.push_back(5);
                    }

                }
            }
        
            return collisionType;
        }
        std::vector<int> checkWallCollision3(Entity* playerComponent)
        {

            glm::vec3 playerPos = playerComponent->localTransform.position;
            glm::vec3 playerForward3D = glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 0));
            glm::vec3 playerRight3D   = glm::vec3(playerComponent->getLocalToWorldMatrix() * glm::vec4(1, 0, 0, 0));
            
            // Project onto XZ plane (ignore Y)
            playerForward3D.y = 0.0f;
            playerRight3D.y = 0.0f;
            
            glm::vec3 playerForward = glm::normalize(playerForward3D);
            glm::vec3 playerRight   = glm::normalize(playerRight3D);
            std::vector<int>collisionType;
            float collisionThreshold = 1.0f; // distance threshold
            float angleThreshold = 0.7f;     // dot product threshold (~60 degrees cone)
        
            for (const auto& vertex : vertices3)
            {
                glm::vec3 toVertex = vertex - playerPos;
                toVertex.y = 0.0f;
                float distance = glm::length(toVertex);
        
                if (distance <= collisionThreshold)
                {
                    // std::cout << distance << std::endl;
                    glm::vec3 toVertexDir = glm::normalize(toVertex);
                    float facingDot = glm::dot(playerForward, toVertexDir);
                    float dotRight = glm::dot(playerRight, toVertexDir);
                    if (facingDot > angleThreshold )
                    {
                        
                        collisionType.push_back(2);
                    }
                    else if(facingDot< -angleThreshold)
                    {
                        collisionType.push_back(3);
                    }
                    else if(dotRight > angleThreshold)
                    {
                        collisionType.push_back(4);
                    }
                    else if(dotRight < -angleThreshold)
                    {
                        collisionType.push_back(5);
                    }
                }
            }
        
            return collisionType; // No collision
        }
        
        std::vector<int> update(World *world, float deltaTime)
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
            std::vector<int> result;

            for(const auto &entity : world->getEntities())
            {
                //hazawed vector ylem kol dol ma3a ba3d aw yqaren hena we yraga3 vector wa7ed bs fehom 34an 
                // ele 7asel eno byreturn 3and el monster 8aleban
              
                if (entity->name == "monster" || entity->name == "monster2") {
                    auto collision = checkCollision(entity, player);
                    result.insert(result.end(), collision.begin(), collision.end());
                }
                else if (entity->name == "wall") {
                    auto collision = checkWallCollision1(player);
                    result.insert(result.end(), collision.begin(), collision.end());
                }
                else if (entity->name == "wall2") {
                    auto collision = checkWallCollision2(player);
                    result.insert(result.end(), collision.begin(), collision.end());
                }
                else if (entity->name == "hallway") {
                    auto collision = checkWallCollision3(player);
                    result.insert(result.end(), collision.begin(), collision.end());
                }
                
                // Remove duplicates to get only unique values
                std::sort(result.begin(), result.end());
                result.erase(std::unique(result.begin(), result.end()), result.end());
                
                
            }

            return result;
        }
       
    };

}