#pragma once

#include "../components/player.hpp"
#include "../ecs/world.hpp"

#include "../application.hpp"
#include "components/car.hpp"
#include "components/collision.hpp"
#include "components/free-camera-controller.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/trigonometric.hpp>

namespace our {

class CarController {
    Application *app;

public:
    void enter(Application *app) { this->app = app; }

    void update(World *world, float deltaTime) {

        // get a player component
        PlayerComponent *player = nullptr;
        CarComponent *car = nullptr;
        for (auto entity : world->getEntities()) {
            player = entity->getComponent<PlayerComponent>();
            car = entity->getComponent<CarComponent>();
            if (player && car) {
                break;
            }
        }
        if (!(player && car)) {
            return;
        }
        Entity *pent = player->getOwner();
        Entity *cent = car->getOwner();

        glm::vec3 &playerScale = pent->localTransform.scale;

        glm::vec3 &carPosition = cent->localTransform.position;
        glm::vec3 &carRotation = cent->localTransform.rotation;
        glm::vec3 &carScale = cent->localTransform.scale;

        // loop through all entities and check for collisions with collision components
        for (auto other : world->getEntities()) {
            auto otherCollision = other->getComponent<CollisionComponent>();
            if (otherCollision) {
                Entity *otherEntity = otherCollision->getOwner();
                glm::vec3 &otherPosition = otherEntity->localTransform.position;

                // check if the two entities are close enough to collide
                bool xclose = glm::abs(carPosition.x - otherPosition.x) <= 5.0f;
                bool yclose = glm::abs(carPosition.y - otherPosition.y) <= 3.0f;

                int effect = otherCollision->obstacleEffect;
                if (xclose && yclose) {
                    if (otherCollision->obstacleType == "health") {
                        player->health += effect;
                        if (player->health > 100) {
                            player->health = 100;
                            playerScale.x = 1.0f;
                        } else {
                            playerScale.x += player->health * 0.1f;
                        }
                    } else if (otherCollision->obstacleType == "barrier") {
                        player->health -= effect;
                        if (player->health <= 0) {
                            player->health = 0;
                            playerScale.x = 0.0f;
                            // TODO: game over
                        } else {
                            playerScale.x -= effect * 0.1f;
                        }
                    }
                    carPosition.x += 10;
                    break;
                }
            }
        }
    }

    void exit() {}
};

} // namespace our
