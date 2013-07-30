#ifndef TABLE_H
#define TABLE_H

#include "child.h"

class Form;
class Pane;

// ---------------------------------------------------------------------
class Table : public Child
{
  Q_OBJECT

public:
  Table(string n, string s, Form *f, Pane *p);

  void set(string p, string v);
  string get(string p, string v);
  string state();

private slots:
  void on_cellChanged(int,int);
  void on_currentCellChanged(int,int,int,int);
  void on_stateChanged(int);
  void on_headerClicked(int);

private:
  void applyhdralign();
  Qt::Alignment getalign(int i);
  QVector<int> getcellvec(QVector<int>);
  void initglobals();

  string readcell(int row,int col);
  string readcellvalue(int row,int col);
  string readcolvalue(int col);
  string readrowvalue(int row);
  string readtable(string v);

  void resetlen(QVector<int> *v, QVector<int> def);

  void setalign(string v,int mode);
  void setblock(string v);
  void setcell(string v);
  void setdata(string s,int mode);
  void setprotect(string v,int mode);
  void sethdr(string v);
  void sethdralign(string v);
  void setlab(string v);
  void setresizecol();
  void setshape(QStringList);
  void setsort(string v);
  void settype(string v,int mode);

  bool vecin(QVector<int>vec,QVector<int>values,QString id);
  bool vecisbool(QVector<int>vec,QString id);

  int cls;
  int len;
  int rws;

  int col;
  int row;
  int lastcol;
  int lastrow;

  int row1,row2,col1,col2;
  bool ifhdr;

  QVector<int> defcellalign;
  QVector<int> defcellprotect;
  QVector<int> defcelltype;

  QVector<int> cellalign;
  QVector<int> cellprotect;
  QVector<int> celltype;
  QVector<QWidget *> cellwidget;
  QVector<int> hdralign;

  QStringList dat;
};

#endif
