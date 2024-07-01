#include "Image.h"
#include "Box.h"
#include "GameManager.h"
#include "Math.h"
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

  SDL_RenderCopyEx(GameManager::renderer, texture, srcRectPtr, &renderRect,
                   (180 / M_PI) * angle.radians, nullptr, flip);
}

Image::Image(std::string _path) : path(_path) { texture = loadTexture(path); }

SDL_Texture *Image::loadTexture(std::filesystem::path filename,
                                bool forceReload) {
  if (loadedTextures.find(filename) == loadedTextures.end() || forceReload) {
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(GameManager::renderer, filename.c_str());

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
  for (auto it = loadedTextures.begin(); it != loadedTextures.end(); it++) {
    SDL_DestroyTexture(it->second);
  }
  loadedTextures.clear();
}

void Image::reloadAllTextures() {
  for (auto &[name, texture] : loadedTextures) {
    loadTexture(name, true);
  }
}
