#include "car.hpp"
#include "../ecs/entity.hpp"
#include "asset-loader.hpp"
#include "deserialize-utils.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace our {
void CarComponent::deserialize(const nlohmann::json &data) {
    if (!data.is_object())
        return;
    MeshRendererComponent::deserialize(data);
}

glm::mat4 CarComponent::getViewMatrix() const {
    auto owner = getOwner();
    auto M = owner->getLocalToWorldMatrix();

    auto eye = glm::vec3(0.0f);
    auto center = glm::vec3(0.0f, 0.0f, -1.0f);
    auto up = glm::vec3(0.0f, 1.0f, 0.0f);

    eye = glm::vec3(M * glm::vec4(eye, 1.0f));
    center = glm::vec3(M * glm::vec4(center, 1.0f));
    up = glm::vec3(M * glm::vec4(up, 0.0f));
    return glm::lookAt(eye, center, up);
}

} // namespace our
