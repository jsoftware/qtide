
#include <QImage>
#include <QPainter>
#include <QScrollArea>

#include "cmd.h"
#include "form.h"
#include "image.h"
#include "pane.h"
#include "wd.h"

// ---------------------------------------------------------------------
Image::Image(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="image";

  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"transparent ignore keep expand")) return;

  lab=new Image2();
  imageFile="";

  if (opt.contains("ignore")) aspectRatio=0;
  else if (opt.contains("keep")) aspectRatio=1;
  else if (opt.contains("expand")) aspectRatio=2;
  else aspectRatio=-1; // notuse
  lab->aspectRatio=aspectRatio;
  lab->setContentsMargins(0,0,0,0);

  if (-1==aspectRatio) {
    lab->setBackgroundRole(QPalette::Base);
    lab->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QScrollArea *w = new QScrollArea;
    widget=(QWidget *) w;
    w->setObjectName(qn);
    if (opt.contains("transparent")) {
      w->setFrameShape(QFrame::NoFrame);
      w->setAttribute(Qt::WA_TranslucentBackground);
    } else
      w->setBackgroundRole(QPalette::Dark);
    w->setContentsMargins(0,0,0,0);
    w->setWidget(lab);
  } else {
    lab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    widget=(QWidget *) lab;
    lab->setObjectName(qn);
    if (opt.contains("transparent")) {
      lab->transparent=true;
      lab->setAttribute(Qt::WA_TranslucentBackground);
    } else {
      lab->transparent=false;
      lab->setBackgroundRole(QPalette::Dark);
    }
  }
  childStyle(opt);
}

// ---------------------------------------------------------------------
std::string Image::get(std::string p,std::string v)
{
  std::string r;
  if (p=="property") {
    r+=std::string("image")+"\012";
    r+=Child::get(p,v);
  } else if (p=="image")
    r=imageFile;
  else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void Image::set(std::string p,std::string v)
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
    QPixmap pix=QPixmap::fromImage(image);
    if (-1==aspectRatio) lab->resize(pix.size());
    lab->setPixmap(pix);
    lab->update();
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string Image::state()
{
  return "";
}

// ---------------------------------------------------------------------
Image2::Image2(QWidget *parent) :
  QWidget(parent)
{
}

// ---------------------------------------------------------------------
void Image2::paintEvent(QPaintEvent *event)
{
  QWidget::paintEvent(event);
  if (pix.isNull()) return;

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  if (-1!=aspectRatio) {
    QSize pixSize = pix.size();
    pixSize.scale(event->rect().size(), (Qt::AspectRatioMode)aspectRatio);
    QPixmap scaledPix = pix.scaled(pixSize, (Qt::AspectRatioMode)aspectRatio, Qt::SmoothTransformation );
    if (!transparent) painter.fillRect(rect(), QPalette::Dark);
    painter.drawPixmap(QPoint(0,0), scaledPix);
  } else {
    if (!transparent) painter.fillRect(rect(), QPalette::Dark);
    painter.drawPixmap(QPoint(0,0), pix);
  }
}

// ---------------------------------------------------------------------
const QPixmap* Image2::pixmap() const
{
  return &pix;
}

// ---------------------------------------------------------------------
void Image2::setPixmap (const QPixmap &pixmap)
{
  pix = pixmap;
}

