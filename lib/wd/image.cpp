
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

  lab=new QLabel();
  lab->setBackgroundRole(QPalette::Base);
  lab->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  //lab->setScaledContents(true);

  QScrollArea *w = new QScrollArea;
  widget=(QWidget *) w;
  w->setObjectName(qn);
  w->setBackgroundRole(QPalette::Dark);
  w->setWidget(lab);
}

// ---------------------------------------------------------------------
void Image::set(string p)
{
  QString s=s2q(p);
  if (s.isEmpty()) {
    info("Image","needs image filename");
    return;
  }
  QImage image(s);
  if (image.isNull()) {
    info("Image","cannot load image " + s);
    return;
  }

  lab->setPixmap(QPixmap::fromImage(image));
  lab->adjustSize();
}

// ---------------------------------------------------------------------
void Image::setp(string p,string v)
{
  //if (p=="caption")
  //set(v);
  //else
  Child::setp(p,v);
}
