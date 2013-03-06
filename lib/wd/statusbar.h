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
  StatusBar(string n, string s, Form *f, Pane *p);
  void set(string p,string v);

private slots:

private:
  QList<QLabel *> labelw;
  QList<QString> labels;

};

#endif
