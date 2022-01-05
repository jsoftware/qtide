#ifndef QGRID_H
#define QGRID_H

#include <QAbstractScrollArea>
#include <QBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QScrollBar>

#include "qutil.h"
#include "../wd/child.h"

// using namespace std;

class CubeMargin;
class Wasa;
class WGrid;

// ---------------------------------------------------------------------
class QGrid : public QWidget
{
  Q_OBJECT

public:
  QGrid(QString,Child *,QWidget *);
  ~QGrid();

  bool drawcube();
  void set(std::string,std::string);
  std::string state(std::string);

  bool GGridInit;
  Wasa *sa;
  QHBoxLayout *hbmain;
  QVBoxLayout *vbmain;
  CubeMargin *mcols, *mrows, *mslice;
  QString style;

  QStringList AxisNames;
  QList<QStringList> AxisLabels;
  QList<int> AxisRows;
  QList<int> AxisCols;
  QStringList AxisOrder;
  QList<int> AxisSlice;
  QVector<int> AxisIndex;
  QList<int> AxisShape;
  QStringList CellData;

public slots:
  void redraw();

private:
  void drawcuberows();
  void drawcubecols();
  void drawcubeslice();
  void drawbuttons(QBoxLayout *,QList<int>);
  void setaxisorder(QStringList s);
  void setaxislabels(QStringList s);
};

// ---------------------------------------------------------------------
class CubeCombo : public QWidget
{
  Q_OBJECT

public:
  CubeCombo(QString text, QWidget *parent = 0);

  QGrid *p;
  QComboBox *combobox;

private slots:
  void indexChanged(int);
  void makeDrag();
};

// ---------------------------------------------------------------------
class CubeButton : public QPushButton
{
  Q_OBJECT

public:
  CubeButton(QString text,QWidget *parent = 0);

private slots:
  void makeDrag();

private:
  void moveitem(int n,int from, int to);
};

// ---------------------------------------------------------------------
class CubeMargin : public QWidget
{
  Q_OBJECT

public:
  CubeMargin(QBoxLayout::Direction d, QWidget *parent = 0);

  void itemadd(QList<int> v, int n);
  int itemremove(QList<int> v, int n);
  void itemmovelast(QList<int> v,QList<int> w);

  QBoxLayout *layout;
  QGrid *p;

protected:
  void dropEvent(QDropEvent *de);
  void dragMoveEvent(QDragMoveEvent *de);
  void dragEnterEvent(QDragEnterEvent *event);
};

// ---------------------------------------------------------------------
class Wasa : public QAbstractScrollArea
{
  Q_OBJECT

public:
  Wasa(QString,Child *,QWidget *);

  void setmargins(int,int,int,int);
  void setscroll();
  QScrollBar *barh, *barv;

  WGrid *g;
  QGrid *p;
  QWidget *v;
  QString style;

protected:
  void resizeEvent(QResizeEvent *event);
};

// ---------------------------------------------------------------------

#endif
