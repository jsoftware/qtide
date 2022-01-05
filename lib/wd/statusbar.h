#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "child.h"

class QLabel;
class Form;
class Pane;

// ---------------------------------------------------------------------
class StatusBar : public Child
{
  Q_OBJECT

public:
  StatusBar(std::string n, std::string s, Form *f, Pane *p);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();

private slots:

private:
  QList<QLabel *> labelw;
  QList<QString> labels;

};

#endif
