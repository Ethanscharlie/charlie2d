#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// #include "GameManager.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <array>
#include <map>
#include <string>

class ResourceManager {
public:
  static ResourceManager &getInstance(SDL_Renderer *renderer);
  SDL_Texture *getTexture(std::string filename);
  SDL_Texture *getColoredTexture(std::array<Uint8, 3> color,
                                 std::string textureName);
  SDL_Texture *getWhiteTexture(std::string textureName);
  Mix_Chunk *getAudio(std::string filename);
  void clear();

private:
  ResourceManager(SDL_Renderer *renderer);
  ResourceManager(const ResourceManager &) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;
  ~ResourceManager();

  SDL_Renderer *renderer_;
  std::map<std::string, SDL_Texture *> textures_;
  std::map<std::string, Mix_Chunk *> sounds_;
};

#endif // RESOURCE_MANAGER_H
