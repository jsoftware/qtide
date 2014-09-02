
#include "qgrid.h"
#include "wgrid.h"
#include "../wd/wd.h"
#include "../wd/cmd.h"

QStringList AxisNames;
QList<QStringList> AxisLabels;
QList<int> AxisRows;
QList<int> AxisCols;
QList<int> AxisSlice;
QVector<int> AxisIndex;
QList<int> AxisShape;
QStringList CellData;

// ---------------------------------------------------------------------
QGrid::QGrid(QString s,Child *c,QWidget *p) : QWidget(p)
{
  style=s;
  sa=new Wasa(s,c,this);
  vbmain=new QVBoxLayout();
  if (style.isEmpty()) {
    GGridInit=true;
    vbmain->setContentsMargins(0,0,0,0);
    vbmain->addWidget(sa);
  } else if (style=="cube") {
    GGridInit=false;
    mslice=mcols=mrows=0;
    hbmain=new QHBoxLayout();
    hbmain->addWidget(sa,1);
  }
  setLayout(vbmain);
}

// ---------------------------------------------------------------------
QGrid::~QGrid()
{
}

// ---------------------------------------------------------------------
void QGrid::redraw()
{
  GGridInit=false;
  sa->g->WGridInit=false;
  drawcube();
  sa->g->draw();
}

// ---------------------------------------------------------------------
void QGrid::set(string p,string v)
{
  QStringList s=qsplit(v);
  if (style!="cube") {
    sa->g->set(p,v);
    return;
  }
  GGridInit=false;
  sa->g->WGridInit=false;
  if (p=="data")
    CellData=s;
  else if (p=="labels")
    setaxislabels(s);
  else if (p=="names")
    AxisNames=s;
  else if (p=="order")
    setaxisorder(s);
  else if (p=="shape") {
    AxisShape=qsl2intlist(s);
    AxisIndex.fill(0,AxisShape.size());
  } else
    sa->g->set(p,v);
}

// ---------------------------------------------------------------------
void QGrid::setaxislabels(QStringList s)
{
  int i,len,p=0;
  AxisLabels.clear();
  for (i=0; i<AxisShape.size(); i++) {
    len=AxisShape[i];
    AxisLabels.append(s.mid(p,len));
    p+=len;
  }
}

// ---------------------------------------------------------------------
void QGrid::setaxisorder(QStringList s)
{
  AxisRows=qs2intlist(s[0]);
  AxisCols=qs2intlist(s[1]);
  AxisSlice=qs2intlist(s[2]);
  QList<int> ndx=qs2intlist(s[3]);
  for (int i=0; i<AxisSlice.size(); i++)
    AxisIndex.replace(AxisSlice[i],ndx[i]);
}

// ---------------------------------------------------------------------
string QGrid::state(string event)
{
  return sa->g->state(event);
}

// ---------------------------------------------------------------------
Wasa::Wasa(QString s,Child *c, QWidget *parent) : QAbstractScrollArea(parent)
{
  style=s;
  p=(QGrid*) parent;
  v=new QWidget(p);
  g=new WGrid(c,v,this);
  g->resize(400,300);
  setViewport(v);

  barh=horizontalScrollBar();
  barv=verticalScrollBar();

  connect(barh, SIGNAL(valueChanged(int)),
          g, SLOT(barhchanged(int)));
  connect(barv, SIGNAL(valueChanged(int)),
          g, SLOT(barvchanged(int)));
}

// ---------------------------------------------------------------------
void Wasa::resizeEvent(QResizeEvent *event)
{
  QAbstractScrollArea::resizeEvent(event);
}

// ---------------------------------------------------------------------
void Wasa::setmargins(int l,int t,int r,int b)
{
  setViewportMargins(l,t,r,b);
}

// ---------------------------------------------------------------------
void Wasa::setscroll()
{
  int w=width();
  barh->setRange(0,qMax(0,g->width()-w));
  g->maxscrollc(w);

  int h=height();
  barv->setRange(0,qMax(0,g->height()-h));
  g->maxscrollr(h);
}

