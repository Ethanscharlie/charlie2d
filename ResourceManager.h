#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <map>
#include <SDL2/SDL.h>

class ResourceManager {
public:
    static ResourceManager& getInstance(SDL_Renderer* renderer);
    SDL_Texture* getTexture(std::string filename);
    void clear();

private:
    ResourceManager(SDL_Renderer* renderer);
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ~ResourceManager();

    SDL_Renderer* renderer_;
    std::map<std::string, SDL_Texture*> textures_;
};

#endif // RESOURCE_MANAGER_H
