#include "common/Average.h"
#include "common/TimeProfiler.h"
#include "core/AmbientLight.h"
#include "core/BlinnPhong.h"
#include "core/DirectionalLight.h"
#include "core/Mesh.h"
#include "core/OrthogonalCamera.h"
#include "core/PerspectiveCamera.h"
#include "core/PointLight.h"
#include "core/Scene.h"
#include "core/Sphere.h"
#include "RayTraceRenderer.h"
#include "SdlScreen.h"

#include "SDL2/SDL.h"

#include <chrono>
#include <iostream>
#include <memory>

using namespace cg;
using namespace cg::angle_literals;

constexpr int screenWidth = 1200;
constexpr int screenHeight = 800;

static void quit(int code);

int main(int argc, char* argv[]) {
    SDL_Window* window =
        SDL_CreateWindow("raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, 0);
    if (window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create window and renderer: %s\n", SDL_GetError());
        quit(1);
    }

    auto result = SdlScreen::create(screenWidth, screenHeight, window);
    if (!result.has_value()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to create screen: %s\n", result.error().message().c_str());
        quit(1);
    }

    SdlScreen screen = std::move(result.value());

    // Prepare scene
    Scene scene;

    std::unique_ptr<PerspectiveCamera> camera = std::make_unique<PerspectiveCamera>();
    camera->setResolution(Camera::Resolution(screenWidth, screenHeight));
    camera->setPosition(Point(0, 0, 0));
    camera->setViewDirection(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
    camera->setViewPlaneDistance(0.5f);
    camera->setViewPlaneSize(Size2d(screenWidth, screenHeight));
    camera->setFieldOfView(80_deg);
    camera->update();

    constexpr Color ambientColor = Color(1, 1, 1);
    constexpr float ambientIntensity = 0.1f;
    std::unique_ptr<AmbientLight> ambientLight = std::make_unique<AmbientLight>(ambientIntensity * ambientColor);

    constexpr Color pointLightColor(1, 1, 1);
    constexpr float pointLightIntensity = 200;
    std::unique_ptr<PointLight> pointLight = std::make_unique<PointLight>(pointLightIntensity * pointLightColor);
    pointLight->setPosition(Point(-4, 8, 3));

    std::unique_ptr<Sphere> sphere1 = std::make_unique<Sphere>(2.0f);
    sphere1->setPosition(Point(10, 0, -3));
    sphere1->setAmbientReflectance(Color(1, 0, 0));
    sphere1->setMaterial(std::make_unique<BlinnPhong>(Color(1, 0, 0), Color(0.4, 0.4, 0.4), 32));

    std::unique_ptr<Sphere> sphere2 = std::make_unique<Sphere>(1.0f);
    sphere2->setPosition(Point(8, 1, 1));
    sphere2->setAmbientReflectance(Color(0, 1, 0));
    sphere2->setMaterial(std::make_unique<BlinnPhong>(Color(0, 1, 0), Color(0.4, 0.4, 0.4), 32));

    constexpr float floorWidth = 1000;
    constexpr float floorLength = 100;
    constexpr float floorHeight = -4;
    std::vector<Point> floorVertices = {{floorLength / 2, 0, -floorWidth / 2},
                                        {floorLength / 2, 0, floorWidth / 2},
                                        {-floorLength / 2, 0, floorWidth / 2},
                                        {-floorLength / 2, 0, -floorWidth / 2}};
    std::vector<Mesh::TriangleIndices> floorTriangles = {{0, 3, 2}, {0, 2, 1}};
    std::unique_ptr<Mesh> floor = std::make_unique<Mesh>(std::move(floorVertices), std::move(floorTriangles));
    floor->setPosition(Point(0, floorHeight, 0));
    floor->setMaterial(std::make_unique<BlinnPhong>(Color(0.5, 0.5, 0.5), Color(0, 0, 0), 1));

    scene.addShape(std::move(sphere1));
    scene.addShape(std::move(sphere2));
    scene.addShape(std::move(floor));
    scene.addLight(std::move(pointLight));
    scene.addLight(std::move(ambientLight));
    scene.setCamera(std::move(camera));

    // Prepare render loop
    RayTraceRenderer renderer;

    float timeElapsed = 0;
    float fps = 0;
    auto averageCalc = averageOf(20);
    TimeProfiler profiler;

    SDL_Event event;
    bool done = false;

    // Render loop
    while (!done) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    done = true;
                }
                break;
            case SDL_QUIT:
                done = true;
                break;
            }
        }

        renderer.renderScene(scene, screen);

        screen.flush();

        auto timeToRenderFrame = profiler.timeAndReset();
        averageCalc.addEntry(timeToRenderFrame / 1000.0f);

        timeElapsed += timeToRenderFrame;
        if (timeElapsed > 1000000) {
            fps = 1000 / averageCalc.average();
            timeElapsed = 0;
            std::cout << "AVR: " << averageCalc.average() << ", FPS: " << fps << std::endl;
        }
    }

    quit(0);
    return 0;
}

void quit(int code) {
    SDL_Quit();
    if (code) {
        exit(code);
    }
}
