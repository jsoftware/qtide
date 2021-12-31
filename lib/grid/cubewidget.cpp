
#include <QAction>
#include <QDrag>
#include <QMimeData>
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
  p->AxisIndex.replace(p->AxisNames.indexOf(id),n);
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

  e->acceptProposedAction();
  n=p->AxisNames.indexOf(e->mimeData()->text());

  if ((this==p->mcols && n==p->AxisCols.last()) ||
      (this==p->mrows && n==p->AxisRows.last()) ||
      (this==p->mslice && n==p->AxisSlice.last())) {
    ((QToolButton *)e->source())->setDown(false);
    return;
  }

  c=itemremove(p->AxisCols,n);
  r=itemremove(p->AxisRows,n);
  s=itemremove(p->AxisSlice,n);
  Q_UNUSED(s);

  if (this==p->mcols) {
    if (r==0)
      itemmovelast(p->AxisCols,p->AxisRows);
    itemadd(p->AxisCols,n);
  } else if (this==p->mrows) {
    if (c==0)
      itemmovelast(p->AxisRows,p->AxisCols);
    itemadd(p->AxisRows,n);
  } else {
    if (c==0)
      itemmovelast(p->AxisSlice,p->AxisCols);
    else if (r==0)
      itemmovelast(p->AxisSlice,p->AxisRows);
    itemadd(p->AxisSlice,n);
  }

// timeout crashes OSX (10.11.1, qt5.5), but interval works
#ifdef __MACH__
  QTimer::singleShot(150,p,SLOT(redraw()));
#else
  QTimer *timer = new QTimer(this);
  timer->setSingleShot(true);
  connect(timer, SIGNAL(timeout()), p, SLOT(redraw()));
  timer->start();

#endif
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
  if (v==p->AxisCols)
    p->AxisCols.append(n);
  else if (v==p->AxisRows)
    p->AxisRows.append(n);
  else
    p->AxisSlice.append(n);
}

// ---------------------------------------------------------------------
void CubeMargin::itemmovelast(QList<int> v, QList<int> w)
{
  int n;

  int to=(w==p->AxisCols) ? 0 : (w==p->AxisRows) ? 1 : 2;

  if (v.isEmpty()) {
    if (w.isEmpty())
      itemmovelast(p->AxisRows.isEmpty() ? p->AxisCols : p->AxisRows,w);
    return;
  }

  if (v==p->AxisCols) {
    n=p->AxisCols.last();
    p->AxisCols.removeLast();
  } else if (v==p->AxisRows) {
    n=p->AxisRows.last();
    p->AxisRows.removeLast();
  } else {
    n=p->AxisSlice.last();
    p->AxisSlice.removeLast();
  }
  if (to==0)
    p->AxisCols.append(n);
  else if (to==1)
    p->AxisRows.append(n);
  else
    p->AxisSlice.append(n);
}

// ---------------------------------------------------------------------
int CubeMargin::itemremove(QList<int> v, int n)
{
  int r;
  if (v==p->AxisCols) {
    p->AxisCols.removeOne(n);
    r=p->AxisCols.size();
  } else if (v==p->AxisRows) {
    p->AxisRows.removeOne(n);
    r=p->AxisRows.size();
  } else {
    p->AxisSlice.removeOne(n);
    r=p->AxisSlice.size();
  }
  return r;
}

// ---------------------------------------------------------------------
void dragstart(QWidget *s,QDrag *dr)
{
#if defined(QT50)
  QPixmap pixmap=s->grab();
#else
  QPixmap pixmap=QPixmap::grabWidget(s);
#endif
  dr->setPixmap(pixmap);
  dr->setHotSpot(QPoint(s->width()/2,s->height()/2));
  QMimeData *data = new QMimeData;
  data->setText(s->objectName());
  dr->setMimeData(data);
  dr->exec();
}
