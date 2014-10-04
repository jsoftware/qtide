
#include <QDesktopWidget>
#include <QApplication>
#include <QLayout>
#include <QTimer>
#include <QSysInfo>
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#else
#include <QPrinter>
#include <QPrinterInfo>
#endif
#endif
#ifdef QT_OS_ANDROID
#include <QFontDatabase>
#endif

#include "wd.h"
#include "bitmap.h"
#include "child.h"
#include "clipboard.h"
#include "cmd.h"
#include "font.h"
#include "form.h"
#include "pane.h"
#include "isigraph.h"
#include "opengl.h"
#include "menus.h"
#include "qtstate.h"
#include "tabs.h"
#ifdef QT_OS_ANDROID
#include "../base/androidextras.h"
#endif
#ifdef QTWEBSOCKET
#include "../base/wssvr.h"
#include "../base/wscln.h"
#endif
#ifndef QT_NO_QUICKVIEW1
#include "quickview1.h"
#endif
#ifndef QT_NO_QUICKVIEW2
#include "quickview2.h"
#endif
#include "../base/term.h"
#include "../base/state.h"
#include "../base/view.h"
extern char* jegetlocale();
extern QApplication *app;
extern Term * term;
extern "C" Dllexport void dirmatch(const char *s,const char *t);
extern "C" Dllexport void openj(const char *s);

#include "math.h"

void wd1();
void wdbin();
void wdcc();
void wdclipcopy();
void wdclipcopyx();
void wdclippaste();
void wdclippastex();
void wdcmd();
void wdcn();
void wddefprint();
void wddirmatch();
void wdend();
void wdfontdef();
#ifdef QT_OS_ANDROID
void wdfontfile();
#endif
void wdget();
void *wdgetparentid(void *s);
void wdgrid();
void wdgroupbox(string c);
void wdide();
void wdimmexj();
void wdline(string);
void wdmb();
void wdmenu(string);
void wdmsgs();
void wdnb();
void wdnotyet();
void wdopenj();
void wdp(string c);
void wdpactive();
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
void wdptimer();
void wdptop();
void wdq();
void wdqtstate(string p);
void wdqueries(string);
#ifndef QT_NO_QUICKVIEW1
void wdquickview1();
QuickView1 *quickview1;
#endif
#ifndef QT_NO_QUICKVIEW2
void wdquickview2();
QuickView2 *quickview2;
#endif
void wdrem();
void wdreset();
void wdset();
void wdsetx(string);
void wdset1(string n,string p,string v);
void wdsm(string);
void wdsplit(string c);
void wdstate(Form *,int);
void wdtab(string);
void wdtextview();
void wdtimer();
void wdversion();
void wdwh();
#ifdef QTWEBSOCKET
void wdws();
extern string ws(string p);
#endif

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

int FormSeq=0;
int rc;
string lasterror="";
string result="";

string cmdstr;
string ccmd;

// ---------------------------------------------------------------------
int wd(char *s,int slen,char *&res,int &len)
{
  rc=0;
  result.clear();
  cmd.init(s,slen);
  wd1();
  len=(int)result.size();
  res=(char *)result.c_str();
  int r=rc;
  rc=0;
  return r;
}

