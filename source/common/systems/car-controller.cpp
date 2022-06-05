#pragma once

#include "../components/car.hpp"
#include "../ecs/world.hpp"

#include "../application.hpp"
#include "components/free-camera-controller.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>

#include <iostream>

namespace our {

class CarController {
    Application *app;
    bool mouse_locked = false;

public:
    bool jumped = false;
    double seconds = 1;
    double jumpTime = 0.5;

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
        glm::vec3 current_sensitivity = controller->positionSensitivity;
        glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)), up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                  right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

        // shift == speeding up
        if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT)) {
            current_sensitivity *= controller->speedupFactor;
        }
        // w = jump
        if (app->getKeyboard().isPressed(GLFW_KEY_W)) {
            jumped = true;
            seconds = 1;
            position.y = 1.5;
        }
        // a, d == left and right
        if (app->getKeyboard().isPressed(GLFW_KEY_D) && position.x > -10.0f) {
            position += right * (deltaTime * current_sensitivity.x);
        }
        if (app->getKeyboard().isPressed(GLFW_KEY_A) && position.x < 10.f) {
            position -= right * (deltaTime * current_sensitivity.x);
        }
        if (jumped) {
            seconds -= deltaTime;
            if (seconds <= 0)
                jumped = false;
            if (jumpTime <= seconds)
                position.y += 0.1;
            else
                position.y -= 0.1;
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
