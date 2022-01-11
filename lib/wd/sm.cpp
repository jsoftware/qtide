
#include <QApplication>
#include <QStyleFactory>

#include "cmd.h"
#include "font.h"
#include "wd.h"
#include "../base/bedit.h"
#include "../base/dlog.h"
#include "../base/jsvr.h"
#include "../base/note.h"
#include "../base/state.h"
#include "../base/tedit.h"
#include "../base/term.h"

// sm does its own parsing of the wd command
extern Cmd cmd;
extern int rc;

static std::string smact();
static std::string smactive();
static std::string smclose();
static std::string smerror(std::string p);
static std::string smfocus();
static std::string smfont();
static std::string smget();
static std::string smgetactive();
static std::string smgetinputlog();
static std::string smgetscript(std::string);
static std::string smgettabs(QString);
static std::string smgettermcwh();
static std::string smgetwin(std::string);
static std::string smgetwin1(QWidget *);
static std::string smgetwin2(Note *n);
static std::string smgetxywh();
static std::string smgetxywh1(QWidget *);
static std::string smhtml();
static std::string smopen();
static std::string smprofont();
static std::string smprompt();
static std::string smreplace();
static std::string smsave();
static std::string smsaveactive();
static std::string smsaveall();
static std::string smset();
static std::string smsetinputlog(std::string,std::string);
static std::string smsetscroll(Bedit *,std::string);
static std::string smsetselect(Bedit *,std::string);
static std::string smsettext(std::string,std::string);
static std::string smsetxywh(std::string,std::string);
static std::string smstyle();
static std::string smtheme();

// ---------------------------------------------------------------------
std::string sm(std::string c)
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
  if (c=="html")
    return smhtml();
  if (c=="new")
    return smopen();
  if (c=="open")
    return smopen();
  if (c=="profont")
    return smprofont();
  if (c=="prompt")
    return smprompt();
  if (c=="replace")
    return smreplace();
  if (c=="save")
    return smsave();
  if (c=="set")
    return smset();
  if (c=="style")
    return smstyle();
  if (c=="theme")
    return smtheme();
  cmd.getparms();
  return smerror("unrecognized sm command: " + c);
}

// ---------------------------------------------------------------------
std::string smact()
{
  cmd.getparms();
  term->smact();
  return"";
}

