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

#include <iostream>

namespace our {

class CollisionController {
    Application *app;

public:
    void enter(Application *app) { this->app = app; }

    void update(World *world, bool &collision) {

        // get a player component
        PlayerComponent *player = nullptr;
        CarComponent *car = nullptr;
        for (auto entity : world->getEntities()) {
            if(!player) player = entity->getComponent<PlayerComponent>();
            if(!car) car = entity->getComponent<CarComponent>();
        }
        if (!(player && car)) {
            return;
        }
        Entity *pent = player->getOwner();
        Entity *cent = car->getOwner();

        std::cout<<"+++++++++++++++++++++++\n";
        std::cout<<player->health<<std::endl;

        glm::vec3 &playerScale = pent->localTransform.scale;
        //glm::vec3 &carPosition = cent->localTransform.position;
        glm::vec3 carPosition = cent->getGlobalPosition();
        glm::vec3 &carRotation = cent->localTransform.rotation;
        glm::vec3 &carScale = cent->localTransform.scale;

        // loop through all entities and check for collisions with collision components
        for (auto other : world->getEntities()) {
            auto otherCollision = other->getComponent<CollisionComponent>();
            if (otherCollision) {
                Entity *otherEntity = otherCollision->getOwner();
                glm::vec3 otherPosition = otherEntity->localTransform.position;

                // check if the two entities are close enough to collide
                float xdiff = glm::abs(carPosition.x - otherPosition.x);
                float ydiff = glm::abs(carPosition.y - otherPosition.y);

                // std::cout << "###############################"<<std::endl;
                // std::cout<<xdiff<<' ' <<ydiff<<std::endl;

                bool xclose = (xdiff <= 2.0f);
                bool yclose = (ydiff <= 0.3f);
                float carMaxX = carPosition.x + car->mesh->maxVertexX;
                float carMinX = carPosition.x + car->mesh->minVertexX;
                float collisionMaxX = otherPosition.x + other->getComponent<MeshRendererComponent>()->mesh->maxVertexX;
                float collisionMinX = otherPosition.x + other->getComponent<MeshRendererComponent>()->mesh->minVertexX;

                std::cout << "###############################"<<std::endl;
                std::cout << carPosition.x << ' ' << otherPosition.x << std::endl;
                // std::cout << collisionMaxX << ' ' << collisionMinX << std::endl;
                // std::cout << carPosition.z << ' ' << otherPosition.z << std::endl;

                int effect = otherCollision->obstacleEffect;

                if (!otherCollision->taken && xclose && glm::abs(carPosition.z - otherPosition.z) < 0.1f)
                {
                    otherCollision->taken = true;
                    if (otherCollision->obstacleType == "health") {
                        player->health += effect;
                        if (player->health > 100) {
                            player->health = 100;
                            playerScale.x = 1.0f;
                        } else {
                            playerScale.x = player->health /100.0 * 1.75;
                        }
                        carPosition.z += 20;
                        carPosition.y -= 20;
                    } else if (otherCollision->obstacleType == "barrier") {
                        player->health -= effect;
                        if (player->health <= 0) {
                            player->health = 0;
                            playerScale.x = 0.0f;
                            // TODO: game over
                            collision = true;
                        } else {
                            playerScale.x = player->health /100.0  * 1.75;
                        }
                    }
                }
            }
        }
    }

    void exit() {}
};

} // namespace our
