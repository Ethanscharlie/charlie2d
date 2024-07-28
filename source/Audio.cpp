#include "Audio.h"
#include "SDL_mixer.h"

std::map<std::filesystem::path, Mix_Chunk *> Audio::loadedChunks;

void Audio::play(bool loop) {
  int maxChannels =
      Mix_AllocateChannels(-1); // Get the total number of channels
  int availableChannel = -1;

  for (int channel = 0; channel < maxChannels; ++channel) {
    if (Mix_Playing(channel) == 0) {
      availableChannel = channel;
      break;
    }
  }

  if (availableChannel != -1) {
    Mix_PlayChannel(availableChannel, chunk, 0);
  } else {
    printf("Failed to play sound effect: %s\n", Mix_GetError());
  }
}

auto Audio::loadChunk(std::filesystem::path filename, bool forceReload) -> Mix_Chunk * {
  if (loadedChunks.find(filename) == loadedChunks.end()) {
    std::cout << "Loading Audio " << filename << std::endl;

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1048) < 0) {
      printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
             Mix_GetError());
      return nullptr;
    }

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
  for (auto & loadedChunk : loadedChunks) {
    Mix_FreeChunk(loadedChunk.second);
  }
  loadedChunks.clear();
}

void Audio::reloadAllChunks() {
  for (auto &[name, texture] : loadedChunks) {
    loadChunk(name, true);
  }
}
