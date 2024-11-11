#include "GameManager.hpp"
#include "Audio.hpp"
#include "Component.hpp"
#include "ControllerManager.hpp"
#include "Entity.hpp"
#include "Event.hpp"
#include "InputManager.hpp"
#include "SDL_events.h"
#include "SDL_gamecontroller.h"
#include "SDL_mouse.h"
#include "SDL_render.h"
#include "ShadowFilter.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <algorithm>
#include <memory>
#include <random>

static std::vector<std::unique_ptr<Entity>> entities;

static bool running = true;
static bool resortNextFrame = false;
static Vector2f gameWindowSize = {0, 0};
static Vector2f currentWindowSize = {0, 0};
static float screen_change_scale = 0;
static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;
static bool updateEntities = true;
static float deltaTime = 0;
static Uint64 lastTime;
static ShadowFilter *shadowFilter;
static FPSmanager fpsManager;

// static void destroyEntity(Entity *entity);

#ifdef __EMSCRIPTEN__
EM_JS(void, resize_callback, (), {
  window.addEventListener(
      'resize',
      function() { _on_resize(window.innerWidth, window.innerHeight); });
});

extern "C" {
// This is a C++ function that is called from JavaScript
void EMSCRIPTEN_KEEPALIVE on_resize(int width, int height) {
  printf("Window resized to %dx%d\n", width, height);
  setWindowSize({static_cast<float>(width), static_cast<float>(height)});
}
}
#endif

namespace GameManager {

void init(Vector2f windowSize) {
  srand(time(nullptr));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Init(SDL_INIT_AUDIO);
  SDL_Init(SDL_INIT_TIMER);
  SDL_Init(SDL_INIT_GAMECONTROLLER);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  SDL_initFramerate(&fpsManager);
  SDL_setFramerate(&fpsManager, 200);

  gameWindowSize = windowSize;
  currentWindowSize = windowSize;

  screen_change_scale =
      ((float)currentWindowSize.x + (float)currentWindowSize.y) /
      (gameWindowSize.x + gameWindowSize.y);

  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

  window = SDL_CreateWindow("Charlie2D Game", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, currentWindowSize.x,
                            currentWindowSize.y,
                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_RenderSetLogicalSize(renderer, windowSize.x, windowSize.y);

  // IMGUI -------------------------------------------------------

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);

  // ImGui::StyleColorsLight();

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // -------------------------------------------------------------

  shadowFilter = &createEntity("ShadowFilter").addComponent<ShadowFilter>();
  shadowFilter->entity.setLayer(99);

#ifdef __EMSCRIPTEN__
  resize_callback();
  float width = emscripten_run_script_int("window.innerWidth");
  float height = emscripten_run_script_int("window.innerHeight");
  setWindowSize({width, height});
#endif
}

static void manageEvents() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type != SDL_MOUSEMOTION) {
      ImGui_ImplSDL2_ProcessEvent(&event);
    }

    InputManager::onEvent(event);

    if (event.type == SDL_QUIT) {
      quit();
      return;
    }

    // Controller
    else if (event.type == SDL_CONTROLLERDEVICEADDED) {
      ControllerManager::onControllerConnect(event.cdevice.which);
    }

    else if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
      ControllerManager::onControllerDisconnect(event.cdevice.which);
    }

    else if (event.type == SDL_CONTROLLERBUTTONDOWN) {
      ControllerManager::findController(event.cdevice.which)
          ->triggerButtons[event.cbutton.button] = true;
    }

    else if (event.type == SDL_CONTROLLERAXISMOTION) {
      ControllerManager::onStickEvent(event.cdevice.which, event.caxis.axis,
                                      event.caxis.value);
    }

    // Window
    else if (event.type == SDL_WINDOWEVENT) {
      switch (event.window.event) {
      case SDL_WINDOWEVENT_RESIZED:
        currentWindowSize.x = event.window.data1;
        currentWindowSize.y = event.window.data2;

        screen_change_scale =
            ((float)currentWindowSize.x + (float)currentWindowSize.y) /
            (gameWindowSize.x + gameWindowSize.y);

        // IMGUI
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize =
            ImVec2((float)currentWindowSize.x, (float)currentWindowSize.y);
        io.DisplayFramebufferScale =
            ImVec2(io.DisplaySize.x / gameWindowSize.x,
                   io.DisplaySize.y / gameWindowSize.y);
        // Change camera aspect ratio

        Event::fireEvent("screenResize");

        break;
      }
      break;
    }
  }
}

