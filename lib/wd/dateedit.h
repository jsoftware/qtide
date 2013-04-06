#ifndef DATEEDIT_H
#define DATEEDIT_H

#include <QDate>

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class DateEdit : public Child
{
  Q_OBJECT

public:
  DateEdit(string n, string s, Form *f, Pane *p);
  void set(string p,string v);
  string state();

private slots:
  void valueChanged();

private:
  QDate min;
  QDate max;
  QString format;

};

#endif
