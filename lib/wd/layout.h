#ifndef LAYOUT_H
#define LAYOUT_H

#include <QWidget>
#include <QLayout>
#include <QGridLayout>
#include <QBoxLayout>

class Child;
class Pane;

#include "wd.h"
#include "pane.h"
#include "child.h"

class Layout : public QObject
{
  Q_OBJECT

public:
  Layout(QChar type, int stretch, Pane *p);
  void addWidget(QWidget *b);
  void addLayout(Layout *b);
  void addSpacing(int n);
  void addStretch(int n);
  void removeWidget(QWidget *b);

  Pane *ppane;
  QLayout *bin;
  QChar type;
  int r,c,rs,cs;
  int alignment;
  int stretch;
  int spacing;
  bool razed;
  int rmax,cmax;

};

#endif
