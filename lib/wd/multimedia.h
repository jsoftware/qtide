#ifndef MULTIMEDIA_H
#define MULTIMEDIA_H

#include <QMediaPlayer>
#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Multimedia : public Child
{
  Q_OBJECT

public:
  Multimedia(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();
  bool isVideo;

private slots:
  void bufferStatusChanged(int percentFilled);
  void durationChanged(qint64 duration);
  void merror(QMediaPlayer::Error error);
  void mediaStatusChanged(QMediaPlayer::MediaStatus status);
  void positionChanged(qint64 position);
  void stateChanged(QMediaPlayer::State state);
  void volumeChanged(int volume);

private:
  QMediaPlayer mediaPlayer;

};

#endif
