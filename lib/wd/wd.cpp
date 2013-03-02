
#include <QDesktopWidget>
#include <QApplication>
#include <QLayout>
#include <QTimer>

#include "wd.h"
#include "bitmap.h"
#include "child.h"
#include "cmd.h"
#include "font.h"
#include "form.h"
#include "pane.h"
#include "isigraph.h"
#include "menus.h"
#include "tabs.h"
#include "../base/term.h"

#include "math.h"

extern "C" {
  Dllexport int wd(char *s,char *&r,int &len,char *loc);

// TODO
  Dllexport int wdisparent(char *s);
  Dllexport void *wdgetparentid(void *s);
}

extern int jedo(char *);

void wd1();
void wdbin();
void wdcc();
void wdcn();
void wdend();
void wdfontdef();
void wdgroupbox(string c);
void wdline(string);
void wdmb();
void wdmenu(string);
void wdnotyet();
void wdpactive();
void wdp(string c);
void wdpc();
void wdpclose();
void wdpmovex();
void wdpn();
void wdpsel();
void wdpshow();
void wdpstylesheet();
void wdptop();
void wdq();
void wdqd();
void wdqueries(string);
void wdrem();
void wdreset();
void wdset();
void wdsetenable();
void wdsetp();
void wdsetx(string);
void wdsplit(string c);
void wdstate(int);
void wdtab(string);
void wdtimer();
void wdxywh(int);
void wdversion();
void wdwh();

void error(string s);
bool nochild();
bool nochildset(string id);
bool noform();
bool notab();
int setchild(string id);

Cmd cmd;
Child *cc=0;
Form *form=0;
Form *evtform=0;
Font *fontdef=0;

QList<Form *>Forms;

int rc;
string lasterror="";
string result="";
string tlocale="";

// TODO  for debug
string cmdstr;
string ccmd;

// TODO
// ---------------------------------------------------------------------
int wdisparent(char *s)
{
  string p= string(s);
  Form *f;
  void *n=(void *) c_strtol(p);
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (n==f || p==f->id)
      return 1;
  }
  return 0;
}

// ---------------------------------------------------------------------
void *wdgetparentid(void *s)
{
  Form *f;
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (f->ischild((Child *) s))
      return (void *)(f->id).c_str();
  }
  return 0;
}

// ---------------------------------------------------------------------
int wd(char *s,char *&res,int &len,char *loc)
{
  rc=0;
  result.clear();
  tlocale=loc;
  cmd.init(s);
//  noevents(1);
  wd1();
//  noevents(0);
  len=result.size();
  res=(char *)result.c_str();
  return rc;
}

// ---------------------------------------------------------------------
// subroutines may set int rc and wd1 returns if non-zero
void wd1()
{
  string c,p;
  while ((rc==0) && cmd.more()) {
// TODO
    cmdstr=cmd.getcmdstr();
    c=cmd.getid();
    ccmd=c;
    if (c.empty()) continue;
    if (c=="q")
      wdq();
    else if (c=="bin")
      wdbin();
    else if (c=="cc")
      wdcc();
    else if (c=="cn")
      wdcn();
    else if (c=="end")
      wdend();
    else if (c=="fontdef")
      wdfontdef();
    else if (c.substr(0,8)=="groupbox")
      wdgroupbox(c);
    else if (c.substr(0,4)=="line")
      wdline(c);
    else if (c.substr(0,2)=="mb")
      wdmb();
    else if (c.substr(0,4)=="menu")
      wdmenu(c);
    else if (c[0]=='p')
      wdp(c);
    else if (c[0]=='q')
      wdqueries(c);
    else if (c=="rem")
      wdrem();
    else if (c=="reset")
      wdreset();
    else if (c=="set") {
      noevents(1);
      wdset();
      noevents(0);
    } else if (c=="setp") {
      noevents(1);
      wdsetp();
      noevents(0);
    } else if (c=="setenable")
      wdsetenable();
    else if (c.substr(0,3)=="set")
      wdsetx(c);
    else if (c.substr(0,5)=="split")
      wdsplit(c);
    else if (c.substr(0,3)=="tab")
      wdtab(c);
    else if (c=="timer")
      wdtimer();
    else if (c=="xywh")
      wdxywh(2);
    else if (c=="version")
      wdversion();
    else if (c=="wh")
      wdwh();
    else if (((c.substr(0,4)=="tbar") || (c.substr(0,4)=="sbar") || c=="msgs") || 0) {
      cmd.getparms();
      wdnotyet();
    } else
      error("command not found");
  }
}