// ---------------------------------------------------------------------
// subroutines may set int rc and wd1 returns if non-zero
void wd1()
{
  string c,p;
  while ((rc==0) && cmd.more()) {
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
    else if (c=="clipcopy")
      wdclipcopy();
    else if (c=="clipcopyx")
      wdclipcopyx();
    else if (c=="clippaste")
      wdclippaste();
    else if (c=="clippastex")
      wdclippastex();
    else if (c=="cmd")
      wdcmd();
    else if (c=="cn")
      wdcn();
    else if (c=="defprint")
      wddefprint();
    else if (c=="dirmatch")
      wddirmatch();
    else if (c=="end")
      wdend();
    else if (c=="fontdef")
      wdfontdef();
#ifdef QT_OS_ANDROID
    else if (c=="fontfile")
      wdfontfile();
#endif
    else if (c=="get")
      wdget();
    else if (c=="grid")
      wdgrid();
    else if (c.substr(0,8)=="groupbox")
      wdgroupbox(c);
    else if (c=="ide")
      wdide();
    else if (c=="immexj")
      wdimmexj();
    else if (c.substr(0,4)=="line")
      wdline(c);
    else if (c.substr(0,2)=="mb")
      wdmb();
    else if (c.substr(0,4)=="menu")
      wdmenu(c);
    else if (c=="msgs")
      wdmsgs();
    else if (c=="openj")
      wdopenj();
    else if (c=="NB.")
      wdnb();
    else if (c[0]=='p')
      wdp(c);
#ifndef QT_NO_QUICKVIEW1
    else if (c=="quickview1")
      wdquickview1();
#endif
#ifndef QT_NO_QUICKVIEW2
    else if (c=="quickview2")
      wdquickview2();
#endif
    else if (c[0]=='q')
      wdqueries(c);
    else if (c=="rem")
      wdrem();
    else if (c=="reset")
      wdreset();
    else if (c=="set")
      wdset();
    else if (c.substr(0,3)=="set")
      wdsetx(c);
    else if (c.substr(0,2)=="sm")
      wdsm(c);
    else if (c.substr(0,5)=="split")
      wdsplit(c);
    else if (c.substr(0,3)=="tab")
      wdtab(c);
    else if (c=="textview")
      wdtextview();
    else if (c=="timer")
      wdtimer();
    else if (c=="version")
      wdversion();
    else if (c=="minwh")
      wdwh();
#ifdef QTWEBSOCKET
    else if (c=="ws")
      wdws();
#endif
    else if (0) {
      cmd.getparms();
      wdnotyet();
    } else
      error("command not found");
  }
}

// ---------------------------------------------------------------------
void wdactivateform()
{
  if (form) {
    form->setVisible(true);
    form->activateWindow();
    form->raise();
    form->repaint();
  } else if (0==Forms.size()) {
    if (!term) return;
    showide(true);
    if (ShowIde) {
      term->activateWindow();
      term->raise();
      term->repaint();
    }
  }
}

// ---------------------------------------------------------------------
void wdbin()
{
  string p=cmd.getparms();
  if (noform()) return;
  form->pane->bin(p);
}

// ---------------------------------------------------------------------
void wdcc()
{
  if (noform()) {
    cmd.getparms();
    return;
  }
  string c,n,p;
  n=cmd.getid();
  c=cmd.getid();
  p=cmd.getparms();
  if (form->pane->addchild(n,c,p)) return;
  error ("child not supported: " + c);
}

// ---------------------------------------------------------------------
void wdclipcopy()
{
  string p=cmd.getparms();
  if (!app) {
    error("command failed: no QApplication");
    return;
  }
  wdclipwrite((char *)p.c_str());
}

// ---------------------------------------------------------------------
void wdclipcopyx()
{
  string n=cmd.getid();
  string p=cmd.getparms();
  if (!app) {
    error("command failed: no QApplication");
    return;
  }
  if ("image"==n) {
    if (wdclipwriteimage((char *)p.c_str()))
      error("clipboard or filename error: " + n + " " + p);
  } else
    error("clipboard format not supported: " + n);
}

// ---------------------------------------------------------------------
void wdclippaste()
{
  string p=cmd.getparms();
  if (!app) {
    error("command failed: no QApplication");
    return;
  }
  int len=-1;
  char *m;
  if ((m=(char *)wdclipread(&len))) {
    rc=-1;
    result= std::string(m,len);
  } else if ("1"==p)
    error("clipboard is empty");
  else {
    rc=-1;
    result= "";
  }
}

// ---------------------------------------------------------------------
void wdclippastex()
{
  string n=cmd.getid();
  string p=cmd.getparms();
  if (!app) {
    error("command failed: no QApplication");
    return;
  }
  char *m;
  if ("image"==n) {
    if (!(m=(char *)wdclipreadimage((char *)p.c_str())))
      error("clipboard or filename error: " + n + " " + p);
  } else
    error("clipboard format not supported: " + n);
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
  else
    error("bad child id");
}

// ---------------------------------------------------------------------
void wdcn()
{
  string p=remquotes(cmd.getparms());
  if (noform()) return;
  cc=form->child;
  if (nochild()) return;
  cc->set("caption",p);
}

