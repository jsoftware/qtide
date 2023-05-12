
#if defined(QT57)
#include <QScreen>
#else
#include <QDesktopWidget>
#endif
#include <QApplication>
#include <QLayout>
#include <QTimer>
#include <QSysInfo>
#include <vector>
#ifndef QT_NO_PRINTER
#ifdef QT50
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>
#else
#include <QPrinter>
#include <QPrinterInfo>
#endif
#endif
#ifndef QT_NO_OPENGL
#ifdef QT50
#include <QOpenGLContext>
#endif
#endif

#include "bitmap.h"
#include "child.h"
#include "clipboard.h"
#include "cmd.h"
#include "font.h"
#include "form.h"
#include "menus.h"
#include "pane.h"
#include "tabs.h"
#include "wd.h"
#include "gl2.h"
#include "glz.h"
#include "ogl2.h"

#ifdef QTWEBSOCKET
#include "../base/wssvr.h"
#include "../base/wscln.h"
#else
#include "../base/jsvr.h"
#endif
#ifndef QT_NO_QUICKVIEW1
#include "quickview1.h"
#endif
#ifndef QT_NO_QUICKVIEW2
#include "quickview2.h"
#endif
#include "../base/eview.h"
#include "../base/qtstate.h"
#include "../base/state.h"
#include "../base/svr.h"
#include "../base/tedit.h"
#include "../base/term.h"
extern char* jegetlocale();
extern Term * term;
extern "C" Dllexport void dirmatch(const char *s,const char *t);
extern "C" Dllexport void openj(const char *s);
// extern std::string wdQuery;

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#include "math.h"
#if defined(_MSC_VER) && !defined(__clang__)
#undef INT
#define INT 4
#endif

static void wd1();
static void wdbeep();
static void wdbin();
static void wdcc();
static void wdclipcopy();
static void wdclipcopyx();
static void wdclippaste();
static void wdclippastex();
static void wdcmd();
static void wdcn();
static void wddefprint();
static void wddirmatch();
static void wdend();
static void wdfontdef();
static void wdget();
static void wdgetp();
static void wdgrid();
static void wdgroupbox(std::string c);
static void wdide();
static void wdimmexj();
static void wdline(std::string);
static void wdmb();
static void wdmenu(std::string);
static void wdmsgs();
static void wdnb();
static void wdnotyet();
static void wdopenj();
static void wdp(std::string c);
static void wdpactive();
static void wdparse();
static void wdpas();
static void wdpc();
static void wdpcenter();
static void wdpclose();
static void wdpicon();
static void wdpmove();
static void wdpmoves();
static void wdpmove1(std::string);
static void wdpn();
static void wdpsel();
static void wdpshow();
static void wdpstylesheet();
static void  wdptheme();
static void wdptimer();
static void wdptop();
static void wdq();
static void wdqtstate(std::string);
static void wdqueries(std::string);
#ifndef QT_NO_QUICKVIEW1
static void wdquickview1();
static QuickView1 *quickview1;
#endif
#ifndef QT_NO_QUICKVIEW2
static void wdquickview2();
static QuickView2 *quickview2;
#endif
static void wdrem();
void wdreset();
static void wdset();
static void wdsetp();
static void wdsetx(std::string);
static void wdset1(std::string n,std::string p,std::string v);
static void wdsm(std::string);
static void wdsplit(std::string c);
static void wdstate(Form *,int);
static void wdtab(std::string);
static void wdtextview();
static void wdtimer();
static void wdverbose();
static void wdversion();
static void wdmaxwh();
static void wdminwh();
#ifdef QTWEBSOCKET
static void wdws();
extern std::string ws(std::string p);
#endif

static bool nochild();
static bool noform();
static bool notab();
static int setchild(std::string id);
static std::string formchildid();

Cmd cmd;
static Child *cc=0;
Form *form=0;
Form *evtform=0;
Font *fontdef=0;
Child *isigraph=0;
Child *opengl=0;
std::string locale="";

QList<Form *>Forms;

int FormSeq=0;
int rc;
std::string lasterror="";
std::string result="";
int resulttype=0;   // 0 LIT   1 int   2 I
std::vector<int> intresult= {};
std::vector<I> longresult= {};
int resultshape[2];

static std::string ccmd="";

static int verbose=0;
static std::string cmdstrparms="";
QStringList defChildStyle=QStringList("flush");

