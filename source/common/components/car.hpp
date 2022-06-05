#pragma once

#include "../ecs/component.hpp"

#include "mesh-renderer.hpp"
#include <glm/mat4x4.hpp>

namespace our {

class CarComponent : public MeshRendererComponent {
public:
    // car velocity ==> from the Movment component
    // mesh, material ==> from the Mesh Renderer

    // The ID of this component type is "Camera"
    static std::string getID() { return "Car"; }

    // Reads camera parameters from the given json object
    void deserialize(const nlohmann::json &data) override;

    // Creates and returns the camera view matrix
    glm::mat4 getViewMatrix() const;

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 getProjectionMatrix(glm::ivec2 viewportSize) const;
};

} // namespace our