// ---------------------------------------------------------------------
void wdbin()
{
  if (noform()) return;
  form->pane->bin(cmd.getparms());
}

// ---------------------------------------------------------------------
void wdcc()
{
  if (noform()) return;
  string c,n,p;
  n=cmd.getid();
  c=cmd.getid();
  p=cmd.getparms();
  if (form->pane->addchild(n,c,p)) return;
  error ("child not supported: " + c);
}

// ---------------------------------------------------------------------
void wdcn()
{
  if (noform()) return;
  cc=form->child;
  if (nochild()) return;
  string p=remquotes(cmd.getparms());
  cc->setp("caption",p);
}

// ---------------------------------------------------------------------
void wdend()
{
  cmd.end();
}

// ---------------------------------------------------------------------
void wdfontdef()
{
  string p=cmd.getparms();
  fontdef = new Font(p);
}

// ---------------------------------------------------------------------
void wdgroupbox(string c)
{
  if (noform()) return;
  string p=cmd.getparms();
  if (!form->pane->groupbox(c,p))
    error("unrecognized command: " + c + " " + p);
}

// ---------------------------------------------------------------------
void wdline(string c)
{
  if (noform()) return;
  string p=cmd.getparms();
  if (!form->pane->line(c,p))
    error("unrecognized command: " + c + " " + p);
}

// ---------------------------------------------------------------------
void wdmb()
{
  result=q2s(mb(cmd.getparms()));
  rc=-1;
}

// ---------------------------------------------------------------------
void wdmenu(string s)
{
  int rc=0;
  if (noform()) return;
  if (form->menubar==0) form->addmenu();
  string c,p;
  if (s=="menu") {
    c=cmd.getid();
    p=cmd.getparms();
    rc=form->menubar->menu(c,p);
  } else if (s=="menupop") {
    p=remquotes(cmd.getparms());
    rc=form->menubar->menupop(p);
  } else if (s=="menupopz") {
    p=cmd.getparms();
    rc=form->menubar->menupopz();
  } else if (s=="menusep") {
    p=cmd.getparms();
    rc=form->menubar->menusep();
  } else {
    p=cmd.getparms();
    error("menu command not found");
  }
  if (rc) error("menu command failed");
}

// ---------------------------------------------------------------------
// not yet
void wdnotyet()
{
  cmd.getparms();
}

// ---------------------------------------------------------------------
void wdp(string c)
{
  if (c=="pc")
    wdpc();
  else if (c=="pclose")
    wdpclose();
  else if (c=="pmovex")
    wdpmovex();
  else if (c=="pn")
    wdpn();
  else if (c=="psel")
    wdpsel();
  else if (c=="pshow")
    wdpshow();
  else if (c=="pstylesheet")
    wdpstylesheet();
  else if (c=="pactive")
    wdpactive();
  else if (c=="ptop")
    wdptop();
  else if (c=="pas" || c=="pcenter" || 0) {
    cmd.getparms();
    wdnotyet();
  } else
    error("parent command not found: " + c);
}

// ---------------------------------------------------------------------
void wdpactive()
{
  if (noform()) return;
  cmd.getparms();
#ifndef ANDROID
  form->activateWindow();
  form->raise();
#endif
}

// ---------------------------------------------------------------------
void wdpc()
{
  string c,p;
  c=cmd.getid();
  p=cmd.getparms();
// QWidget must be parentless to be top-level window
  QStringList m=s2q(p).split(' ',QString::SkipEmptyParts);
  form=new Form(c,p,tlocale,m.contains("owner")?form:0);
  evtform=form;
  Forms.append(form);
}

// ---------------------------------------------------------------------
void wdpclose()
{
  if (noform()) return;
  cmd.getparms();
  if (form->closed) return;
  form->closed=true;
  form->close();
}

// ---------------------------------------------------------------------
void wdpmovex()
{
  if (noform()) return;
  string p=cmd.getparms();
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=4)
    error("pmovex requires 4 numbers: " + p);
  else {
#ifndef ANDROID
    if (c_strtoi(q2s(n.at(0)))!=-1 && c_strtoi(q2s(n.at(1)))!=-1)
      form->move(c_strtoi(q2s(n.at(0))),c_strtoi(q2s(n.at(1))));
    if (c_strtoi(q2s(n.at(2)))!=-1 && c_strtoi(q2s(n.at(3)))!=-1)
      form->resize(c_strtoi(q2s(n.at(2))),c_strtoi(q2s(n.at(3))));
#endif
  }
}

