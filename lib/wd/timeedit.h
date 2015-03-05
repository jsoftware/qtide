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
  TimeEdit(string n, string s, Form *f, Pane *p);
  string get(string p,string v);
  void set(string p,string v);
  string state();

private slots:
  void valueChanged();

private:
  QTime min;
  QTime max;
  QString format;

};

#endif
