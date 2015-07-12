
#include <QApplication>

#include "cmd.h"
#include "font.h"
#include "wd.h"
#include "../base/bedit.h"
#include "../base/jsvr.h"
#include "../base/note.h"
#include "../base/state.h"
#include "../base/tedit.h"
#include "../base/term.h"

// sm does its own parsing of the wd command
extern Cmd cmd;
extern int rc;

static string smact();
static string smactive();
static string smclose();
static string smerror(string p);
static string smfocus();
static string smfont();
static string smget();
static string smgetactive();
static string smgetscript(string);
static string smgettabs(QString);
static string smgetwin(string);
static string smgetwin1(Bedit *);
static string smgetwin2(Note *n);
static string smgetxywh();
static string smgetxywh1(QWidget *);
static string smopen();
static string smprompt();
static string smreplace();
static string smsave();
static string smsaveactive();
static string smsaveall();
static string smset();
static string smsetselect(Bedit *,string);
static string smsettext(string,string);
static string smsetxywh(string,string);

// ---------------------------------------------------------------------
string sm(string c)
{
  rc=0;
  if (c=="act")
    return smact();
  if (c=="active")
    return smactive();
  if (c=="close")
    return smclose();
  if (c=="focus")
    return smfocus();
  if (c=="font")
    return smfont();
  if (c=="get")
    return smget();
  if (c=="new")
    return smopen();
  if (c=="open")
    return smopen();
  if (c=="replace")
    return smreplace();
  if (c=="save")
    return smsave();
  if (c=="set")
    return smset();
  else if (c=="prompt")
    return smprompt();
  cmd.getparms();
  return smerror("unrecognized sm command: " + c);
}

// ---------------------------------------------------------------------
string smact()
{
  cmd.getparms();
  term->smact();
  return"";
}

// ---------------------------------------------------------------------
string smactive()
{
  string p=cmd.getparms();
  QStringList opt=qsplit(p);
  if (note==0 || note->editIndex()<0)
    return smerror ("No active edit window");
  if (opt[0]!="tab")
    return smerror ("unrecognized sm command parameters: " + p);
  int ndx=opt[1].toInt();
  if (ndx<0 || ndx>=note->count())
    return smerror ("invalid tab index: " + p);
  note->setindex(ndx);
  return "";
}

// ---------------------------------------------------------------------
string smclose()
{
  string c=cmd.getid();
  string p=cmd.getparms();
  if (c=="tab") {
    if (note==0 || note->editIndex()<0)
      return smerror ("No active edit window");
    int ndx=s2q(p).toInt();
    if (ndx<0 || ndx>=note->count())
      return smerror ("invalid tab index: " + p);
    note->tabclose(ndx);
  } else if (c=="edit") {
    if (note==0)
      return smerror ("No edit window");
    note->close();
  } else if (c=="edit2") {
    if (note2==0)
      return smerror ("No edit2 window");
    note2->close();
  } else
    return smerror ("unrecognized sm command parameters: " + p);
  return "";
}

// ---------------------------------------------------------------------
string smerror(string p)
{
  rc=1;
  return p;
}

// ---------------------------------------------------------------------
string smfocus()
{
  string p=cmd.getparms();
  if (p.empty())
    return smerror("sm focus needs additional parameters");
  if (p=="term")
    term->smact();
  else if (p=="edit") {
    if (note==0 || note->editIndex()==-1)
      return smerror("No active edit window");
    note->activateWindow();
    note->raise();
    note->repaint();
  } else if (p=="edit2") {
    if (note2==0 || note2->editIndex()==-1)
      return smerror("No active edit2 window");
    setnote(note2);
    note->activateWindow();
    note->raise();
    note->repaint();
  } else
    return smerror("unrecognized sm command: focus " + p);
  return "";
}

// ---------------------------------------------------------------------
string smfont()
{
  string p=cmd.getparms();
  if (!p.empty()) {
    Font *fnt = new Font(p);
    if (fnt->error) {
      delete fnt;
      return smerror("unrecognized sm command: font " + p);
    } else {
      config.Font=fnt->font;
      delete fnt;
    }
  }
  fontset(config.Font);
  return "";
}

// ---------------------------------------------------------------------
string smget()
{
  string p=cmd.getparms();
  if (p.size()==0)
    return smerror("sm get needs additional parameters");
  if (p=="active")
    return smgetactive();
  if (p=="term" || p=="edit" || p=="edit2")
    return smgetwin(p);
  if (p=="xywh")
    return smgetxywh();
  QStringList s=qsplit(p);
  if (s[0]=="tabs") {
    if(s.size()<=1)
      return smerror("sm command requires another parameter: get tabs");
    else
      return smgettabs(s[1]);
  }
  return smerror("unrecognized sm command: get " + p);
}

// ---------------------------------------------------------------------
string smgetactive()
{
  rc=-1;
  return (note && ActiveWindows.indexOf(note)<ActiveWindows.indexOf(term))
         ? "edit" : "term";
}

// ---------------------------------------------------------------------
string smgetscript(string f)
{
  return dors(">{.getscripts_j_ '" + f + "'");
}