static void calculateDeltaTime() {
  Uint64 currentTime = SDL_GetPerformanceCounter();
  deltaTime =
      static_cast<float>((currentTime - lastTime) * 1000 /
                         static_cast<double>(SDL_GetPerformanceFrequency())) *
      0.001;

  if (deltaTime >= 1) {
    deltaTime = 0;
  }

  lastTime = currentTime;
  lastTime = SDL_GetPerformanceCounter();
}

void Update() {
  InputManager::update();
  ControllerManager::resetTriggerButtons();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  int mouseX = 0, mouseY = 0;
  SDL_GetMouseState(&mouseX, &mouseY);
  Vector2f realMouse = (Vector2f(mouseX, mouseY));
  io.MousePos = {realMouse.x, realMouse.y};

  manageEvents();
  calculateDeltaTime();

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  shadowFilter->resetShadowFilter();

  std::vector<int> entitesToRemove;
  for (std::unique_ptr<Entity> &entityPtr : entities) {
    Entity &entity = *entityPtr.get();
    if (entity.isQueuedForDestruction()) {
      entitesToRemove.push_back(entity.iid);
      continue;
    }
    if (entity.skipUpdate) {
      entity.skipUpdate = false;
      continue;
    }
    if (!entity.active)
      continue;

    entity.update();
  }

  if (resortNextFrame) {
    std::sort(entities.begin(), entities.end(), [](auto &a, auto &b) {
      if (a->getLayer() == b->getLayer())
        return a->iid < b->iid;
      return a->getLayer() < b->getLayer();
    });
  }

  for (int iid : entitesToRemove) {
    auto ne =
        std::remove_if(entities.begin(), entities.end(),
                       [iid](auto &entity) { return entity->iid == iid; });

    entities.erase(ne, entities.end());
  }

  SDL_RenderPresent(renderer);
  SDL_framerateDelay(&fpsManager);
}

void setWindowSize(Vector2f size) {
  if (window == nullptr) {
    std::cerr << "Window was nullptr" << std::endl;
    return;
  }

  SDL_SetWindowSize(window, static_cast<int>(size.x), static_cast<int>(size.y));
  currentWindowSize = size;

  screen_change_scale =
      ((float)currentWindowSize.x + (float)currentWindowSize.y) /
      (gameWindowSize.x + gameWindowSize.y);

  Event::fireEvent("screenResize");
}

void doUpdateLoop() {
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(Update, 0, 1);
#else
  while (running) {
    Update();
  }
#endif
}

void quit() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  Mix_Quit();
  SDL_Quit();

  entities.clear();

  running = false;
}

void destroyAll() {
  for (auto &entityPtr : entities) {
    if (entityPtr->iid == shadowFilter->entity.iid)
      continue;
    entityPtr->destroy();
  }
}

std::vector<Entity *> getEntities(std::string tag) {
  std::vector<Entity *> entitiesWithTag;
  for (auto &entityPtr : entities) {
    if (entityPtr->tag == tag) {
      entitiesWithTag.push_back(entityPtr.get());
    }
  }
  return entitiesWithTag;
}

std::vector<Entity *> getEntities() {
  std::vector<Entity *> entitiesWithoutOwnership;
  for (auto &entityPtr : entities) {
    entitiesWithoutOwnership.push_back(entityPtr.get());
  }
  return entitiesWithoutOwnership;
}

Entity &createEntity(std::string tag) {
  std::unique_ptr<Entity> entity = std::make_unique<Entity>();
  entity->tag = tag;

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100000); //
  entity->iid = dist(rng);

  Entity &entityRef = *entity.get();
  entities.push_back(std::move(entity));

  resortEntitiesNextFrame();
  return entityRef;
}

SDL_Renderer *getRenderer() { return renderer; }

SDL_Window *getWindow() { return window; }

float getDeltaTime() { return deltaTime; }

Vector2f getGameWindowSize() { return gameWindowSize; }

Vector2f getCurrentWindowSize() { return currentWindowSize; }

void resortEntitiesNextFrame() { resortNextFrame = true; }
} // namespace GameManager
