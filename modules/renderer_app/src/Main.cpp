#include "common/Average.h"
#include "common/TimeProfiler.h"
#include "core/BlinnPhong.h"
#include "core/DirectionalLight.h"
#include "core/Mesh.h"
#include "core/OrthogonalCamera.h"
#include "core/PerspectiveCamera.h"
#include "core/PointLight.h"
#include "core/Scene.h"
#include "core/Sphere.h"
#include "mesh/MeshGenerator.h"
#include "rasterizer/RasterizerRenderer.h"
#include "rasterizer/RasterizerRendererParallel.h"
#include "RasterizerShaderFactory.h"
#include "ray_tracer/RayTraceRenderer.h"
#include "RayTracerShaderFactory.h"
#include "SdlScreen.h"
#include "ShapeFactory.h"

#include "SDL2/SDL.h"

#include <iostream>
#include <memory>

using namespace cg;
using namespace cg::angle_literals;

constexpr int screenWidth = 1200;
constexpr int screenHeight = 800;

template <typename Renderer, template <typename ShapeType> typename ShaderFactory, typename Screen>
void runApp(Screen& screen);
static void quit(int code);

int main(int argc, char* argv[]) {
    SDL_Window* window = SDL_CreateWindow("renderer_app", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth,
                                          screenHeight, 0);
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

    runApp<RayTraceRenderer, RayTracerShaderFactory>(screen);

    quit(0);
    return 0;
}

template <typename Renderer, template <typename ShapeType> typename ShaderFactory, typename Screen>
void runApp(Screen& screen) {
    // Prepare scene
    Scene scene;
    scene.setAmbientLight(0.05f * Color::white());

    std::unique_ptr<PerspectiveCamera> camera = std::make_unique<PerspectiveCamera>();
    camera->setResolution(Camera::Resolution(screenWidth, screenHeight));
    camera->setPosition(Point(0, 0, 0));
    camera->setViewDirection(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
    camera->setViewPlaneDistance(0.5f);
    camera->setViewPlaneSize(Size2d(screenWidth, screenHeight));
    camera->setFieldOfView(80_deg);
    camera->update();

    std::unique_ptr<PointLight> pointLight1 = std::make_unique<PointLight>(250 * Color::white());
    pointLight1->setPosition(Point(-3, 6, 6));

    std::unique_ptr<PointLight> pointLight2 = std::make_unique<PointLight>(100 * Color::white());
    pointLight2->setPosition(Point(-4, 4, -8));

    std::unique_ptr<Sphere> sphere1 = ShapeFactory::create<Sphere, ShaderFactory>(2.0f);
    sphere1->setPosition(Point(10, 0, -3));
    sphere1->setAmbientReflectance(Color::red());
    sphere1->setMaterial(std::make_unique<BlinnPhong>(Color::red(), 0.4f * Color::white(), 32, 0.3f * Color::white()));
    sphere1->update();

    std::unique_ptr<Sphere> sphere2 = ShapeFactory::create<Sphere, ShaderFactory>(1.0f);
    sphere2->setPosition(Point(6, 1.7f, 0.5f));
    sphere2->setAmbientReflectance(Color::green());
    sphere2->setMaterial(
        std::make_unique<BlinnPhong>(Color::green(), 0.4f * Color(1, 1, 1), 32, 0.3f * Color::white()));
    sphere2->update();

    std::unique_ptr<Sphere> sphere3 = ShapeFactory::create<Sphere, ShaderFactory>(20.0f);
    sphere3->setPosition(Point(0, 5, 0));
    sphere3->setAmbientReflectance(Color::blue());
    sphere3->setMaterial(std::make_unique<BlinnPhong>(Color::blue(), 0.2f * Color::white(), 32, 0.3f * Color::white()));
    sphere3->update();

    std::unique_ptr<Mesh> floor =
        ShapeFactory::create<Mesh, ShaderFactory>(MeshGenerator::generateRectangle({100, 1000}, 10, 10));
    floor->setRotation(-90_deg, 0_deg, 0_deg);
    floor->setPosition(Point(0, -2, 0));
    floor->update();
    floor->setMaterial(std::make_unique<BlinnPhong>(0.5f * Color::white(), Color::black(), 1, 0.3f * Color::white()));
    floor->update();

    scene.addShape(std::move(sphere1));
    scene.addShape(std::move(sphere2));
    // scene.addShape(std::move(sphere3));
    scene.addShape(std::move(floor));
    scene.addLight(std::move(pointLight1));
    scene.addLight(std::move(pointLight2));
    scene.setCamera(std::move(camera));

    // Prepare render loop
    Renderer renderer;

    float timeElapsed = 0;
    float fps = 0;
    auto averageCalc = averageOf(5);
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
        screen.clear();

        renderer.renderScene(scene, screen);

        screen.flush();

        uint64_t timeToRenderFrame = profiler.timeAndReset().count();
        averageCalc.addEntry(timeToRenderFrame / 1000.0f);

        timeElapsed += timeToRenderFrame;
        if (timeElapsed > 1000000) {
            fps = 1000 / averageCalc.average();
            timeElapsed = 0;
            std::cout << "AVR: " << averageCalc.average() << ", FPS: " << fps << std::endl;
        }
    }
}

void quit(int code) {
    SDL_Quit();
    if (code) {
        exit(code);
    }
}
