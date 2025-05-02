// source/common/systems/timer-system.hpp
#pragma once
#include <ecs/world.hpp>
#include "../components/timer-component.hpp"

class TimerSystem : public our::System {
public:
    void update(our::World* world, float deltaTime) override {
        for(auto entity : world->getEntities()){
            if(auto timer = entity->getComponent<TimerComponent>()){
                if(timer->isRunning) {
                    timer->timeRemaining -= deltaTime;
                    if(timer->timeRemaining <= 0) {
                        if(timer->isRepeating) {
                            timer->timeRemaining = timer->initialTime;
                        } else {
                            timer->isRunning = false;
                        }
                    }
                }
            }
        }
    }
};