// ---------------------------------------------------------------------
void wddefprint()
{
#ifndef QT_NO_PRINTER
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
  } else if (c=="printer") {
    config.Printer->setPrinterName(QString::fromStdString(p));
    if (! config.Printer->isValid()) error("printer name invalid: " + p);
  } else if (c=="font") {
    QStringList f=qsplit(p);
    if (f.size()<2) {
      error("font requires family name and point size");
      return;
    }
    int ps=c_strtoi(q2s(f.at(1)));
    config.Font.setFamily(f.at(0));
    config.Font.setPointSize(ps);
    if (f.size()>2) {
      if (f.contains("italic")) config.Font.setItalic(1);
      if (f.contains("bold")) config.Font.setWeight (QFont::Bold);
    }
  } else error("invalid option: " + c);
#else
  cmd.getparms();
#endif
}

// ---------------------------------------------------------------------
void wddirmatch()
{
  string p=cmd.getparms();
  QStringList f=qsplit(p);
  if (f.size()!=2) {
    error("dirmatch requires 2 directories");
    return;
  }
  dirmatch(q2s(f.at(0)).c_str(),q2s(f.at(1)).c_str());
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

#ifdef QT_OS_ANDROID
// ---------------------------------------------------------------------
void wdfontfile()
{
  string p=cmd.getparms();
  int id=QFontDatabase::addApplicationFont(s2q(p));
  result=i2s(id);
  rc=-1;
}
#endif

// ---------------------------------------------------------------------
void wdget()
{
  string n=cmd.getid();
  string p=cmd.getid();
  string v=cmd.getparms();
  int type=setchild(n);
  if (type)
    result=cc->get(p,v);
  else
    error("bad child id");
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
void wdgrid()
{
  if (noform()) {
    cmd.getparms();
    return;
  }
  string n=cmd.getid();
  string v=cmd.getparms();
  form->pane->grid(n,v);
}

// ---------------------------------------------------------------------
void wdgroupbox(string c)
{
  string p=cmd.getparms();
  if (noform()) return;
  if (!form->pane->groupbox(c,p))
    error("unrecognized command: " + c + " " + p);
}

// ---------------------------------------------------------------------
void wdide()
{
  string p=cmd.getparms();
  if (!jt) {
    error("command failed: no interpreter");
    return;
  }
  if (p=="hide")
    showide(false);
  else if (p=="show")
    showide(true);
  else
    error("unrecognized command: ide " + p);
}

// ---------------------------------------------------------------------
void wdimmexj()
{
  string p=cmd.getparms();
  immexj(p.c_str());
}

// ---------------------------------------------------------------------
void wdline(string c)
{
  string p=cmd.getparms();
  if (noform()) return;
  if (!form->pane->line(c,p))
    error("unrecognized command: " + c + " " + p);
}

// ---------------------------------------------------------------------
void wdmb()
{
  string c=cmd.getid();
  string p=cmd.getparms(true);
  if (!QApplication::focusWidget()) {
    error("command failed: no QApplication::focusWidget()");
    return;
  }
  result=q2s(mb(c,p));
  if (1==rc)
    result="";
  else
    rc=-1;
}

// ---------------------------------------------------------------------
void wdmenu(string s)
{
  int rc=0;
  if (noform()) {
    cmd.getparms();
    return;
  }
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
  if (!app) {
    error("command failed: no QApplication");
    return;
  }
  QApplication::processEvents(QEventLoop::AllEvents);
}

// ---------------------------------------------------------------------
void wdnb()
{
  cmd.getline();
}

// ---------------------------------------------------------------------
// not yet
void wdnotyet()
{
  cmd.getparms();
}

// ---------------------------------------------------------------------
void wdopenj()
{
  string p=cmd.getparms();
  openj(p.c_str());
}

// ---------------------------------------------------------------------
void wdp(string c)
{
  if (c=="pactive")
    wdpactive();
  else if (c=="pas")
    wdpas();
  else if (c=="pc")
    wdpc();
  else if (c=="pclose")
    wdpclose();
  else if (c=="pcenter")
    wdpcenter();
  else if (c=="picon")
    wdpicon();
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
  else if (c=="ptimer")
    wdptimer();
  else if (c=="ptop")
    wdptop();
  else if (c=="notyet") {
    cmd.getparms();
    wdnotyet();
  } else
    error("parent command not found: " + c);
}

// ---------------------------------------------------------------------
void wdpactive()
{
  cmd.getparms();
  if (noform()) return;
#ifdef QT_OS_ANDROID
  if(form!=Forms.last()) return;
#endif
  form->activateWindow();
  form->raise();
}

// ---------------------------------------------------------------------
void wdpas()
{
  string p=cmd.getparms();
  if (noform()) return;
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
  if (!jt) {
    error("command failed: no interpreter");
    return;
  }
  string tlocale=jegetlocale();
  c=cmd.getid();
  p=cmd.getparms();
// QWidget must be parentless to be top-level window
  QStringList m=s2q(p).split(' ',QString::SkipEmptyParts);
  form=new Form(c,p,tlocale,m.contains("owner")?form:0);
  evtform=form;
  Forms.append(form);
}

// ---------------------------------------------------------------------
void wdpcenter()
{
  cmd.getparms();
  if (noform()) return;
#ifndef QT_OS_ANDROID
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
  cmd.getparms();
  if (noform()) return;
  if (form->closed) return;
  form->closed=true;
  form->close();
}

// ---------------------------------------------------------------------
void wdpicon()
{
  string p=remquotes(cmd.getparms());
  if (noform()) return;
  form->setpicon(p);
}

// ---------------------------------------------------------------------
void wdpmove()
{
  string p=cmd.getparms();
  if (noform()) return;
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (n.size()!=4)
    error("pmove requires 4 numbers: " + p);
  else {
#ifndef QT_OS_ANDROID
    if (c_strtoi(q2s(n.at(0)))!=-1 && c_strtoi(q2s(n.at(1)))!=-1)
      form->move(c_strtoi(q2s(n.at(0))),c_strtoi(q2s(n.at(1))));
#endif
    if (c_strtoi(q2s(n.at(2)))!=-1 && c_strtoi(q2s(n.at(3)))!=-1)
      form->resize(c_strtoi(q2s(n.at(2))),c_strtoi(q2s(n.at(3))));
  }
}

// ---------------------------------------------------------------------
void wdpn()
{
  string p=remquotes(cmd.getparms());
  if (noform()) return;
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
    if ((!f->closed) && (n==f || p==f->id)) {
      form=f;
      return;
    }
  }
  error("command failed: psel");
}

// ---------------------------------------------------------------------
void wdpshow()
{
  cmd.getparms();
  if (noform()) return;
  form->showit();
}

// ---------------------------------------------------------------------
void wdpstylesheet()
{
  string p=cmd.getparms();
  if (noform()) return;
  form->setStyleSheet(s2q(p));
}

// ---------------------------------------------------------------------
void wdptimer()
{
  string p=cmd.getparms();
  if (noform()) return;
  form->settimer(p);
}

// ---------------------------------------------------------------------
void wdptop()
{
  string p=cmd.getparms();
  if (noform()) return;
#ifdef QT_OS_ANDROID
  if(form!=Forms.last()) return;
#endif
  Qt::WindowFlags f=form->windowFlags();
  if (p=="1")
    f|=Qt::WindowStaysOnTopHint;
  else
    f=f&~Qt::WindowStaysOnTopHint;
  form->setWindowFlags(f);
  form->show();
}

// ---------------------------------------------------------------------
void wdq()
{
  cmd.getparms();
  wdstate(evtform,1);
}

// ---------------------------------------------------------------------
void wdqtstate(string p)
{
  rc=-2;
  result=qtstate(p);
}

// ---------------------------------------------------------------------
void wdqueries(string s)
{
  string p=cmd.getparms();

  if (s=="qd") {
    wdstate(form,0);
    return;
  }

  if (s=="qtstate") {
    wdqtstate(p);
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
    if (!app) {
      error("command failed: no QApplication");
      return;
    }
    QDesktopWidget* dw=QApplication::desktop();
#ifdef QT_OS_ANDROID
    android_getdisplaymetrics(0);
    int dpix=DM_xdpi;
    int dpiy=DM_ydpi;
    int w=DM_widthPixels;
    int h=DM_heightPixels;
#else
    QRect screenGeometry = dw->screenGeometry(-1);
    int dpix=dw->logicalDpiX();
    int dpiy=dw->logicalDpiY();
    int w=screenGeometry.width();
    int h=screenGeometry.height();
#endif
    int mmx=25.4*w/dpix;
    int mmy=25.4*h/dpiy;
    int dia=sqrt((float)dpix*dpix+dpiy*dpiy);
    result=i2s(mmx) + " " + i2s(mmy) + " " + i2s(w) + " " + i2s(h) + " " + i2s(dpix) + " " + i2s(dpiy) + " " + i2s(dw->depth()) + " 1 " + i2s(dw->colorCount()) + " " + i2s(dpix) + " " + i2s(dpiy) + " " + i2s(dia);
    return;
  } else if (s=="qwd") {
    result="jqt";
    return;
  } else if (s=="qosver") {
#ifdef _WIN32
    QSysInfo qsi;
    Q_UNUSED(qsi);
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
    if (qsi.MacintoshVersion > QSysInfo::MV_10_8) {
      result="10.9";
      return;
    } else if (qsi.MacintoshVersion == QSysInfo::MV_10_8) {
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
    if (!app) {
      error("command failed: no QApplication");
      return;
    }
    string q="";
#ifndef QT_NO_PRINTER
    QPrinterInfo pd=QPrinterInfo::defaultPrinter();
    if ((!config.Printer) || !config.Printer->isValid()) q = string("\012");
    else q = q2s(config.Printer->printerName()) + "\012";
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
      for (int i=0; i<Forms.size(); i++) {
        Form *f=Forms.at(i);
        q = q + f->id + "\t" + p2s((void *)f) + "\t" + f->locale + "\t\t" + i2s(f->seq) + "\t\012";
      }
      result=q;
    }
    return;
  } else if (s=="qfile") {
    bool done=false;
    QFile f(s2q(remquotes(p)));
    QFileInfo info=QFileInfo(f);
    if (info.exists() && info.isFile() && info.size()>0) {
      qint64 ssize=info.size();
      if(f.open(QFile::ReadOnly)) {
        char * sdata=(char *)malloc(ssize);
        QDataStream in(&f);
        if (ssize==in.readRawData(sdata,ssize)) {
          result=std::string(sdata,ssize);
          done=true;
        }
        f.close();
        free(sdata);
      }
    }
    if (!done) result="";
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
    if ((cc=form->id2child(p)) && cc->widget) {
      QWidget *p0, *p1;
      p0=p1=cc->widget;
      while (p1) {
        p0=p1;
        p1=p0->parentWidget();
      }
      QPoint pos=cc->widget->mapTo(p0,cc->widget->pos());
      QSize size=cc->widget->size();
      result=i2s(pos.x())+" "+i2s(pos.y())+" "+i2s(size.width())+" "+i2s(size.height());
    } else error("command failed: " + s);
  } else if (s=="qpid") {
    SI m, n = c_strtoi(p);
    if ((m=(SI)wdgetparentid((void *)n))) {
      result= p2s((void *)m);
    } else {
      result= "0";
    }
  } else
    error("command not found");
}

#ifndef QT_NO_QUICKVIEW1
// ---------------------------------------------------------------------
void wdquickview1()
{
  string p=cmd.getparms();
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (!jt) {
    error("command failed: no interpreter");
    return;
  }
  int mode=1;
  if (n.size()==0) {
    if (quickview1) {
      quickview1->close();
      quickview1=0;
    }
  } else if (n.size()<2)
    error("quickview1 requires at least 2 parameters: " + p);
  else {
    string t=remquotes(q2s(n.at(0)));
    string f=remquotes(q2s(n.at(1)));
    if (!QFile(s2q(f)).exists()) {
      error("quickview1 file error: " + p);
      return;
    }
    if (n.size()>2 && (n.at(2)=="0"||n.at(2)=="1")) mode=!!c_strtoi(q2s(n.at(2)));
    if (quickview1) quickview1->close();
    quickview1=new QuickView1(t,f,mode);
#ifdef QT_OS_ANDROID
    quickview1->showFullScreen();
#else
    quickview1->show();
#endif
    quickview1->raise();
#ifdef QT_OS_ANDROID
    showide(false);
    if (Forms.size()>0)
      (Forms.at(Forms.size()-1))->setVisible(false);
#endif
  }
}

#endif
#ifndef QT_NO_QUICKVIEW2
// ---------------------------------------------------------------------
void wdquickview2()
{
  string p=cmd.getparms();
  QStringList n=s2q(p).split(" ",QString::SkipEmptyParts);
  if (!jt) {
    error("command failed: no interpreter");
    return;
  }
  int mode=1;
  if (n.size()==0) {
    if (quickview2) {
      quickview2->close();
      quickview2=0;
    }
  } else if (n.size()<2)
    error("quickview2 requires at least 2 parameters: " + p);
  else {
    string t=remquotes(q2s(n.at(0)));
    string f=remquotes(q2s(n.at(1)));
    if (!QFile(s2q(f)).exists()) {
      error("quickview2 file error: " + p);
      return;
    }
    string glver="";
    if (n.size()>2) {
      if (n.at(2)=="0"||n.at(2)=="1") mode=!!c_strtoi(q2s(n.at(2)));
      int l=n.indexOf("version");
      if ((l!=-1) && (l<n.size()-1) && 0!=n.at(l+1).toDouble()) {
        glver=q2s(n.at(l+1));
      }
    }
    if (quickview2) quickview2->close();
    quickview2=new QuickView2(t,f,mode,glver);
#ifdef QT_OS_ANDROID
    quickview2->showFullScreen();
#else
    quickview2->show();
#endif
    quickview2->raise();
#ifdef QT50
    quickview2->requestActivate();
#endif
#ifdef QT_OS_ANDROID
    showide(false);
    if (Forms.size()>0)
      (Forms.at(Forms.size()-1))->setVisible(false);
#endif
  }
}
#endif

// ---------------------------------------------------------------------
void wdrem()
{
  cmd.getparms();
}

// ---------------------------------------------------------------------
void wdreset()
{
  cmd.getparms();
  if (timer) timer->stop();
#ifndef QT_NO_QUICKVIEW1
  if (quickview1) {
    quickview1->close();
    quickview1=0;
  }
#endif
#ifndef QT_NO_QUICKVIEW2
  if (quickview2) {
    quickview2->close();
    quickview2=0;
  }
#endif

  foreach (Form *f,Forms) {
    f->closed=true;
    f->close();
  }
  Forms.clear();
  form=0;
  evtform=0;
  fontdef=0;
  if (FontExtent) {
    delete FontExtent;
    FontExtent=0;
  }
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
  default :
    error("bad child id");
  }
  noevents(0);
}

