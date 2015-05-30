#ifndef IMAGE_H
#define IMAGE_H

#include "child.h"

class QWidget;
class QPixmap;
class Form;
class Pane;
class Image2;

// ---------------------------------------------------------------------
class Image : public Child
{
  Q_OBJECT

public:
  Image(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

private:
  Image2 *lab;
  string imageFile;
  int aspectRatio;
};

// ---------------------------------------------------------------------
class Image2 : public QWidget
{
  Q_OBJECT

public:
  explicit Image2(QWidget *parent = 0);
  const QPixmap* pixmap() const;
  int aspectRatio;
  bool transparent;

public slots:
  void setPixmap(const QPixmap&);

protected:
  void paintEvent(QPaintEvent *);

private:
  QPixmap pix;
};

#endif
