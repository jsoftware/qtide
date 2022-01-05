
#include "wgrid.h"
#include "qgrid.h"
#include "../base/state.h"
#include "../wd/cmd.h"
#include "../wd/form.h"
#include "../wd/wd.h"

// ---------------------------------------------------------------------
WGrid::WGrid(Child *c,QGrid *qgrid,QWidget *parent,Wasa *s) : QWidget(parent)
{
  pchild=c;
  p=qgrid;
  sa=s;
  setAttribute(Qt::WA_DeleteOnClose);
  setContentsMargins(0,0,0,0);
  setFocusPolicy(Qt::StrongFocus);
  setMouseTracking(true);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  pmall=pmtop=pmhdr=pmlab=pmcell=0;
  QFontMetrics fm=QFontMetrics(font,0);
  defs();
}

// ---------------------------------------------------------------------
void WGrid::barhchanged(int v)
{
  Scls=v;
}

// ---------------------------------------------------------------------
void WGrid::barvchanged(int v)
{
  Srws=v;
}

// ---------------------------------------------------------------------
void WGrid::getcellpos(QPoint p)
{
  int x=p.x();
  int y=p.y();
  col=getindexof(x,Gwp)-Lcls;
  row=getindexof(y,Ghp)-Hrws;
  if (col>=0)
    col=Scls+col;
  if (row>=0)
    row=Srws+row;
}

// ---------------------------------------------------------------------
void WGrid::maxscrollc(int w)
{
  int wid=w-Lw-(0<sa->barv->maximum() ? sbx : 0);
  Sclsmax=maxscroll(wid,Cws);
  Scls=qMin(Scls,Sclsmax);
  sa->barh->setRange(0,Sclsmax);
  sa->barh->setValue(Scls);
}

// ---------------------------------------------------------------------
void WGrid::maxscrollr(int h)
{
  int hit=h-Hh-(0<sa->barh->maximum() ? sbx : 0);
  Srwsmax=maxscroll(hit,Chs);
  Srws=qMin(Srws,Srwsmax);
  sa->barv->setRange(0,Srwsmax);
  sa->barv->setValue(Srws);
}

