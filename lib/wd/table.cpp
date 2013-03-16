
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

  if (opt.size()==2) {
    setshape(opt);
    opt=opt.mid(2);
  }

  w->resizeColumnsToContents();
  w->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
  w->horizontalHeader()->setHighlightSections(false);
  w->horizontalHeader()->setStretchLastSection(true);
  w->horizontalHeader()->setVisible(false);

  w->verticalHeader()->setHighlightSections(false);
  w->verticalHeader()->setVisible(false);
  QFontMetrics fm(w->font());
  w->verticalHeader()->setDefaultSectionSize(fm.height() + 6);

  w->setSelectionMode(QAbstractItemView::SingleSelection);

  w->setAlternatingRowColors(true);

  if (opt.contains("selectrows")) {
    w->setSelectionBehavior(QAbstractItemView::SelectRows);
    w->selectRow(0);
  }

  connect(w,SIGNAL(cellChanged(int,int)),
          this,SLOT(on_cellChanged(int,int)));
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
void Table::initglobals()
{
  if (CellAligns.size()) return;
  CellAligns << 0 << 1 << 2;
  CellTypes << 0 << 100;
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
  } else
    v->fill(0);
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
  else if (p=="shape")
    setshape(qsplit(v));
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
        item->setTextAlignment(getalign(cellalign[p]));
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
  w->resizeColumnsToContents();
  w->horizontalHeader()->setStretchLastSection(true);
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
void Table::setshape(QStringList opt)
{
  if (opt.size()<2) {
    error("table shape must have rows and columns: " + q2s(opt.join(" ")));
    return;
  }
  rws=opt.at(0).toInt();
  cls=opt.at(1).toInt();
  len=rws*cls;

  QTableWidget *w=(QTableWidget*) widget;
  w->setRowCount(rws);
  w->setColumnCount(cls);

  resetlen(&cellalign,defcellalign);
  resetlen(&celledit,defcelledit);
  resetlen(&celltype,defcelltype);
  cellwidget.resize(len);
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
  defcelltype=a;
  celltype=getcellvec(a);
}

// ---------------------------------------------------------------------
string Table::state()
{
  if (this!=pform->evtchild) return "";
  string r;

  if (event=="change") {
    r+=spair(id,readcell(row,col));
    r+=spair(id+"_cell",i2s(row)+" "+i2s(col));
  } else if (event=="mark") {
    r+=spair(id,i2s(row)+" "+i2s(col));
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
void	Table::on_cellChanged (int r,int c)
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
void	Table::on_currentCellChanged (int r,int c, int pr, int pc)
{
  if (NoEvents) return;
  event="mark";
  row=r;
  col=c;
  lastrow=pr;
  lastcol=pc;
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
// for checkbox
void	Table::on_stateChanged (int n)
{
  Q_UNUSED(n);
  if (NoEvents) return;
  event="change";
  int p=sender()->objectName().toInt();
  row=p/cls;
  col=p-row*cls;
  pform->signalevent(this);
}