// ---------------------------------------------------------------------
int wd(char *s,int slen,char *&res,int &len)
{
  locale=jegetlocale();
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
  std::string c;
  while ((rc==0) && cmd.more()) {
    c=cmd.getid();
    if (c.empty()) continue;
    ccmd=c;
    if (verbose && c!="qer") {
      cmd.markpos();
      cmdstrparms=c + " " + cmd.getparms();
      cmd.rewindpos();
      if (2==verbose || 3==verbose) {
        std::string indent="";
        if (form && form->pane) indent.append(2*max(0,form->pane->layouts.size()-1),' ');
        if (2==verbose && tedit && ShowIde) tedit->append_smoutput("wd command: " + s2q(indent+cmdstrparms));
        if (3==verbose) qDebug() << "wd command: " + s2q(indent+cmdstrparms);
      }
    }
    if (c=="q")
      wdq();
    else if (c=="beep")
      wdbeep();
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
    else if (c=="get")
      wdget();
    else if (c=="getp")
      wdgetp();
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
    else if (c=="setp")
      wdsetp();
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
    else if (c=="verbose")
      wdverbose();
    else if (c=="version")
      wdversion();
    else if (c=="maxwh")
      wdmaxwh();
    else if (c=="minwh")
      wdminwh();
#ifdef QTWEBSOCKET
    else if (c=="ws")
      wdws();
#endif
    else if (0) {
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
void wdbeep()
{
  cmd.getparms();
  if (!app) {
    error("command failed: no QApplication");
    return;
  }
  app->beep();
}

// ---------------------------------------------------------------------
void wdbin()
{
  std::string p=remquotes(cmd.getparms());
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
  std::string c,n,p;
  n=cmd.getid();
  c=cmd.getid();
  p=cmd.getparms();
  form->pane->addchild(n,c,p);
}

// ---------------------------------------------------------------------
void wdclipcopy()
{
  std::string p=cmd.getparms();
  if (!app) {
    error("command failed: no QApplication");
    return;
  }
  wdclipwrite((char *)p.c_str());
}

// ---------------------------------------------------------------------
void wdclipcopyx()
{
  std::string n=cmd.getid();
  std::string p=cmd.getparms();
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
  std::string p=cmd.getparms();
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
  std::string n=cmd.getid();
  std::string p=cmd.getparms();
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
  std::string n=cmd.getid();
  std::string p=cmd.getid();
  std::string v=cmd.getparms(true);
  int type=setchild(n);
  if (type)
    cc->cmd(p,v);
  else
    error("bad child id: " + n);
}

// ---------------------------------------------------------------------
void wdcn()
{
  std::string p=remquotes(cmd.getparms());
  if (noform()) return;
  cc=form->child;
  if (nochild()) return;
  cc->set("caption",p);
}

// ---------------------------------------------------------------------
void wddefprint()
{
#ifndef QT_NO_PRINTER
  std::string c=cmd.getid();
  std::string p=cmd.getparms();
  if (c=="orient") {
    if (p=="landscape")
      config.Printer->setPageOrientation(QPageLayout::Landscape);
    else
      config.Printer->setPageOrientation(QPageLayout::Portrait);
  } else if (c=="margin") {
    QStringList n=s2q(p).split(" ",_SkipEmptyParts);
    if (n.size()==1) {
      qreal m=c_strtod(q2s(n.at(0)));
      config.Printer->setPageMargins(QMarginsF(m, m, m, m), QPageLayout::Millimeter);
    } else if (n.size()==2) {
      qreal l=c_strtod(q2s(n.at(0)));
      qreal t=c_strtod(q2s(n.at(1)));
      config.Printer->setPageMargins(QMarginsF(l, t, l, t), QPageLayout::Millimeter);
    } else if (n.size()==4) {
      qreal l=c_strtod(q2s(n.at(0)));
      qreal t=c_strtod(q2s(n.at(1)));
      qreal r=c_strtod(q2s(n.at(2)));
      qreal b=c_strtod(q2s(n.at(3)));
      config.Printer->setPageMargins(QMarginsF(l, t, r, b), QPageLayout::Millimeter);
    } else
      error("margin requires 1, 2, or 4 numbers: " + p);
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
  } else
    error("invalid option: " + c);
#else
  cmd.getparms();
#endif
}

// ---------------------------------------------------------------------
void wddirmatch()
{
  std::string p=cmd.getparms();
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
  std::string p=cmd.getparms();
  if (!p.size()) {
    if (fontdef) delete fontdef;
    fontdef=0;
  } else {
    Font *fnt = new Font(p);
    if (fnt->error) {
      delete fnt;
      error("fontdef command failed: "+ p);
    } else
      fontdef=fnt;
  }
}

// ---------------------------------------------------------------------
void wdget()
{
  std::string n=cmd.getid();
  std::string p=cmd.getid();
  std::string v=cmd.getparms();
  rc=-1;
  if (n=="_") n=formchildid();
  int type=setchild(n);
  if (type)
    result=cc->get(p,v);
  else
    error("bad child id: " + n);
}

// ---------------------------------------------------------------------
void wdgetp()
{
  std::string p=cmd.getid();
  std::string v=cmd.getparms();
  rc=-1;
  if (noform()) return;
  result=form->get(p,v);
  return;
}

// ---------------------------------------------------------------------
void wdgrid()
{
  if (noform()) {
    cmd.getparms();
    return;
  }
  std::string n=cmd.getid();
  std::string v=cmd.getparms();
  form->pane->grid(n,v);
}

// ---------------------------------------------------------------------
void wdgroupbox(std::string c)
{
  std::string p=cmd.getparms();
  if (noform()) return;
  if (!form->pane->groupbox(c,p))
    error("unrecognized command: " + c + " " + p);
}

// ---------------------------------------------------------------------
void wdide()
{
  std::string p=remquotes(cmd.getparms());
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
  std::string p=cmd.getparms();
  immexj(p.c_str());
}

// ---------------------------------------------------------------------
void wdline(std::string c)
{
  std::string p=cmd.getparms();
  if (noform()) return;
  if (!form->pane->line(c,p))
    error("unrecognized command: " + c + " " + p);
}

// ---------------------------------------------------------------------
void wdmb()
{
  std::string c=cmd.getid();
  std::string p=cmd.getparms(true);
  result=q2s(mb(c,p));
  if (1==rc)
    result="";
  else
    rc=-1;
}

// ---------------------------------------------------------------------
void wdmenu(std::string s)
{
  int rc=0;
  if (noform()) {
    cmd.getparms();
    return;
  }
  if (form->menubar==0) form->addmenu();
  std::string c,p;
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
  std::string p=cmd.getparms();
  if (p.size()) {
    error("extra parameters: " + p);
    return;
  }
  if (!app) {
    error("command failed: no QApplication");
    return;
  }
  app->processEvents(QEventLoop::AllEvents);
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
  std::string p=cmd.getparms();
  openj(p.c_str());
}

// ---------------------------------------------------------------------
void wdp(std::string c)
{
  if (c=="pactive")
    wdpactive();
  else if (c=="parse")
    wdparse();
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
  else if (c=="pmoves")
    wdpmoves();
  else if (c=="pn")
    wdpn();
  else if (c=="psel")
    wdpsel();
  else if (c=="pshow")
    wdpshow();
  else if (c=="pstylesheet")
    wdpstylesheet();
  else if (c=="ptheme")
    wdptheme();
  else if (c=="ptimer")
    wdptimer();
  else if (c=="ptop")
    wdptop();
  else if (c=="notyet") {
    wdnotyet();
  } else
    error("parent command not found: " + c);
}

// ---------------------------------------------------------------------
void wdpactive()
{
  std::string p=cmd.getparms();
  if (p.size()) {
    error("extra parameters: " + p);
    return;
  }
  if (noform()) return;
  form->activateWindow();
  form->raise();
}

// ---------------------------------------------------------------------
void wdparse()
{
  std::string r;
  std::vector<std::string> s=ssplit(cmd.getparms());
  int n=(int)s.size();
  for(int i=0; i<n; i++)
    r+=spair(i2s(i),s[i]);
  result=r;
  rc=-2;
}

// ---------------------------------------------------------------------
void wdpas()
{
  std::string p=cmd.getparms();
  if (noform()) return;
  QStringList n=s2q(p).split(" ",_SkipEmptyParts);
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
  } else
    error("pas requires 2 or 4 numbers: " + p);
}

// ---------------------------------------------------------------------
void wdpc()
{
  std::string c,p;
  if (!jt) {
    error("command failed: no interpreter");
    return;
  }
  std::string tlocale=jegetlocale();
  c=cmd.getid();
  p=cmd.getparms();
// QWidget must be parentless to be top-level window
  QStringList m=s2q(p).split(' ',_SkipEmptyParts);
  Form *f=new Form(c,p,tlocale,m.contains("owner")?form:0);
  if (rc==1) {
    delete f;
    return;
  }
  evtform=form=f;
  Forms.append(form);
}

// ---------------------------------------------------------------------
void wdpcenter()
{
  std::string p=cmd.getparms();
  if (p.size()) {
    error("extra parameters: " + p);
    return;
  }
  if (noform()) return;
#if defined(QT57)
  QRect screenGeometry = app->primaryScreen()->geometry();
#else
  QDesktopWidget* dw=app->desktop();
  QRect screenGeometry = dw->screenGeometry(-1);
#endif
  int sw=screenGeometry.width();
  int sh=screenGeometry.height();
  int w=form->size().width();
  int h=form->size().height();
  int x=(sw-w)/2;
  int y=(sh-h)/2;
  form->move((x<0)?0:x,(y<0)?0:y);
}

// ---------------------------------------------------------------------
void wdpclose()
{
  std::string p=cmd.getparms();
  if (p.size()) {
    error("extra parameters: " + p);
    return;
  }
  if (noform()) return;
  if (form->closed) return;
  form->closeit();
}

// ---------------------------------------------------------------------
void wdpicon()
{
  std::string p=remquotes(cmd.getparms());
  if (noform()) return;
  form->setpicon(p);
}

// ---------------------------------------------------------------------
void wdpmove()
{
  std::string p=cmd.getparms();
  if (noform()) return;
  wdpmove1(p);
}

// ---------------------------------------------------------------------
void wdpmoves()
{
  std::string p=cmd.getparms();
  if (noform()) return;
  form->savepos=true;
  std::string s=config.formpos_read(s2q(form->id));
  if (s.length() > 0)
    p.assign(s);
  wdpmove1(p);
}

// ---------------------------------------------------------------------
void wdpmove1(std::string p)
{
  QStringList n=s2q(p).split(" ",_SkipEmptyParts);
  if (n.size()!=4)
    error("pmove requires 4 numbers: " + p);
  else {
    if (c_strtoi(q2s(n.at(0)))!=-1 && c_strtoi(q2s(n.at(1)))!=-1)
      form->move(c_strtoi(q2s(n.at(0))),c_strtoi(q2s(n.at(1))));
    if (c_strtoi(q2s(n.at(2)))!=-1 && c_strtoi(q2s(n.at(3)))!=-1)
      form->resize(c_strtoi(q2s(n.at(2))),c_strtoi(q2s(n.at(3))));
  }
}

// ---------------------------------------------------------------------
void wdpn()
{
  std::string p=remquotes(cmd.getparms());
  if (noform()) return;
  form->setpn(p);
}

// ---------------------------------------------------------------------
void wdpsel()
{
  std::string p=cmd.getparms();
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
  std::string p=remquotes(cmd.getparms());
  if (noform()) return;
  form->showit(p);
}

// ---------------------------------------------------------------------
void wdpstylesheet()
{
  std::string p=remquotes(cmd.getparms());
  if (noform()) return;
  form->setStyleSheet(s2q(p));
}

// ---------------------------------------------------------------------
void wdptheme()
{
  std::string p=remquotes(cmd.getparms());
  if (noform()) return;
  form->settheme(p);
}

// ---------------------------------------------------------------------
void wdptimer()
{
  std::string p=remquotes(cmd.getparms());
  if (noform()) return;
  form->settimer(p);
}

// ---------------------------------------------------------------------
void wdptop()
{
  std::string p=remquotes(cmd.getparms());
  if (noform()) return;
  Qt::WindowFlags f=form->windowFlags();
  form->setWindowFlags(f|Qt::WindowStaysOnTopHint);
  form->show();
  if (p!="1") {
    form->setWindowFlags(f);
    form->show();
  }
}

// ---------------------------------------------------------------------
void wdq()
{
  std::string p=cmd.getparms();
  if (p.size()) {
    error("extra parameters: " + p);
    return;
  }
  result = wdQuery;
  rc = -2;
}

// ---------------------------------------------------------------------
void wdqtstate(std::string p)
{
  rc=-2;
  result=qtstate(p);
}

// ---------------------------------------------------------------------
void wdqueries(std::string s)
{
  std::string p=cmd.getparms();

  if (p.size() && (s=="qd" || s=="qverbose" ||s=="qopenglmod" || s=="qscreen" || s=="qwd" || s=="qosver" || s=="qprinters" || s=="qpx" || s=="qhwndp" || s=="qhwndx" || s=="qform")) {
    error("extra parameters: " + p);
    return;
  }

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
    if (verbose)
      result=lasterror + "\012" + cmdstrparms;
    else result=lasterror;
    if (2==verbose && tedit && ShowIde) tedit->append_smoutput("wd **error: " + s2q(lasterror));
    if (3==verbose) qDebug() << "wd **error: " + s2q(lasterror);
    return;
  }
// queries that form not needed
  if (s=="qverbose") {
    result=i2s(verbose);
    return;
  } else if (s=="qopenglmod") {
#ifndef QT_NO_OPENGL
    if (!app) {
      result=i2s(0);
      return;
    }
#ifdef QT50
    result=i2s(1+QOpenGLContext::openGLModuleType());
#else
    result=i2s(1);
#endif
    return;
#else
    result=i2s(0);
    return;
#endif
  } else if (s=="qscreen") {
    if (!app) {
      error("command failed: no QApplication");
      return;
    }
#if defined(QT57)
    QScreen *dw = app->primaryScreen();
    QRect screenGeometry = dw->geometry();
    int dpix=dw->logicalDotsPerInchX();
    int dpiy=dw->logicalDotsPerInchX();
#else
    QDesktopWidget* dw=app->desktop();
    QRect screenGeometry = dw->screenGeometry(-1);
    int dpix=dw->logicalDpiX();
    int dpiy=dw->logicalDpiY();
#endif
    int w=screenGeometry.width();
    int h=screenGeometry.height();
    int mmx=25.4*w/dpix;
    int mmy=25.4*h/dpiy;
    int dia=sqrt((float)dpix*dpix+dpiy*dpiy);
#if defined(QT512)
    result=i2s(mmx) + " " + i2s(mmy) + " " + i2s(w) + " " + i2s(h) + " " + i2s(dpix) + " " + i2s(dpiy) + " " + i2s(dw->depth()) + " 1 " + i2s(INT_MAX) + " " + i2s(dpix) + " " + i2s(dpiy) + " " + i2s(dia);
#else
    result=i2s(mmx) + " " + i2s(mmy) + " " + i2s(w) + " " + i2s(h) + " " + i2s(dpix) + " " + i2s(dpiy) + " " + i2s(dw->depth()) + " 1 " + i2s(dw->colorCount()) + " " + i2s(dpix) + " " + i2s(dpiy) + " " + i2s(dia);
#endif
    return;
  } else if (s=="qwd") {
    result="jqt";
    return;
  } else if (s=="qosver") {
#ifdef QT57
    QSysInfo qsi;
    result=q2s(qsi.prettyProductName()+" "+qsi.kernelVersion());
#else
    result="";
#endif
    return;
  } else if (s=="qpath") {
    result=q2s(QApplication::applicationDirPath());
    return;
  } else if (s=="qprinters") {
    if (!app) {
      error("command failed: no QApplication");
      return;
    }
    std::string q="";
#ifndef QT_NO_PRINTER
    QPrinterInfo pd=QPrinterInfo::defaultPrinter();
    if ((!config.Printer) || !config.Printer->isValid()) q = std::string("\012");
    else q = q2s(config.Printer->printerName()) + "\012";
    if (pd.isNull()) q = q + std::string("\012");
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
      std::string q;
      for (int i=0; i<Forms.size(); i++) {
        Form *f=Forms.at(i);
        q = q + f->id + "\t" + p2s((void *)f) + "\t" + f->locale + "\t\t" + i2s(f->seq) + "\t\012";
      }
      result=q;
    }
    return;
  } else if (s=="qhwndx") {
    if (!term) result="";
    else result=p2s((void*)term->winId());
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
  if (s=="qhwndp") {
    if (noform()) return;
    result=form->hsform();
  } else if (s=="qform") {
    if (noform()) return;
    result=form->qform();
// queries expecting parameter
  } else if (s=="qhwndc") {
    if (noform()) return;
    if (!p.size()) {
      error("missing parameters");
      return;
    }
    Child *cc;
    if (p=="_") p=formchildid();
    if ((cc=form->id2child(p))) result=p2s(cc);
    else
      error("no child selected: " + p);
    if (rc!=1) form->child=cc;
  } else if (s=="qchildxywh") {
    if (noform()) return;
    if (!p.size()) {
      error("missing parameters");
      return;
    }
    Child *cc;
    if (p=="_") p=formchildid();
    if ((cc=form->id2child(p)) && cc->widget) {
      QPoint pos=cc->widget->mapTo(cc->widget->window(),cc->widget->pos());
      QSize size=cc->widget->size();
      result=i2s(pos.x())+" "+i2s(pos.y())+" "+i2s(size.width())+" "+i2s(size.height());
    } else
      error("no child selected: " + p);
    if (rc!=1) form->child=cc;
  } else
    error("command not found");
}

#ifndef QT_NO_QUICKVIEW1
// ---------------------------------------------------------------------
void wdquickview1()
{
  std::string p=cmd.getparms();
  QStringList n=s2q(p).split(" ",_SkipEmptyParts);
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
    std::string t=remquotes(q2s(n.at(0)));
    std::string f=remquotes(q2s(n.at(1)));
    if (!QFile(s2q(f)).exists()) {
      error("quickview1 file error: " + p);
      return;
    }
    if (n.size()>2 && (n.at(2)=="0"||n.at(2)=="1")) mode=!!c_strtoi(q2s(n.at(2)));
    if (quickview1) quickview1->close();
    quickview1=new QuickView1(t,f,mode);
    quickview1->show();
    quickview1->raise();
  }
}

