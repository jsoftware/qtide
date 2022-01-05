
#include <QTimeEdit>

#include <math.h>

#include "cmd.h"
#include "form.h"
#include "pane.h"
#include "timeedit.h"
#include "wd.h"

// optional parms are:
// minimum
// single step
// maximum
// value

static void totime(double v, int *h, int *m, int *se, int *mi);

// ---------------------------------------------------------------------
TimeEdit::TimeEdit(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="timeedit";
  QTimeEdit *w=new QTimeEdit;
  QString qn=s2q(n);
  widget=(QWidget*) w;
  QStringList opt=qsplit(s);
  if (invalidoptn(n,opt,"")) return;
  w->setObjectName(qn);
  childStyle(opt);

  int i=0;
  double v;
  int h,m,se,mi;
  if (i<opt.size()) {
    v=c_strtod(q2s(opt.at(i)));
    totime(v, &h, &m, &se, &mi);
    w->setMinimumTime(QTime(h,m,se,mi));
    i++;
  }
  if (i<opt.size()) {
    v=c_strtod(q2s(opt.at(i)));
    totime(v, &h, &m, &se, &mi);
    w->setMaximumTime(QTime(h,m,se,mi));
    i++;
  }
  if (i<opt.size()) {
    v=c_strtod(q2s(opt.at(i)));
    totime(v, &h, &m, &se, &mi);
    w->setTime(QTime(h,m,se,mi));
    i++;
  }
  connect(w,SIGNAL(timeChanged(QTime)),
          this,SLOT(valueChanged()));
}

// ---------------------------------------------------------------------
void TimeEdit::valueChanged()
{
  event="changed";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
std::string TimeEdit::get(std::string p,std::string v)
{
  QTimeEdit *w=(QTimeEdit*) widget;
  std::string r;
  if (p=="property") {
    r+=std::string("format")+"\012"+ "max"+"\012"+ "min"+"\012"+ "readonly"+"\012"+ "value"+"\012";
    r+=Child::get(p,v);
  } else if (p=="format")
    r=q2s(w->displayFormat());
  else if (p=="max") {
    QTime q=w->maximumTime();
    r=d2s((10000*q.hour())+(100*q.minute())+q.second()+(((double)q.msec())/1000.0));
  } else if (p=="min") {
    QTime q=w->minimumTime();
    r=d2s((10000*q.hour())+(100*q.minute())+q.second()+(((double)q.msec())/1000.0));
  } else if (p=="readonly")
    r=i2s(w->isReadOnly());
  else if (p=="value") {
    QTime q=w->time();
    r=d2s((10000*q.hour())+(100*q.minute())+q.second()+(((double)q.msec())/1000.0));
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void TimeEdit::set(std::string p,std::string v)
{
  QTimeEdit *w=(QTimeEdit*) widget;
  QString cmd=s2q(p);
  QStringList arg=qsplit(v);
  if (arg.isEmpty()) {
    Child::set(p,v);
    return;
  }
  double i;
  int h,m,se,mi;
  if (cmd=="format") {
    w->setDisplayFormat(s2q(remquotes(v)));
  } else if (cmd=="min") {
    i=c_strtod(q2s(arg.at(0)));
    totime(i, &h, &m, &se, &mi);
    w->setMinimumTime(QTime(h,m,se,mi));
  } else if (cmd=="max") {
    i=c_strtod(q2s(arg.at(0)));
    totime(i, &h, &m, &se, &mi);
    w->setMaximumTime(QTime(h,m,se,mi));
  } else if (p=="readonly") {
    w->setReadOnly(remquotes(v)!="0");
  } else if (cmd=="value") {
    i=c_strtod(q2s(arg.at(0)));
    totime(i, &h, &m, &se, &mi);
    w->setTime(QTime(h,m,se,mi));
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string TimeEdit::state()
{
  QTimeEdit *w=(QTimeEdit*) widget;
  QTime v=w->time();
  return spair(id,d2s((10000*v.hour())+(100*v.minute())+v.second()+(((double)v.msec())/1000.0)));
}

// ---------------------------------------------------------------------
void totime(double v, int *h, int *m, int *se, int *mi)
{
  int v1=(int)floor(v);
  *h=(int)floor((float)v1/10000);
  v1=v1%10000;
  *m=(int)floor((float)v1/100);
  *se=v1%100;
  *mi=(int)floor(1000 * (v-((*h *10000)+(*m *100)+ *se)));
}
