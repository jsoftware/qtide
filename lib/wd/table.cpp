
#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QHeaderView>
#include <QTableWidget>
#include <QPushButton>

#include "cmd.h"
#include "font.h"
#include "form.h"
#include "pane.h"
#include "table.h"
#include "wd.h"
#include "../base/plaintextedit.h"
#include "../base/state.h"

static QVector<int>CellAligns;
static QVector<int>CellTypes;
extern int rc;

// -------------------------------------------------------------------
QTableWidgex::QTableWidgex(Table *parent)
{
  p=parent;
}

// -------------------------------------------------------------------
void QTableWidgex::mousePressEvent(QMouseEvent* event)
{
  switch (event->button()) {
  case Qt::LeftButton:
    p->lmr = "l";
    break;
  case Qt::MiddleButton:
    p->lmr = "m";
    break;
  case Qt::RightButton:
    p->lmr = "r";
    break;
  default:
    ;
  }
  QTableWidget::mousePressEvent(event);
}

// -------------------------------------------------------------------
Table::Table(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="table";
  initglobals();
  ifhdr=false;
  rws=cls=len=0;
  row1=col1=0;
  row2=col2=-1;
  markrow=markcol=0;
  dblclick=QDateTime::currentDateTime();

  QTableWidgex *w=new QTableWidgex(this);
  widget=(QWidget*) w;
  w->setObjectName(s2q(n));
  QStringList opt=qsplit(s);
  if (invalidoptn(n,opt,"selectrows sortable nostretchlast")) return;
  QStringList shape;

  if (opt.size()>=2) {
    if ((isint(q2s(opt.at(0)))) && ((isint(q2s(opt.at(1)))))) {
      shape=qsplit((q2s(opt.at(0)))+" "+(q2s(opt.at(1))));
      setshape(shape);
    }
  }

  w->resizeColumnsToContents();
  w->resizeRowsToContents();
  w->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
  w->horizontalHeader()->setHighlightSections(false);
  w->horizontalHeader()->setVisible(false);

  if (opt.contains("nostretchlast"))
    w->horizontalHeader()->setStretchLastSection(false);
  else
    w->horizontalHeader()->setStretchLastSection(true);

  w->verticalHeader()->setHighlightSections(false);
  w->verticalHeader()->setVisible(false);
  QFontMetrics fm(w->font());
  w->verticalHeader()->setDefaultSectionSize(fm.height() + 6);

  w->setSelectionMode(QAbstractItemView::ContiguousSelection);
  w->setAlternatingRowColors(true);

  if (opt.contains("selectrows")) {
    w->setSelectionBehavior(QAbstractItemView::SelectRows);
    w->selectRow(0);
  }

// this needs updating for Qt5...
#ifndef QT50
  if (opt.contains("sortable")) {
    w->horizontalHeader()->setClickable(true);
    connect(w->horizontalHeader(),SIGNAL(sectionDoubleClicked(int)),this,SLOT(on_headerClicked(int)));
  }
#endif

  connect(w,SIGNAL(cellChanged(int,int)),
          this,SLOT(on_cellChanged(int,int)));
  connect(w,SIGNAL(cellClicked(int,int)),
          this,SLOT(on_cellClicked(int,int)));
  connect(w,SIGNAL(cellDoubleClicked(int,int)),
          this,SLOT(on_cellDoubleClicked(int,int)));
  connect(w,SIGNAL(currentCellChanged(int,int,int,int)),
          this,SLOT(on_currentCellChanged(int,int,int,int)));
}

// ---------------------------------------------------------------------
void Table::applyhdralign()
{
  QTableWidget *w=(QTableWidget*) widget;
  if (hdralign.isEmpty() || !ifhdr) return;
  if (hdralign.size()==1)
    w->horizontalHeader()->setDefaultAlignment(getalign(hdralign.at(0)));
  else {
    for (int i=0; i<cls; i++)
      (w->horizontalHeaderItem(i))->setTextAlignment(getalign(hdralign.at(i)));
  }
}

// ---------------------------------------------------------------------
std::string Table::get(std::string p,std::string v)
{
  QStringList opt;
  int r,c;

  if (p=="property") {
    std::string r;
    r+=std::string("cell")+"\012"+ "col"+"\012"+ "row"+"\012"+ "table"+"\012";
    r+=Child::get(p,v);
    return r;
  } else if (p=="cell") {
    opt=qsplit(v);
    if (!(opt.size()==2)) {
      error("get cell must specify row, column: " + q2s(opt.join(" ")));
      return"";
    }
    r=c_strtoi(q2s(opt.at(0)));
    c=c_strtoi(q2s(opt.at(1)));
    if (!(((r>=0) && (r<rws)) && ((c>=0) && (c<cls)))) {
      error("cell index out of bounds: " + q2s(opt.join(" ")));
      return"";
    }
    rc=-1;
    return (readcellvalue(r,c));
  } else if (p=="row") {
    opt=qsplit(v);
    if (!(opt.size()==1)) {
      error("get row must specify row: " + q2s(opt.join(" ")));
      return"";
    }
    r=c_strtoi(q2s(opt.at(0)));
    if (!((r>=0) && (r<rws))) {
      error("row index out of bounds: " + q2s(opt.join(" ")));
      return"";
    }
    rc=-1;
    return (readrowvalue(r));
  } else if (p=="col") {
    opt=qsplit(v);
    if (!(opt.size()==1)) {
      error("get col must specify column: " + q2s(opt.join(" ")));
      return"";
    }
    c=c_strtoi(q2s(opt.at(0)));
    if (!((c>=0)&&(c<cls))) {
      error("col index out of bounds: " + q2s(opt.join(" ")));
      return"";
    }
    rc=-1;
    return (readcolvalue(c));
  } else if (p=="table") {
    return(readtable(v));
  } else
    return Child::get(p,v);
}

