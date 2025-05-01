#pragma once

#include "../ecs/world.hpp"
#include "../ecs/entity.hpp"

namespace our
{
    class ArrowCollisionSystem
    {
        std::unordered_set<Entity*> monster_entities;
        std::unordered_map<Entity*,int> arrow_entities;
        bool initialized = false;
    public:
        void update(World* world);
        void initialize(World* world);
        float distanceBetweenTwoPoints(glm::vec3 point1, glm::vec3 point2);
        void addArrow(Entity*arrow_entity);
    };
}