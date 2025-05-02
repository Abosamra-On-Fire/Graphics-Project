#include "arrow-collision-system.h"
#include <iostream>

#include "components/arrow.h"
glm::vec3 constant_monster_center_correction = {0.1387785, 4.841342, 0.2150395};
const int ARROW_MAX_LIFETIME_FRAMES= 200;
namespace our {
    void ArrowCollisionSystem::initialize(World *world) {
        for (auto entity: world->getEntities())
            if (entity->name == "monster") {
                monster_entities.insert({entity,0});
            }
        // std::cout<<"inserted"<<monster_entities.size()<<"monsters"<<std::endl;
        initialized = true;
    }

    float ArrowCollisionSystem::distanceBetweenTwoPoints(glm::vec3 point1, glm::vec3 point2) {
        return glm::length(point1 - point2);
    }

    void ArrowCollisionSystem::addArrow(Entity *arrow_entity) {
        arrow_entities.insert({arrow_entity,0});
    }

    void ArrowCollisionSystem::update(World* world) {
        static int number_of_hits = 0;
        if (!initialized) {
            initialize(world);
        }

        // Collect arrows to remove first
        std::vector<Entity*> arrows_to_remove;

        // First pass: detect collisions and expired arrows
        for (auto& arrow_entity : arrow_entities) {
            // Check collision with monsters
            bool hit=false;
            for (auto&monster_entity : monster_entities) {
                float distance = distanceBetweenTwoPoints(
                    arrow_entity.first->localTransform.position,
                    monster_entity.first->localTransform.position + constant_monster_center_correction
                );

                if (distance < 6.75f * monster_entity.first->localTransform.scale.x) {
                    arrows_to_remove.push_back(arrow_entity.first);
                    hit=true;
                    break;
                }
            }
            if(hit)
                std::cout << "Arrow hit monster! " << ++number_of_hits << " times" << std::endl;
            else std::cout << "Arrow did not hit monster! " <<std::endl;
        }
        for (auto& arrow_pair : arrow_entities) {
            if(++arrow_pair.second>ARROW_MAX_LIFETIME_FRAMES)
                arrows_to_remove.push_back(arrow_pair.first);
        }
        // Second pass: perform removals
        for (auto arrow : arrows_to_remove) {
            world->markForRemoval(arrow);
            arrow_entities.erase(arrow);
        }
        // Update frame counters for surviving arrows
        world->deleteMarkedEntities();
    }
}
