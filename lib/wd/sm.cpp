
#include <QApplication>

#include "wd.h"
#include "cmd.h"
#include "../base/bedit.h"
#include "../base/note.h"
#include "../base/tedit.h"
#include "../base/term.h"

extern int rc;

string smerror(string);
string smfocus(string);
string smget(string);
string smgetactive();
string smgetwin(string);
string smgetwin1(Bedit *);
string smgetxywh();
string smgetxywh1(QWidget *);
string smset(string);
string smsetselect(Bedit *,QStringList);
string smsettext(Bedit *,QStringList);
string smsetxywh(QWidget *,QStringList);

// ---------------------------------------------------------------------
// c is type, p is parameter
string sm(string c,string p)
{
  rc=0;
  if (c=="focus")
    return smfocus(p);
  if (c=="get")
    return smget(p);
  if (c=="set")
    return smset(p);
  if (c=="act")
    term->smact();
  else if (c=="prompt")
    term->smprompt(s2q(p));
  else
    return smerror("unrecognized sm command: " + c);
  return "";
}

// ---------------------------------------------------------------------
string smerror(string p)
{
  rc=1;
  return p;
}

// ---------------------------------------------------------------------
string smfocus(string p)
{
  if (p.size()==0)
    return smerror("sm focus needs additional parameters");
  if (p=="term")
    term->smact();
  else if (p=="edit") {
    if (note==0 || note->editIndex()==-1)
      return smerror("No active edit window");
    note->activateWindow();
    note->raise();
    note->repaint();
  } else
    return smerror("unrecognized sm command: focus " + p);
  return "";
}

// ---------------------------------------------------------------------
string smget(string p)
{
  if (p.size()==0)
    return smerror("sm get needs additional parameters");
  if (p=="active")
    return smgetactive();
  if (p=="term" || p=="edit" || p=="edit2")
    return smgetwin(p);
  if (p=="xywh")
    return smgetxywh();
  return smerror("unrecognized sm command: get " + p);
}

// ---------------------------------------------------------------------
string smgetactive()
{
  rc=-1;
  QWidget *w=QApplication::focusWidget();
  bool ifnote=note && w==(QWidget *)note->editPage();
  return ifnote ? "edit" : "term";
}

// ---------------------------------------------------------------------
string smgetwin(string p)
{
  rc=-2;
  if (p=="term")
    return smgetwin1(tedit);
  if (p=="edit") {
    if (note==0)
      return smerror("No active edit window");
    if (note->editIndex()==-1)
      return smgetwin1((Bedit *)0);
    return smgetwin1((Bedit *)note->editPage());
  }
  if (note2==0)
    return smerror("No active edit2 window");
  if (note2->editIndex()==-1)
    return smgetwin1((Bedit *)0);
  return smgetwin1((Bedit *)note2->editPage());
}

// ---------------------------------------------------------------------
string smgetwin1(Bedit *t)
{
  string r;

  if (t==0) {
    r+=spair("text",(string)"");
    r+=spair("select",(string)"");
  } else {
    QTextCursor c=t->textCursor();
    int b=c.selectionStart();
    int e=c.selectionEnd();
    r+=spair("text",t->toPlainText());
    r+=spair("select",QString::number(b)+" "+QString::number(e));
  }
  return r;
}

// ---------------------------------------------------------------------
string smgetxywh()
{
  rc=-2;
  string r;
  r+=spair("text",smgetxywh1(term));
  if (note)
    r+=spair("edit",smgetxywh1(note));
  if (note2)
    r+=spair("edit2",smgetxywh1(note2));
  return r;
}

// ---------------------------------------------------------------------
string smgetxywh1(QWidget *w)
{
  QPoint p=w->pos();
  QSize z=w->size();
  return q2s(QString::number(p.rx())+" "+QString::number(p.ry())+
             " "+QString::number(z.width())+" "+QString::number(z.height()));
}

// ---------------------------------------------------------------------
string smset(string arg)
{
  QStringList opt=qsplit(arg);

  QString p,q;
  Bedit *e;
  QWidget *w;

  if (opt.size()==0)
    return smerror("sm set parameters not given");
  if (opt.size()==1)
    return smerror ("sm set " + q2s(opt[0]) + " parameters not given");
  p=opt[0];
  q=opt[1];
  opt=opt.mid(2);

  if (p=="term") {
    e=tedit;
    w=term;
  } else if (p=="edit") {
    if (note==0)
      return smerror("No active edit window");
    e=(Bedit *)note->editPage();
    w=note;
  } else if (p=="edit2") {
    if (note2==0)
      return smerror("No active edit2 window");
    e=(Bedit *)note2->editPage();
    w=note2;
  } else
    return smerror("unrecognized sm command: set " + q2s(p));

  if (e==0 && (q=="select" || q=="text"))
    return smerror("no edit window for sm command: set " + q2s(q));

  if (q=="select")
    return smsetselect(e,opt);
  if (q=="text")
    return smsettext(e,opt);
  if (q=="xywh")
    return smsetxywh(w,opt);

  return smerror("unrecognized sm command: set " + q2s(p) + " " + q2s(q));
}

// ---------------------------------------------------------------------
string smsetselect(Bedit *e,QStringList opt)
{
  QList<int> s=qsl2intlist(opt);

  if (s.size()!= 2)
    return smerror ("sm set select should have begin and end parameters");
  int m=e->toPlainText().size();
  if (s[1]==-1) s[1]=m;
  s[1]=qMin(m,s[1]);
  s[0]=qMin(s[0],s[1]);
  e->setselect(s[0],s[1]-s[0]);
  return"";
}

// ---------------------------------------------------------------------
string smsettext(Bedit *e,QStringList opt)
{
  e->setPlainText(opt[0]);
  return"";
}

// ---------------------------------------------------------------------
string smsetxywh(QWidget *w,QStringList opt)
{
  QList<int> s=qsl2intlist(opt);
  QPoint p=w->pos();
  QSize z=w->size();
  if (s[0]==-1) s[0]=p.rx();
  if (s[1]==-1) s[1]=p.ry();
  if (s[2]==-1) s[2]=z.width();
  if (s[3]==-1) s[3]=z.height();
  w->move(s[0],s[1]);
  w->resize(s[2],s[3]);
  return"";
}
