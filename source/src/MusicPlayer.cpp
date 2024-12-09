#include "MusicPlayer.h"
#include <stdexcept>
#include <algorithm>

MusicPlayer &MusicPlayer::getInstance()
{
  static MusicPlayer instance;
  return instance;
}

MusicPlayer::MusicPlayer() : globalVolume(30)
{
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
  {
    throw std::runtime_error("SDL_mixer could not initialize! Error: " + std::string(Mix_GetError()));
  }
}

MusicPlayer::~MusicPlayer()
{
  destroy();
}

void MusicPlayer::playSound(const std::string &soundPath, int loops)
{
  if (soundEffects.find(soundPath) == soundEffects.end())
  {
    Mix_Chunk *sound = Mix_LoadWAV(soundPath.c_str());
    if (!sound)
    {
      throw std::runtime_error("Failed to load sound effect! Error: " + std::string(Mix_GetError()));
    }
    soundEffects[soundPath] = sound;
    soundOrder.push_back(soundPath);
  }

  auto it = std::find(soundOrder.begin(), soundOrder.end(), soundPath);
  if (it == soundOrder.end())
  {
    throw std::runtime_error("Sound path not found in order list!");
  }

  int channel = std::distance(soundOrder.begin(), it);

  if (Mix_PlayChannel(channel, soundEffects[soundPath], loops) == -1)
  {
    throw std::runtime_error("Failed to play sound effect! Error: " + std::string(Mix_GetError()));
  }

  activeChannels[soundPath] = channel;
  Mix_Volume(channel, globalVolume);
}

void MusicPlayer::stopSound(const std::string &soundPath)
{
  auto it = activeChannels.find(soundPath);
  if (it != activeChannels.end())
  {
    Mix_HaltChannel(it->second);
    activeChannels.erase(it);
  }
}

void MusicPlayer::setVolume(int volume)
{
  globalVolume = std::clamp(volume, 0, 128);

  Mix_VolumeMusic(globalVolume);

  for (const auto &[path, channel] : activeChannels)
  {
    Mix_Volume(channel, globalVolume);
  }
}

void MusicPlayer::destroy()
{
  for (auto &[path, sound] : soundEffects)
  {
    Mix_FreeChunk(sound);
  }
  soundEffects.clear();

  for (const auto &[path, channel] : activeChannels)
  {
    Mix_HaltChannel(channel);
  }
  activeChannels.clear();

  Mix_CloseAudio();
}