#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <SDL2/SDL_mixer.h>

#include <map>
#include <memory>
#include <string>

class MusicPlayer
{
public:
  static MusicPlayer &getInstance();

  void playMusic(const std::string &musicPath, int loops);
  void playSound(const std::string &soundPath, int loops);
  void setVolume(int volume);
  void destroy();

private:
  MusicPlayer();
  ~MusicPlayer();

  MusicPlayer(const MusicPlayer &) = delete;
  MusicPlayer &operator=(const MusicPlayer &) = delete;

  std::map<std::string, Mix_Music *> musicTracks;
  std::map<std::string, Mix_Chunk *> soundEffects;
  int globalVolume;
};

#endif
