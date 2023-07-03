#include "ResourceManager.h"
#include <iostream>

ResourceManager& ResourceManager::getInstance(SDL_Renderer* renderer) {
    static ResourceManager instance(renderer);
    return instance;
}

SDL_Texture* ResourceManager::getTexture(std::string filename) {
    if (textures_.find(filename) == textures_.end()) {
        std::cout << "Loading image " << filename << std::endl;

        //SDL_Surface* surface = IMG_Load(filename.c_str());

        //if (surface == nullptr) {
        //    std::cout << "Failed to load image surface" << filename << std::endl;
        //    return nullptr;
        //}

        //SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        //SDL_FreeSurface(surface);

        //if (texture == nullptr) {
        //    std::cout << "Failed to load image texture" << filename << std::endl;
        //    return nullptr;
        //}
        
        SDL_Texture* texture = NULL;
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

SDL_Texture* ResourceManager::getColoredTexture(std::array<Uint8, 3> color, std::string name) {
    if (textures_.find(name) == textures_.end()) {
        //std::cout << "Loading color image " << name << std::endl;

        SDL_Surface* surface = IMG_Load("img/panel.png");

        if (surface == nullptr) {
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        SDL_FreeSurface(surface);

        if (texture == nullptr) {
            return nullptr;
        }

        SDL_SetTextureColorMod(texture, color[0], color[1], color[2]);

        textures_[name] = texture;
        //std::cout << "Loaded image " << name << std::endl;
    }

    return textures_[name];
}

Mix_Chunk* ResourceManager::getAudio(std::string filename)
{
    if (sounds_.find(filename) == sounds_.end()) {
        std::cout << "Loading Audio " << filename << std::endl;

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1048) < 0) {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            return nullptr;
        }

        Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
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

    for (auto it = sounds_.begin(); it != sounds_.end(); it++) {
        //SDL_DestroyTexture(it->second);
    }
    sounds_.clear();
}
