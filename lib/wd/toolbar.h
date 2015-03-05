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

  void makeact(QStringList opt);
  string get(string p,string v);
  void set(string p,string v);
  QAction * getaction(QString id);
  void setbutton(string p, QStringList opt);
  string state();

private slots:
  void actionTriggered(QAction *action);

private:
  QList<QAction *>acts;

};

#endif
