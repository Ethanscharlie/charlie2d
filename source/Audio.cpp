#include "Audio.hpp"
#include "SDL_mixer.h"

std::map<std::filesystem::path, Mix_Chunk *> Audio::loadedChunks;
bool Audio::mixerInitalizd = false;

void Audio::play() {
  if (!mixerInitalizd)
    initMixer();

  if (Mix_PlayChannel(-1, chunk, 0) == -1) {
    printf("Failed to play sound effect: %s\n", Mix_GetError());
  }
}

void Audio::initMixer() {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1048) < 0) {
    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
           Mix_GetError());
    return;
  }

  Mix_AllocateChannels(32);

  mixerInitalizd = true;
}

auto Audio::loadChunk(std::filesystem::path filename,
                      bool forceReload) -> Mix_Chunk * {

  if (!mixerInitalizd)
    initMixer();

  if (loadedChunks.find(filename) == loadedChunks.end()) {
    std::cout << "Loading Audio " << filename << std::endl;

    Mix_Chunk *sound = Mix_LoadWAV(filename.string().c_str());
    if (sound == nullptr) {
      printf("Failed to load sound effect: %s\n", Mix_GetError());
      Mix_CloseAudio();
      return nullptr;
    }

    loadedChunks[filename] = sound;
    std::cout << "Audio image " << filename << std::endl;
  }

  return loadedChunks[filename];
}

void Audio::clearAllChunks() {
  for (auto &loadedChunk : loadedChunks) {
    Mix_FreeChunk(loadedChunk.second);
  }
  loadedChunks.clear();
}

void Audio::reloadAllChunks() {
  for (auto &[name, texture] : loadedChunks) {
    loadChunk(name, true);
  }
}