// ---------------------------------------------------------------------
Qt::Alignment Table::getalign(int i)
{
  switch (i) {
  case 0:
    return Qt::AlignLeft|Qt::AlignVCenter;
  case 1:
    return Qt::AlignHCenter|Qt::AlignVCenter;
  default :
    return Qt::AlignRight|Qt::AlignVCenter;
  }
}

// ---------------------------------------------------------------------
QVector<int> Table::getcellvec(QVector<int> v)
{
  if (len==v.size())
    return v;
  if (1==v.size())
    return QVector<int>(len,v.at(0));
  QVector<int> r(len);
  for(int i=0; i<rws; i++)
    for(int j=0; j<cls; j++)
      r[j+i*cls]=v[j];
  return r;
}

// ---------------------------------------------------------------------
// get range for set block, set select
// r1 [r2] c1 [c2] or empty for all
bool Table::getrange(std::string v,int &r1, int &r2, int &c1, int &c2)
{
  QStringList arg=qsplit(v);
  int n=arg.size();
  if (0==n) {
    r1= c1= 0;
    r2= c2= -1;
  } else if (2==n) {
    r1= r2= c_strtoi(q2s(arg.at(0)));
    c1= c2= c_strtoi(q2s(arg.at(1)));
  } else if (4==n) {
    r1= c_strtoi(q2s(arg.at(0)));
    r2= c_strtoi(q2s(arg.at(1)));
    c1= c_strtoi(q2s(arg.at(2)));
    c2= c_strtoi(q2s(arg.at(3)));
  } else {
    error("set range incorrect length: " + q2s(arg.join(" ")));
    return false;
  }
  if (!(r1>=0 && r1<rws && c1>=0 && c1<cls && r2>=-1 && r2<rws && c2>=-1 && c2<cls && (-1==r2 || r1<=r2) && (-1==c2 || c1<=c2))) {
    error("set range row1 row2 col1 col2 out of bound: " + q2s(QString::number(r1)) + " " + q2s(QString::number(r2)) + " " + q2s(QString::number(c1)) + " " + q2s(QString::number(c2)));
    return false;
  }
  return true;
}

// ---------------------------------------------------------------------
void Table::initglobals()
{
  if (CellAligns.size()) return;
  CellAligns << 0 << 1 << 2;
  CellTypes << 0 << 10 << 100 << 200 << 300 << 400;
}

