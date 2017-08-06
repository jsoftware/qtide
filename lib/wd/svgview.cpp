
#include "cmd.h"
#include "form.h"
#include "svgview2.h"
#include "svgview.h"
#include "pane.h"

extern Form *form;

// ---------------------------------------------------------------------
SvgView::SvgView(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="svgview";
  SvgView2 *w= new SvgView2(this);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  QStringList opt=qsplit(s);
  if (invalidopt(n,opt,"zoom")) return;
  w->setObjectName(qn);
  imageFile="";
  imageXml="";
  w->showZoom(opt.contains("zoom"));
  childStyle(opt);
}

// ---------------------------------------------------------------------
string SvgView::get(string p,string v)
{
  SvgView2 *w = (SvgView2 *)widget;
  string r;
  if (p=="property") {
    r+=string("image")+"\012"+string("showzoom")+"\012"+string("windoworg")+"\012"+string("xml")+"\012"+string("zoom")+"\012";
    r+=Child::get(p,v);
  } else if (p=="image") {
    r=imageFile;
  } else if (p=="showzoom") {
    r=i2s(w->m_showzoom);
  } else if (p=="windoworg") {
    r=i2s(w->getOrigin().x())+" "+i2s(w->getOrigin().y());
  } else if (p=="xml") {
    r=imageXml;
  } else if (p=="zoom") {
    r=i2s(w->getZoom());
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void SvgView::set(string p,string v)
{
  SvgView2 *w = (SvgView2 *)widget;
  if (p=="image") {
    QString s=s2q(remquotes(v));
    if (s.isEmpty()) {
      error("set image needs image filename: " + id + " " + p + " " + v);
      return;
    }
    w->setFile(s);
    imageFile=q2s(s);
  } else if (p=="showzoom") {
    w->showZoom(remquotes(v)!="0");
  } else if (p=="windoworg") {
    QStringList n=s2q(v).split(" ",QString::SkipEmptyParts);
    if (n.size()!=2) {
      error("set windoworg requires 2 numbers: " + v);
    } else {
      int x=c_strtoi(q2s(n.at(0)));
      int y=c_strtoi(q2s(n.at(1)));
      w->setOrigin(x,y);
    }
  } else if (p=="xml") {
    w->setXml(imageXml=v);
  } else if (p=="zoom") {
    w->setZoom(c_strtoi(v));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
void SvgView::cmd(string p,string v)
{
  SvgView2 *w = (SvgView2 *)widget;
  if (p=="print") {
    w->printSVG();
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
string SvgView::state()
{
  return "";
}
