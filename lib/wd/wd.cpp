
#include <QDesktopWidget>
#include <QApplication>
#include <QLayout>
#include <QTimer>
#include <QSysInfo>
#include <QPrinter>

#ifndef ANDROID
#include <QPrinterInfo>
extern QPrinter *Printer;
#endif

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
#include "../base/state.h"

#if defined(ANDROID) && defined(QT_OPENGL)
extern Term *term;
#endif

#include "math.h"

extern "C" {
  Dllexport int wd(char *s,char *&r,int &len,char *loc);

// TODO
  Dllexport int wdisparent(char *s);
  Dllexport void *wdgetparentid(void *s);
}

void wd1();
void wdbin();
void wdcc();
void wdcmd();
void wdcn();
void wddefprint();
void wdend();
void wdfontdef();
void wdget();
void wdgroupbox(string c);
void wdide();
void wdline(string);
void wdmb();
void wdmenu(string);
void wdmsgs();
void wdnotyet();
void wdpactive();
void wdp(string c);
void wdpas();
void wdpc();
void wdpcenter();
void wdpclose();
void wdpicon();
void wdpmove();
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
void wdsetx(string);
void wdset1(string n,string p,string v);
void wdsplit(string c);
void wdstate(int);
void wdtab(string);
void wdtimer();
void wdversion();
void wdwh();

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
    else if (c=="cmd")
      wdcmd();
    else if (c=="cn")
      wdcn();
    else if (c=="defprint")
      wddefprint();
    else if (c=="end")
      wdend();
    else if (c=="fontdef")
      wdfontdef();
    else if (c=="get")
      wdget();
    else if (c.substr(0,8)=="groupbox")
      wdgroupbox(c);
    else if (c=="ide")
      wdide();
    else if (c.substr(0,4)=="line")
      wdline(c);
    else if (c.substr(0,2)=="mb")
      wdmb();
    else if (c.substr(0,4)=="menu")
      wdmenu(c);
    else if (c=="msgs")
      wdmsgs();
    else if (c[0]=='p')
      wdp(c);
    else if (c[0]=='q')
      wdqueries(c);
    else if (c=="rem")
      wdrem();
    else if (c=="reset")
      wdreset();
    // nonce leave in set
    else if (c=="set")
      wdset();
    else if (c.substr(0,3)=="set")
      wdsetx(c);
    else if (c.substr(0,5)=="split")
      wdsplit(c);
    else if (c.substr(0,3)=="tab")
      wdtab(c);
    else if (c=="timer")
      wdtimer();
    else if (c=="version")
      wdversion();
    else if (c=="wh")
      wdwh();
