#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <components/arrow.h>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State {
    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets")) {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world")) {
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float) deltaTime);
        cameraController.update(&world, (float) deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto &keyboard = getApp()->getKeyboard();

        if (keyboard.justPressed(GLFW_KEY_ESCAPE)) {
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }

    void createArrow(glm::vec3 camera_position, glm::vec3 camera_front_direction) {
        our::Entity *arrow_entity = world.add();
        auto arrow_config = getApp()->getConfig()["arrow_config"];

        // Set transform
        arrow_entity->localTransform.position = camera_position;
        if (arrow_config.contains("scale")) {
            auto scaleArray = arrow_config["scale"];
            arrow_entity->localTransform.scale = glm::vec3(
                scaleArray[0].get<float>(),
                scaleArray[1].get<float>(),
                scaleArray[2].get<float>()
            );
        }
        glm::vec3 dir = glm::normalize(camera_front_direction);
        float yaw = atan2(dir.x, dir.z);
        float roll = -asin(dir.y);
        float correction = glm::half_pi<float>();
        arrow_entity->localTransform.rotation = glm::vec3(0, yaw + correction, roll);

        // Add movement component
        auto *movement = arrow_entity->addComponent<our::MovementComponent>();
        movement->linearVelocity = camera_front_direction * arrow_config["speed"].get<float>();
        if (arrow_config.contains("angularVelocity")) {
            auto scaleArray = arrow_config["angularVelocity"];
            movement->angularVelocity = glm::vec3(
                scaleArray[0].get<float>(),
                scaleArray[1].get<float>(),
                scaleArray[2].get<float>()
            );
        }

        // Add rendering
        auto *renderer = arrow_entity->addComponent<our::MeshRendererComponent>();
        renderer->mesh = our::AssetLoader<our::Mesh>::get("arrow");
        renderer->material = our::AssetLoader<our::Material>::get("arrow_material");

        // Add Arrow Component
        arrow_entity->addComponent<our::ArrowComponent>();
    }

    void onMouseButtonEvent(int button, int action, int mods) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            createArrow(cameraController.get_last_camera_postion(),
                        cameraController.get_last_front_direction());
        }
    }
};
