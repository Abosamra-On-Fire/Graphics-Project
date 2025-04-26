#include "arrow.h"
#include<iostream>
namespace our {
    void ArrowComponent::deserialize(const nlohmann::json& data) {
        if (!data.is_object()) return;
        std::cout << "Deserializing ArrowComponent i dont know why we should not call it " << std::endl;
        }
}
