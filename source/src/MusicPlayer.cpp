#include "MusicPlayer.h"

#include <stdexcept>

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

MusicPlayer::~MusicPlayer() { destroy(); }

void MusicPlayer::playMusic(const std::string &musicPath, int loops = -1)
{
  if (musicTracks.find(musicPath) == musicTracks.end())
  {
    Mix_Music *music = Mix_LoadMUS(musicPath.c_str());
    if (!music)
    {
      throw std::runtime_error("Failed to load music! Error: " + std::string(Mix_GetError()));
    }
    musicTracks[musicPath] = music;
  }

  if (Mix_PlayMusic(musicTracks[musicPath], loops) == -1)
  {
    throw std::runtime_error("Failed to play music! Error: " + std::string(Mix_GetError()));
  }
  Mix_VolumeMusic(globalVolume);
}

void MusicPlayer::playSound(const std::string &soundPath, int loops = 0)
{
  if (soundEffects.find(soundPath) == soundEffects.end())
  {
    Mix_Chunk *sound = Mix_LoadWAV(soundPath.c_str());
    if (!sound)
    {
      throw std::runtime_error("Failed to load sound effect! Error: " + std::string(Mix_GetError()));
    }
    soundEffects[soundPath] = sound;
  }

  if (Mix_PlayChannel(-1, soundEffects[soundPath], loops) == -1)
  {
    throw std::runtime_error("Failed to play sound effect! Error: " + std::string(Mix_GetError()));
  }
  Mix_VolumeMusic(globalVolume);
}

void MusicPlayer::setVolume(int volume)
{
  if (volume < 0)
    volume = 0;
  if (volume > 128)
    volume = 128;
  globalVolume = volume;
  Mix_VolumeMusic(globalVolume);
}

void MusicPlayer::destroy()
{
  for (auto &[path, music] : musicTracks)
  {
    Mix_FreeMusic(music);
  }
  musicTracks.clear();

  for (auto &[path, sound] : soundEffects)
  {
    Mix_FreeChunk(sound);
  }
  soundEffects.clear();

  Mix_HaltMusic();
  Mix_CloseAudio();
}
