#include "ResourceManager.h"
#include <iostream>

ResourceManager &ResourceManager::getInstance(SDL_Renderer *renderer) {
  static ResourceManager instance(renderer);
  return instance;
}

SDL_Texture *ResourceManager::getTexture(std::string filename) {
  if (textures_.find(filename) == textures_.end()) {
    std::cout << "Loading image " << filename << std::endl;

    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer_, filename.c_str());

    if (texture == nullptr) {
      std::cout << "Failed to load image texture " << filename << std::endl;
      std::cout << IMG_GetError() << std::endl;

      return nullptr;
    }

    textures_[filename] = texture;
    std::cout << "Loaded image " << filename << std::endl;
  }

  return textures_[filename];
}

SDL_Texture *ResourceManager::getColoredTexture(std::array<Uint8, 3> color,
                                                std::string textureName) {
  std::string name = std::to_string(color[0]) + " " + std::to_string(color[1]) +
                     " " + std::to_string(color[2]) + " " + textureName;
  if (textures_.find(name) == textures_.end()) {
    // std::cout << "Loading color image " << name << std::endl;

    SDL_Surface *surface = IMG_Load(textureName.c_str());

    if (surface == nullptr) {
      return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr) {
      return nullptr;
    }

    SDL_SetTextureColorMod(texture, color[0], color[1], color[2]);

    textures_[name] = texture;
    // std::cout << "Loaded image " << name << std::endl;
  }

  return textures_[name];
}

SDL_Texture *ResourceManager::getWhiteTexture(std::string textureName) {
  std::string name =
      "White " + textureName; // Adjust the name for the white texture

  if (textures_.find(name) == textures_.end()) {
    // Load the image
    SDL_Surface *surface = IMG_Load(textureName.c_str());

    if (surface == nullptr) {
      return nullptr;
    }

    SDL_PixelFormat *fmt = surface->format;

    // Create a new surface with the same dimensions and 32-bit color depth
    SDL_Surface *whiteSurface =
        SDL_CreateRGBSurface(0, surface->w, surface->h, 32, fmt->Rmask,
                             fmt->Gmask, fmt->Bmask, fmt->Amask);

    // Lock surfaces to manipulate pixel data
    SDL_LockSurface(surface);
    SDL_LockSurface(whiteSurface);

    // Copy each pixel from the original surface and set it to white, skipping
    // fully transparent pixels
    for (int y = 0; y < surface->h; ++y) {
      for (int x = 0; x < surface->w; ++x) {
        Uint32 pixel =
            *((Uint32 *)surface->pixels + y * surface->pitch / 4 + x);
        Uint8 alpha = (pixel >> 24) & 0xFF;

        if (alpha > 0) {
          *((Uint32 *)whiteSurface->pixels + y * whiteSurface->pitch / 4 + x) =
              SDL_MapRGBA(fmt, 255, 255, 255, alpha);
        } else {
          *((Uint32 *)whiteSurface->pixels + y * whiteSurface->pitch / 4 + x) =
              pixel;
        }
      }
    }

    // Unlock surfaces
    SDL_UnlockSurface(surface);
    SDL_UnlockSurface(whiteSurface);

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(renderer_, whiteSurface);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(whiteSurface);

    if (texture == nullptr) {
      return nullptr;
    }

    textures_[name] = texture;
  }

  return textures_[name];
}

Mix_Chunk *ResourceManager::getAudio(std::string filename) {
  if (sounds_.find(filename) == sounds_.end()) {
    std::cout << "Loading Audio " << filename << std::endl;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1048) < 0) {
      printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
             Mix_GetError());
      return nullptr;
    }

    Mix_Chunk *sound = Mix_LoadWAV(filename.c_str());
    if (sound == NULL) {
      printf("Failed to load sound effect: %s\n", Mix_GetError());
      Mix_CloseAudio();
      return nullptr;
    }

    sounds_[filename] = sound;
    std::cout << "Audio image " << filename << std::endl;
  }

  return sounds_[filename];
}

ResourceManager::ResourceManager(SDL_Renderer *renderer) : renderer_(renderer) {
  // Private constructor to prevent instantiation
}

ResourceManager::~ResourceManager() { clear(); }

void ResourceManager::clear() {
  for (auto it = textures_.begin(); it != textures_.end(); it++) {
    SDL_DestroyTexture(it->second);
  }
  textures_.clear();

  for (auto it = sounds_.begin(); it != sounds_.end(); it++) {
    // SDL_DestroyTexture(it->second);
  }
  sounds_.clear();
}
