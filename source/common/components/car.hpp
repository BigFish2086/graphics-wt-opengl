#pragma once

#include "../ecs/component.hpp"

#include "../material/material.hpp"
#include "../mesh/mesh.hpp"
#include <glm/mat4x4.hpp>

namespace our {

class CarComponent : public Component {
public:
    // car velocity, mesh, material
    glm::vec3 velocity = glm::vec3(0,0,.01f);
    Mesh *mesh;
    Material *material;

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
