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
  string readtable();

  void resetlen(QVector<int> *v, QVector<int> def);

  void setalign(string v);
  void setcell(string v);
  void setdata(string s);
  void setedit(string v);
  void sethdr(string v);
  void sethdralign(string v);
  void setlab(string v);
  void setshape(QStringList);
  void setsort(string v);
  void settype(string v);

  bool vecin(QVector<int>vec,QVector<int>values,QString id);
  bool vecisbool(QVector<int>vec,QString id);

  int cls;
  int len;
  int rws;

  int col;
  int row;
  int lastcol;
  int lastrow;

  bool ifhdr;

  QVector<int> defcellalign;
  QVector<int> defcelledit;
  QVector<int> defcelltype;

  QVector<int> cellalign;
  QVector<int> celledit;
  QVector<int> celltype;
  QVector<QWidget *> cellwidget;
  QVector<int> hdralign;

  QStringList dat;
};

#endif
