#include "Image.hpp"

#include <utility>
#include "Box.hpp"
#include "GameManager.hpp"
#include "Math.hpp"
#include "SDL_rect.h"

std::map<std::filesystem::path, SDL_Texture *> Image::loadedTextures;

void Image::render(const Box &box, const Angle &angle) {
  if (!texture)
    return;

  SDL_SetTextureBlendMode(texture, blendMode);
  SDL_SetTextureAlphaMod(texture, alpha);
  SDL_SetTextureColorMod(texture, color[0], color[1], color[2]);

  SDL_Rect *srcRectPtr = nullptr;
  if (srcRect.w != 0 || srcRect.h != 0) {
    srcRectPtr = &srcRect;
  }

  SDL_Rect renderRect = box;
  renderRect.w += 1;
  renderRect.h += 1;

  SDL_RenderCopyEx(GameManager::renderer, texture, srcRectPtr, &renderRect,
                   (180 / M_PI) * angle.radians, nullptr, flip);
}

Image::Image(std::string _path) : path(std::move(_path)) { texture = loadTexture(path); }

auto Image::loadTexture(std::filesystem::path filename,
                                bool forceReload) -> SDL_Texture * {
  if (loadedTextures.find(filename) == loadedTextures.end() || forceReload) {
    SDL_Texture *texture = nullptr;
    texture = IMG_LoadTexture(GameManager::renderer, filename.string().c_str());

    if (texture == nullptr) {
      std::cout << "Failed to load image texture " << filename << std::endl;
      std::cout << IMG_GetError() << std::endl;

      return nullptr;
    }

    loadedTextures[filename] = texture;
  }

  return loadedTextures[filename];
}

void Image::clearAllTextures() {
  for (auto & loadedTexture : loadedTextures) {
    SDL_DestroyTexture(loadedTexture.second);
  }
  loadedTextures.clear();
}

void Image::reloadAllTextures() {
  for (auto &[name, texture] : loadedTextures) {
    loadTexture(name, true);
  }
}
