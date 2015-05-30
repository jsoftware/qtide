
#ifdef QT54
#include <QVideoWidget>
#endif

#include "wd.h"
#include "multimedia.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Multimedia::Multimedia(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
  , mediaPlayer(f)
{
  type="multimedia";
  isVideo=false;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("video")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + n + " " + q2s(unopt.join(" ")));
    return;
  }
  childStyle(opt);
#ifdef QT54
  if (opt.contains("video")) {
    isVideo=true;
    QVideoWidget *w=new QVideoWidget;
    mediaPlayer.setVideoOutput(w);
    widget=(QWidget *) w;
    widget->setObjectName(qn);
  }
#endif
  QObject::connect(&mediaPlayer, SIGNAL( bufferStatusChanged(int)), this, SLOT( bufferStatusChanged(int)));
  QObject::connect(&mediaPlayer, SIGNAL( durationChanged(qint64)), this, SLOT( durationChanged(qint64)));
  QObject::connect(&mediaPlayer, SIGNAL( error(QMediaPlayer::Error)), this, SLOT( merror(QMediaPlayer::Error)));
  QObject::connect(&mediaPlayer, SIGNAL( mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT( mediaStatusChanged(QMediaPlayer::MediaStatus)));
  QObject::connect(&mediaPlayer, SIGNAL( positionChanged(qint64)), this, SLOT( positionChanged(qint64)));
  QObject::connect(&mediaPlayer, SIGNAL( stateChanged(QMediaPlayer::State)), this, SLOT( stateChanged(QMediaPlayer::State)));
  QObject::connect(&mediaPlayer, SIGNAL( volumeChanged(int)), this, SLOT( volumeChanged(int)));
}

// ---------------------------------------------------------------------
void Multimedia::bufferStatusChanged(int percentFilled)
{
  event="bufferstatus";
  sysdata=i2s(percentFilled);
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Multimedia::durationChanged(qint64 duration)
{
  event="duration";
  sysdata=i2s(duration);
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
static  char * errortab[]= {
  (char *)"no",
  (char *)"resource",
  (char *)"format",
  (char *)"network",
  (char *)"accessdenied",
  (char *)"servicemissing",
};

// ---------------------------------------------------------------------
void Multimedia::merror(QMediaPlayer::Error error)
{
  event="error";
  sysdata=string(errortab[error]);
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
static  char * statustab[]= {
  (char *)"unknownmediastatus",
  (char *)"nomedia",
  (char *)"loadingmedia",
  (char *)"loadedmedia",
  (char *)"stalledmedia",
  (char *)"bufferingmedia",
  (char *)"bufferedmedia",
  (char *)"endofmedia",
  (char *)"invalidmedia",
};

// ---------------------------------------------------------------------
void Multimedia::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
  event="mediastatus";
  sysdata=string(statustab[status]);
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Multimedia::positionChanged(qint64 position)
{
  event="position";
  sysdata=i2s(position);
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
static char * statetab[]= {
  (char *)"stopped",
  (char *)"playing",
  (char *)"paused",
};

// ---------------------------------------------------------------------
void Multimedia::stateChanged(QMediaPlayer::State state)
{
  event="playstate";
  sysdata=string(statetab[state]);
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Multimedia::volumeChanged(int volume)
{
  event="volume";
  sysdata=i2s(volume);
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
static char * artab[]= {
  (char *)"ignore",
  (char *)"keep",
  (char *)"expand",
};

// ---------------------------------------------------------------------
string Multimedia::get(string p,string v)
{
#ifdef QT54
  QVideoWidget *w=(QVideoWidget*) widget;
#else
  QWidget *w=widget;
#endif
  Q_UNUSED(w);
  string r;
  if (p=="property") {
    if (isVideo && w)
      r+=string("aspectratio")+"\012"+ "brightness"+"\012"+ "contrast"+"\012"+ "duration"+"\012"+ "error"+"\012"+ "fullscreen"+"\012"+ "hue"+"\012"+ "mute"+"\012"+ "playstate"+"\012"+ "position"+"\012"+ "saturation"+"\012"+ "seekable"+"\012"+ "status"+"\012"+ "volume"+"\012";
    else
      r+=string("duration")+"\012"+ "error"+"\012"+ "mute"+"\012"+ "playstate"+"\012"+ "position"+"\012"+ "seekable"+"\012"+ "status"+"\012"+ "volume"+"\012";
    r+=Child::get(p,v);
  } else if (p=="duration")
    r=d2s(mediaPlayer.duration());
  else if (p=="error")
    r=string(errortab[mediaPlayer.error()]);
  else if (p=="mute")
    r=i2s(mediaPlayer.isMuted());
  else if (p=="playstate")
    r=string(statetab[mediaPlayer.state()]);
  else if (p=="position")
    r=i2s(mediaPlayer.position());
  else if (p=="seekable")
    r=i2s(mediaPlayer.isSeekable());
  else if (p=="status")
    r=string(statustab[mediaPlayer.mediaStatus()]);
  else if (p=="volume")
    r=i2s(mediaPlayer.volume());
  else if (isVideo && w) {
#ifdef QT54
    if (p=="aspectratio")
      r=string(artab[w->aspectRatioMode()]);
    else if (p=="brightness")
      r=i2s(w->brightness());
    else if (p=="contrast")
      r=i2s(w->contrast());
    else if (p=="fullscreen")
      r=i2s(w->isFullScreen());
    else if (p=="hue")
      r=i2s(w->hue());
    else if (p=="saturation")
      r=i2s(w->saturation());
    else
      r=Child::get(p,v);
#endif
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Multimedia::set(string p,string v)
{
#ifdef QT54
  QVideoWidget *w=(QVideoWidget*) widget;
#else
  QWidget *w=widget;
#endif
  if ((p=="pause" || p=="play" || p=="stop") && v.size()) {
    error("extra parameters: " + p + " " + v);
    return;
  }
  if (p=="media") {
    QString f=s2q(remquotes(v));
    if (f.contains("://"))
      mediaPlayer.setMedia(QUrl(f));
    else
      mediaPlayer.setMedia(QUrl::fromLocalFile(f));
  } else if (p=="mute")
    mediaPlayer.setMuted(remquotes(v)!="0");
  else if (p=="pause")
    mediaPlayer.pause();
  else if (p=="play")
    mediaPlayer.play();
  else if (p=="playbackrate")
    mediaPlayer.setPlaybackRate(c_strtod(v));
  else if (p=="position")
    mediaPlayer.setPosition(c_strtol(v));
  else if (p=="stop")
    mediaPlayer.stop();
  else if (p=="volume")
    mediaPlayer.setVolume(c_strtoi(v));
  else if (isVideo && w) {
#ifdef QT54
    if (p=="aspectratio") {
      if (v=="ignore")
        w->setAspectRatioMode(Qt::IgnoreAspectRatio);
      else if (v=="keep")
        w->setAspectRatioMode(Qt::KeepAspectRatio);
      else if (v=="expand")
        w->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
      else {
        error("invalid option: " + p + " " + v);
        return;
      }
    } else if (p=="brightness")
      w->setBrightness(c_strtoi(v));
    else if (p=="contrast")
      w->setContrast(c_strtoi(v));
    else if (p=="fullscreen")
      w->setFullScreen(remquotes(v)!="0");
    else if (p=="hue")
      w->setHue(c_strtoi(v));
    else if (p=="saturation")
      w->setSaturation(c_strtoi(v));
    else
      Child::set(p,v);
#endif
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
string Multimedia::state()
{
  return "";
}
