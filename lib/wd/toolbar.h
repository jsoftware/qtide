#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "child.h"

class Form;
class Pane;
class QAction;
class QToolBar;

// ---------------------------------------------------------------------
class ToolBar : public Child
{
  Q_OBJECT

public:
  ToolBar(string n, string s, Form *f, Pane *p);
  //string state();

  void makeact(QStringList opt);
  void setp(string p,string v);

private slots:
//void triggered(QAction *action);
  void actionTriggered(QAction *action);

private:

};

#endif
