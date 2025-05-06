#pragma once

#include <application.hpp>
#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/collision.hpp>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp> // For distance squared
#include <components/arrow.h>
#include <systems/arrow-collision-system.h>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State {
    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::CollisionSystem collisionSystem;

    our::TexturedMaterial *Timemat;
    our::Mesh *timerRectangle;
    our::TexturedMaterial *crosshairMat;
    our::Mesh *crosshairMesh;
    our::ArrowCollisionSystem arrowCollisionSystem;
    // Timer variables
    float timeRemaining;
    float totalTime;
    float printTimer = 0.0f;

    // Win position and threshold
    glm::vec3 winPosition = glm::vec3(0.0f, 1.0f, -85.0f);
    float winDistanceThreshold = 5.0f; // Distance threshold for winning

    void onInitialize() override {
        // Timer initialization (your existing code)
        timerRectangle = new our::Mesh({
                                           {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                                           {{1.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                                           {{1.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                                           {{0.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                                       }, {
                                           0, 1, 2,
                                           2, 3, 0
                                       });

        Timemat = new our::TexturedMaterial();
        Timemat->shader = new our::ShaderProgram();
        Timemat->shader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
        Timemat->shader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
        Timemat->shader->link();
        Timemat->texture = our::texture_utils::loadImage("assets/textures/timer.png");
        Timemat->tint = glm::vec4(1.0f);

        // Crosshair initialization
        crosshairMesh = new our::Mesh({
                                          {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                                          {{1.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
                                          {{1.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                                          {{0.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
                                      }, {
                                          0, 1, 2,
                                          2, 3, 0
                                      });

        crosshairMat = new our::TexturedMaterial();
        crosshairMat->shader = new our::ShaderProgram();
        crosshairMat->shader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
        crosshairMat->shader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
        crosshairMat->shader->link();
        crosshairMat->texture = our::texture_utils::loadImage("assets/textures/crosshair.png");
        crosshairMat->tint = glm::vec4(1.0f);

        // Rest of your initialization code...
        auto &config = getApp()->getConfig()["scene"];

        if (config.contains("time-limit")) {
            totalTime = config["time-limit"];
        } else {
            totalTime = 2000;
        }
        timeRemaining = totalTime;

        if (config.contains("assets")) {
            our::deserializeAllAssets(config["assets"]);
        }
        if (config.contains("world")) {
            world.deserialize(config["world"]);
        }
        cameraController.enter(getApp());
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);

        collisionSystem.init_collision_wall1(&world);
        arrowCollisionSystem.initialize(&world);
    }

    // void drawCrosshair() {
    //     glEnable(GL_BLEND);
    //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //     glm::ivec2 size = getApp()->getFrameBufferSize();
    //     glViewport(0, 0, size.x, size.y);

    //     glm::mat4 projection = glm::ortho(0.0f, (float) size.x, (float) size.y, 0.0f, -1.0f, 1.0f);

    //     // Crosshair size (assuming it's a square)
    //     float crosshairSize = 32.0f; // Adjust as needed

    //     // Center the crosshair
    //     glm::mat4 model = glm::mat4(1.0f);
    //     model = glm::translate(model, glm::vec3((size.x - crosshairSize) / 2.0f,
    //                                             (size.y - crosshairSize) / 2.0f,
    //                                             0.0f));
    //     model = glm::scale(model, glm::vec3(crosshairSize, crosshairSize, 1.0f));

    //     crosshairMat->setup();
    //     crosshairMat->shader->set("transform", projection * model);
    //     crosshairMesh->draw();

    //     glDisable(GL_BLEND);
    // }

    // void timerDraw(float timeRemaining) {
    //     glEnable(GL_BLEND);
    //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //     glm::ivec2 size = getApp()->getFrameBufferSize();
    //     glViewport(0, 0, size.x, size.y);

    //     glm::mat4 projection = glm::ortho(0.0f, (float) size.x, (float) size.y, 0.0f, -1.0f, 1.0f);

    //     float timerWidth = 324.0f;
    //     float timerHeight = 120.0f;
    //     float margin = 30.0f;

    //     glm::mat4 model = glm::mat4(1.0f);
    //     model = glm::translate(model, glm::vec3(size.x - (timerWidth + size.x) / 2, margin, 0.0f));
    //     model = glm::scale(model, glm::vec3(timerWidth, timerHeight, 1.0f));

    //     float timeRatio = timeRemaining / totalTime;
    //     Timemat->tint = glm::vec4(1.0f, timeRatio, timeRatio, 1.0f);

    //     Timemat->setup();
    //     Timemat->shader->set("transform", projection * model);
    //     timerRectangle->draw();

    //     glDisable(GL_BLEND);
    // }

    void drawCrosshair() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::ivec2 size = getApp()->getFrameBufferSize();
        glViewport(0, 0, size.x, size.y);

        glm::mat4 projection = glm::ortho(0.0f, (float) size.x, (float) size.y, 0.0f, -1.0f, 1.0f);

        // Crosshair size (assuming it's a square)
        float crosshairSize = 32.0f; // Adjust as needed

        // Center the crosshair
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3((size.x - crosshairSize) / 2.0f,
                                                (size.y - crosshairSize) / 2.0f,
                                                0.0f));
        model = glm::scale(model, glm::vec3(crosshairSize, crosshairSize, 1.0f));

        crosshairMat->setup();
        crosshairMat->shader->set("transform", projection * model);
        crosshairMesh->draw();

        glDisable(GL_BLEND);
    }

    void timerDraw(float timeRemaining) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glm::ivec2 size = getApp()->getFrameBufferSize();
        glViewport(0, 0, size.x, size.y);

        glm::mat4 projection = glm::ortho(0.0f, (float) size.x, (float) size.y, 0.0f, -1.0f, 1.0f);

        float timerWidth = 324.0f;
        float timerHeight = 120.0f;
        float margin = 30.0f;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(size.x - (timerWidth + size.x) / 2, margin, 0.0f));
        model = glm::scale(model, glm::vec3(timerWidth, timerHeight, 1.0f));

        float timeRatio = timeRemaining / totalTime;
        Timemat->tint = glm::vec4(1.0f, timeRatio, timeRatio, 1.0f);

        Timemat->setup();
        Timemat->shader->set("transform", projection * model);
        timerRectangle->draw();

        glDisable(GL_BLEND);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        int  collision_val=collisionSystem.update(&world, (float)deltaTime);
        bool is_collidied_forword=(collision_val==2);
        bool is_collidied_backword=(collision_val==3);
        bool is_collidied_right=(collision_val==4);
        bool is_collidied_left=(collision_val==5);
        cameraController.update(&world, (float)deltaTime,is_collidied_forword,is_collidied_backword,is_collidied_right,is_collidied_left);
        auto& keyboard = getApp()->getKeyboard();


        // And finally we use the renderer system to draw the scene
        renderer.render(&world);
        // We also run the arrow collision system to check for collisions
        arrowCollisionSystem.update(&world);
        // Draw HUD elements
        timerDraw(timeRemaining);
        drawCrosshair();

        // Search for the camera entity by component
        bool cameraFound = false;
        for (auto entity: world.getEntities()) {
            // Check if the entity has a CameraComponent
            if (entity->getComponent<our::CameraComponent>()) {
                glm::vec3 cameraPosition = entity->localTransform.position;
                // std::cerr << "Camera position: ("
                //         << cameraPosition.x << ", "
                //         << cameraPosition.y << ", "
                //         << cameraPosition.z << ")" << std::endl;
                cameraFound = true;

                // Check win condition
                float distance = glm::distance(cameraPosition, winPosition);
                if (distance < winDistanceThreshold) {
                    std::cerr << "WIN! Distance to target: " << distance << std::endl;
                    getApp()->changeState("win");
                    return;
                }
                break; // Exit loop after finding the first camera
            }
        }

        if (!cameraFound) {
            std::cerr << "WARNING: No camera entity found (missing CameraComponent)!" << std::endl;
        }

        // Timer and escape key logic...
        timeRemaining -= (float) deltaTime;
        if (timeRemaining <= 0) {
            getApp()->changeState("lose");
        }

        // auto &keyboard = getApp()->getKeyboard();
        if (keyboard.justPressed(GLFW_KEY_ESCAPE)) {
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override {
        renderer.destroy();
        cameraController.exit();
        world.clear();

        // Clean up crosshair resources
        delete crosshairMat->shader;
        delete crosshairMat;
        delete crosshairMesh;

        // Clean up timer resources
        delete Timemat->shader;
        delete Timemat;
        delete timerRectangle;

        our::clearAllAssets();
    }

    void createArrow(glm::vec3 camera_position, glm::vec3 camera_front_direction) {
        our::Entity *arrow_entity = world.add();
        auto arrow_config = getApp()->getConfig()["arrow_config"];

        // Set transform
        // Calculate offset vectors based on camera orientation
        glm::vec3 front = glm::normalize(camera_front_direction);
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0, 1, 0))); // World up vector is (0,1,0)
        glm::vec3 up = glm::normalize(glm::cross(right, front));
        // Calculate final spawn position
        glm::vec3 spawn_position = camera_position
                                   + right * arrow_config["right_offset"].get<float>()
                                   + up * arrow_config["up_offset"].get<float>()
                                   + front * arrow_config["forward_offset"].get<float>();

        // Set transform with offset position
        arrow_entity->localTransform.position = spawn_position;

        //Set scale
        if (arrow_config.contains("scale")) {
            auto scaleArray = arrow_config["scale"];
            arrow_entity->localTransform.scale = glm::vec3(
                scaleArray[0].get<float>(),
                scaleArray[1].get<float>(),
                scaleArray[2].get<float>()
            );
        }

        // Set rotation
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
        arrowCollisionSystem.addArrow(arrow_entity);
    }

    void onMouseButtonEvent(int button, int action, int mods) override {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            createArrow(cameraController.get_last_camera_postion(),
                        cameraController.get_last_front_direction());
        }
    }
};
