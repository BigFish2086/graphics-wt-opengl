#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>

namespace our
{
    enum class LightType
    {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class LightComponent : public Component
    {
    public:
        LightType lightType;
        glm::vec3 direction;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 attenuation;
        glm::vec2 cone_angles;

        static std::string getID() { return "Light"; }

        
        void deserialize(const nlohmann::json &data) override;

        glm::vec3 getDirection() const;

    };

} // namespace our
