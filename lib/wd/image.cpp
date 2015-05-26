
#include <QLabel>
#include <QImage>
#include <QScrollArea>

#include "wd.h"
#include "image.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Image::Image(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="image";

  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  QStringList unopt=qsless(qsless(opt,qsplit("noframe transparent")),defChildStyle);
  if (unopt.size()) {
    error("unrecognized child style: " + n + " " + q2s(unopt.join(" ")));
    return;
  }

  lab=new QLabel();
  lab->setBackgroundRole(QPalette::Base);
  lab->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  //lab->setScaledContents(true);
  lab->setContentsMargins(0,0,0,0);
  imageFile="";

  QScrollArea *w = new QScrollArea;
  widget=(QWidget *) w;
  w->setObjectName(qn);
  childStyle(opt);
  if (opt.contains("noframe")||opt.contains("transparent"))
    w->setFrameShape(QFrame::NoFrame);
  if (opt.contains("transparent"))
    w->setAttribute(Qt::WA_TranslucentBackground);
  else
    w->setBackgroundRole(QPalette::Dark);
  w->setContentsMargins(0,0,0,0);
  w->setWidget(lab);
}

// ---------------------------------------------------------------------
string Image::get(string p,string v)
{
  string r;
  if (p=="property") {
    r+=string("image")+"\012";
    r+=Child::get(p,v);
  } else if (p=="image")
    r=imageFile;
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Image::set(string p,string v)
{
  if (p=="image") {

    QString s=s2q(remquotes(v));
    if (s.isEmpty()) {
      error("set image needs image filename: " + id + " " + p + " " + v);
      return;
    }
    QImage image(s);
    if (image.isNull()) {
      error("set image cannot load image " + id + " " + p + " " + v);
      return;
    }
    imageFile=q2s(s);
    lab->setPixmap(QPixmap::fromImage(image));
    lab->adjustSize();
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
string Image::state()
{
  return "";
}
