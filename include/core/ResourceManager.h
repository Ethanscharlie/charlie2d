// #ifndef RESOURCE_MANAGER_H
// #define RESOURCE_MANAGER_H
//
// // #include "GameManager.h"
//
// #include <SDL.h>
// #include <SDL_image.h>
// #include <SDL_mixer.h>
// #include <array>
// #include <map>
// #include <string>
//
// /**
//  * \brief The ResourceManager is a static class which is uses for managing image
//  * and audio files
//  *
//  * It loads them into a map so there is no need to worry about calling
//  * getTexture multiple times
//  *
//  * It can be accessed by `ResourceManager::getInstance(GameManager::renderer)`
//  */
// class ResourceManager {
// public:
//   /**
//    * \brief Gets the manager, use as
//    * `ResourceManager::getInstance(GameManager::renderer)`
//    */
//   static ResourceManager &getInstance(SDL_Renderer *renderer);
//   /**
//    * \brief Gets a texture from filename, also loads the texture permenently
//    * into textures_
//    */
//   SDL_Texture *getTexture(std::string filename, bool forceReload = false);
//   /**
//    * \brief Works like getTexture but adds a colored tint
//    * \param color The color tint, Ex `{255, 24, 100}`
//    */
//   SDL_Texture *getColoredTexture(std::array<Uint8, 3> color,
//                                  std::string textureName);
//   /**
//    * \brief Works like getColoredTexture but gets it as pure white
//    */
//   SDL_Texture *getWhiteTexture(std::string textureName);
//   /**
//    * \brief Gets a audio from filename, also loads the file permenently into
//    * sounds_
//    */
//   Mix_Chunk *getAudio(std::string filename);
//   /**
//    * \brief Clears all sounds and textures
//    */
//   void clear();
//
//   void reloadAllTextures();
//
// private:
//   ResourceManager(SDL_Renderer *renderer);
//   ResourceManager(const ResourceManager &) = delete;
//   ResourceManager &operator=(const ResourceManager &) = delete;
//   ~ResourceManager();
//
//   SDL_Renderer *renderer_;
//   std::map<std::string, SDL_Texture *> textures_;
//   std::map<std::string, Mix_Chunk *> sounds_;
// };
//
// #endif // RESOURCE_MANAGER_H