// ---------------------------------------------------------------------
void wdsm(string s)
{
  string c,p;
  if (s=="sm")
    c=cmd.getid();
  else if (s=="smact")
    c="act";
  else
    c=s;
  p=cmd.getparms();
  result=sm(c,p);
  if (rc==1)
    error(result);
}

// ---------------------------------------------------------------------
void wdsplit(string c)
{
  string p=cmd.getparms();
  if (noform()) return;
  if (!form->pane->split(c,p))
    error("unrecognized command: " + c + " " + p);
}

// ---------------------------------------------------------------------
void wdstate(Form * f,int event)
{
  if (f)
    result=f->state(event);
  rc=-2;
}

// ---------------------------------------------------------------------
void wdtab(string c)
{
  string p=cmd.getparms();
  if (notab()) return;
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
void wdtextview()
{
  QString p, title,header,text;
  QChar d;
  int n;
  p=boxj2utf8(cmd.getparms());
  if (p.isEmpty()) return;
  d=p[0];
  p=p.mid(1);
  n=p.indexOf(d);
  title=p.left(n);
  p=p.mid(n+1);
  n=p.indexOf(d);
  header=p.left(n);
  text=p.mid(n+1);
  textview(title,header,text);
}

// ---------------------------------------------------------------------
void wdtimer()
{
  string p=cmd.getparms();
  int n=atoi(p.c_str());
  if (!jt) {
    error("command failed: no interpreter");
    return;
  }
  if (n)
    timer->start(n);
  else
    timer->stop();
}

#ifdef QTWEBSOCKET
// ---------------------------------------------------------------------
void wdws()
{
  result=ws(cmd.getparms());
  if (!jt) {
    error("command failed: no interpreter");
    return;
  }
  if (1==rc)
    result="";
  else
    rc=-1;
}
#endif

// ---------------------------------------------------------------------
void wdversion()
{
  cmd.getparms();
  result=APP_VERSION;
#ifdef QT_NO_WEBKIT
  result=result+"s";
#endif
  result=result+"/"+qVersion();
  rc=-1;
}

// ---------------------------------------------------------------------
void wdwh()
{
  string p=cmd.getparms();
  if (noform()) return;
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
  qDebug() << "no child selected (nochild) " + s2q(cmdstr);
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
  int len=(int)s.size();
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
  qDebug() << "no child selected (setchild) " + s2q(cmdstr);
//  nochild();
  return 0;
}
