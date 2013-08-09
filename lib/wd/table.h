#ifndef TABLE_H
#define TABLE_H

#include "child.h"

class QTableWidgetItem;
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
  void on_cellClicked();

private:
  void applyhdralign();
  Qt::Alignment getalign(int i);
  QVector<int> getcellvec(QVector<int>);
  void initglobals();

  QTableWidgetItem * newitem(int r, int c,QString s);
  string readcell(int row,int col);
  string readcellvalue(int row,int col);
  string readcolvalue(int col);
  string readrowvalue(int row);
  string readtable(string v);
  void resetlen(QVector<int> *v, QVector<int> def);
  void set_cell(int r,int c,QString v);

  void setalign(string v);
  void setbackforeground(bool foreground, string s);
  void setblock(string v);
  void setcell(string v);
  void setcolwidth(string v);
  void setdata(string s);
  void setfont(string s);
  void sethdr(string v);
  void sethdralign(string v);
  void setlab(string v);
  void setprotect(string v);
  void setresizecol();
  void setrowheight(string v);
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
