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
  DateEdit(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private slots:
  void valueChanged();

private:
  QDate min;
  QDate max;
  QString format;

};

#endif