// ---------------------------------------------------------------------
void WGrid::mouseDoubleClickEvent(QMouseEvent *e)
{
  Qt::MouseButton b=e->button();
  if (b==Qt::LeftButton)
    pchild->event="mbldbl";
  else if (b==Qt::RightButton)
    pchild->event="mbrdbl";
  else {
    WGrid::mousePressEvent(e);
    return;
  }
  getcellpos(e->pos());
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void WGrid::mousePressEvent(QMouseEvent *e)
{
  Qt::MouseButton b=e->button();
  if (b==Qt::LeftButton)
    pchild->event="mbldown";
  else if (b==Qt::RightButton)
    pchild->event="mbrdown";
  else {
    WGrid::mousePressEvent(e);
    return;
  }
  getcellpos(e->pos());
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void WGrid::paintEvent(QPaintEvent *e)
{
  if (NoEvents) return;
  noevents(1);
  Q_UNUSED(e);
  draw();
  if (pmall) {
    QPainter p(this);
    p.drawPixmap(0,0,*pmall);
    p.drawPixmap(Lw,Hh,*pmcell);
    if (Hh>0)
      p.drawPixmap(Lw,0,*pmhdr);
    if (Lw>0)
      p.drawPixmap(0,Hh,*pmlab);
    if (Hh>0 && Lw>0)
      p.drawPixmap(0,0,*pmtop);
  }
  noevents(0);
}

// ---------------------------------------------------------------------
void WGrid::set(std::string p,std::string v)
{
  QStringList s=qsplit(v);
  WGridInit=false;

  if (p=="align")
    setdataalign(s);
  else if (p=="data")
    Data=s;
  else if (p=="hdr")
    setheader(s);
  else if (p=="hdralign")
    setheaderalign(s);
  else if (p=="hdrmerge")
    setheadermerge(s);
  else if (p=="lab")
    setlabel(s);
  else if (p=="labalign")
    setlabelalign(s);
  else if (p=="labmerge")
    setlabelmerge(s);
  else if (p=="shape") {
    setshape(s);
  }
}

// ---------------------------------------------------------------------
void WGrid::setdataalign(QStringList s)
{
  QVector<int> a=qlist2ints(s);
  if (a.size()==1 || a.size()==Crws*Ccls)
    DataAlign=a;
  else if (a.size()==Ccls)
    DataAlign=vshape((Crws*Ccls),a);
  else
    error("data align length of " + i2s(a.size()) +
          " does not match shape " + i2s(Crws) + " " + i2s(Ccls));
}

// ---------------------------------------------------------------------
// should be set before align and merge
// if re-set, then check if matches previous header length
void WGrid::setheader(QStringList s)
{
  int h=Hdr.size();
  Hdr=s;
  if (h==0 || h==s.size())
    HdrX=expandstringlist(Hdr,HdrMerge);
  else {
    HdrAlign=DefAlign;
    HdrAlignX=DefAlign;
    HdrMerge=DefMerge;
    HdrX=Hdr;
  }
}

// ---------------------------------------------------------------------
void WGrid::setheaderalign(QStringList s)
{
  QVector<int> a=qlist2ints(s);
  if (a.size()==1 || a.size()==Hdr.size())
    HdrAlign=a;
  else
    error("header align length of " + i2s(a.size()) +
          " does not match header length of " + i2s(Hdr.size()));
}

// ---------------------------------------------------------------------
void WGrid::setheadermerge(QStringList s)
{
  QVector<int> m=qlist2ints(s);
  if (!(m.size() == 1 || m.size() == Hdr.size())) {
    error("header merge length of " + i2s(m.size()) +
          " does not match header length of " + i2s(Hdr.size()));
    return;
  }
  HdrMerge=m;
  HdrX=expandstringlist(Hdr,HdrMerge);
  HdrAlignX=expandvectorint(HdrAlign,HdrMerge);
}

// ---------------------------------------------------------------------
// same comments as for header
void WGrid::setlabel(QStringList s)
{
  int w=Lab.size();
  Lab=s;
  if (w==0 || w==s.size())
    LabX=expandstringlist(Lab,LabMerge);
  else {
    LabAlign=DefAlign;
    LabAlignX=DefAlign;
    LabMerge=DefMerge;
    LabX=Lab;
  }
}

// ---------------------------------------------------------------------
void WGrid::setlabelalign(QStringList s)
{
  QVector<int> a=qlist2ints(s);
  if (a.size()==1 || a.size()==Lab.size())
    LabAlign=a;
  else
    error("label align length of " + i2s(a.size()) +
          " does not match label length of " + i2s(Lab.size()));
}

// ---------------------------------------------------------------------
void WGrid::setlabelmerge(QStringList s)
{
  QVector<int> m=qlist2ints(s);
  if (!(m.size() == 1 || m.size() == Lab.size())) {
    error("label merge length of " + i2s(m.size()) +
          " does not match label length of " + i2s(Lab.size()));
    return;
  }
  LabMerge=m;
  LabX=expandstringlist(Lab,LabMerge);
  LabAlignX=expandvectorint(LabAlign,LabMerge);
}

// ---------------------------------------------------------------------
void WGrid::setshape(QStringList s)
{
  QVector<int> a=qlist2ints(s);
  if (a.size()!=2) {
    error("grid shape must be rows,columns: " + v2s(a));
    return;
  }
  Crws=a[0];
  Ccls=a[1];
}

// ---------------------------------------------------------------------
std::string WGrid::state(std::string event)
{
  std::string r;
  if ("mb"==event.substr(0,2))
    r+=spair(pchild->id,i2s(row)+" "+i2s(col));
  return r;
}
