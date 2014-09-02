
#include <QAction>
#include <QTimer>
#include <QToolButton>

#include "qgrid.h"

void dragstart(QWidget *s,QDrag *dr);

// ---------------------------------------------------------------------
CubeButton::CubeButton(QString text, QWidget *p) : QPushButton(text,p)
{
  setObjectName(text);
  connect(this, SIGNAL(pressed()), this, SLOT(makeDrag()));
}

// ---------------------------------------------------------------------
void CubeButton::makeDrag()
{
  dragstart((QWidget *) QObject::sender(),new QDrag(this));
}

// ---------------------------------------------------------------------
CubeCombo::CubeCombo(QString text, QWidget *parent) : QWidget(parent)
{
  p=(QGrid *)parent;
  QHBoxLayout *hb=new QHBoxLayout();
  hb->setContentsMargins(0,0,0,0);
  hb->setSpacing(0);
  QAction *pbact=new QAction(text+":",this);
  QToolButton *pb = new QToolButton();
  pb->setObjectName(text);
  pb->setDefaultAction(pbact);
  pb->setAutoRaise(true);
  pb->setToolButtonStyle(Qt::ToolButtonTextOnly);
  pb->setToolTip("");
  connect(pb, SIGNAL(pressed()), this, SLOT(makeDrag()));
  hb->addWidget(pb);
  combobox=new QComboBox();
  combobox->setObjectName(text);
  connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
  hb->addWidget(combobox);
  setLayout(hb);
}

// ---------------------------------------------------------------------
void CubeCombo::indexChanged(int n)
{
  if (NoEvents) return;
  QString id=QObject::sender()->objectName();
  AxisIndex.replace(AxisNames.indexOf(id),n);
  p->redraw();
}

// ---------------------------------------------------------------------
void CubeCombo::makeDrag()
{
  QWidget *s=(QWidget *) QObject::sender();
  dragstart(s,new QDrag(this));
  ((QToolButton *)s)->setDown(false);
}

// ---------------------------------------------------------------------
CubeMargin::CubeMargin(QBoxLayout::Direction d, QWidget *parent) : QWidget(parent)
{
  p=(QGrid *)parent;
  layout=new QBoxLayout(d);
  layout->setContentsMargins(0,0,0,0);
  setLayout(layout);
  setAcceptDrops(true);
}

// ---------------------------------------------------------------------
void CubeMargin::dropEvent(QDropEvent *e)
{
  int c,n,r,s;

  n=AxisNames.indexOf(e->mimeData()->text());

  c=itemremove(AxisCols,n);
  r=itemremove(AxisRows,n);
  s=itemremove(AxisSlice,n);
  Q_UNUSED(s);

  if (this==p->mcols) {
    if (r==0)
      itemmovelast(AxisCols,AxisRows);
    itemadd(AxisCols,n);
  } else if (this==p->mrows) {
    if (c==0)
      itemmovelast(AxisRows,AxisCols);
    itemadd(AxisRows,n);
  } else {
    if (c==0)
      itemmovelast(AxisSlice,AxisCols);
    else if (r==0)
      itemmovelast(AxisSlice,AxisRows);
    itemadd(AxisSlice,n);
  }

  QTimer *timer = new QTimer(this);
  timer->setSingleShot(true);
  connect(timer, SIGNAL(timeout()), p, SLOT(redraw()));
  timer->start();
}

// ---------------------------------------------------------------------
void CubeMargin::dragMoveEvent(QDragMoveEvent *e)
{
  e->accept();
}

// ---------------------------------------------------------------------
void CubeMargin::dragEnterEvent(QDragEnterEvent *e)
{
  e->acceptProposedAction();
}

// ---------------------------------------------------------------------
void CubeMargin::itemadd(QList<int> v, int n)
{
  if (v==AxisCols)
    AxisCols.append(n);
  else if (v==AxisRows)
    AxisRows.append(n);
  else
    AxisSlice.append(n);
}

// ---------------------------------------------------------------------
void CubeMargin::itemmovelast(QList<int> v, QList<int> w)
{
  int n;
  if (v==AxisCols) {
    n=AxisCols.last();
    AxisCols.removeLast();
  } else if (v==AxisRows) {
    n=AxisRows.last();
    AxisRows.removeLast();
  } else {
    n=AxisSlice.last();
    AxisSlice.removeLast();
  }
  if (w==AxisCols)
    AxisCols.append(n);
  else if (w==AxisRows)
    AxisRows.append(n);
  else {
    AxisSlice.append(n);
  }
}

// ---------------------------------------------------------------------
int CubeMargin::itemremove(QList<int> v, int n)
{
  int r;
  if (v==AxisCols) {
    AxisCols.removeOne(n);
    r=AxisCols.size();
  } else if (v==AxisRows) {
    AxisRows.removeOne(n);
    r=AxisRows.size();
  } else {
    AxisSlice.removeOne(n);
    r=AxisSlice.size();
  }
  return r;
}

// ---------------------------------------------------------------------
void dragstart(QWidget *s,QDrag *dr)
{
  QPixmap pixmap=QPixmap::grabWidget(s);
  dr->setPixmap(pixmap);
  dr->setHotSpot(QPoint(s->width()/2,s->height()/2));
  QMimeData *data = new QMimeData;
  data->setText(s->objectName());
  dr->setMimeData(data);
  dr->start();
}
