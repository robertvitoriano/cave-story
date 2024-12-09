#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL_mixer.h>

class MusicPlayer
{
private:
  std::unordered_map<std::string, Mix_Chunk *> soundEffects;
  std::unordered_map<std::string, int> activeChannels;
  std::vector<std::string> soundOrder;
  int globalVolume;

  MusicPlayer();
  ~MusicPlayer();
  void destroy();

public:
  static MusicPlayer &getInstance();
  void playSound(const std::string &soundPath, int loops);
  void stopSound(const std::string &soundPath);
  void setVolume(int volume);
};

#endif // MUSICPLAYER_H
