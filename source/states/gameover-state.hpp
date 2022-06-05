#pragma once

#include "../common/components/player.hpp"
#include "imgui.h"
#include "play-state.hpp"
#include <application.hpp>
#include <asset-loader.hpp>
#include <cstring>
#include <ecs/world.hpp>
#include <string>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>

// This state shows how to use the ECS framework and deserialization.
class GameOverState : public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::PlayerComponent *player;

    void onImmediateGui() override {
        int state = getApp()->state;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui::Begin("gameover State");

        ImGui::SetWindowFontScale(2.0f);

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));

        if (state == 0)
            ImGui::Text("Game Over");
        else if (state == 1) {
            ImGui::Text("Game Over");
            ImGui::Text("you failed to collect at least half the coins");
        } else if (state == 2) {
            ImGui::Text("wow, you collect all the coins");
        }

        ImGui::PopStyleColor();

        ImGui::Text(" ");
        ImGui::Text("Do you want to exit the game ?");
        ImGui::Text(" ");
        if (ImGui::Button("Yes") || getApp()->getKeyboard().isPressed(GLFW_KEY_ENTER)) {
            exit(1);
        }
        ImGui::End();
    }

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets")) {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("menu")) {
            world.deserialize(config["menu"]);
        }
    }

    void onDraw(double deltaTime) override {
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};
