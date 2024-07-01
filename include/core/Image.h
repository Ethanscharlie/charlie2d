#pragma once
#include "Angle.h"
#include "SDL_render.h"
#include <array>
#include <filesystem>
#include <map>
#include <string>

struct Box;

struct Image {
  Image() : path(""), texture(nullptr) {}
  Image(std::string _path);
  Image(SDL_Texture *_texture) : path(""), texture(_texture) {}

  void render(const Box &box, const Angle &angle = Angle());

  SDL_Texture *texture;
  std::string path;

  SDL_Rect srcRect = {0, 0, 0, 0};
  std::array<Uint8, 3> color = {255, 255, 255};
  Uint8 alpha = 255;
  SDL_RendererFlip flip = SDL_FLIP_NONE;
  SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

  static void clearAllTextures();
  static void reloadAllTextures();

private:
  static SDL_Texture *loadTexture(std::filesystem::path filename,
                                  bool forceReload = false);
  static std::map<std::filesystem::path, SDL_Texture *> loadedTextures;
};
