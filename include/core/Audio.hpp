#ifndef AUDIO_H
#define AUDIO_H

#include "Angle.hpp"
#include "SDL_mixer.h"
#include "SDL_render.h"
#include <array>
#include <filesystem>
#include <map>
#include <string>

struct Box;

struct Audio {
  Audio() : chunk(nullptr) {}
  Audio(Mix_Chunk *_chunk) : chunk(_chunk) {}
  Audio(std::filesystem::path _path) : path(_path) { chunk = loadChunk(_path); }

  void play();

  std::filesystem::path path;
  Mix_Chunk *chunk;

  static void clearAllChunks();
  static void reloadAllChunks();
  static void initMixer();

private:
  static Mix_Chunk *loadChunk(std::filesystem::path filename,
                              bool forceReload = false);
  static std::map<std::filesystem::path, Mix_Chunk *> loadedChunks;

  static bool mixerInitalizd;
};

#endif
