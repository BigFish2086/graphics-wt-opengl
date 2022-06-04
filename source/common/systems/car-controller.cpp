#pragma once

#include "../components/car.hpp"
#include "../ecs/world.hpp"

#include "../application.hpp"
#include "components/free-camera-controller.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>

namespace our {

class CarController {
    Application *app;
    bool mouse_locked = false;

public:
    void enter(Application *app) { this->app = app; }

    void update(World *world, float deltaTime) {

        // get a car component and a free camera controller component
        CarComponent *car = nullptr;
        FreeCameraControllerComponent *controller = nullptr;
        for (auto entity : world->getEntities()) {
            car = entity->getComponent<CarComponent>();
            controller = entity->getComponent<FreeCameraControllerComponent>();
            if (car && controller) {
                break;
            }
        }

        // if no cars found, return
        if (!(car && controller)) {
            return;
        }

        // get the car owner entity
        Entity *entity = car->getOwner();

        if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked) {
            app->getMouse().lockMouse(app->getWindow());
            mouse_locked = true;

        } else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked) {
            app->getMouse().unlockMouse(app->getWindow());
            mouse_locked = false;
        }

        glm::vec3 &position = entity->localTransform.position;
        glm::vec3 &rotation = entity->localTransform.rotation;
        glm::vec3 &scale = entity->localTransform.scale;

        if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1)) {
            glm::vec2 delta = app->getMouse().getMouseDelta();
            rotation.x -= delta.y * controller->rotationSensitivity;
            rotation.y -= delta.x * controller->rotationSensitivity;
        }

        if (rotation.x < -glm::half_pi<float>() * 0.99f) {
            rotation.x = -glm::half_pi<float>() * 0.99f;
        }
        if (rotation.x > glm::half_pi<float>() * 0.99f) {
            rotation.x = glm::half_pi<float>() * 0.99f;
        }

        rotation.y = glm::wrapAngle(rotation.y);

        glm::mat4 matrix = entity->localTransform.toMat4();

        glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)), up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                  right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

        glm::vec3 current_sensitivity = controller->positionSensitivity;

        // shift == speeding up
        if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) {
            current_sensitivity *= controller->speedupFactor;
        }

        // w, s == forward, backward
        if (app->getKeyboard().isPressed(GLFW_KEY_W)) {
            position += front * (deltaTime * current_sensitivity.z);
        }
        if (app->getKeyboard().isPressed(GLFW_KEY_S)) {
            position -= front * (deltaTime * current_sensitivity.z);
        }

        // q, e == up and down
        if (app->getKeyboard().isPressed(GLFW_KEY_Q)) {
            position += up * (deltaTime * current_sensitivity.y);
        }
        // if (app->getKeyboard().isPressed(GLFW_KEY_E)) {
        //     position -= up * (deltaTime * current_sensitivity.y);
        // }

        // a, d == left and right
        if (app->getKeyboard().isPressed(GLFW_KEY_D)) {
            position += right * (deltaTime * current_sensitivity.x);
        }
        if (app->getKeyboard().isPressed(GLFW_KEY_A)) {
            position -= right * (deltaTime * current_sensitivity.x);
        }
    }

    void exit() {
        if (mouse_locked) {
            mouse_locked = false;
            app->getMouse().unlockMouse(app->getWindow());
        }
    }
};

} // namespace our
