
#include <QLayout>
#include <QTimer>

#include "wd.h"
#include "bitmap.h"
#include "child.h"
#include "cmd.h"
#include "font.h"
#include "form.h"
#include "isigraph.h"
#include "menus.h"
#include "../base/term.h"

extern "C" {
  int wd(char *s,char *&r,int &len,char *loc);
// TODO
  int wdisparent(char *s);
  void *wdgetparentid(void *s);
}

extern int jedo(char *);

void wd1();
void wdbin();
void wdcc();
void wdcn();
void wdfontdef();
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
void wdstate(int);
void wdtimer();
void wdxywh(int);
void wdwh();

void error(string s);
bool nochild();
bool nochildset(string id);
bool noform();
int setchild(string id);

Cmd cmd;
Child *cc=0;
Form *form=0;
Form *evtform=0;

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
  string q= p;
  Form *f;
  if (q[0]=='_') q[0]='-';
  void *n=(void *) strtol(q.c_str(),NULL,0);
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
  wd1();
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
    else if (c=="fontdef")
      wdfontdef();
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
    else if (c=="set")
      wdset();
    else if (c=="setp")
      wdsetp();
    else if (c=="setenable")
      wdsetenable();
    else if (c.substr(0,3)=="set")
      wdsetx(c);
    else if (c=="timer")
      wdtimer();
    else if (c=="xywh")
      wdxywh(2);
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
  form->bin(cmd.getparms());
}

// ---------------------------------------------------------------------
void wdcc()
{
  if (noform()) return;
  string c,n,p;
  n=cmd.getid();
  c=cmd.getid();
  p=cmd.getparms();
  if (form->addchild(n,c,p)) return;
  error ("child not supported: " + c);
}

// ---------------------------------------------------------------------
void wdcn()
{
  if (noform()) return;
  cc=form->child;
  if (nochild()) return;
  string p=cmd.remquotes(cmd.getparms());
  cc->setp("caption",p);
}

// ---------------------------------------------------------------------
void wdfontdef()
{
  if (noform()) return;
  string p=cmd.getparms();
  form->fontdef = new Font(p);
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
    p=cmd.remquotes(cmd.getparms());
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
  form=new Form(c,p,tlocale);
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
    if (!((n.at(0)==QString("-1"))
          ||(n.at(0)==QString("_1"))
          ||(n.at(1)==QString("-1"))
          ||(n.at(1)==QString("_1"))))
      form->move(n.at(0).toInt(),n.at(1).toInt());
    form->resize(n.at(2).toInt(),n.at(3).toInt());
#endif
  }
}

// ---------------------------------------------------------------------
void wdpn()
{
  if (noform()) return;
  string p=cmd.getparms();
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
  string q=p;
  if (q[0]=='_') q[0]='-';
  void *n=(void *) strtol(q.c_str(),NULL,0);
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
  if (s=="qm"||s=="qscreen"||s=="qcolor") {
    error("command not found");
    return;
  }
  if (noform()) return;
  if (s=="qhwndp")
    result=form->hsform();
  else if (s=="qhwndc") {
    Child *cc;
    if ((cc=form->id2child(p))) result=p2s(cc);
    else error("command failed: " + s);
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
}

// ---------------------------------------------------------------------
void wdset()
{
  string n=cmd.getid();
  string p=cmd.getparms();
  switch (setchild(n)) {
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
    form->setstretch(cc,v);
  else
    cc->setp(p,v);
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
void wdstate(int event)
{
  if (evtform)
    result=evtform->state(event);
  rc=-2;
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
    form->sizew=mul*n.at(2).toInt();
    form->sizeh=mul*n.at(3).toInt();
  }
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
    form->sizew=n.at(0).toInt();
    form->sizeh=n.at(1).toInt();
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

// ---------------------------------------------------------------------
bool noform()
{
  if (form) return false;
  error("no parent selected");
  return true;
}

