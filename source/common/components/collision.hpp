#pragma once

#include "../ecs/component.hpp"

#include "../material/material.hpp"
#include "../mesh/mesh.hpp"
#include <glm/mat4x4.hpp>

namespace our {

class CollisionComponent : public Component {
public:
    std::string obstacleType = "";
    int obstacleEffect = 1;
    bool taken = false;

    // The ID of this component type is "Camera"
    static std::string getID() { return "Collision"; }

    // Reads camera parameters from the given json object
    void deserialize(const nlohmann::json &data) override;

    // Creates and returns the camera view matrix
    glm::mat4 getViewMatrix() const;

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 getProjectionMatrix(glm::ivec2 viewportSize) const;
};

} // namespace our
