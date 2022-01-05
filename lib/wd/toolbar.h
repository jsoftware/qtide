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
  ToolBar(std::string n, std::string s, Form *f, Pane *p);

  void makeact(QStringList opt);
  void makeactright(QStringList opt);
  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  QAction * getaction(QString id);
  void setbutton(std::string p, QStringList opt);
  std::string state();

private slots:
  void actionTriggered(QAction *action);

private:
  QList<QAction *>acts;

};

#endif