#endif
#ifndef QT_NO_QUICKVIEW2
// ---------------------------------------------------------------------
void wdquickview2()
{
  std::string p=cmd.getparms();
  QStringList n=s2q(p).split(" ",_SkipEmptyParts);
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
    std::string t=remquotes(q2s(n.at(0)));
    std::string f=remquotes(q2s(n.at(1)));
    if (!QFile(s2q(f)).exists()) {
      error("quickview2 file error: " + p);
      return;
    }
    std::string glver="";
    if (n.size()>2) {
      if (n.at(2)=="0"||n.at(2)=="1") mode=!!c_strtoi(q2s(n.at(2)));
      int l=n.indexOf("version");
      if ((l!=-1) && (l<n.size()-1) && 0!=n.at(l+1).toDouble()) {
        glver=q2s(n.at(l+1));
      }
    }
    if (quickview2) quickview2->close();
    quickview2=new QuickView2(t,f,mode,glver);
    quickview2->show();
    quickview2->raise();
#ifdef QT50
    quickview2->requestActivate();
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
  std::string p=cmd.getparms();
  if (p.size()) {
    error("extra parameters: " + p);
    return;
  }
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
  isigraph=0;
  opengl=0;
  if (fontdef) delete fontdef;
  fontdef=0;
  if (FontExtent) delete FontExtent;
  FontExtent=0;
  lasterror="";
  result="";
  verbose=0;
  cmdstrparms="";
}

