// source/common/components/timer-component.hpp
#pragma once

struct TimerComponent {
    float timeRemaining;
    bool isRunning = true;
    bool isRepeating = false;
    float initialTime;
    
    TimerComponent(float duration = 60.0f, bool repeat = false) 
        : timeRemaining(duration), initialTime(duration), isRepeating(repeat) {}
};