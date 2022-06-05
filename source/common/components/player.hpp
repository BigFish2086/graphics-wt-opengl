#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>

namespace our {

class PlayerComponent: public Component {
public:
    float health = 100.0f;

    // 0 = 
    int state = 4;

    // The ID of this component type is "Camera"
    static std::string getID() { return "Player"; }

    // Reads camera parameters from the given json object
    void deserialize(const nlohmann::json &data) override;

    // Creates and returns the camera view matrix
    glm::mat4 getViewMatrix() const;

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 getProjectionMatrix(glm::ivec2 viewportSize) const;
};

} // namespace our

