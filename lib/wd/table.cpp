
#include <QCheckBox>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QTableWidget>

#include "wd.h"
#include "table.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Table::Table(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="listbox";
  QTableWidget *w=new QTableWidget;
  widget=(QWidget*) w;
  QString qn=s2q(n);
  w->setObjectName(qn);

// nonce for pacman, should be set properties:
  w->resizeColumnsToContents();
  w->setAlternatingRowColors(true);

  w->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
  w->horizontalHeader()->setHighlightSections(false);
  w->horizontalHeader()->setStretchLastSection(true);
  w->verticalHeader()->setVisible(false);

  w->setEditTriggers(QAbstractItemView::NoEditTriggers);
  w->setSelectionBehavior(QAbstractItemView::SelectRows);
  w->setSelectionMode(QAbstractItemView::SingleSelection);

  QFontMetrics fm(w->font());
  w->verticalHeader()->setDefaultSectionSize(fm.height() + 6);
}

// ---------------------------------------------------------------------
void Table::set(string p)
{
  int c,r,d,cls,rws;
  QTableWidget *w=(QTableWidget*) widget;
  QCheckBox *cb;
  QTableWidgetItem *item;
  QStringList dat=qsplit(p);

  if (dat.size()==0) return;

  cls=w->columnCount();
  rws=dat.size()/cls;

  if (dat.size() != cls*rws) {
    QString m="incorrect data length - ";
    m+= "given " + QString::number(dat.size());
    m+=" require " + QString::number(cls*rws);
    info("table",m);
    return;
  }

  w->setRowCount(rws);

  for (r=0; r<rws; r++) {
    d=r*cls;
    for (c=0; c<cls; c++) {
      switch (coltypes[c]) {
      case 0 :
        item=new QTableWidgetItem(dat[d+c]);
        w->setItem(r,c,item);
        break;
      case 1 :
        cb=new QCheckBox();
        cb->setChecked(dat[d+c]=="1");
        w->setCellWidget(r,c,cb);
        break;
      }
    }
  }

  w->selectRow(0);
  w->resizeColumnsToContents();
}

// ---------------------------------------------------------------------
void Table::setcolnames(string v)
{
  QTableWidget *w=(QTableWidget*) widget;
  QStringList s=qsplit(v);
  if (w->columnCount()==0)
    w->setColumnCount(s.size());
  else if (s.size()!=w->columnCount()) {
    QString m=QString::number(s.size());
    m+= " column names do not match column count of ";
    m+= QString::number(w->columnCount());
    info("table",m);
    return;
  }
  w->setHorizontalHeaderLabels(s);
}

// ---------------------------------------------------------------------
void Table::setp(string p, string v)
{
  QTableWidget *w=(QTableWidget*) widget;
  if (p=="colnames")
    setcolnames(v);
  else if (p=="coltypes") {
    coltypes=qs2intlist(s2q(v));
    w->setColumnCount(coltypes.size());
  } else Child::setp(p,v);
}

// ---------------------------------------------------------------------
string Table::state()
{
  //QTableWidget *w=(QTableWidget*) widget;
  string r;

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
