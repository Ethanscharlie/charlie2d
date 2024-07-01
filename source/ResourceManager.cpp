// #include "ResourceManager.h"
// #include <iostream>
//
// ResourceManager &ResourceManager::getInstance(SDL_Renderer *renderer) {
//   static ResourceManager instance(renderer);
//   return instance;
// }
//
// SDL_Texture *ResourceManager::getTexture(std::string filename,
//                                          bool forceReload) {
//   if (textures_.find(filename) == textures_.end() || forceReload) {
//     std::cout << "Loading image " << filename << std::endl;
//
//     SDL_Texture *texture = NULL;
//     texture = IMG_LoadTexture(renderer_, filename.c_str());
//
//     if (texture == nullptr) {
//       std::cout << "Failed to load image texture " << filename << std::endl;
//       std::cout << IMG_GetError() << std::endl;
//
//       return nullptr;
//     }
//
//     textures_[filename] = texture;
//     std::cout << "Loaded image " << filename << std::endl;
//   }
//
//   return textures_[filename];
// }
//
// void ResourceManager::reloadAllTextures() {
//   for (auto &[name, texture] : textures_) {
//     getTexture(name, true);
//   }
// }
//
// SDL_Texture *ResourceManager::getColoredTexture(std::array<Uint8, 3> color,
//                                                 std::string textureName) {
//   std::string name = std::to_string(color[0]) + " " + std::to_string(color[1]) +
//                      " " + std::to_string(color[2]) + " " + textureName;
//   if (textures_.find(name) == textures_.end()) {
//
//     SDL_Surface *surface = IMG_Load(textureName.c_str());
//
//     if (surface == nullptr) {
//       return nullptr;
//     }
//
//     SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer_, surface);
//     SDL_FreeSurface(surface);
//
//     if (texture == nullptr) {
//       return nullptr;
//     }
//
//     SDL_SetTextureColorMod(texture, color[0], color[1], color[2]);
//
//     textures_[name] = texture;
//   }
//
//   return textures_[name];
// }
//
// SDL_Texture *ResourceManager::getWhiteTexture(std::string textureName) {
//   std::string name = "White " + textureName;
//
//   // Check if the texture already exists in the map
//   if (textures_.find(name) == textures_.end()) {
//     // Load the image
//     SDL_Surface *surface = IMG_Load(textureName.c_str());
//     if (surface == nullptr) {
//       return nullptr;
//     }
//
//     // Create a new surface with the same dimensions and 32-bit color depth
//     SDL_Surface *whiteSurface =
//         SDL_CreateRGBSurface(0, surface->w, surface->h, 32, 0xFF000000,
//                              0x00FF0000, 0x0000FF00, 0x000000FF);
//     if (whiteSurface == nullptr) {
//       SDL_FreeSurface(surface);
//       return nullptr;
//     }
//
//     // Lock surfaces to manipulate pixel data
//     SDL_LockSurface(surface);
//     SDL_LockSurface(whiteSurface);
//
//     // Get the pixel format
//     SDL_PixelFormat *fmt = surface->format;
//
//     // Iterate through each pixel
//     for (int y = 0; y < surface->h; ++y) {
//       for (int x = 0; x < surface->w; ++x) {
//         Uint32 pixel =
//             *((Uint32 *)surface->pixels + y * surface->pitch / 4 + x);
//         Uint8 alpha = (pixel >> 24) & 0xFF;
//
//         if (alpha > 0) {
//           // If the pixel is not fully transparent, set it to white with
//           // original alpha
//           *((Uint32 *)whiteSurface->pixels + y * whiteSurface->pitch / 4 + x) =
//               SDL_MapRGBA(fmt, 255, 255, 255, alpha);
//         } else {
//           // If the pixel is fully transparent, keep it transparent
//           *((Uint32 *)whiteSurface->pixels + y * whiteSurface->pitch / 4 + x) =
//               pixel;
//         }
//       }
//     }
//
//     // Unlock surfaces
//     SDL_UnlockSurface(surface);
//     SDL_UnlockSurface(whiteSurface);
//
//     // Create texture from the modified surface
//     SDL_Texture *texture =
//         SDL_CreateTextureFromSurface(renderer_, whiteSurface);
//     if (texture == nullptr) {
//       SDL_FreeSurface(surface);
//       SDL_FreeSurface(whiteSurface);
//       return nullptr;
//     }
//
//     // Free surfaces
//     SDL_FreeSurface(surface);
//     SDL_FreeSurface(whiteSurface);
//
//     // Store the texture in the map
//     textures_[name] = texture;
//   }
//
//   // Return the texture
//   return textures_[name];
// }
//
// Mix_Chunk *ResourceManager::getAudio(std::string filename) {
//   if (sounds_.find(filename) == sounds_.end()) {
//     std::cout << "Loading Audio " << filename << std::endl;
//
//     if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1048) < 0) {
//       printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
//              Mix_GetError());
//       return nullptr;
//     }
//
//     Mix_Chunk *sound = Mix_LoadWAV(filename.c_str());
//     if (sound == NULL) {
//       printf("Failed to load sound effect: %s\n", Mix_GetError());
//       Mix_CloseAudio();
//       return nullptr;
//     }
//
//     sounds_[filename] = sound;
//     std::cout << "Audio image " << filename << std::endl;
//   }
//
//   return sounds_[filename];
// }
//
// ResourceManager::ResourceManager(SDL_Renderer *renderer) : renderer_(renderer) {
//   // Private constructor to prevent instantiation
// }
//
// ResourceManager::~ResourceManager() { clear(); }
//
// void ResourceManager::clear() {
//   for (auto it = textures_.begin(); it != textures_.end(); it++) {
//     SDL_DestroyTexture(it->second);
//   }
//   textures_.clear();
//
//   for (auto it = sounds_.begin(); it != sounds_.end(); it++) {
//     // SDL_DestroyTexture(it->second);
//   }
//   sounds_.clear();
// }
