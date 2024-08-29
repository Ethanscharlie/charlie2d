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
#include <random>

bool GameManager::running = true;

std::map<std::type_index, std::vector<Component *>> GameManager::components;
std::map<std::string, std::vector<Entity *>>
    GameManager::entities; // Sorted by tags

Vector2f GameManager::gameWindowSize = {0, 0};
Vector2f GameManager::currentWindowSize = {0, 0};

float GameManager::screen_change_scale = 0;

SDL_Window *GameManager::window = nullptr;
SDL_Renderer *GameManager::renderer = nullptr;

bool GameManager::updateEntities = true;
float GameManager::deltaTime = 0;
Uint64 GameManager::lastTime;

ShadowFilter *GameManager::shadowFilter;

FPSmanager GameManager::fpsManager;

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
  GameManager::setWindowSize(
      {static_cast<float>(width), static_cast<float>(height)});
}
}
#endif

GameManager::GameManager() = default;

GameManager::~GameManager() {

  for (auto &[type, vector] : components) {
    for (Component *component : vector) {
      delete component;
    }
    vector.clear();
  }
  components.clear();

  for (auto &[tag, vector] : entities) {
    for (Entity *entity : vector) {
      delete entity;
    }
    vector.clear();
  }
  entities.clear();
}

void GameManager::init(Vector2f windowSize) {
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
                            currentWindowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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

  shadowFilter = createEntity("ShadowFilter")->add<ShadowFilter>();
  shadowFilter->entity->useLayer = true;
  shadowFilter->entity->layer = 99;

#ifdef __EMSCRIPTEN__
  resize_callback();
  float width = emscripten_run_script_int("window.innerWid.hpp");
  float height = emscripten_run_script_int("window.innerHeight");
  GameManager::setWindowSize({width, height});
#endif
}

void GameManager::Update() {
  InputManager::update();
  ControllerManager::resetTriggerButtons();
  SDL_Event event;

  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  int mouseX = 0, mouseY = 0;
  SDL_GetMouseState(&mouseX, &mouseY);
  Vector2f realMouse = (Vector2f(mouseX, mouseY));
  io.MousePos = {realMouse.x, realMouse.y};

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

        for (auto &c : components) {
          for (Component *component : c.second) {
            component->onScreenChange();
          }
        }

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

  // OLD SCENE UPDATE LOOP
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

  // if (1.0f / deltaTime < 20)
  //   std::cout << "FPS: " << 1.0f / deltaTime << std::endl;

  SDL_SetRenderDrawColor(GameManager::renderer, 0, 0, 0, 255);
  SDL_RenderClear(GameManager::renderer);

  shadowFilter->resetShadowFilter();

  std::vector<Entity *> layeredEntities;
  std::vector<Entity *> entitesToRemove;
  for (Entity *entity : getAllObjects()) {
    if (entity->toDestroy) {
      entitesToRemove.push_back(entity);
      for (auto [type, component] : entity->components) {
        component->onDestroy();
      }
      continue;
    }
    if (entity->skipUpdate) {
      entity->skipUpdate = false;
      continue;
    }
    if (!entity->active)
      continue;
    if (entity->useLayer) {
      layeredEntities.push_back(entity);
      continue;
    }

    entity->update();
  }

  std::sort(layeredEntities.begin(), layeredEntities.end(),
            [](Entity *a, Entity *b) {
              if (a->layer == b->layer)
                return a->iid < b->iid;
              return a->layer < b->layer;
            });

  for (Entity *entity : layeredEntities) {
    entity->update();
  }

  // Destroyer
  for (Entity *entity : entitesToRemove) {
    destroyEntity(entity);
  }
  // END OF OLD SCENE LOOP

  SDL_RenderPresent(renderer);
  SDL_framerateDelay(&fpsManager);
}

void GameManager::setWindowSize(Vector2f size) {
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

  for (auto &c : components) {
    for (Component *component : c.second) {
      component->onScreenChange();
    }
  }
}

void GameManager::changeEntityTag(Entity *entity, std::string newTag) {
  auto &list = entities[entity->tag];
  list.erase(std::remove(list.begin(), list.end(), entity), list.end());
  entities[newTag].push_back(entity);
  entity->tag = newTag;
}

void GameManager::doUpdateLoop() {
#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(Update, 0, 1);
#else
  while (running) {
    Update();
  }
#endif
}

void GameManager::playSound(std::string filename, bool loop) {
  Audio(filename).play(loop);
}

void GameManager::quit() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(GameManager::renderer);
  SDL_DestroyWindow(GameManager::window);

  TTF_Quit();
  Mix_Quit();
  SDL_Quit();

  GameManager::running = false;
}

void GameManager::destroyAll() {
  for (Entity *entity : getAllObjects()) {
    if (entity == shadowFilter->entity)
      continue;
    entity->toDestroy = true;
  }
}

auto GameManager::getEntities(std::string tag) -> std::vector<Entity *> {
  return entities[tag];
}

auto GameManager::getAllObjects() -> std::vector<Entity *> {
  std::vector<Entity *> out;
  for (const auto &pair : entities) {
    const std::vector<Entity *> &entityPointers = pair.second;
    out.insert(out.end(), entityPointers.begin(), entityPointers.end());
  }
  return out;
}

auto GameManager::createEntity(std::string tag) -> Entity * {
  auto *entity = new Entity();
  entity->tag = tag;

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(1, 100000); //
  entity->iid = dist(rng);

  entities[tag].push_back(entity);
  return entity;
}

void GameManager::removeComponent(Component *component, std::type_index type) {
  components[type].erase(
      std::remove(components[type].begin(), components[type].end(), component),
      components[type].end());
}

void GameManager::destroyEntity(Entity *entity) {
  if (entity == nullptr)
    return;
  if (std::find(getAllObjects().begin(), getAllObjects().end(), entity) ==
      getAllObjects().end())
    return;

  // components
  for (auto [ctype, component] : entity->components) {
    removeComponent(component, ctype);
    delete component;
  }

  entities[entity->tag].erase(std::remove(entities[entity->tag].begin(),
                                          entities[entity->tag].end(), entity),
                              entities[entity->tag].end());

  delete entity;
}
