#ifndef TABLE_H
#define TABLE_H

#include <QDateTime>
#include <QTableWidget>

#include "child.h"

class QTableWidgetItem;
class Form;
class Pane;
class Table;

// ---------------------------------------------------------------------
class QTableWidgex : public QTableWidget
{
  Q_OBJECT

public:
  QTableWidgex(Table *parent);
  Table *p;

protected:
  void mousePressEvent(QMouseEvent *event);
};

// ---------------------------------------------------------------------
class Table : public Child
{
  Q_OBJECT

public:
  Table(std::string n, std::string s, Form *f, Pane *p);

  // header/hover events and capability tokens (see docs/wd-table-extensions.md)
  static std::string capabilities();

  std::string get(std::string p,std::string v);
  void set(std::string p,std::string v);
  std::string state();
  std::string lmr;
  QDateTime dblclick;

protected:
  bool eventFilter(QObject *o, QEvent *e);

private slots:
  void on_sectionResized(int,int,int);
  void on_sectionClicked(int);
  void on_cellChanged(int,int);
  void on_currentCellChanged(int,int,int,int);
  void on_stateChanged(int);
  void on_headerClicked(int);
  void on_cellClicked(int,int);
  void on_cellDoubleClicked(int,int);
  void on_cellClicked_button();

private:
  friend class TableSortTest;
  void sortrows(int c, Qt::SortOrder order);
  void applyhdralign();
  Qt::Alignment getalign(int i);
  QVector<int> getcellvec(QVector<int>);
  bool getrange(std::string,int&, int&, int&, int&);
  void initglobals();

  QTableWidgetItem * newitem(int r, int c,QString s);
  std::string readcell(int row,int col);
  std::string readcellvalue(int row,int col);
  std::string readcolvalue(int col);
  std::string readrowvalue(int row);
  std::string readtable(std::string v);
  void resetlen(QVector<int> *v, QVector<int> def);
  void set_cell(int r,int c,QString v);

  void setalign(std::string v);
  void setbackforeground(int colortype, std::string s);
  void setblock(std::string v);
  void setcell(std::string v);
  void setcelltip(std::string v);
  void setcolminwidth(std::string v);
  void setcolsizing(std::string v);
  void setcolwidth(std::string v);
  void setheaderclick(std::string v);
  void sethover(std::string v);
  void setsortindicator(std::string v);
  void settooltips(std::string v);
  std::string getcolsizing();
  std::string getcolwidth(std::string v);
  std::string getsortindicator();
  std::string getcelltip(std::string v);
  void emithdr(const char *ev, int c);
  void setdata(std::string s);
  void setfontstr(std::string s);
  void sethdr(std::string v);
  void sethdralign(std::string v);
  void setlab(std::string v);
  void setprotect(std::string v);
  void setresizecol();
  void setresizerow();
  void setrowheight(std::string v);
  void setscroll(std::string v);
  void setselect(std::string v);
  void setshape(QStringList);
  void setsort(std::string v);
  void settype(std::string v);

  bool vecin(QVector<int>vec,QVector<int>values,QString id);
  bool vecisbool(QVector<int>vec,QString id);
  QVector<int> vecreshape(QVector<int> vec,int rws,int cls,int rws0,int cls0);

  int cls;
  int len;
  int rws;

  int col;
  int row;
  int lastcol;
  int lastrow;
  int markcol;
  int markrow;

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

  // r9 additions. All state is per Table instance (no shared/static state).
  bool autosize;          // resize columns to contents after set data / cell change (stretch, contents)
  int progdepth;          // >0 while the program (not the user) is changing widths
  bool dragging;          // user is dragging a header section edge
  int dragcol;
  int hdrmode;            // 0 off, 1 event, 2 native sort + event
  bool hoveron;
  int hoverrow, hovercol;
  std::string colsizingstr;
  int hdrwidth;           // width of the section last resized by the user
  int hdrcol;             // section last clicked / resized by the user
  int sortcol, sortorder; // indicator reported with hdrclick
};

#endif
