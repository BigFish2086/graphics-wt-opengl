#include "light.hpp"
#include "deserialize-utils.hpp"
#include <glm/glm.hpp>

namespace our
{
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
        {
            return;
        }
        std::string lightTypeStr = data.value("lightType", "directional");
        if (lightTypeStr == "directional")
        {
            lightType = LightType::DIRECTIONAL;
        }
        else if (lightTypeStr == "spot")
        {
            lightType = LightType::SPOT;
        }
        else{
            lightType = LightType::POINT;
        }

        
        diffuse = data.value("diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
        specular = data.value("specular", glm::vec3(0.0f, 0.0f, 0.0f));
        attenuation = data.value("attenuation", glm::vec3(0.0f, 0.0f, 0.0f));
        cone_angles = data.value("cone_angles", glm::vec2(0.0f, 0.0f));
    }
} // namespace our