// not yet implemented
    else if (0) {
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
void wdcmd()
{
  string n=cmd.getid();
  string p=cmd.getid();
  string v=cmd.getparms();
  int type=setchild(n);
  if (type)
    cc->cmd(p,v);
}

// ---------------------------------------------------------------------
void wdcn()
{
  if (noform()) return;
  cc=form->child;
  if (nochild()) return;
  string p=remquotes(cmd.getparms());
  cc->set("caption",p);
}

// ---------------------------------------------------------------------
void wddefprint()
{
#ifndef ANDROID
  string c=cmd.getid();
  string p=cmd.getparms();
  if (c=="orient") {
    if (p=="landscape")
      config.Printer->setOrientation(QPrinter::Landscape);
    else
      config.Printer->setOrientation(QPrinter::Portrait);
  } else if (c=="margin") {
    QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
    if (n.size()==1) {
      qreal m=c_strtod(q2s(n.at(0)));
      config.Printer->setPageMargins(m, m, m, m, QPrinter::Millimeter);
    } else if (n.size()==2) {
      qreal l=c_strtod(q2s(n.at(0)));
      qreal t=c_strtod(q2s(n.at(1)));
      config.Printer->setPageMargins(l, t, l, t, QPrinter::Millimeter);
    } else if (n.size()==4) {
      qreal l=c_strtod(q2s(n.at(0)));
      qreal t=c_strtod(q2s(n.at(1)));
      qreal r=c_strtod(q2s(n.at(2)));
      qreal b=c_strtod(q2s(n.at(3)));
      config.Printer->setPageMargins(l, t, r, b, QPrinter::Millimeter);
    } else error("margin requires 1, 2, or 4 numbers: " + p);
  } else error("invalid option: " + c);
#endif
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
void wdget()
{
  string n=cmd.getid();
  string p=cmd.getid();
  string v=cmd.getparms();
  int type=setchild(n);
  if (type)
    cc->get(p,v);
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
void wdide()
{
  string p=cmd.getparms();
  if (p=="hide")
    showide(false);
  else if (p=="show")
    showide(true);
  else
    error("unrecognized command: ide " + p);
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
  if (1==rc)
    result="";
  else
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
void wdmsgs()
{
  cmd.getparms();
  QApplication::processEvents(QEventLoop::AllEvents);
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
  if (c=="pas")
    wdpas();
  else if (c=="pc")
    wdpc();
  else if (c=="pclose")
    wdpclose();
  else if (c=="pcenter")
    wdpcenter();
  else if (c=="pmove")
    wdpmove();
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
  else if (c=="picon")
    wdpicon();
  else if (c=="notyet") {
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
void wdpas()
{
  if (noform()) return;
  string p=cmd.getparms();
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  int l,t,r,b;
  if (n.size()==2) {
    l=r=c_strtoi(q2s(n.at(0)));
    t=b=c_strtoi(q2s(n.at(1)));
    form->setpadding(l,t,r,b);
  } else if (n.size()==4) {
    l=c_strtoi(q2s(n.at(0)));
    t=c_strtoi(q2s(n.at(1)));
    r=c_strtoi(q2s(n.at(2)));
    b=c_strtoi(q2s(n.at(3)));
    form->setpadding(l,t,r,b);
  } else error("pas requires 2 or 4 numbers: " + p);
}

// ---------------------------------------------------------------------
void wdpc()
{
  string c,p;
  c=cmd.getid();
  p=cmd.getparms();
// QWidget must be parentless to be top-level window
  QStringList m=s2q(p).split(' ',QString::SkipEmptyParts);
#if defined(ANDROID) && defined(QT_OPENGL)
  if (!form) {
    showide(false);
    form=new Form(c,p,tlocale,term);
  } else form=new Form(c,p,tlocale,form);
#else
  form=new Form(c,p,tlocale,m.contains("owner")?form:0);
#endif
  evtform=form;
  Forms.append(form);
}

// ---------------------------------------------------------------------
void wdpcenter()
{
  if (noform()) return;
  cmd.getparms();
#ifndef ANDROID
  QDesktopWidget* dw=QApplication::desktop();
  QRect screenGeometry = dw->screenGeometry(-1);
  int sw=screenGeometry.width();
  int sh=screenGeometry.height();
  int w=form->size().width();
  int h=form->size().height();
  int x=(sw-w)/2;
  int y=(sh-h)/2;
  form->move((x<0)?0:x,(y<0)?0:y);
#endif
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
void wdpicon()
{
  if (noform()) return;
  string p=remquotes(cmd.getparms());
  form->setpicon(p);
}

// ---------------------------------------------------------------------
void wdpmove()
{
  if (noform()) return;
  string p=cmd.getparms();
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=4)
    error("pmove requires 4 numbers: " + p);
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
    QDesktopWidget* dw=QApplication::desktop();
    QRect screenGeometry = dw->screenGeometry(-1);
    int dpix=dw->logicalDpiX();
    int dpiy=dw->logicalDpiY();
    int w=screenGeometry.width();
    int h=screenGeometry.height();
    int mmx=25.4*w/dpix;
    int mmy=25.4*h/dpiy;
    int dia=sqrt((float)dpix*dpix+dpiy*dpiy);
    result=i2s(mmx) + " " + i2s(mmy) + " " + i2s(w) + " " + i2s(h) + " " + i2s(dpix) + " " + i2s(dpiy) + " " + i2s(dw->depth()) + " 1 " + i2s(dw->colorCount()) + " " + i2s(dpix) + " " + i2s(dpix) + " " + i2s(dia);
    return;
  } else if (s=="qwd") {
    result="jqt";
    return;
  } else if (s=="qosver") {
#ifdef _WIN32
    QSysInfo qsi;
    if (qsi.WindowsVersion == QSysInfo::WV_WINDOWS8)  {
      result="6.2";
      return;
    } else if (qsi.WindowsVersion == QSysInfo::WV_WINDOWS7) {
      result="6.1";
      return;
    } else if (qsi.WindowsVersion == QSysInfo::WV_VISTA) {
      result="6.0";
      return;
    } else if (qsi.WindowsVersion == QSysInfo::WV_2003) {
      result="5.2";
      return;
    } else if (qsi.WindowsVersion == QSysInfo::WV_XP) {
      result="5.1";
      return;
    } else {
      result="5.0";
      return;
    }
#else
#ifdef  __MACH__
    QSysInfo qsi;
    if (qsi.MacintoshVersion == QSysInfo::MV_10_8) {
      result="10.8";
      return;
    } else if (qsi.MacintoshVersion == QSysInfo::MV_10_7) {
      result="10.7";
      return;
    } else if (qsi.MacintoshVersion == QSysInfo::MV_10_6) {
      result="10.6";
      return;
    } else if (qsi.MacintoshVersion == QSysInfo::MV_10_5) {
      result="10.5";
      return;
    }
#else
    result="";
    return;
#endif
#endif
  } else if (s=="qprinters") {
    string q="";
#ifndef ANDROID
    QPrinterInfo pd=QPrinterInfo::defaultPrinter();
    if ((!Printer) || !Printer->isValid()) q = string("\012");
    else q = q2s(Printer->printerName()) + "\012";
    if (pd.isNull()) q = q + string("\012");
    else q = q + q2s(pd.printerName()) + "\012";
    QList<QPrinterInfo> pl=QPrinterInfo::availablePrinters();
    if (pl.size()) {
      for (int i=0; i<pl.size(); i++)
        q = q + q2s(pl.at(i).printerName()) + "\012";
    }
#endif
    result=q;
    return;
  } else if (s=="qpx") {
    if (!Forms.size()) result="";
    else {
      string q;
      for (int i=0; i<Forms.size(); i++)
        q = q + Forms.at(i)->id + "\t"  + p2s((void *)Forms.at(i)) + "\t"  + Forms.at(i)->locale + "\t\012";
      result=q;
    }
    return;
  }
// queries that form is needed
  if (noform()) return;
  if (s=="qhwndp")
    result=form->hsform();
  else if (s=="qform")
    result=form->qform();
  else if (s=="qhwndc") {
    Child *cc;
    if ((cc=form->id2child(p))) result=p2s(cc);
    else
      error("command failed: " + s);
  } else if (s=="qchildxywh") {
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
  string p=cmd.getid();
  string v=cmd.getparms();
  wdset1(n,p,v);
}

// ---------------------------------------------------------------------
void wdsetx(string c)
{
  string n=cmd.getid();
  string p=c.substr(3);
  string v=cmd.getparms();
  wdset1(n,p,v);
}

// ---------------------------------------------------------------------
void wdset1(string n,string p,string v)
{
  noevents(1);
  int type=setchild(n);
  switch (type) {
  case 1 :
    if (p=="stretch")
      form->pane->setstretch(cc,v);
    else {
      if (v.size()==0 && isint(p)) {
        v=p;
        p="value";
      }
      cc->set(p,v);
    }
    break;
  case 2 :
    cc->set(n+" "+p,v);
    break;
  }
  noevents(0);
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

// ---------------------------------------------------------------------1
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