// ---------------------------------------------------------------------
void wdpn()
{
  if (noform()) return;
  string p=remquotes(cmd.getparms());
  form->setpn(p);
}

// ---------------------------------------------------------------------
void wdpsel()
{
  string p=cmd.getparms();
  if (p.size()==0) {
    form=0;
    return;
  }
  Form *f;
  void *n=(void *) c_strtol(p);
  for (int i=0; i<Forms.size(); i++) {
    f=Forms.at(i);
    if (n==f || p==f->id) {
      form=f;
      return;
    }
  }
  error("command failed: psel");
}

// ---------------------------------------------------------------------
void wdpshow()
{
  if (noform()) return;
  cmd.getparms();
  form->showit();
}

// ---------------------------------------------------------------------
void wdpstylesheet()
{
  if (noform()) return;
  string p=cmd.getparms();
  form->setStyleSheet(s2q(p));
}

// ---------------------------------------------------------------------
void wdptop()
{
  if (noform()) return;
  cmd.getparms();
// TODO
#ifndef ANDROID
  form->raise();
#endif
}

// ---------------------------------------------------------------------
void wdq()
{
  string p=cmd.getparms();
  wdstate(1);
}

// ---------------------------------------------------------------------
void wdqueries(string s)
{
  string p=cmd.getparms();

  if (s=="qd") {
    wdstate(0);
    return;
  }

  rc=-1;
  if (s=="qer") {
    result=lasterror;
    return;
  }
// queries that form not needed
  if (s=="qm"||s=="qcolor") {
    error("command not found");
    return;
  } else if (s=="qscreen") {
    int dpix=QApplication::desktop()->logicalDpiX();
    int dpiy=QApplication::desktop()->logicalDpiY();
    int w=QApplication::desktop()->width();
    int h=QApplication::desktop()->height();
    int mmx=25.4*w/dpix;
    int mmy=25.4*h/dpiy;
    int dia=sqrt(dpix*dpix+dpiy*dpiy);
    result=i2s(mmx) + " " + i2s(mmy) + " " + i2s(w) + " " + i2s(h) + " " + i2s(dpix) + " " + i2s(dpiy) + " " + i2s(QApplication::desktop()->depth()) + " 1 " + i2s(QApplication::desktop()->colorCount()) + " " + i2s(dpix) + " " + i2s(dpix) + " " + i2s(dia);
    return;
  } else if (s=="qwd") {
    result="jqt";
    return;
  } else if (s=="qpx") {
    if (!Forms.size()) result="";
    else {
      string q;
      for (int i=0; i<Forms.size(); i++)
        q = q + p2s((void *)Forms.at(i)) + "\t"  + Forms.at(i)->id + "\t"  + Forms.at(i)->locale + "\t\012";
      result=q;
    }
    return;
  }
// queries that form is needed
  if (noform()) return;
  if (s=="qhwndp")
    result=form->hsform();
  else if (s=="qformx")
    result=form->qformx();
  else if (s=="qhwndc") {
    Child *cc;
    if ((cc=form->id2child(p))) result=p2s(cc);
    else
      error("command failed: " + s);
  } else if (s=="qchildxywhx") {
    Child *cc;
    if ((cc=form->id2child(p))) {
      if (!cc->widget) error("command failed: " + s);
      else {
        QPoint pos=cc->widget->mapTo(form,cc->widget->pos());
        QSize size=cc->widget->size();
        result=i2s(pos.x())+" "+i2s(pos.y())+" "+i2s(size.width())+" "+i2s(size.height());
      }
    } else error("command failed: " + s);
  } else
    error("command not found");
}

// ---------------------------------------------------------------------
void wdrem()
{
  cmd.getparms();
// TODO getline infinite loop bug?
//  cmd.getline();
}

// ---------------------------------------------------------------------
void wdreset()
{
  cmd.getparms();
  foreach (Form *f,Forms) {
    f->closed=true;
    f->close();
  }
  form=0;
  evtform=0;
  fontdef=0;
}

