#include "arrow-collision-system.h"
#include <iostream>

#include "components/arrow.h"
glm::vec3 constant_monster_center_correction = {0.1387785, 4.841342, 0.2150395};
const int ARROW_MAX_LIFETIME_FRAMES= 250;
float MONSTER_BOUNDING_SPHERE_RADIUS = 6.75;
namespace our {
    void ArrowCollisionSystem::initialize(World *world) {
        monster_entities.clear();
        arrow_entities.clear();
        for (auto entity: world->getEntities())
            if (entity->name.find("monster")!=std::string::npos) {
                float scale = entity->localTransform.scale.x;
                int numberOfHitsNeeded;
                if(scale<1)
                    numberOfHitsNeeded=1;
                else if(scale<2)
                    numberOfHitsNeeded=2;
                else numberOfHitsNeeded=3;
                monster_entities.insert({entity,numberOfHitsNeeded});
            }
        number_of_hits=0;
    }

    float ArrowCollisionSystem::distanceBetweenTwoPoints(glm::vec3 point1, glm::vec3 point2) {
        return glm::length(point1 - point2);
    }

    void ArrowCollisionSystem::addArrow(Entity *arrow_entity) {
        arrow_entities.insert({arrow_entity,0});
    }

    bool ArrowCollisionSystem::killedAllMonsters() {
        return monster_entities.empty();
    }

    void ArrowCollisionSystem::update(World* world) {
        // Collect arrows to remove first
        std::vector<Entity*> arrows_to_remove;
        // Collect monsters to remove
        std::vector<Entity*> monsters_hit;
        // First pass: detect collisions and expired arrows
        for (auto& arrow_entity : arrow_entities) {
            // Check collision with monsters
            bool hit=false;
            for (auto&monster_entity : monster_entities) {
                float distance = distanceBetweenTwoPoints(
                    arrow_entity.first->localTransform.position,
                    monster_entity.first->localTransform.position + constant_monster_center_correction
                );

                if (distance < MONSTER_BOUNDING_SPHERE_RADIUS * monster_entity.first->localTransform.scale.x) {
                    arrows_to_remove.push_back(arrow_entity.first);
                    monsters_hit.push_back(monster_entity.first);
                    hit=true;
                    break;
                }
            }
            if(hit)
                std::cout << "Arrow hit monster! " << ++number_of_hits << " times" << std::endl;
        }
        //monster
        for(auto& monster_entity : monsters_hit){
            if(monster_entities[monster_entity]==3&&monster_entities.size()>1){
                continue;
            }
            if(--monster_entities[monster_entity]==0) {
                monster_entities.erase(monster_entity);
                world->markForRemoval(monster_entity);
            }
        }
        //Arrow
        for (auto& arrow_pair : arrow_entities) {
            if(++arrow_pair.second>ARROW_MAX_LIFETIME_FRAMES) {
                arrows_to_remove.push_back(arrow_pair.first);
                std::cout<<"Removed arrow because of max life time frames"<<std::endl;
            }
        }
        // Second pass: perform removals
        for (auto arrow : arrows_to_remove) {
            world->markForRemoval(arrow);
            arrow_entities.erase(arrow);
        }
        // Update frame after removing monsters/arrows
        world->deleteMarkedEntities();
    }
}
