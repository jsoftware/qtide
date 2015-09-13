#ifndef NMAIN_H
#define NMAIN_H

#include <QToolBar>
#include <QWidget>

class Note;
class Ntabs;

class Nmain : public QWidget
{
  Q_OBJECT

public:
  Nmain(Note *n);

  Ntabs *tabBar;
  QToolBar *toolBar;
  QAction *runallAct;

private:

  void createActions();
  void createTabBar();
  void createToolBar();

  QAction *makeact(QString id, QString icon, QString shortcut);

  QAction *lastprojectAct;
  QAction *openprojectAct;
};

#endif
