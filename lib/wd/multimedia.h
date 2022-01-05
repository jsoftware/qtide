#ifndef MULTIMEDIA_H
#define MULTIMEDIA_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Multimedia : public Child
{
  Q_OBJECT

public:
  Multimedia(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();
  bool isVideo;

private slots:
  void bufferStatusChanged(int percentFilled);
  void durationChanged(qint64 duration);
  void merror(QMediaPlayer::Error error);
  void mediaStatusChanged(QMediaPlayer::MediaStatus status);
  void positionChanged(qint64 position);
  void playbackStateChanged(QMediaPlayer::PlaybackState state);
  void volumeChanged(int volume);

private:
  QMediaPlayer mediaPlayer;

};

#endif