// ---------------------------------------------------------------------
std::string smactive()
{
  std::string p=cmd.getparms();
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
std::string smclose()
{
  std::string c=cmd.getid();
  std::string p=cmd.getparms();
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
std::string smerror(std::string p)
{
  rc=1;
  return p;
}

// ---------------------------------------------------------------------
std::string smfocus()
{
  std::string p=cmd.getparms();
  if (p.empty())
    return smerror("sm focus needs additional parameters");
  if (p=="term")
    term->smact();
  else if (p=="edit") {
    if (note==0)
      return smerror("No active edit window");
    if (note->editIndex()==-1)
      return "";
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
std::string smfont()
{
  std::string p=cmd.getparms();
  if (!p.empty()) {
    Font *fnt = new Font(p);
    if (fnt->error) {
      delete fnt;
      return smerror("unrecognized sm command: font " + p);
    } else {
      config.Font=fnt->font;
      delete fnt;
      fontset(config.Font);
    }
  } else
    fontset(config.DefFont);
  return "";
}

// ---------------------------------------------------------------------
std::string smget()
{
  std::string p=cmd.getparms();
  if (p.size()==0)
    return smerror("sm get needs additional parameters");
  if (p=="active")
    return smgetactive();
  if (p=="term" || p=="edit" || p=="edit2")
    return smgetwin(p);
  if (p=="termcwh")
    return smgettermcwh();
  if (p=="inputlog")
    return smgetinputlog();
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
std::string smgetactive()
{
  rc=-1;
  return (note && ActiveWindows.indexOf(note)<ActiveWindows.indexOf(term))
         ? "edit" : "term";
}

// ---------------------------------------------------------------------
std::string smgetinputlog()
{
  rc=-1;
  return q2s(dlog_get());
}

// ---------------------------------------------------------------------
std::string smgetscript(std::string f)
{
  return dors(">{.getscripts_j_ '" + f + "'");
}

// ---------------------------------------------------------------------
std::string smgettabs(QString p)
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
std::string smgettermcwh()
{
  rc=-1;
  QSize z=tedit->size();
  QFontMetrics fm=QFontMetrics(config.Font,0);
  int sb=app->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
  sb+=4; // padding
  int fh=fm.height();
#ifdef QT512
  int fw=fm.horizontalAdvance("X");
#else
  int fw=fm.width("X");
#endif
  int ch=(z.height()-sb)/fh;
  int cw=(z.width()-sb)/fw;
  return q2s(QString::number(cw)+" "+QString::number(ch));
}

// ---------------------------------------------------------------------
std::string smgetwin(std::string p)
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
std::string smgetwin1(QWidget *t)
{
  std::string r;
  if (t==0) {
    r+=spair("text",(std::string)"");
    r+=spair("select",(std::string)"");
  } else {
    QTextCursor c=getcursor(t);
    int b=c.selectionStart();
    int e=c.selectionEnd();
    r+=spair("text",getplaintext(t));
    r+=spair("select",QString::number(b)+" "+QString::number(e));
  }
  return r;
}

// ---------------------------------------------------------------------
std::string smgetwin2(Note *n)
{
  if (n->editIndex()==-1)
    return smgetwin1((Bedit *)0);
  std::string r=smgetwin1((Bedit *)n->editPage());
  r+=spair("file",n->editFile());
  return r;
}

// ---------------------------------------------------------------------
std::string smgetxywh()
{
  rc=-2;
  std::string r;
  r+=spair("term",smgetxywh1(term));
  if (note)
    r+=spair("edit",smgetxywh1(note));
  if (note2)
    r+=spair("edit2",smgetxywh1(note2));
  return r;
}

// ---------------------------------------------------------------------
std::string smgetxywh1(QWidget *w)
{
  QPoint p=w->pos();
  QSize z=w->size();
  return q2s(QString::number(p.rx())+" "+QString::number(p.ry())+" "+
             QString::number(z.width())+" "+QString::number(z.height()));
}

// ---------------------------------------------------------------------
std::string smhtml()
{
  std::string p=cmd.getparms();
  tedit->appendHtml(s2q(p));
  return"";
}

// ---------------------------------------------------------------------
std::string smopen()
{
  std::string c=cmd.getid();
  std::string p=cmd.getparms();

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
std::string smprofont()
{
  std::string p=cmd.getparms();
  if (!p.empty()) {
    Font *fnt = new Font(p);
    if (fnt->error) {
      delete fnt;
      return smerror("unrecognized sm command: font " + p);
    } else {
      QApplication::setFont(fnt->font);
      delete fnt;
    }
  } else
    QApplication::setFont(config.DefProFont);
  return "";
}

// ---------------------------------------------------------------------
std::string smprompt()
{
  std::string p=cmd.getparms();
  term->smprompt(s2q(p));
  return"";
}

// ---------------------------------------------------------------------
std::string smreplace()
{
  std::string c=cmd.getid();
  std::string p=cmd.getparms();
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
std::string smsave()
{
  std::string p=cmd.getparms();
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
std::string smsaveactive()
{
  note->savecurrent();
  return "";
}

// ---------------------------------------------------------------------
std::string smsaveall()
{
  note->saveall();
  return "";
}

// ---------------------------------------------------------------------
std::string smset()
{
  std::string p=cmd.getid();
  if (p.empty())
    return smerror("sm set parameters not given");

  std::string c=cmd.getid();
  if (c.empty())
    return smerror("sm set " + p + " parameters not given");
  std::string q=cmd.getparms();

  if (p=="inputlog")
    return smsetinputlog(c,q);

  if (p=="term") {
    if (c=="text")
      return smsettext(p,q);
    if (c=="xywh")
      return smsetxywh(p,q);
    return smerror("command applies only to an edit window: " + c);
  }

  Bedit *e;

  if (p=="edit") {
    if (note==0)
      return smerror("No active edit window");
    e=(Bedit *)note->editPage();
  } else if (p=="edit2") {
    if (note2==0)
      return smerror("No active edit2 window");
    e=(Bedit *)note2->editPage();
  } else
    return smerror("unrecognized sm command: set " + p);

  if (e==0 && (c=="scroll" || c=="select" || c=="text"))
    return smerror("no edit window for sm command: set " + c);

  if (c=="scroll")
    return smsetscroll(e,q);
  if (c=="select")
    return smsetselect(e,q);
  if (c=="text")
    return smsettext(p,q);
  if (c=="xywh")
    return smsetxywh(p,q);

  return smerror("unrecognized sm command: set " + p + " " + q);
}

// ---------------------------------------------------------------------
std::string smsetinputlog(std::string c,std::string q)
{
  if (c!="text")
    return smerror("unrecognized sm command: set inputlog " + c + "..." );
  dlog_set(s2q(q));
  return "";
}

// ---------------------------------------------------------------------
// set vertical scroll
std::string smsetscroll(Bedit *e, std::string q)
{
  if (!e) return"";
  QList<int> s=qsl2intlist(qsplit(q));
  if (s.size()!= 1)
    return smerror("sm set scroll should have a single parameter of scroll size");
  e->settop(s[0]);
  return"";
}

// ---------------------------------------------------------------------
std::string smsetselect(Bedit *e, std::string q)
{
  if (!e) return"";
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
std::string smsettext(std::string p,std::string s)
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
std::string smsetxywh(std::string m,std::string q)
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

// ---------------------------------------------------------------------
std::string smstyle()
{
  std::string p=cmd.getparms();
  if (!p.empty())
    QApplication::setStyle(QStyleFactory::create(s2q(p)));
  return "";
}

// ---------------------------------------------------------------------
std::string smtheme()
{
  std::string p=cmd.getparms();
  if (p == "dark") {
    QFile f(":qdarkstyle/dark/style.qss");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    app->setStyleSheet(ts.readAll());
  } else
    app->setStyleSheet("");
  return "";
}
