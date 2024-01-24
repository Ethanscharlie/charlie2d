#include "GameManager.h"
#include "Component.h"
#include "Entity.h"
#include "EntityBox.h"
#include "InputManager.h"
#include "Math.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <functional>

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

SDL_Texture *GameManager::targetRenderSurface;

#ifdef __EMSCRIPTEN__
EM_JS(void, resize_callback, (), {
  window.addEventListener(
      'resize', function() {
        // Call the function _on_resize from JavaScript
        // Module['_on_resize'](window.innerWidth, window.innerHeight);
        _on_resize(window.innerWidth, window.innerHeight);
      });
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

GameManager::GameManager() {}

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
  srand(time(NULL));
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Init(SDL_INIT_AUDIO);
  SDL_Init(SDL_INIT_TIMER);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);
  // SDL_SetWindowResizable(window, SDL_TRUE);

  gameWindowSize = windowSize;
  currentWindowSize = windowSize;

  screen_change_scale =
      ((float)currentWindowSize.x + (float)currentWindowSize.y) /
      (gameWindowSize.x + gameWindowSize.y);

  window = SDL_CreateWindow("SDL2 works!", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, currentWindowSize.x,
                            currentWindowSize.y, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  targetRenderSurface =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, windowSize.x, windowSize.y);
  // SDL_SetRenderTarget(GameManager::renderer, targetRenderSurface);

  SDL_RenderSetLogicalSize(renderer, windowSize.x, windowSize.y);

  // IMGUI -------------------------------------------------------

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  // ImGui& io = ImGui::GetIO(); (void)io;

  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);

  ImGui::StyleColorsLight();

  // -------------------------------------------------------------

#ifdef __EMSCRIPTEN__
  resize_callback();
  float width = emscripten_run_script_int("window.innerWidth");
  float height = emscripten_run_script_int("window.innerHeight");
  GameManager::setWindowSize({width, height});
#endif
}

void GameManager::Update() {
  InputManager::update();
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.MousePos = {InputManager::getMouseScreenPosition().x,
                   InputManager::getMouseScreenPosition().y};
    ImGui_ImplSDL2_ProcessEvent(&event);

    if (event.type == SDL_QUIT) {
      quit();
      return;
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {
      InputManager::mousePressed = true;
      InputManager::mouseHeld = true;
    } else if (event.type == SDL_MOUSEBUTTONUP) {
      InputManager::mouseHeld = false;
    } else if (event.type == SDL_WINDOWEVENT) {
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

        break;
      }
      break;
    } else if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym <= 256) {
        if (true || InputManager::keysUped[event.key.keysym.sym]) {
          InputManager::keys[event.key.keysym.sym] = true;
          // InputManager::keysUped[event.key.keysym.sym] = false;
        } else {
          // InputManager::keys[event.key.keysym.sym] = false;
        }
      }

      switch (event.key.keysym.sym) {
      case SDLK_SPACE:
        InputManager::jumpPressed = true;
      }
    }
  }

  // OLD SCENE UPDATE LOOP
  Uint64 currentTime = SDL_GetPerformanceCounter();
  deltaTime =
      static_cast<float>((currentTime - lastTime) * 1000 /
                         static_cast<double>(SDL_GetPerformanceFrequency())) *
      0.001;

  lastTime = currentTime;
  lastTime = SDL_GetPerformanceCounter();

  // if (1.0f / deltaTime < 20)
  //   std::cout << "FPS: " << 1.0f / deltaTime << std::endl;

  SDL_SetRenderDrawColor(GameManager::renderer, 0, 0, 0, 255);
  SDL_RenderClear(GameManager::renderer);

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
    // if (!updateEntities)
    //   continue;

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
  //
  // END OF OLD SCENE LOOP

  SDL_RenderPresent(renderer);
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

  for (auto &c : components) {
    for (Component *component : c.second) {
      component->onScreenChange();
    }
  }
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
  Mix_Chunk *sound = ResourceManager::getInstance(renderer).getAudio(filename);

  int maxChannels =
      Mix_AllocateChannels(-1); // Get the total number of channels
  int availableChannel = -1;

  for (int channel = 0; channel < maxChannels; ++channel) {
    if (Mix_Playing(channel) == 0) {
      availableChannel = channel;
      break;
    }
  }

  if (availableChannel != -1) {
    Mix_PlayChannel(availableChannel, sound, 0);
  } else {
    printf("Failed to play sound effect: %s\n", Mix_GetError());
  }
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
    entity->toDestroy = true;
  }
}

std::vector<Entity *> GameManager::getEntities(std::string tag) {
  return entities[tag];
}

std::vector<Entity *> GameManager::getAllObjects() {
  std::vector<Entity *> out;
  for (const auto &pair : entities) {
    const std::vector<Entity *> &entityPointers = pair.second;
    out.insert(out.end(), entityPointers.begin(), entityPointers.end());
  }
  return out;
}

Entity *GameManager::createEntity(std::string tag) {
  Entity *entity = new Entity();
  entity->tag = tag;
  entity->iid = getAllObjects().size();
  entities[tag].push_back(entity);
  entity->add<entityBox>();
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
  for (Entity *child : entity->getChildren()) {
    if (child == nullptr)
      continue;
    child->toDestroy = true;
    child->parent = nullptr;
  }

  // components
  for (auto [ctype, component] : entity->components) {
    removeComponent(component, ctype);
    delete component;
  }

  entities[entity->tag].erase(std::remove(entities[entity->tag].begin(),
                                          entities[entity->tag].end(), entity),
                              entities[entity->tag].end());

  if (entity->getParent() != nullptr) {
    entity->getParent()->children.erase(
        std::remove(entity->getParent()->children.begin(),
                    entity->getParent()->children.end(), entity),
        entity->getParent()->children.end());
  }

  delete entity;
}
