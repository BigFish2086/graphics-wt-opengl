#include "player.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace our {
void PlayerComponent::deserialize(const nlohmann::json &data) {
    if (!data.is_object())
        return;
    this->health = data.value("health", health);
}

glm::mat4 PlayerComponent::getViewMatrix() const {
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
