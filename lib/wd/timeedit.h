#ifndef TIMEEDIT_H
#define TIMEEDIT_H

#include <QTime>

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class TimeEdit : public Child
{
  Q_OBJECT

public:
  TimeEdit(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private slots:
  void valueChanged();

private:
  QTime min;
  QTime max;
  QString format;

};

#endif
