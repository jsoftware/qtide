
#include <QBoxLayout>
#include <QCheckBox>
#include <QHeaderView>
#include <QTableWidget>

#include "wd.h"
#include "table.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

QVector<int>CellAligns;
QVector<int>CellTypes;

// ---------------------------------------------------------------------
Table::Table(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="table";
  initglobals();
  ifhdr=false;

  QTableWidget *w=new QTableWidget(p);
  widget=(QWidget*) w;
  w->setObjectName(s2q(n));
  QStringList opt=qsplit(s);

  if (opt.size()<2) {
    error("row and column count must be given");
    return;
  }
  initsizes(opt);
  opt=opt.mid(2);

  w->resizeColumnsToContents();
  w->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
  w->horizontalHeader()->setHighlightSections(false);
  w->horizontalHeader()->setStretchLastSection(true);
  w->horizontalHeader()->setVisible(false);

  w->verticalHeader()->setHighlightSections(false);
  w->verticalHeader()->setVisible(false);
  QFontMetrics fm(w->font());
  w->verticalHeader()->setDefaultSectionSize(fm.height() + 6);

  // turn off cell editing:
  //w->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // single cell vs cell blocks
  w->setSelectionMode(QAbstractItemView::SingleSelection);

  w->setAlternatingRowColors(true);

  if (opt.contains("selectrow"))
    w->setSelectionBehavior(QAbstractItemView::SelectRows);

  connect(w,SIGNAL(cellChanged(int,int)),
          this,SLOT(on_cellChanged(int,int)));
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
Qt::Alignment Table::getalign(int i)
{
  switch (cellalign[i]) {
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
void Table::initglobals()
{
  if (CellAligns.size()) return;
  CellAligns << 0 << 1 << 2;
  CellTypes << 0 << 100;
}

// ---------------------------------------------------------------------
void Table::initsizes(QStringList opt)
{
  rws=opt.at(0).toInt();
  cls=opt.at(1).toInt();
  len=rws*cls;
  cellalign.resize(len);
  celltype.resize(len);
  cellwidget.resize(len);
  QTableWidget *w=(QTableWidget*) widget;
  w->setRowCount(rws);
  w->setColumnCount(cls);
}

// ---------------------------------------------------------------------
string Table::readcell(int row,int col)
{
  QTableWidget *w=(QTableWidget*) widget;
  QTableWidgetItem *m=w->item(row,col);
  int p=col+row*cls;
  switch (celltype[p]) {
  case 0 :
    return q2s(m->text());
  case 100 :
    return ((QCheckBox *)cellwidget[p])->isChecked()?"1":"0";
  default :
    return "";
  }
}

// ---------------------------------------------------------------------
void Table::set(string p, string v)
{
  if (p=="align")
    setalign(v);
  else if (p=="data")
    setdata(v);
  else if (p=="edit")
    setedit(v);
  else if (p=="hdr")
    sethdr(v);
  else if (p=="hdralign")
    sethdralign(v);
  else if (p=="lab")
    setlab(v);
  else if (p=="type")
    settype(v);
  else Child::set(p,v);
}

// ---------------------------------------------------------------------
void Table::setalign(string v)
{
  QVector<int> a=qs2intvector(s2q(v));
  int n=a.size();
  if (!(n==1 || n==cls || n==len)) {
    error("incorrect align length: " + q2s(QString::number(n)));
    return;
  }
  if(!vecin(a,CellAligns,"align")) return;
  defcellalign=a;
  cellalign=getcellvec(a);
}

// ---------------------------------------------------------------------
void Table::setdata(string s)
{
  int c,d,p,r;
  QCheckBox *cb;
  QTableWidget *w=(QTableWidget*) widget;
  QTableWidgetItem *item;
  QHBoxLayout *y;
  QWidget *m;

  item=new QTableWidgetItem("");
  Qt::ItemFlags fdef=item->flags();
  Qt::ItemFlags fnoedit=fdef &
      ~(Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);

  dat=qsplit(s);

  if (dat.size()!=len) {
    QString m="incorrect data length - ";
    m+= "given " + QString::number(dat.size());
    m+=" cells, require " + QString::number(len) + " cells";
    error(q2s(m));
    return;
  }

  for (r=0; r<rws; r++) {
    d=r*cls;
    for (c=0; c<cls; c++) {
      p=d+c;
      switch (celltype[p]) {
      case 0 :
        item=new QTableWidgetItem(dat[d+c]);
        item->setTextAlignment(getalign(p));
        if (!celledit[p])
          item->setFlags(fnoedit);
        w->setItem(r,c,item);
        break;
      case 100 :
        cb=new QCheckBox();
        cb->setChecked(dat[p]=="1");
        cb->setObjectName(QString::number(p));
        cellwidget[p]=(QWidget*) cb;
        m=new QWidget();
        y=new QHBoxLayout();
        y->setContentsMargins(0,0,0,0);
        y->setSpacing(0);
        y->addStretch(1);
        y->addWidget(cb);
        y->addStretch(1);
        m->setLayout(y);
        w->setCellWidget(r,c,m);
        connect(cb,SIGNAL(stateChanged(int)),
                this,SLOT(on_stateChanged(int)));
        break;
      }
    }
  }
  w->selectRow(0);
  w->resizeColumnsToContents();
}

// ---------------------------------------------------------------------
void Table::setedit(string v)
{
  QVector<int> a=qs2intvector(s2q(v));
  int n=a.size();
  if (!(n==1 || n==cls || n==len)) {
    error("incorrect edit length: " + q2s(QString::number(n)));
    return;
  }
  if(!vecisbool(a,"edit")) return;
  defcelledit=a;
  celledit=getcellvec(a);
}

// ---------------------------------------------------------------------
void Table::sethdr(string v)
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
void Table::sethdralign(string v)
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
void Table::setlab(string v)
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
void Table::settype(string v)
{
  QVector<int> a=qs2intvector(s2q(v));
  int n=a.size();
  if (!(n==1 || n==cls || n==len)) {
    error("incorrect type length: " + q2s(QString::number(n)));
    return;
  }
  if(!vecin(a,CellTypes,"type")) return;
  defcellalign=a;
  celltype=getcellvec(a);
}

// ---------------------------------------------------------------------
string Table::state()
{
  //if (this==pform->evtchild && event=="changed")
  if (this!=pform->evtchild) return "";


  //QTableWidget *w=(QTableWidget*) widget;
  string r;

  if (event=="changed") {
    r+=spair(id,readcell(row,col));
    r+=spair(id+"_cell",i2s(row)+" "+i2s(col));
    return r;
  }

  //int n=w->currentRow();
  //if (n<0) {
  //r+=spair(id,"");
  //r+=spair(id+"_select","");
  //} else {
  //r+=spair(id,q2s(w->item(n)->text()));
  //r+=spair(id+"_select",i2s(n));
  //}

  return r;
}

// ---------------------------------------------------------------------
//void Table::tbinfo(QString s)
//{
//  info("Table",s);
//}
//
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
void	Table::on_cellChanged (int r,int c)
{
  if (NoEvents) return;
  event="changed";
  row=r;
  col=c;
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
// for checkbox
void	Table::on_stateChanged (int n)
{
  Q_UNUSED(n);
  if (NoEvents) return;
  event="changed";
  int p=sender()->objectName().toInt();
  row=p/cls;
  col=p-row*cls;
  pform->signalevent(this);
}
