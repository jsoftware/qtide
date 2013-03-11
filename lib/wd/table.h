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
  string state();

private slots:
  void on_cellChanged(int,int);
  void on_stateChanged(int);

private:
  void applyhdralign();
  Qt::Alignment getalign(int i);
  QVector<int> getcellvec(QVector<int>);
  void initglobals();
  void initsizes(QStringList);

  string readcell(int row,int col);

  void setalign(string v);
  void setdata(string s);
  void setedit(string v);
  void sethdr(string v);
  void sethdralign(string v);
  void setlab(string v);
  void settype(string v);

//  void tbinfo(QString);
  bool vecin(QVector<int>vec,QVector<int>values,QString id);
  bool vecisbool(QVector<int>vec,QString id);

  int cls;
  int len;
  int rws;

  int col;
  int row;

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