// ---------------------------------------------------------------------
string smgettabs(QString p)
{
  Note *n;
  if (p=="edit") {
    if (note==0)
      return smerror("No active edit window");
    n=note;
  } else if (p=="edit2") {
    if (note2==0)
      return smerror("No active edit2 window");
    n=note2;
  } else
    return smerror("sm get tabs needs edit or edit2 parameter");
  rc=-2;
  return n->gettabstate();
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
    return smgetwin2(note);
  }
  if (note2==0)
    return smerror("No active edit2 window");
  return smgetwin2(note2);
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
string smgetwin2(Note *n)
{
  if (n->editIndex()==-1)
    return smgetwin1((Bedit *)0);
  string r=smgetwin1((Bedit *)n->editPage());
  r+=spair("file",n->editFile());
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
  return q2s(QString::number(p.rx())+" "+QString::number(p.ry())+" "+
             QString::number(z.width())+" "+QString::number(z.height()));
}

// ---------------------------------------------------------------------
string smopen()
{
  string c=cmd.getid();
  string p=cmd.getparms();

  if (c=="edit")
    term->vieweditor();
  if (c=="edit2") {
    if (note==0)
      return smerror("no edit window open");
    note->on_winotherAct_triggered();
  }
  if (c=="edit" || c=="edit2")
    return "";
  if (c!="tab") {
    return smerror("unrecognized sm command: open " + c);
  }
  term->vieweditor();
  if (p.empty())
    note->newtemp();
  else {
    QString f=s2q(smgetscript(p));
    if (!cfexist(f))
      return smerror("file not found: " + q2s(f));
    note->fileopen(f);
  }
  rc=-1;
  return i2s(note->editIndex());
}

// ---------------------------------------------------------------------
string smprompt()
{
  string p=cmd.getparms();
  term->smprompt(s2q(p));
  return"";
}

// ---------------------------------------------------------------------
string smreplace()
{
  string c=cmd.getid();
  string p=cmd.getparms();
  if (note==0 || note->editIndex()<0)
    return smerror ("No active edit window");
  if (c!="edit")
    return smerror("unrecognized sm command: replace " + c);
  if (p.empty())
    return smerror("replace needs 2 parameters: edit filename");
  QString f=s2q(smgetscript(p));
  if (!cfexist(f))
    return smerror("file not found: " + q2s(f));
  note->filereplace(f);
  return"";
}

// ---------------------------------------------------------------------
// string smrun()
// {
//   string p=cmd.getparms();
//   if (p!="edit")
//     return smerror("unrecognized sm command: run " + p);
//   if (note==0 || note->editIndex()<0)
//     return smerror("No active edit window");
//   note->runlines(true,false);  // all lines, no display
//   return"";
// }
//
// ---------------------------------------------------------------------
string smsave()
{
  string p=cmd.getparms();
  if (note==0)
    return smerror("No active edit window");
  if (p.empty())
    return smerror("sm save parameter not given");
  if (p=="edit")
    return smsaveactive();
  if (p=="tabs")
    return smsaveall();
  return smerror("sm save parameter should be 'edit' or 'tabs': " + p);
}

// ---------------------------------------------------------------------
string smsaveactive()
{
  note->savecurrent();
  return "";
}

// ---------------------------------------------------------------------
string smsaveall()
{
  note->saveall();
  return "";
}

// ---------------------------------------------------------------------
string smset()
{
  string p=cmd.getid();
  if (p.empty())
    return smerror("sm set parameters not given");
  string c=cmd.getid();
  if (c.empty())
    return smerror("sm set " + p + " parameters not given");
  string q=cmd.getparms();
  Bedit *e;

  if (p=="term") {
    e=tedit;
  } else if (p=="edit") {
    if (note==0)
      return smerror("No active edit window");
    e=(Bedit *)note->editPage();
  } else if (p=="edit2") {
    if (note2==0)
      return smerror("No active edit2 window");
    e=(Bedit *)note2->editPage();
  } else
    return smerror("unrecognized sm command: set " + p);

  if (e==0 && (q=="select" || q=="text"))
    return smerror("no edit window for sm command: set " + q);

  if (c=="select")
    return smsetselect(e,q);
  if (c=="text")
    return smsettext(p,q);
  if (c=="xywh")
    return smsetxywh(p,q);

  return smerror("unrecognized sm command: set " + p + " " + q);
}

// ---------------------------------------------------------------------
string smsetselect(Bedit *e, string q)
{
  QList<int> s=qsl2intlist(qsplit(q));
  if (s.size()!= 2)
    return smerror("sm set select should have begin and end parameters");
  int m=e->toPlainText().size();
  if (s[1]==-1) s[1]=m;
  s[1]=qMin(m,s[1]);
  s[0]=qMin(s[0],s[1]);
  e->setselect(s[0],s[1]-s[0]);
  return"";
}

// ---------------------------------------------------------------------
string smsettext(string p,string s)
{
  QString t=s2q(s);
  if (p=="term")
    tedit->setPlainText(t);
  else if (p=="edit")
    note->settext(t);
  else
    note2->settext(t);
  return"";
}

// ---------------------------------------------------------------------
string smsetxywh(string m,string q)
{
  QWidget *w;
  if (m=="term")
    w=term;
  else if (m=="edit")
    w=note;
  else
    w=note2;
  QList<int> s=qsl2intlist(qsplit(q));
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

