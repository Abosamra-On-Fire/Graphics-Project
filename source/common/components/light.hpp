#pragma once

#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include <glm/glm.hpp>



#include "../ecs/component.hpp"
namespace our
{
    class lighting : public Component
    {
    public:
        int lightType;
        glm::vec3 color = glm::vec3(1.0f);
        static std::string getID() { return "lighting"; }
        void deserialize(const nlohmann::json &data) override
        {
            if (!data.is_object())
                return;
           
            lightType = data.value("lightType", lightType);
            color = data.value("color", color);
            // printf("Light Component : color : %f %f %f",color.x,color.y,color.z);
        }
    };
}