// ---------------------------------------------------------------------
QTableWidgetItem * Table::newitem(int r, int c,QString s)
{
  int p= c + r*cls;
  QTableWidgetItem * m= new QTableWidgetItem(s);
  m->setTextAlignment(getalign(cellalign[p]));
  if (cellprotect[p]) {
    Qt::ItemFlags fdef=m->flags();
    Qt::ItemFlags fnoedit=fdef & ~(Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
    m->setFlags(fnoedit);
  }
  return m;
}

// ---------------------------------------------------------------------
std::string Table::readcell(int row,int col)
{
  QTableWidget *w=(QTableWidget*) widget;
  QTableWidgetItem *m=w->item(row,col);
  int p=col+row*cls;
  QWidget *g=cellwidget[p];
  if (0==celltype[p])
    return (!m)?"":q2s(m->text());
  else if (10==celltype[p])
    return (!g)?"":q2s(((PlainTextEdit *)g)->toPlainText());
  else if (100==celltype[p])
    return (!g)?"":((QCheckBox *)g)->isChecked()?"1":"0";
  else if ((200==celltype[p]) || (300==celltype[p]))
    return (!g)?"":i2s(((QComboBox *)g)->currentIndex());
  else
    return "";
}

// ---------------------------------------------------------------------
std::string Table::readcellvalue(int row,int col)
{
  QTableWidget *w=(QTableWidget*) widget;
  QTableWidgetItem *m=w->item(row,col);
  int p=col+row*cls;
  QWidget *g=cellwidget[p];
  if (0==celltype[p])
    return (!m)?"":q2s(m->text());
  else if (10==celltype[p])
    return (!g)?"":q2s(((PlainTextEdit *)g)->toPlainText());
  else if (100==celltype[p])
    return (!g)?"":((QCheckBox *)g)->isChecked()?"1":"0";
  else if ((200==celltype[p]) || (300==celltype[p]))
    return (!g)?"":q2s(((QComboBox *)g)->currentText());
  else
    return "";
}

// ---------------------------------------------------------------------
std::string Table::readcolvalue(int col)
{
  std::string colout="";
  int r;

  for (r=0; r<rws; r++)
    colout += readcellvalue(r,col) + "\177";
  return (colout);
}

// ---------------------------------------------------------------------
std::string Table::readrowvalue(int row)
{
  std::string rowout="";
  int c;

  for (c=0; c<cls; c++)
    rowout += readcellvalue(row,c) + "\177";
  return (rowout);
}

// ---------------------------------------------------------------------
std::string Table::readtable(std::string v)
{
  std::string tableout="";
  int r1,r2,c1,c2;

  QStringList opt;
  opt=qsplit(v);
  if (0==opt.size()) {
    r1= 0;
    r2= rws-1;
    c1= 0;
    c2= cls-1;
  } else if (2==opt.size()) {
    r1= r2= c_strtoi(q2s(opt.at(0)));
    c1= c2= c_strtoi(q2s(opt.at(1)));
  } else if (4==opt.size()) {
    r1= c_strtoi(q2s(opt.at(0)));
    r2= c_strtoi(q2s(opt.at(1)));
    c1= c_strtoi(q2s(opt.at(2)));
    c2= c_strtoi(q2s(opt.at(3)));
  } else {
    error("get table incorrect number of arguments: " + q2s(opt.join(" ")));
    return "";
  }

  if (!(r1>=0 && r1<rws && c1>=0 && c1<cls && r2>=-1 && r2<rws && c2>=-1 && c2<cls && (-1==r2 || r1<=r2) && (-1==c2 || c1<=c2))) {
    error("get table row1 row2 col1 col2 out of bound: " + q2s(QString::number(r1)) + " " + q2s(QString::number(r2)) + " " + q2s(QString::number(c1)) + " " + q2s(QString::number(c2)));
    return "";
  }
  if (r2==-1) r2=rws-1;
  if (c2==-1) c2=cls-1;

  for (int r=r1; r<=r2; r++) {
    for (int c=c1; c<=c2; c++) {
      tableout += readcellvalue(r,c) + "\177";
    }
  }
  rc=-1;
  return (tableout);
}

// ---------------------------------------------------------------------
void Table::resetlen(QVector<int> *v, QVector<int> def)
{
  v->resize(len);
  if (len==0) return;

  if (def.size()==1)
    v->fill(def.at(0));
  else if (def.size()==cls) {
    int i,j;
    for (i=0; i<rws; i++)
      for (j=0; j<cls; j++)
        v->replace(j+i*cls,def[j]);
  } else if (def.size()==len) {
    for (int i=0; i<len; i++) v->replace(i,def[i]);
  } else
    v->fill(0);
}

// ---------------------------------------------------------------------
void Table::set(std::string p,std::string v)
{
  if (p=="align")
    setalign(v);
  else if (p=="background")
    setbackforeground(0,v);
  else if (p=="block")
    setblock(v);
  else if (p=="color")
    setbackforeground(2,v);
  else if (p=="colwidth")
    setcolwidth(v);
  else if (p=="data")
    setdata(v);
  else if (p=="font")
    setfontstr(v);
  else if (p=="foreground")
    setbackforeground(1,v);
  else if (p=="hdr")
    sethdr(v);
  else if (p=="hdralign")
    sethdralign(v);
  else if (p=="lab")
    setlab(v);
  else if (p=="protect")
    setprotect(v);
  else if (p=="resizecol")
    setresizecol();
  else if (p=="resizerow")
    setresizerow();
  else if (p=="rowheight")
    setrowheight(v);
  else if (p=="scroll")
    setscroll(v);
  else if (p=="select")
    setselect(v);
  else if (p=="shape")
    setshape(qsplit(v));
  else if (p=="type")
    settype(v);
  else if (p=="cell")
    setcell(v);
  else if (p=="sort")
    setsort(v);
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
void Table::setalign(std::string v)
{
  QVector<int> a=qs2intvector(s2q(v));
  int n=a.size();

  int len1,r1,r2,c1,c2;
  r1=row1;
  r2=row2;
  c1=col1;
  c2=col2;
  if (!((r1>=0 && r1<rws && c1>=0 && c1<cls && r2>=-1 && r2<rws && c2>=-1 && c2<cls && (-1==r2 || r1<=r2) && (-1==c2 || c1<=c2)) || (0==rws && ((((c2==-1)?(cls-1):c2)-c1+1)==n || 1==n || 0==n)))) {
    error("set align row1 row2 col1 col2 out of bound: " + q2s(QString::number(r1)) + " " + q2s(QString::number(r2)) + " " + q2s(QString::number(c1)) + " " + q2s(QString::number(c2)));
    return;
  }
  if (r2==-1) r2=rws-1;
  if (c2==-1) c2=cls-1;
  if (0==rws || 0==n) return;
  bool colmode= (c2-c1+1)==n;

  if (!(n==1 || n== (len1=(r2-r1+1)*(c2-c1+1)) || colmode)) {
    QString m="incorrect align length - ";
    m+= "given " + QString::number(n);
    m+=" cells, require " + QString::number(len1) + " cells";
    error(q2s(m));
    return;
  }
  if(!vecin(a,CellAligns,"align")) return;
  if (0==defcellalign.size()) {
    defcellalign=QVector<int>(len,0);
    cellalign=QVector<int>(len,0);
  }

  QTableWidget *w=(QTableWidget*) widget;
  QTableWidgetItem *m;
  int q=0;
  for (int r=r1; r<=r2; r++) {
    for (int c=c1; c<=c2; c++) {
      int p=c + r*cls;
      if (colmode && c==c1) q=0;
      defcellalign[p]=a.at(q);
      cellalign[p]=a.at(q);
      if ((m=w->item(r,c))) m->setTextAlignment(getalign(cellalign[p]));
      if (n!=1) q++;
    }
  }
}

// ---------------------------------------------------------------------
void Table::setbackforeground(int colortype, std::string s)
{
  QTableWidget *w=(QTableWidget*) widget;

  QStringList opt=qsplit(s);
  int len1,r1,r2,c1,c2;
  int n=opt.size();

  if (2==colortype) {
    if (!(0==n%2)) {
      error("set color requires paired background and foreground values");
      return;
    }
    n=n/2;
  }
  r1=row1;
  r2=row2;
  c1=col1;
  c2=col2;
  if (!((r1>=0 && r1<rws && c1>=0 && c1<cls && r2>=-1 && r2<rws && c2>=-1 && c2<cls && (-1==r2 || r1<=r2) && (-1==c2 || c1<=c2)) || (0==rws && ((((c2==-1)?(cls-1):c2)-c1+1)==n || 1==n || 0==n)))) {
    error("set back/foreground row1 row2 col1 col2 out of bound: " + q2s(QString::number(r1)) + " " + q2s(QString::number(r2)) + " " + q2s(QString::number(c1)) + " " + q2s(QString::number(c2)));
    return;
  }
  if (r2==-1) r2=rws-1;
  if (c2==-1) c2=cls-1;
  if (0==rws || 0==n) return;
  bool colmode= (c2-c1+1)==n;

  if (!(n==1 || n== (len1=(r2-r1+1)*(c2-c1+1)) || colmode)) {
    QString m="incorrect back/foreground length - ";
    m+= "given " + QString::number(n);
    m+=" cells, require " + QString::number(len1) + " cells";
    error(q2s(m));
    return;
  }

  QTableWidgetItem *m;
  QWidget *g;
  QBrush brush,brush2;
  int q=0;
  for (int r=r1; r<=r2; r++) {
    for (int c=c1; c<=c2; c++) {
      int p= c + r*cls;
      if (colmode && c==c1) q=0;
      if (0==celltype[p]) {
        if (!(m=w->item(r,c))) {
          m= newitem(r,c,"");
          w->setItem(r,c,m);
        }
        brush = QBrush(QColor(opt.at(q)));
        if (0==colortype) m->setBackground(brush);
        else if (1==colortype) m->setForeground(brush);
        else if (2==colortype) {
          m->setBackground(brush);
          brush2 = QBrush(QColor(opt.at(q+1)));
          m->setForeground(brush2);
        }
      } else if ((g=w->cellWidget(r,c))) {
        if ((0==colortype) && (200>celltype[p])) g->setStyleSheet("background-color: " + opt.at(q));
        else if (1==colortype) g->setStyleSheet("color: " + opt.at(q));
        else if (2==colortype) {
          if (200>celltype[p]) g->setStyleSheet("background-color: " + opt.at(q) + "; color: " + opt.at(q+1));
          else  g->setStyleSheet("color: " + opt.at(q+1));
        }
      }
      if (n!=1) {
        q++;
        if (2==colortype) q++;
      }
    }
  }
}

// ---------------------------------------------------------------------
void Table::setblock(std::string v)
{
  int r1,r2,c1,c2;
  if (!getrange(v,r1,r2,c1,c2)) return;
  row1=r1;
  row2=r2;
  col1=c1;
  col2=c2;
}

// ---------------------------------------------------------------------
// called by setcell and setdata
void Table::set_cell(int r,int c,QString v)
{
  int p=c+r*cls;
  QTableWidget *w=(QTableWidget*) widget;
  QStringList dat;

  if (0==celltype[p]) {
    QTableWidgetItem *m=w->item(r,c);
    QWidget *g=w->cellWidget(r,c);
    if (g) {
      w->removeCellWidget(r,c);
      cellwidget[p]=0;
    }
    if (!m) {
      m= newitem(r,c,v);
      w->setItem(r,c,m);
    } else
      m->setText(v);
  } else if (10==celltype[p]) {
    if (w->item(r,c)) delete w->item(r,c);
    QWidget *g=cellwidget[p];
    if (!(g && QString("PlainTextEdit")==g->metaObject()->className())) {
      if (w->cellWidget(r,c)) w->removeCellWidget(r,c);
      PlainTextEdit *ed=new PlainTextEdit();
      ed->setObjectName(QString::number(p));
      g=cellwidget[p]=(QWidget*) ed;
      QWidget *m=new QWidget();
      QHBoxLayout *y=new QHBoxLayout();
      y->setContentsMargins(0,0,0,0);
      y->setSpacing(0);
      y->addStretch(1);
      y->addWidget(ed);
      y->addStretch(1);
      m->setLayout(y);
      w->setCellWidget(r,c,m);
    }
    ((PlainTextEdit  *)cellwidget[p])->setPlainText(v);
  } else if (100==celltype[p]) {
    if (w->item(r,c)) delete w->item(r,c);
    QWidget *g=cellwidget[p];
    if (!(g && QString("QCheckBox")==g->metaObject()->className())) {
      if (w->cellWidget(r,c)) w->removeCellWidget(r,c);
      QCheckBox *cb=new QCheckBox();
      cb->setObjectName(QString::number(p));
      g=cellwidget[p]=(QWidget*) cb;
      QWidget *m=new QWidget();
      QHBoxLayout *y=new QHBoxLayout();
      y->setContentsMargins(0,0,0,0);
      y->setSpacing(0);
      y->addStretch(1);
      y->addWidget(cb);
      y->addStretch(1);
      m->setLayout(y);
      w->setCellWidget(r,c,m);
      connect(cb,SIGNAL(stateChanged(int)),
              this,SLOT(on_stateChanged(int)));
    }
    if ("1"==q2s(v))
      ((QCheckBox *)cellwidget[p])->setChecked(true);
    else
      ((QCheckBox *)cellwidget[p])->setChecked(false);
  } else if ((200==celltype[p]) || (300==celltype[p])) {
    if (w->item(r,c)) delete w->item(r,c);
    QWidget *g=cellwidget[p];
    if (!(g && QString("QComboBox")==g->metaObject()->className())) {
      if (w->cellWidget(r,c)) w->removeCellWidget(r,c);
      QComboBox *cm=new QComboBox();
      cm->setObjectName(QString::number(p));
      if (300==celltype[p])
        cm->setEditable(true);
      g=cellwidget[p]=(QWidget*) cm;
      QWidget *m=new QWidget();
      QHBoxLayout *y=new QHBoxLayout();
      y->setContentsMargins(0,0,0,0);
      y->setSpacing(0);
      y->addWidget(cm);
      m->setLayout(y);
      w->setCellWidget(r,c,m);
      connect(cm,SIGNAL(currentIndexChanged(int)),
              this,SLOT(on_stateChanged(int)));
    }
    int cmind=0;
    dat= qsplit(q2s(v));
    if (1==dat.size() && isint(q2s(dat.at(0)))) {
      cmind=c_strtoi(q2s(dat.at(0)));
      ((QComboBox *)cellwidget[p])->setCurrentIndex(cmind);
      return;
    }
    if (1<dat.size() && isint(q2s(dat.at(0)))) {
      cmind=c_strtoi(q2s(dat.at(0)));
      dat.removeFirst();
    }
    ((QComboBox *)cellwidget[p])->clear();
    ((QComboBox *)cellwidget[p])->addItems(dat);
    ((QComboBox *)cellwidget[p])->setCurrentIndex(cmind);
  } else if (400==celltype[p]) {
    if (w->item(r,c)) delete w->item(r,c);
    QWidget *g=cellwidget[p];
    if (!(g && QString("QPushButton")==g->metaObject()->className())) {
      if (w->cellWidget(r,c)) w->removeCellWidget(r,c);
      QPushButton *pb=new QPushButton(v);
      pb->setObjectName(QString::number(p));
      g=cellwidget[p]=(QWidget*) pb;
      w->setCellWidget(r,c,pb);
      connect(pb,SIGNAL(clicked()),this,SLOT(on_cellClicked_button()));
    } else
      ((QPushButton *)g)->setText(v);
  }
}

// ---------------------------------------------------------------------
void Table::setcell(std::string v)
{
  QStringList opt;

  opt=qsplit(v);
  if (!(opt.size()==3)) {
    error("set cell must specify row, column, and data: " + q2s(opt.join(" ")));
    return;
  }
  int r=c_strtoi(q2s(opt.at(0)));
  int c=c_strtoi(q2s(opt.at(1)));
  if (!(((r>=0) && (r<rws)) && ((c>=0) && (c<cls)))) {
    error("cell index out of bounds: " + q2s(opt.join(" ")));
    return;
  }
  set_cell(r,c,opt.at(2));
}

// ---------------------------------------------------------------------
void Table::setcolwidth(std::string s)
{
  QTableWidget *w=(QTableWidget*) widget;
  QStringList opt;
  int c,c1,c2,cs,i,width;

  c1=col1;
  c2=col2;
  if (!((c1>=0 && c1<cls && c2>=-1 && c2<cls && (-1==c2 || c1<=c2)) || (0==cls))) {
    error("set colwidth col1 col2 out of bound: " + q2s(QString::number(c1)) + " " + q2s(QString::number(c2)));
    return;
  }
  if (c2==-1) c2=cls-1;
  if (0==cls) return;
  cs=1+(c2-c1);
  opt=qsplit(s);
  if (!((opt.size()==1) || opt.size()==cs)) {
    error("set colwidth must specify a single width or one for each column in block: " + q2s(opt.join(" ")));
    return;
  }
  bool eqwid= (1==opt.size());
  i=0;
  for (c=c1; c<=c2; c++) {
    width= c_strtoi(q2s(opt.at(i)));
    w->setColumnWidth(c,width);
    if (!eqwid) i++;
  }
  return;
}

// ---------------------------------------------------------------------
void Table::setdata(std::string s)
{
  QTableWidget *w=(QTableWidget*) widget;

  dat=qsplit(s);
  int n=dat.size();

  int len1,r1,r2,c1,c2;
  r1=row1;
  r2=row2;
  c1=col1;
  c2=col2;
  if (!((r1>=0 && r1<rws && c1>=0 && c1<cls && r2>=-1 && r2<rws && c2>=-1 && c2<cls && (-1==r2 || r1<=r2) && (-1==c2 || c1<=c2)) || (0==rws && ((((c2==-1)?(cls-1):c2)-c1+1)==n || 1==n || 0==n)))) {
    error("set data row1 row2 col1 col2 out of bound: " + q2s(QString::number(r1)) + " " + q2s(QString::number(r2)) + q2s(QString::number(c1)) + " " + q2s(QString::number(c2)));
    return;
  }
  if (r2==-1) r2=rws-1;
  if (c2==-1) c2=cls-1;
  if (0==rws || 0==n) return;
  bool colmode= (c2-c1+1)==n;

  if (!(n==1 || n== (len1=(r2-r1+1)*(c2-c1+1)) || colmode)) {
    QString m="incorrect data length - ";
    m+= "given " + QString::number(n);
    m+=" cells, require " + QString::number(len1) + " cells";
    error(q2s(m));
    return;
  }

  int q=0;
  for (int r=r1; r<=r2; r++) {
    for (int c=c1; c<=c2; c++) {
      if (colmode && c==c1) q=0;
      set_cell(r,c,dat[q]);
      if (n!=1) q++;
    }
  }
  w->setVisible(false);
  w->resizeColumnsToContents();
  w->resizeRowsToContents();
  w->setVisible(true);
}

// ---------------------------------------------------------------------
void Table::setfontstr(std::string s)
{
  int c,r;
  QTableWidget *w=(QTableWidget*) widget;

  int r1,r2,c1,c2;
  r1=row1;
  r2=row2;
  c1=col1;
  c2=col2;
  if (!((r1>=0 && r1<rws && c1>=0 && c1<cls && r2>=-1 && r2<rws && c2>=-1 && c2<cls && (-1==r2 || r1<=r2) && (-1==c2 || c1<=c2)) || (0==rws))) {
    error("set font row1 row2 col1 col2 out of bound: " + q2s(QString::number(r1)) + " " + q2s(QString::number(r2)) + q2s(QString::number(c1)) + " " + q2s(QString::number(c2)));
    return;
  }
  if (r2==-1) r2=rws-1;
  if (c2==-1) c2=cls-1;
  if (0==rws) return;

  QFont font= (Font(s)).font;

  QTableWidgetItem *m;
  QWidget *g;
  for (r=r1; r<=r2; r++) {
    for (c=c1; c<=c2; c++) {
      int p= c + r*cls;
      if (0==celltype[p]) {
        if (!(m=w->item(r,c))) {
          m= newitem(r,c,"");
          w->setItem(r,c,m);
        }
        m->setFont(font);
      } else if ((g=w->cellWidget(r,c))) {
        g->setFont(font);
      }
    }
  }
}

// ---------------------------------------------------------------------
void Table::sethdr(std::string v)
{
  QTableWidget *w=(QTableWidget*) widget;
  QStringList s=qsplit(v);
  if (s.size()!=cls) {
    QString m=QString::number(s.size());
    m+= " column headers do not match column count of ";
    m+= QString::number(cls);
    error(q2s(m));
    return;
  }
  w->setHorizontalHeaderLabels(s);
  w->horizontalHeader()->setVisible(true);
  ifhdr=true;
  applyhdralign();
}

// ---------------------------------------------------------------------
void Table::sethdralign(std::string v)
{
  QVector<int> a=qs2intvector(s2q(v));
  if (!(a.size()==1 || a.size()==cls)) {
    QString m=QString::number(a.size());
    m+= " column header alignments do not match column count of ";
    m+= QString::number(cls);
    error(q2s(m));
    return;
  }
  hdralign=a;
  applyhdralign();
}

// ---------------------------------------------------------------------
void Table::setlab(std::string v)
{
  QTableWidget *w=(QTableWidget*) widget;
  QStringList s=qsplit(v);
  if (s.size()!=rws) {
    QString m=QString::number(s.size());
    m+= " row labels do not match row count of ";
    m+= QString::number(rws);
    error(q2s(m));
    return;
  }
  w->setVerticalHeaderLabels(s);
  w->verticalHeader()->setVisible(true);
}

// ---------------------------------------------------------------------
void Table::setprotect(std::string v)
{
  QVector<int> a=qs2intvector(s2q(v));
  int n=a.size();

  int len1,r1,r2,c1,c2;
  r1=row1;
  r2=row2;
  c1=col1;
  c2=col2;
  if (!((r1>=0 && r1<rws && c1>=0 && c1<cls && r2>=-1 && r2<rws && c2>=-1 && c2<cls && (-1==r2 || r1<=r2) && (-1==c2 || c1<=c2)) || (0==rws && ((((c2==-1)?(cls-1):c2)-c1+1)==n || 1==n || 0==n)))) {
    error("set protect row1 row2 col1 col2 out of bound: " + q2s(QString::number(r1)) + " " + q2s(QString::number(r2)) + " " + q2s(QString::number(c1)) + " " + q2s(QString::number(c2)));
    return;
  }
  if (r2==-1) r2=rws-1;
  if (c2==-1) c2=cls-1;
  if (0==rws || 0==n) return;
  bool colmode= (c2-c1+1)==n;

  if (!(n==1 || n== (len1=(r2-r1+1)*(c2-c1+1)) || colmode)) {
    QString m="incorrect protect length - ";
    m+= "given " + QString::number(n);
    m+=" cells, require " + QString::number(len1) + " cells";
    error(q2s(m));
    return;
  }
  if(!vecisbool(a,"protect")) return;
  if (0==defcellprotect.size()) {
    defcellprotect=QVector<int>(len,0);
    cellprotect=QVector<int>(len,0);
  }

  QTableWidget *w=(QTableWidget*) widget;
  QTableWidgetItem *m;
  int q=0;
  for (int r=r1; r<=r2; r++) {
    for (int c=c1; c<=c2; c++) {
      int p=c + r*cls;
      if (colmode && c==c1) q=0;
      defcellprotect[p]=a.at(q);
      cellprotect[p]=a.at(q);
      if ((m=w->item(r,c))) {
        Qt::ItemFlags fdef=m->flags();
        Qt::ItemFlags fnoedit;
        if (cellprotect[p])
          fnoedit=fdef & ~(Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
        else
          fnoedit=fdef | (Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
        m->setFlags(fnoedit);
      }
      if (n!=1) q++;
    }
  }
}

// ---------------------------------------------------------------------
void Table::setresizecol()
{
  QTableWidget *w=(QTableWidget*) widget;
  w->resizeColumnsToContents();
}

// ---------------------------------------------------------------------
void Table::setresizerow()
{
  QTableWidget *w=(QTableWidget*) widget;
  w->resizeRowsToContents();
}

// ---------------------------------------------------------------------
void Table::setrowheight(std::string s)
{
  QTableWidget *w=(QTableWidget*) widget;
  QStringList opt;
  int r,r1,r2,rs,i,height;

  r1=row1;
  r2=row2;
  if (!((r1>=0 && r1<rws && r2>=-1 && r2<rws && (-1==r2 || r1<=r2)) || (0==rws))) {
    error("set rowheight row1 row2 out of bound: " + q2s(QString::number(r1)) + " " + q2s(QString::number(r2)));
    return;
  }
  if (r2==-1) r2=rws-1;
  if (0==rws) return;
  rs=1+(r2-r1);
  opt=qsplit(s);
  if (!((opt.size()==1) || opt.size()==rs)) {
    error("set rowheight must specify a single height or one for each row in block: " + q2s(opt.join(" ")));
    return;
  }
  bool eqht= (1==opt.size());
  i=0;
  for (r=r1; r<=r2; r++) {
    height= c_strtoi(q2s(opt.at(i)));
    w->setRowHeight(r,height);
    if (!eqht) i++;
  }
  return;
}

// ---------------------------------------------------------------------
void Table::setscroll(std::string v)
{
  QStringList opt;
  QTableWidget *w=(QTableWidget*) widget;

  opt=qsplit(v);
  if (!(opt.size()==2)) {
    error("scroll must specify row and column: " + q2s(opt.join(" ")));
    return;
  }
  int r=c_strtoi(q2s(opt.at(0)));
  int c=c_strtoi(q2s(opt.at(1)));
  if (!(((r>=0) && (r<rws)) && ((c>=0) && (c<cls)))) {
    error("scroll index out of bounds: " + q2s(opt.join(" ")));
    return;
  }

  QModelIndex index = w->currentIndex();
  Q_UNUSED(index);
  w->scrollTo(w->model()->index(r,cls-1), QAbstractItemView::PositionAtTop);
  w->scrollTo(w->model()->index(r,c), QAbstractItemView::PositionAtTop);
  w->setFocus();
}

// ---------------------------------------------------------------------
void Table::setselect(std::string v)
{
  int r1,r2,c1,c2;
  if (!getrange(v,r1,r2,c1,c2)) return;
  QTableWidget *w=(QTableWidget*) widget;
  markrow=qMin(r1,r2);
  markcol=qMin(c1,c2);
  row=qMax(r1,r2);
  col=qMax(c1,c2);
  foreach(QTableWidgetSelectionRange r,w->selectedRanges())
    w->setRangeSelected(r,false);
  QTableWidgetSelectionRange r(markrow,markcol,row,col);
  w->setRangeSelected(r,true);
  w->scrollToItem(w->item(markrow,markcol),QAbstractItemView::EnsureVisible);
  w->scrollToItem(w->item(row,col),QAbstractItemView::EnsureVisible);
}

// ---------------------------------------------------------------------
void Table::setshape(QStringList opt)
{
  if (opt.size()<2) {
    error("table shape must have rows and columns: " + q2s(opt.join(" ")));
    return;
  }
  int rws0=rws, cls0=cls;
  rws=c_strtoi(q2s(opt.at(0)));
  cls=c_strtoi(q2s(opt.at(1)));
  len=rws*cls;
  if (rws==rws0 && cls==cls0) return;

  QTableWidget *w=(QTableWidget*) widget;
  w->setRowCount(rws);
  w->setColumnCount(cls);

  cellalign=vecreshape(cellalign,rws,cls,rws0,cls0);
  defcellalign=vecreshape(defcellalign,rws,cls,rws0,cls0);
  cellprotect=vecreshape(cellprotect,rws,cls,rws0,cls0);
  defcellprotect=vecreshape(defcellprotect,rws,cls,rws0,cls0);
  celltype=vecreshape(celltype,rws,cls,rws0,cls0);
  defcelltype=vecreshape(defcelltype,rws,cls,rws0,cls0);

//  resetlen(&cellalign,defcellalign);
//  resetlen(&cellprotect,defcellprotect);
//  resetlen(&celltype,defcelltype);
//  cellwidget.resize(len);
  QVector<QWidget *> cw;
  cw.fill(0,rws*cls);
  for(int r=0; r<qMin(rws,rws0); r++)
    for(int c=0; c<qMin(cls,cls0); c++)
      cw[c + r*cls]=cellwidget[c + r*cls0];
  cellwidget=cw;
}

// ---------------------------------------------------------------------
void Table::setsort(std::string v)
{
  int c;
  QTableWidget *w=(QTableWidget*) widget;
  QStringList opt;

  opt=qsplit(v);
  if (!(opt.size()>0)) {
    error("set sort must specify column: " + q2s(opt.join(" ")));
    return;
  }
  c=c_strtoi(q2s(opt.at(0)));
  if (opt.contains("descending"))
    w->sortItems(c,Qt::DescendingOrder);
  else
    w->sortItems(c,Qt::AscendingOrder);
}

// ---------------------------------------------------------------------
void Table::settype(std::string v)
{
  QVector<int> a=qs2intvector(s2q(v));
  int n=a.size();

  int len1,r1,r2,c1,c2;
  r1=row1;
  r2=row2;
  c1=col1;
  c2=col2;
  row=col=-1;
  if (!((r1>=0 && r1<rws && c1>=0 && c1<cls && r2>=-1 && r2<rws && c2>=-1 && c2<cls && (-1==r2 || r1<=r2) && (-1==c2 || c1<=c2)) || (0==rws && ((((c2==-1)?(cls-1):c2)-c1+1)==n || 1==n || 0==n)))) {
    error("set type row1 row2 col1 col2 out of bound: " + q2s(QString::number(r1)) + " " + q2s(QString::number(r2)) + " " + q2s(QString::number(c1)) + " " + q2s(QString::number(c2)));
    return;
  }
  if (r2==-1) r2=rws-1;
  if (c2==-1) c2=cls-1;
  if (0==rws || 0==n) return;
  bool colmode= (c2-c1+1)==n;

  if (!(n==1 || n== (len1=(r2-r1+1)*(c2-c1+1)) || colmode)) {
    QString m="incorrect type length - ";
    m+= "given " + QString::number(n);
    m+=" cells, require " + QString::number(len1) + " cells";
    error(q2s(m));
    return;
  }
  if(!vecin(a,CellTypes,"type")) return;
  if (0==defcelltype.size()) {
    defcelltype=QVector<int>(len,0);
    celltype=QVector<int>(len,0);
  }
  int q=0;
  for (int r=r1; r<=r2; r++) {
    for (int c=c1; c<=c2; c++) {
      int p=c + r*cls;
      if (colmode && c==c1) q=0;
      defcelltype[p]=a.at(q);
      celltype[p]=a.at(q);
      if (n!=1) q++;
    }
  }
}

// ---------------------------------------------------------------------
std::string Table::state()
{
  std::string r;

  if (this!=pform->evtchild || event=="mark" || event.substr(0,2)=="mb") {
    r+=spair(id,i2s(row)+" "+i2s(col));
    QTableWidgex *w=(QTableWidgex*) widget;
    QModelIndexList ml=w->selectionModel()->selectedIndexes();
    std::string mark="";
    if (!ml.isEmpty()) {
      int r0,r1,c0,c1;
      r0=rws;
      c0=cls;
      r1=c1=-1;
      foreach (QModelIndex m,ml) {
        r0=qMin(r0,m.row());
        r1=qMax(r1,m.row());
        c0=qMin(c0,m.column());
        c1=qMax(c1,m.column());
      }
      mark=i2s(r0)+" "+i2s(r1)+" "+i2s(c0)+" "+i2s(c1);
    }
    r+=spair(id+"_select",mark);
  } else  if (event=="change") {
    r+=spair(id,readcell(row,col));
    r+=spair(id+"_cell",i2s(row)+" "+i2s(col));
    r+=spair(id+"_value",readcellvalue(row,col));
  } else if (event=="clicked") {
    r+=spair(id+"_cell",i2s(row)+" "+i2s(col));
  }
  return r;
}

// ---------------------------------------------------------------------
bool Table::vecin(QVector<int>vec,QVector<int>values,QString id)
{
  for(int i=0; i<vec.size(); i++)
    if (!values.contains(vec[i])) {
      error(q2s(id) + " invalid value: " + q2s(QString::number(vec[i])));
      return false;
    }
  return true;
}

// ---------------------------------------------------------------------
bool Table::vecisbool(QVector<int>vec,QString id)
{
  for(int i=0; i<vec.size(); i++)
    if (!(vec[i]==0 || vec[i]==1)) {
      error(q2s(id) + " invalid value: " + q2s(QString::number(vec[i])));
      return false;
    }
  return true;
}

// ---------------------------------------------------------------------
QVector<int> Table::vecreshape(QVector<int> vec,int rws, int cls, int rws0, int cls0)
{
  QVector<int> vec1;
  vec1.fill(0,rws*cls);
  for(int r=0; r<qMin(rws,rws0); r++)
    for(int c=0; c<qMin(cls,cls0); c++)
      vec1[c + r*cls]=vec[c + r*cls0];
  return vec1;
}

// ---------------------------------------------------------------------
void Table::on_cellChanged (int r,int c)
{
  if (NoEvents) return;
  QTableWidget *w=(QTableWidget*) widget;
  w->resizeColumnsToContents();
  event="change";
  row=r;
  col=c;
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
// see on_cellDoubleClicked for event ignore code
void Table::on_cellClicked (int r, int c)
{
  if (NoEvents) return;
  if (QDateTime::currentDateTime() < dblclick) return;
  row=r;
  col=c;
  event="mb" + lmr + "down";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
// for button
void Table::on_cellClicked_button ()
{
  if (NoEvents) return;
  event="clicked";
  int p=sender()->objectName().toInt();
  row=p/cls;
  col=p-row*cls;
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
// a cellClicked event is given immediately after this, and
// ignored if within dblclick time
void Table::on_cellDoubleClicked (int r, int c)
{
  if (NoEvents) return;
  row=r;
  col=c;
  event="mb" + lmr + "dbl";
  dblclick=QDateTime::currentDateTime().addMSecs(250);
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Table::on_currentCellChanged (int r,int c, int pr, int pc)
{
  if (NoEvents) return;
  event="mark";
  row=r;
  col=c;
  lastrow=pr;
  lastcol=pc;
  if (!ifshift()) {
    markrow=r;
    markcol=c;
  }
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void Table::on_headerClicked (int c)
{
  QTableWidget *w=(QTableWidget*) widget;
  w->sortItems(c,Qt::AscendingOrder);
}

// ---------------------------------------------------------------------
// for checkbox and combolist
void Table::on_stateChanged (int n)
{
  Q_UNUSED(n);
  if (NoEvents) return;
  event="change";
  int p=sender()->objectName().toInt();
  row=p/cls;
  col=p-row*cls;
  pform->signalevent(this);
}
