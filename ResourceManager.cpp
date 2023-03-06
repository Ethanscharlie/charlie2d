#include "ResourceManager.h"
#include <SDL2/SDL_image.h>
#include <iostream>

ResourceManager& ResourceManager::getInstance(SDL_Renderer* renderer) {
    static ResourceManager instance(renderer);
    return instance;
}

SDL_Texture* ResourceManager::getTexture(std::string filename) {
    if (textures_.find(filename) == textures_.end()) {
        std::cout << "Loading image " << filename << std::endl;

        SDL_Surface* surface = IMG_Load(filename.c_str());

        if (surface == nullptr) {
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        SDL_FreeSurface(surface);

        if (texture == nullptr) {
            return nullptr;
        }

        textures_[filename] = texture;
        std::cout << "Loaded image " << filename << std::endl;
    }

    return textures_[filename];
}

ResourceManager::ResourceManager(SDL_Renderer* renderer) : renderer_(renderer) {
    // Private constructor to prevent instantiation
}

ResourceManager::~ResourceManager() {
    clear();
}

void ResourceManager::clear() {
    for (auto it = textures_.begin(); it != textures_.end(); it++) {
        SDL_DestroyTexture(it->second);
    }
    textures_.clear();
}
