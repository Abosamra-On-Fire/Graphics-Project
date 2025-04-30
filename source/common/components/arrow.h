#pragma once
#include "../ecs/component.hpp"
namespace our {
    class ArrowComponent : public Component {
    public:
        static std::string getID() { return "Arrow"; }
        void deserialize(const nlohmann::json& data) override;
    };
}