// ---------------------------------------------------------------------
void wdset()
{
  std::string n=cmd.getid();
  std::string p=cmd.getid();
  std::string v=cmd.getparms();
  wdset1(n,p,v);
}

// ---------------------------------------------------------------------
void wdsetp()
{
  std::string p=cmd.getid();
  std::string v=cmd.getparms();
  if (noform()) return;
  noevents(1);
  form->set(p,v);
  noevents(0);
  return;
}

// ---------------------------------------------------------------------
void wdsetx(std::string c)
{
  std::string n=cmd.getid();
  std::string p=c.substr(3);
  std::string v=cmd.getparms();
  wdset1(n,p,v);
}

// ---------------------------------------------------------------------
void wdset1(std::string n,std::string p,std::string v)
{
  if (noform()) return;
  noevents(1);
  if (n=="_") n=formchildid();
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
    error("bad child id: " + n);
  }
  if (rc!=1) form->child=cc;
  noevents(0);
}

// ---------------------------------------------------------------------
void wdsm(std::string s)
{
  std::string c,p;
  if (s=="sm")
    c=cmd.getid();
  else if (s=="smact")
    c="act";
  else
    c=s;
  result=sm(c);
  if (rc==1)
    error(result);
}

// ---------------------------------------------------------------------
void wdsplit(std::string c)
{
  std::string p=cmd.getparms();
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
void wdtab(std::string c)
{
  std::string p=cmd.getparms();
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
  std::string p=remquotes(cmd.getparms());
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
  std::string p=cmd.getparms();
  if (!jt) {
    error("command failed: no interpreter");
    return;
  }
  result=ws(p);
  if (1==rc)
    result="";
  else
    rc=-1;
}
#endif

// ---------------------------------------------------------------------
void wdverbose()
{
  std::string p=remquotes(cmd.getparms());
  QStringList n=s2q(p).split(" ",_SkipEmptyParts);
  if (n.empty())
    error("verbose requires 1 number: " + p);
  else {
    int i=c_strtoi(q2s(n.at(0)));
    if (!(i>=0 && i<=3))
      error("verbose should be 0,1,2 or 3: " + p);
    else verbose=i;
  }
}

// ---------------------------------------------------------------------
void wdversion()
{
  result=getversion();
  rc=-1;
}

// ---------------------------------------------------------------------
void wdmaxwh()
{
  std::string p=cmd.getparms();
  if (noform()) return;
  QStringList n=s2q(p).split(" ",_SkipEmptyParts);
  if (n.size()!=2)
    error("maxwh requires 2 numbers: " + p);
  else {
    form->pane->maxsizew=c_strtoi(q2s(n.at(0)));
    form->pane->maxsizeh=c_strtoi(q2s(n.at(1)));
  }
}

// ---------------------------------------------------------------------
void wdminwh()
{
  std::string p=cmd.getparms();
  if (noform()) return;
  QStringList n=s2q(p).split(" ",_SkipEmptyParts);
  if (n.size()!=2)
    error("minwh requires 2 numbers: " + p);
  else {
    form->pane->minsizew=c_strtoi(q2s(n.at(0)));
    form->pane->minsizeh=c_strtoi(q2s(n.at(1)));
  }
}

// ---------------------------------------------------------------------
void wdsetfocuspolicy(QWidget *widget,std::string p)
{
  if (!widget) return;
  if (p=="tab")
    widget->setFocusPolicy(Qt::TabFocus);
  else if (p=="click")
    widget->setFocusPolicy(Qt::ClickFocus);
  else if (p=="strong")
    widget->setFocusPolicy(Qt::StrongFocus);
  else if (p=="no")
    widget->setFocusPolicy(Qt::NoFocus);
  else
    error("set focuspolicy requires option to be tab,click,strong or no: " + p);
}

// enum QSizePolicy::Policy
//
// This enum describes the various per-dimension sizing types used when constructing a QSizePolicy.
//
//           Constant                Value                                   Description
//                                             The QWidget::sizeHint() is the only acceptable alternative, so the
// QSizePolicy::Fixed            0             widget can never grow or shrink (e.g. the vertical direction of a push
//                                             button).
//                                             The sizeHint() is minimal, and sufficient. The widget can be expanded,
// QSizePolicy::Minimum          GrowFlag      but there is no advantage to it being larger (e.g. the horizontal
//                                             direction of a push button). It cannot be smaller than the size
//                                             provided by sizeHint().
//                                             The sizeHint() is a maximum. The widget can be shrunk any amount
// QSizePolicy::Maximum          ShrinkFlag    without detriment if other widgets need the space (e.g. a separator
//                                             line). It cannot be larger than the size provided by sizeHint().
//                               GrowFlag |    The sizeHint() is best, but the widget can be shrunk and still be
// QSizePolicy::Preferred        ShrinkFlag    useful. The widget can be expanded, but there is no advantage to it
//                                             being larger than sizeHint() (the default QWidget policy).
//                               GrowFlag |    The sizeHint() is a sensible size, but the widget can be shrunk and
// QSizePolicy::Expanding        ShrinkFlag |  still be useful. The widget can make use of extra space, so it should
//                               ExpandFlag    get as much space as possible (e.g. the horizontal direction of a
//                                             horizontal slider).
//                               GrowFlag |    The sizeHint() is minimal, and sufficient. The widget can make use of
// QSizePolicy::MinimumExpanding ExpandFlag    extra space, so it should get as much space as possible (e.g. the
//                                             horizontal direction of a horizontal slider).
//                               ShrinkFlag |  The sizeHint() is ignored. The widget will get as much space as
// QSizePolicy::Ignored          GrowFlag |    possible.
//                               IgnoreFlag
//
// enum QSizePolicy::PolicyFlag
//
// These flags are combined together to form the various Policy values:
//
//        Constant         Value                                    Description
// QSizePolicy::GrowFlag   1     The widget can grow beyond its size hint if necessary.
// QSizePolicy::ExpandFlag 2     The widget should get as much space as possible.
// QSizePolicy::ShrinkFlag 4     The widget can shrink below its size hint if necessary.
// QSizePolicy::IgnoreFlag 8     The widget's size hint is ignored. The widget will get as much space as possible.

// ---------------------------------------------------------------------
void wdsetsizepolicy(QWidget *widget,std::string p)
{
  if (!widget) return;
  QString h,v;
  int hoz,ver;
  QStringList n=s2q(p).split(" ",_SkipEmptyParts);
  if (n.empty()) {
    error("set sizepolicy requires 1 or 2 options: " + p);
    return;
  }
  if (n.size()==1)
    h=v=n.at(0);
  else {
    h=n.at(0);
    v=n.at(1);
  }
  if (h=="fixed")
    hoz=QSizePolicy::Fixed;
  else if (h=="minimum")
    hoz=QSizePolicy::Minimum;
  else if (h=="maximum")
    hoz=QSizePolicy::Maximum;
  else if (h=="preferred")
    hoz=QSizePolicy::Preferred;
  else if (h=="expanding")
    hoz=QSizePolicy::Expanding;
  else if (h=="minimumexpanding")
    hoz=QSizePolicy::MinimumExpanding;
  else if (h=="ignored")
    hoz=QSizePolicy::Ignored;
  else {
    error("set sizepolicy requires options to be fixed,minimum,maximum,preferred,expanding,minimumexpanding or ignored: " + p);
    return;
  }
  if (v=="fixed")
    ver=QSizePolicy::Fixed;
  else if (v=="minimum")
    ver=QSizePolicy::Minimum;
  else if (v=="maximum")
    ver=QSizePolicy::Maximum;
  else if (v=="preferred")
    ver=QSizePolicy::Preferred;
  else if (v=="expanding")
    ver=QSizePolicy::Expanding;
  else if (v=="minimumexpanding")
    ver=QSizePolicy::MinimumExpanding;
  else if (v=="ignored")
    ver=QSizePolicy::Ignored;
  else {
    error("set sizepolicy requires options to be fixed,minimum,maximum,preferred,expanding,minimumexpanding or ignored: " + p);
    return;
  }
  if (widget) {
    widget->setSizePolicy((QSizePolicy::Policy)hoz,(QSizePolicy::Policy)ver);
    widget->updateGeometry();
  }
}

// ---------------------------------------------------------------------
void wdsetwh(QWidget *widget,std::string p)
{
  if (!widget) return;
  QStringList n=s2q(p).split(" ",_SkipEmptyParts);
  if (n.size()!=2) {
    error("set wh requires 2 numbers: " + p);
  } else {
    int w=c_strtoi(q2s(n.at(0)));
    int h=c_strtoi(q2s(n.at(1)));
    if (!widget) return;
    if (w!=-1 || h!=-1) {
      if (w==-1)
        widget->setFixedHeight(h);
      else if (h==-1)
        widget->setFixedWidth(w);
      else
        widget->setFixedSize(w,h);
    }
  }
}

// ---------------------------------------------------------------------
static char *qstylesp[]= {
  (char *)"qstyle::sp_titlebarmenubutton",               /* 0          Menu button on a title bar. */
  (char *)"qstyle::sp_titlebarminbutton",                /* 1          Minimize button on title bars (e.g., in QMdiSubWindow). */
  (char *)"qstyle::sp_titlebarmaxbutton",                /* 2          Maximize button on title bars. */
  (char *)"qstyle::sp_titlebarclosebutton",              /* 3          Close button on title bars. */
  (char *)"qstyle::sp_titlebarnormalbutton",             /* 4          Normal (restore) button on title bars. */
  (char *)"qstyle::sp_titlebarshadebutton",              /* 5          Shade button on title bars. */
  (char *)"qstyle::sp_titlebarunshadebutton",            /* 6          Unshade button on title bars. */
  (char *)"qstyle::sp_titlebarcontexthelpbutton",        /* 7          The Context help button on title bars. */
  (char *)"qstyle::sp_dockwidgetclosebutton",            /* 8          Close button on dock windows (see also QDockWidget). */
  (char *)"qstyle::sp_messageboxinformation",            /* 9          The"information" icon. */
  (char *)"qstyle::sp_messageboxwarning",                /* 10         The"warning" icon. */
  (char *)"qstyle::sp_messageboxcritical",               /* 11         The"critical" icon. */
  (char *)"qstyle::sp_messageboxquestion",               /* 12         The"question" icon. */
  (char *)"qstyle::sp_desktopicon",                      /* 13         The"desktop" icon. */
  (char *)"qstyle::sp_trashicon",                        /* 14         The"trash" icon. */
  (char *)"qstyle::sp_computericon",                     /* 15         The"My computer" icon. */
  (char *)"qstyle::sp_drivefdicon",                      /* 16         The floppy icon. */
  (char *)"qstyle::sp_drivehdicon",                      /* 17         The harddrive icon. */
  (char *)"qstyle::sp_drivecdicon",                      /* 18         The CD icon. */
  (char *)"qstyle::sp_drivedvdicon",                     /* 19         The DVD icon. */
  (char *)"qstyle::sp_driveneticon",                     /* 20         The network icon. */
  (char *)"qstyle::sp_diropenicon",                      /* 21         The open directory icon. */
  (char *)"qstyle::sp_dirclosedicon",                    /* 22         The closed directory icon. */
  (char *)"qstyle::sp_dirlinkicon",                      /* 23         The link to directory icon. */
  (char *)"qstyle::sp_dirlinkopenicon",                  /* 24         The link to open directory icon. */
  (char *)"qstyle::sp_fileicon",                         /* 25         The file icon. */
  (char *)"qstyle::sp_filelinkicon",                     /* 26         The link to file icon. */
  (char *)"qstyle::sp_toolbarhorizontalextensionbutton", /* 27         Extension button for horizontal toolbars. */
  (char *)"qstyle::sp_toolbarverticalextensionbutton",   /* 28         Extension button for vertical toolbars. */
  (char *)"qstyle::sp_filedialogstart",                  /* 29         The"start" icon in a file dialog. */
  (char *)"qstyle::sp_filedialogend",                    /* 30         The"end" icon in a file dialog. */
  (char *)"qstyle::sp_filedialogtoparent",               /* 31         The"parent directory" icon in a file dialog. */
  (char *)"qstyle::sp_filedialognewfolder",              /* 32         The"create new folder" icon in a file dialog. */
  (char *)"qstyle::sp_filedialogdetailedview",           /* 33         The detailed view icon in a file dialog. */
  (char *)"qstyle::sp_filedialoginfoview",               /* 34         The file info icon in a file dialog. */
  (char *)"qstyle::sp_filedialogcontentsview",           /* 35         The contents view icon in a file dialog. */
  (char *)"qstyle::sp_filedialoglistview",               /* 36         The list view icon in a file dialog. */
  (char *)"qstyle::sp_filedialogback",                   /* 37         The back arrow in a file dialog. */
  (char *)"qstyle::sp_diricon",                          /* 38         The directory icon. */
  (char *)"qstyle::sp_dialogokbutton",                   /* 39         Icon for a standard OK button in a QDialogButtonBox. */
  (char *)"qstyle::sp_dialogcancelbutton",               /* 40         Icon for a standard Cancel button in a QDialogButtonBox. */
  (char *)"qstyle::sp_dialoghelpbutton",                 /* 41         Icon for a standard Help button in a QDialogButtonBox. */
  (char *)"qstyle::sp_dialogopenbutton",                 /* 42         Icon for a standard Open button in a QDialogButtonBox. */
  (char *)"qstyle::sp_dialogsavebutton",                 /* 43         Icon for a standard Save button in a QDialogButtonBox. */
  (char *)"qstyle::sp_dialogclosebutton",                /* 44         Icon for a standard Close button in a QDialogButtonBox. */
  (char *)"qstyle::sp_dialogapplybutton",                /* 45         Icon for a standard Apply button in a QDialogButtonBox. */
  (char *)"qstyle::sp_dialogresetbutton",                /* 46         Icon for a standard Reset button in a QDialogButtonBox. */
  (char *)"qstyle::sp_dialogdiscardbutton",              /* 47         Icon for a standard Discard button in a QDialogButtonBox. */
  (char *)"qstyle::sp_dialogyesbutton",                  /* 48         Icon for a standard Yes button in a QDialogButtonBox. */
  (char *)"qstyle::sp_dialognobutton",                   /* 49         Icon for a standard No button in a QDialogButtonBox. */
  (char *)"qstyle::sp_arrowup",                          /* 50         Icon arrow pointing up. */
  (char *)"qstyle::sp_arrowdown",                        /* 51         Icon arrow pointing down. */
  (char *)"qstyle::sp_arrowleft",                        /* 52         Icon arrow pointing left. */
  (char *)"qstyle::sp_arrowright",                       /* 53         Icon arrow pointing right. */
  (char *)"qstyle::sp_arrowback",                        /* 54         Equivalent to SP_ArrowLeft when the current layout direction is Qt::LeftToRight, otherwise SP_ArrowRight. */
  (char *)"qstyle::sp_arrowforward",                     /* 55         Equivalent to SP_ArrowRight when the current layout direction is Qt::LeftToRight, otherwise SP_ArrowLeft. */
  (char *)"qstyle::sp_dirhomeicon",                      /* 56         The home directory icon. */
  (char *)"qstyle::sp_commandlink",                      /* 57         Icon used to indicate a Vista style command link glyph. */
  (char *)"qstyle::sp_vistashield",                      /* 58         Icon used to indicate UAC prompts on Windows Vista. This will return a null pixmap or icon on all other platforms. */
  (char *)"qstyle::sp_browserreload",                    /* 59         Icon indicating that the current page should be reloaded. */
  (char *)"qstyle::sp_browserstop",                      /* 60         Icon indicating that the page loading should stop. */
  (char *)"qstyle::sp_mediaplay",                        /* 61         Icon indicating that media should begin playback. */
  (char *)"qstyle::sp_mediastop",                        /* 62         Icon indicating that media should stop playback. */
  (char *)"qstyle::sp_mediapause",                       /* 63         Icon indicating that media should pause playback. */
  (char *)"qstyle::sp_mediaskipforward",                 /* 64         Icon indicating that media should skip forward. */
  (char *)"qstyle::sp_mediaskipbackward",                /* 65         Icon indicating that media should skip backward. */
  (char *)"qstyle::sp_mediaseekforward",                 /* 66         Icon indicating that media should seek forward. */
  (char *)"qstyle::sp_mediaseekbackward",                /* 67         Icon indicating that media should seek backward. */
  (char *)"qstyle::sp_mediavolume",                      /* 68         Icon indicating a volume control. */
  (char *)"qstyle::sp_mediavolumemuted",                 /* 69         Icon indicating a muted volume control. */
  (char *)"qstyle::sp_lineeditclearbutton",              /* 70         Icon for a standard clear button in a QLineEdit. This enum value was added in Qt 5.2. */
};

// ---------------------------------------------------------------------
// widget->style()->standardIcon(i);
int wdstandardicon(std::string s)
{
  for (int i=0; i<71; i++) {
    if (s==std::string(qstylesp[i])) return i;
  }
  return -1;
}

// ---------------------------------------------------------------------
void error(std::string s)
{
  lasterror=ccmd+" : "+s;
  rc=1;
}

// ---------------------------------------------------------------------
// returns: id of current form child
std::string formchildid()
{
  if (noform()) return "";
  if (!form->child) return "";
  return form->child->id;
}

// ---------------------------------------------------------------------
bool invalidopt(std::string n,QStringList opt,std::string valid)
{
  QStringList unopt=qsless(opt,defChildStyle+qsplit(valid));
  if (0==unopt.size()) return false;
  error("unrecognized style for " + n + ": " + q2s(unopt.join(" ")));
  return true;
}

// ---------------------------------------------------------------------
bool invalidoptn(std::string n,QStringList opt,std::string valid)
{
  QStringList unopt=qsless(opt,defChildStyle+qsplit(valid));
  if (0==unopt.size() || qsnumeric(unopt)) return false;
  error("unrecognized style for " + n + ": " + q2s(unopt.join(" ")));
  return true;
}

// ---------------------------------------------------------------------
bool nochild()
{
  if (cc) return false;
  error("no child selected");
  return true;
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
bool notbothopt(std::string n,QStringList opt, std::string a, std::string b)
{
  if (!(opt.contains(s2q(a)) && opt.contains(s2q(b)))) return false;
  error ("options for " + n + " should not include both " + a + " and " + b);
  return true;
}

// ---------------------------------------------------------------------
// returns: 0=id not found
//          1=child id (cc=child)
//          2=menu  id (cc=menubar)
int setchild(std::string id)
{
  Child *c;
  if (noform()) return 0;
  c=form->id2child(id);
  if (c) {
    cc=c;
    return 1;
  }
  c=form->setmenuid(id);
  if (c) {
    cc=c;
    return 2;
  }
  return 0;
}

// ---------------------------------------------------------------------
// translate event.keyboard key to Private Use Area
// invalid key values produce invalid results
int translateqkey(int key)
{
  // ASCII: pass through
  if (key<0x01000000) return key;
  // 0x01000yxx map to F800-F900 (y=0 or 1)
  if (key<0x01000200) return 0xf800 | (key & 0x1ff);
  // 0x010011xx and 0x010012xx to FAxx
  // 0x010100xx to FBxx
  // 0x010200xx to FCxx
  if (key<0x01100000) return 0xfa00 + ((key & 0x00030000) >> 8) + (key & 0xff);
  // 0x011000xx to FDxx
  return 0xfd00 | (key & 0xff);
}

#define LOGD(e) qDebug() << (e)

#if defined(_WIN64)||defined(__LP64__)
#define SZI 8
#else
#define SZI 4
#endif

// ---------------------------------------------------------------------
// caller should free string
char * int2utf8(const int *yi, int nc)
{
  int i;
  char *ys = (char *) malloc(1 + (size_t) nc);
  for (i = 0; i < nc; i++)
    *(ys + i) = *(yi + i);
  *(ys + nc) = 0;
  return ys;
}

// ---------------------------------------------------------------------
int uigl2(I t,int *ptr, int ncnt)
{
  rc=0;
  if (t==glsel2_n) {
    char *g = int2utf8(ptr+2, ncnt-2);
    rc = glsel2(g);
    free(g);
  } else if (t==gl_sel2_n) {
    char *g = int2utf8(ptr+2, ncnt-2);
    rc = gl_sel2(g);
    free(g);
  } else if (t==glqextent_n || t==glqextentw_n || t==glqpixels_n || t==glqpixelm_n
             || t==glqprintpaper_n || t==glqprintwh_n || t==glqtextmetrics_n || t==glqtype_n
             || t==glqwh_n || t==glqhandles_n)
    rc = glquery(t, ptr+2, ncnt-2);
  else if (t==gl_qextent_n || t==gl_qextentw_n || t==gl_qpixels_n || t==gl_qpixelm_n
           || t==gl_qtextmetrics_n || t==gl_qtype_n
           || t==gl_qwh_n || t==gl_qhandles_n)
    rc = gl_query(t, ptr+2, ncnt-2);
  else if (t==glzqextent_n || t==glzqextentw_n
           || t==glzqtextmetrics_n || t==glzqtype_n
           || t==glzqwh_n)
#ifndef QT_NO_PRINTER
    rc = glzquery(t, ptr+2, ncnt-2);
#else
    rc = 1;
#endif
  else if (t==glzqresolution_n || t==glzqcolormode_n || t==glzqduplexmode_n || t==glzqorientation_n
           || t==glzqoutputformat_n || t==glzqpageorder_n || t==glzqpapersize_n || t==glzqpapersource_n
           || t==glzqmargins_n)
#ifndef QT_NO_PRINTER
    rc = glzquery2(t, ptr+2, ncnt-2);
#else
    rc = 1;
#endif
  else if (t>=1000 && t<=2099)
    rc = glcmds(ptr, ncnt);
  else if ((t>=2100 && t<=2199) || t==2312)  // glfontn 2312 hardwired in plot addon
    rc = gl_cmds(ptr, ncnt);
  else if (t>=2200 && t<=2299)
#ifndef QT_NO_PRINTER
    rc = glzcmds(ptr, ncnt);
#else
    rc = 1;
#endif
  else
    rc = 1;
  return rc;
}

// ---------------------------------------------------------------------
int uiwd(I t,I *pinta,void *inarr,I *ointa,char* loc)
{
  Q_UNUSED(loc);
  rc = 0;   // system rc
  locale = std::string(loc);
  result.clear();
  resultshape[0]=resultshape[1]=-1;
  if(0==t) {
    if(LIT!=pinta[0]) {
      LOGD("argument should be string");
      return rc = 1;
    }
    cmd.init((char*)inarr,pinta[3]);
    wd1();
    if(rc<0) {
      ointa[1]=resultshape[0];  // assume LIT rank-1
      ointa[2]=resultshape[1];
      ointa[0] = resulttype = 0;
    }
    return rc;
  } else if (t>=1000 && t<=2999) {
    int *inint=0;
    if (t==2999) {
      if (INT!=pinta[0]) {
        LOGD("argument should be integer");
        return rc = 1;
      }
#if defined(_WIN64)||defined(__LP64__)
      inint = (int*)malloc(sizeof(int)*(pinta[3]));
      for (I i=0; i<pinta[3]; i++) inint[i]=((I*)inarr)[i];
#else
      inint = (int*)inarr;
#endif
      rc = glcmds(inint,pinta[3]);
#if defined(_WIN64)||defined(__LP64__)
      free(inint);
#endif
    } else if (t==gl_updategl_n) {
      if (INT!=pinta[0]) {
        LOGD("argument should be integer");
        return rc = 1;
      }
      if (1!=pinta[3]) {
        LOGD("argument should be 1 integer");
        return rc = 1;
      }
      rc = gl_updategl((void*)((I*)inarr)[0]);
    } else if (t==glsel_n) {
      if (INT!=pinta[0]) {
        LOGD("argument should be integer");
        return rc = 1;
      }
      if (1!=pinta[3]) {
        LOGD("argument should be 1 integer");
        return rc = 1;
      }
      rc = glsel((void*)((I*)inarr)[0]);
    } else if (t==gl_sel_n) {
      if (INT!=pinta[0]) {
        LOGD("argument should be integer");
        return rc = 1;
      }
      if (1!=pinta[3]) {
        LOGD("argument should be 1 integer");
        return rc = 1;
      }
      rc = gl_sel((void*)((I*)inarr)[0]);
    } else {
      if (INT==pinta[0]) {
        inint = (int*)malloc(sizeof(int)*(pinta[3]+2));
        inint[0]=pinta[3]+2;
        inint[1]=t;
        for (I i=0; i<pinta[3]; i++) inint[i+2]=((I*)inarr)[i];
      } else if (LIT==pinta[0]) {
        inint = (int*)malloc(sizeof(int)*(pinta[3]+2));
        inint[0]=pinta[3]+2;
        inint[1]=t;
        for (I i=0; i<pinta[3]; i++) inint[i+2]=((char*)inarr)[i];
      } else {
        LOGD("argument should be string or integer");
        return rc = 1;
      }
      rc = uigl2(t,inint,pinta[3]+2);
      free(inint);
      if(rc<0) {
        ointa[0] = resulttype;
        ointa[1] = resultshape[0];
        ointa[2] = resultshape[1];
      }
    }
  } else if (t==glgetimg_n) {
    if (INT!=pinta[0] || 0>=pinta[1]  || 0>=pinta[2] || 0==pinta[3] ) {
      LOGD("argument should be integer rank-2 array");
      return rc = 1;
    }
    uchar *data = (uchar*)malloc(sizeof(uchar)*(pinta[3]));
    for (int i=0; i<pinta[3]; i++) data[i]=((I*)inarr)[i];
    int *odata = (int*)wdgetimg(data, pinta[3], resultshape);
    free(data);
    if (odata) {
      ointa[0] = resulttype = 1; // int
      ointa[1] = resultshape[1];
      ointa[2] = resultshape[0];
      intresult.resize(resultshape[0]*resultshape[1]);
      memcpy(&intresult[0],odata,sizeof(int)*resultshape[0]*resultshape[1]);
      wdgetimg(0, 0, 0);  // clear temp image
      rc = -1;
    } else return rc = 1;
  } else if (t==glreadimg_n) {
    if (LIT!=pinta[0] || -1!=pinta[1]  || -1!=pinta[2] || 0==pinta[3] ) {
      LOGD("argument should be literal rank-1 array");
      return rc = 1;
    }
    char *data = (char*)malloc(1+sizeof(char)*(pinta[3]));
    for (int i=0; i<pinta[3]; i++) data[i]=((char*)inarr)[i];
    data[pinta[3]]=0;
    int *odata = (int*)wdreadimg(data, resultshape);
    free(data);
    if (odata) {
      ointa[0] = resulttype = 1; // int
      ointa[1] = resultshape[1];
      ointa[2] = resultshape[0];
      intresult.resize(resultshape[0]*resultshape[1]);
      memcpy(&intresult[0],odata,sizeof(int)*resultshape[0]*resultshape[1]);
      wdreadimg(0, 0);  // clear temp image
      rc = -1;
    } else return rc = 1;
  } else if (t==glputimg_n) {
    if (BOX!=pinta[0] || -1!=pinta[1]  || -1!=pinta[2] || 3!=pinta[3] ) {
      LOGD("argument should be box rank-1 array of 4 items");
      return rc = 1;
    }
    A* w1 = (A*)(inarr);
    if(!(INT&AT(w1[0]) && AN(w1[0]) && 2==ARNK(w1[0]))) {
      LOGD("first argument should be integer rank-2 array");
      return rc = 1;
    }
    if(!(LIT&AT(w1[1]) && AN(w1[1]) && 2>ARNK(w1[1]))) {
      LOGD("second argument should be literal rank-1 array");
      return rc = 1;
    }
    if(!(INT&AT(w1[2]) && 1==AN(w1[2]) && 2>ARNK(w1[2]))) {
      LOGD("third argument should be integer rank-1 array");
      return rc = 1;
    }
    int *data;
#if defined(_WIN64)||defined(__LP64__)
    data = (int*)malloc(sizeof(int)*AN(w1[0]));
    for (int i=0; i<AN(w1[0]); i++) data[i]=(IAV(w1[0]))[i];
#else
    data=(int*)IAV(w1[0]);
#endif
    char *fmt = (char*)malloc(1+sizeof(char)*AN(w1[1]));
    for (int i=0; i<AN(w1[1]); i++) fmt[i]=(CAV(w1[1]))[i];
    fmt[AN(w1[1])]=0;
    int wh[2];
    wh[0]= (AS(w1[0]))[1];
    wh[1]= (AS(w1[0]))[0];
    int len;
    uchar *odata = (uchar*)wdputimg((uchar*)data, wh, &len, fmt, (IAV(w1[2])[0]));
#if defined(_WIN64)||defined(__LP64__)
    free(data);
#endif
    free(fmt);
    if (odata) {
      ointa[0] = resulttype = 0; // literal
      intresult.resize(len);
      ointa[1]=resultshape[0];  // assume LIT rank-1
      ointa[2]=resultshape[1];
      result = std::string((char*)odata,len);
      wdputimg(0,0,0,0,0);  // clear temp image
      rc = -1;
    } else return rc = 1;
  } else if (t==glwriteimg_n) {
    if (BOX!=pinta[0] || -1!=pinta[1]  || -1!=pinta[2] || 4!=pinta[3] ) {
      LOGD("argument should be box rank-1 array of 4 items");
      return rc = 1;
    }
    A* w1 = (A*)(inarr);
    if(!(INT&AT(w1[0]) && AN(w1[0]) && 2==ARNK(w1[0]))) {
      LOGD("first argument should be integer rank-2 array");
      return rc = 1;
    }
    if(!(LIT&AT(w1[1]) && AN(w1[1]) && 2>ARNK(w1[1]))) {
      LOGD("second argument should be literal rank-1 array");
      return rc = 1;
    }
    if(!(LIT&AT(w1[2]) && AN(w1[2]) && 2>ARNK(w1[2]))) {
      LOGD("third argument should be literal rank-1 array");
      return rc = 1;
    }
    if(!(INT&AT(w1[3]) && 1==AN(w1[3]) && 2>ARNK(w1[3]))) {
      LOGD("fourth argument should be integer rank-1 array");
      return rc = 1;
    }
    int *data;
#if defined(_WIN64)||defined(__LP64__)
    data = (int*)malloc(sizeof(int)*AN(w1[0]));
    for (int i=0; i<AN(w1[0]); i++) data[i]=(IAV(w1[0]))[i];
#else
    data=(int*)IAV(w1[0]);
#endif
    char *s = (char*)malloc(1+sizeof(char)*AN(w1[1]));
    for (int i=0; i<AN(w1[1]); i++) s[i]=(CAV(w1[1]))[i];
    s[AN(w1[1])]=0;
    char *fmt = (char*)malloc(1+sizeof(char)*AN(w1[2]));
    for (int i=0; i<AN(w1[2]); i++) fmt[i]=(CAV(w1[2]))[i];
    fmt[AN(w1[2])]=0;
    int wh[2];
    wh[0]= (AS(w1[0]))[1];
    wh[1]= (AS(w1[0]))[0];
    int r1 = wdwriteimg((uchar*)data, wh, s, fmt, (IAV(w1[3])[0]));
#if defined(_WIN64)||defined(__LP64__)
    free(data);
#endif
    free(s);
    free(fmt);
    if (r1) {
      rc = 0;
    } else return rc = 1;
  } else {
    LOGD("not implemented");
    return rc = 1;
  }
  return rc;
}

// ---------------------------------------------------------------------
int _stdcall Jwd(JS jt, int type, A w, A *pz, C*loc)
{
  int rc=0;
  if(jt == 0) {
    LOGD("failed to get the method id for wd" );
    return rc = 3;
  }

// check argument type
  if (AN(w) && !((INT+LIT+BOX)&AT(w))) {
    LOGD("argument error" );
    return rc = 3;
  }
  if (rc) {
    return rc;
  }

// inta: datatype AN shape0 shape1 .... repeat for each inarr element
  I pinta[]= {0,-1,-1,0};
  I ointa[]= {0,-1,-1};

  if (LIT&AT(w) || 0==AN(w)) {
    pinta[0] = LIT;
    pinta[3] = AN(w);
    if (ARNK(w)>1 && AN(w)) {
      pinta[1] = (AS(w))[0];
      pinta[2] = (AS(w))[1];
    }
  } else if (INT&AT(w)) {
    pinta[0] = INT;
    pinta[3] = AN(w);
    if (ARNK(w)>1) {
      pinta[1] = (AS(w))[0];
      pinta[2] = (AS(w))[1];
    }
  } else if (BOX&AT(w)) {
    pinta[0] = BOX;
    pinta[3] = AN(w);
    if (ARNK(w)>1) {
      pinta[1] = (AS(w))[0];
      pinta[2] = (AS(w))[1];
    }
  } else {
    LOGD("argument data type not supported" );
    return rc = 3;
  }

  rc = uiwd(type,pinta,(void*)CAV(w),ointa,(char*)loc);

  if (rc<0) {
    I otype= ointa[0];
    I oshape[2];
    oshape[0]= ointa[1];  // -1 if not rank-2
    oshape[1]= ointa[2];

    if (otype==0) {   // LIT
      I len=(int)result.size();
      C* res=(C*)result.c_str();
      if (oshape[0]==-1) {
        oshape[0]=len;
        *pz=jega(LIT,len,1,oshape);
      } else {
        *pz=jega(LIT,len,2,oshape);
      }
      memcpy(CAV(*pz),res,len);
      result.clear();
    } else if (otype==1 || otype==2) {   // int or I
      int len = (otype==1) ? (int)intresult.size() : (int)longresult.size();
      if (oshape[0]==-1) {
        oshape[0]=len;
        *pz=jega(INT,len,1,oshape);
      } else {
        *pz=jega(INT,len,2,oshape);
      }
      if(otype==2) {
        memcpy(CAV(*pz),&longresult[0],SZI*len);   // 32-bit  int == I
        longresult.clear();
      } else if(otype==1) {
#if defined(_WIN64)||defined(__LP64__)
        for(int i=0; i<len; i++) (IAV(*pz))[i] = intresult[i];
#else
        memcpy(CAV(*pz),&intresult[0],SZI*len);   // 32-bit  int == I
#endif
        intresult.clear();
      }
    } else {
      LOGD("result not string or integers");
      rc=3;
    }
  }
  return (rc>0)?3:rc; // coerce to domain error
}