// ---------------------------------------------------------------------
void wdset()
{
  string n=cmd.getid();
  string p=cmd.getparms();
  int type=setchild(n);
  switch (type) {
  case 1 :
    cc->set(p);
    break;
  case 2 :
    cc->setp(n,p);
    break;
  }
}

// ---------------------------------------------------------------------
void wdsetenable()
{
  string n=cmd.getid();
  string p=cmd.getparms();
  switch (setchild(n)) {
  case 1:
    cc->setenable(p);
    break;
  case 2:
    cc->setenable(n+" "+p);
    break;
  }
}

// ---------------------------------------------------------------------
void wdsetp()
{
  string n=cmd.getid();
  if (nochildset(n)) return;
  string p=cmd.getid();
  string v=cmd.getparms();
  if (p=="stretch")
    form->pane->setstretch(cc,v);
  else {
    cc->setp(p,v);
  }
}

// ---------------------------------------------------------------------
void wdsetx(string c)
{
  string n=cmd.getid();
// TODO
  if (1!=setchild(n)) {
    string p=cmd.getparms();
    return;
  }
  if (nochildset(n)) return;
  string p=cmd.getparms();
  cc->setp(c.substr(3),p);
}

// ---------------------------------------------------------------------
void wdsplit(string c)
{
  if (noform()) return;
  string p=cmd.getparms();
  if (!form->pane->split(c,p))
    error("unrecognized command: " + c + " " + p);
}

// ---------------------------------------------------------------------
void wdstate(int event)
{
  if (evtform)
    result=evtform->state(event);
  rc=-2;
}

// ---------------------------------------------------------------------
void wdtab(string c)
{
  if (notab()) return;
  string p=cmd.getparms();
  if (c=="tabend")
    form->tab->tabend();
  else if (c=="tabnew") {
    noevents(1);
    form->tab->tabnew(p);
    noevents(0);
  } else
    error("unrecognized command: " + c + " " + p);
}

// ---------------------------------------------------------------------
void wdtimer()
{
  string p=cmd.getparms();
  int n=atoi(p.c_str());
  if (n)
    timer->start(n);
  else
    timer->stop();
}

// ---------------------------------------------------------------------
void wdxywh(int mul)
{
  if (noform()) return;
  string p=cmd.getparms();
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=4)
    error("xywh requires 2 numbers: " + p);
  else {
    form->pane->sizew=mul*c_strtoi(q2s(n.at(2)));
    form->pane->sizeh=mul*c_strtoi(q2s(n.at(3)));
  }
}

// ---------------------------------------------------------------------
void wdversion()
{
  result=APP_VERSION;
  rc=-1;
}

// ---------------------------------------------------------------------
void wdwh()
{
  if (noform()) return;
  string p=cmd.getparms();
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=2)
    error("wh requires 2 numbers: " + p);
  else {
    form->pane->sizew=c_strtoi(q2s(n.at(0)));
    form->pane->sizeh=c_strtoi(q2s(n.at(1)));
  }
}

// ---------------------------------------------------------------------
void error(string s)
{
  lasterror=ccmd+" : "+s;
  rc=1;
}

// ---------------------------------------------------------------------
bool nochild()
{
  if (cc) return false;
// TODO
  qDebug() << "no child selected " + s2q(cmdstr);
  error("no child selected");
  return true;
}

// ---------------------------------------------------------------------
bool nochildset(string id)
{
  if (noform()) return true;
  cc=form->id2child(id);
  return nochild();
}

// ---------------------------------------------------------------------
bool noform()
{
  if (form) return false;
  error("no parent selected");
  return true;
}

// ---------------------------------------------------------------------
bool notab()
{
  if (noform()) return true;
  if (form->tab) return false;
  error("no tab definition");
  return true;
}

// ---------------------------------------------------------------------
string remquotes(string s)
{
  int len=s.size();
  if (len==0) return s;
  if ((s[0]=='"' && s[len-1]=='"')||(s[0]=='\177' && s[len-1]=='\177'))
    s=s.substr(1,len-2);
  return s;
}

// ---------------------------------------------------------------------
// returns: 0=id not found
//          1=child id (cc=child)
//          2=menu id  (cc=menubar)
int setchild(string id)
{
  if (noform()) return 0;
  cc=form->id2child(id);
  if (cc) return 1;
  cc=form->setmenuid(id);
  if (cc) return 2;
// TODO
  qDebug() << "no child selected " + s2q(cmdstr);
//  nochild();
  return 0;
}
