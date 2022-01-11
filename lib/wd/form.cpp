
#include <QApplication>
#include <QBoxLayout>
#include <QEventLoop>
#include <QMenuBar>
#include <QSignalMapper>
#include <QTimer>

#include <sstream>  //include this to use std::string streams

#include "child.h"
#include "cmd.h"
#include "font.h"
#include "font.h"
#include "form.h"
#include "menus.h"
#include "pane.h"
#include "tabs.h"
#include "wd.h"
#include "../base/jsvr.h"
#include "../base/state.h"
#include "../base/svr.h"
#include "../base/term.h"

extern Form *form;
extern Font *fontdef;
extern bool standAlone;

// ---------------------------------------------------------------------
Form::Form(std::string s, std::string p, std::string loc, QWidget *parent) : QWidget (parent)
{
  Q_UNUSED(parent);
  id=s;
  child=0;
  evtchild=0;
  locale=loc;
  maxvisibleitems=0;
  menubar=0;
  seq=FormSeq++;
  tab=0;
  tabpane=0;
  closed=false;
  savepos=false;
  shown=false;
  lastfocus="";
  setAttribute(Qt::WA_DeleteOnClose);
  QStringList m=s2q(p).split(' ',_SkipEmptyParts);
  if (invalidopt(s,m,"escclose closeok dialog popup minbutton maxbutton closebutton ptop owner nosize activeonly")) return;
  activeonly=m.contains("activeonly");
  escclose=m.contains("escclose");
  closeok=m.contains("closeok");
  setpn(s);
  Qt::WindowFlags flags=Qt::Widget;
  if (m.contains("dialog")) flags=Qt::Dialog|Qt::WindowTitleHint|Qt::WindowStaysOnTopHint|Qt::CustomizeWindowHint;
  if (m.contains("popup")) flags=Qt::Popup;
  if (m.contains("minbutton")) flags|=Qt::WindowMinimizeButtonHint;
  if (m.contains("maxbutton")) flags|=Qt::WindowMaximizeButtonHint;
  if (m.contains("closebutton")) flags|=Qt::WindowCloseButtonHint;
  if (m.contains("ptop")) flags|=Qt::WindowStaysOnTopHint;
  if (m.contains("owner")) {
    flags|=Qt::Window;
    setWindowModality(Qt::WindowModal);
  }
  if (fontdef) setFont(fontdef->font);
  setWindowFlags(flags);

  layout=new QVBoxLayout(this);
  setpadding(0,0,0,0);
  layout->setSpacing(0);
  if (m.contains("nosize")) layout->setSizeConstraint( QLayout::SetFixedSize );
  addpane(0);
  signalMapper=new QSignalMapper(this);
  connect(signalMapper,SIGNAL(mapped(QWidget *)),
          this,SLOT(buttonClicked(QWidget *)));
  timer=new QTimer(this);
  connect(timer, SIGNAL(timeout()),this,SLOT(systimer()));
}

// ---------------------------------------------------------------------
Form::~Form()
{
  for (int i=children.size()-1; 0<=i; i--)
    delete children.at(i);
  if (this==form) form = 0;
  if (this==evtform) evtform = 0;
  Forms.removeOne(this);
  if (Forms.isEmpty()) form=0;
  if (Forms.isEmpty() && (!ShowIde)) {
    if (jdllproc) evloop->exit();
    else {
      jcon->cmd("2!:55[0");
    }
  }
}

// ---------------------------------------------------------------------
QWidget *Form::activetab()
{
  if (!tab) return 0;
  return tab->activetab();
}

// ---------------------------------------------------------------------
void Form::addchild(Child *c)
{
  child=c;
  children.append(c);
}

// ---------------------------------------------------------------------
void Form::addmenu()
{
  menubar= new Menus("menu","",this,0);
  addchild((Child *) menubar);
  layout->insertWidget(0,child->widget);
}

// ---------------------------------------------------------------------
Pane *Form::addpane(int n)
{
  pane=new Pane(n,this);
  panes.append(pane);
  return pane;
}

// ---------------------------------------------------------------------
void Form::buttonClicked(QWidget *w)
{
  Child *child=(Child *) w;
  child->event="button";
  signalevent(child);
}

// ---------------------------------------------------------------------
void Form::changeEvent(QEvent *e)
{
  if(e->type()==QEvent::ActivationChange && isActiveWindow() && seq < FormSeq-1)
    seq=FormSeq++;
  QWidget::changeEvent(e);
}

// ---------------------------------------------------------------------
void Form::closeEvent(QCloseEvent *e)
{
  saveformpos();

  if (closeok || closed) {
    e->accept();
    return;
  }
  e->ignore();
  event="close";
  fakeid="";
  form=this;
  signalevent(0);
  if (closed) {
    e->accept();
  } else e->ignore();
}

// ---------------------------------------------------------------------
void Form::closeit()
{
  saveformpos();
  closed=true;
  close();
}

// ---------------------------------------------------------------------
// close if not the main pane
void Form::closepane()
{
  if (panes.size()<=1) return;
  panes.removeLast();
  pane=panes.last();
}

// ---------------------------------------------------------------------
std::string Form::get(std::string p,std::string v)
{
  std::string r="";
  if (v.size() && p!="extent") {
    error("extra parameters: " + p + " " + v);
    return "";
  }
  if (p=="property") {
    r+=std::string("caption")+"\012"+ "children"+"\012"+ "enable"+"\012"+ "extent"+"\012"+ "focus"+"\012";
    r+=std::string("focuspolicy")+"\012"+ "font"+"\012"+ "hasfocus"+"\012"+ "hwnd"+"\012";
    r+=std::string("id")+"\012"+ "lastfocus"+"\012"+ "locale"+"\012";
    r+=std::string("maxwh")+"\012"+ "minwh"+"\012"+ "property"+"\012"+ "sizepolicy"+"\012"+ "state"+"\012";
    r+=std::string("stylesheet")+"\012"+ "sysdata"+"\012"+ "sysmodifiers"+"\012";
    r+=std::string("tooltip")+"\012"+ "visible"+"\012"+ "wh"+"\012"+ "xywh"+"\012";
  } else if (p=="caption") {
    r=q2s(this->windowTitle());
  } else if (p=="children") {
    for (int i=0; children.size()>i; i++)
      r+=children.at(i)->id+"\012";
  } else if (p=="enable") {
    r=i2s(this->isEnabled());
  } else if (p=="extent") {
    QFontMetrics fm = QFontMetrics(this->font());
#ifdef QT512
    r=i2s(fm.horizontalAdvance(s2q(v)))+" "+i2s(fm.height());
#else
    r=i2s(fm.width(s2q(v)))+" "+i2s(fm.height());
#endif
  } else if (p=="focus") {
    r=this->getfocus();
  } else if (p=="focuspolicy") {
    int f=this->focusPolicy();
    if (f==Qt::TabFocus)
      r="tab";
    else if (f==Qt::ClickFocus)
      r="click";
    else if (f==Qt::StrongFocus)
      r="strong";
    else if (f==Qt::NoFocus)
      r="no";
    else
      r="unknown";
  } else if (p=="font") {
    r=q2s(fontspec(this->font()));
  } else if (p=="hasfocus") {
    r=i2s(this->hasFocus());
  } else if (p=="hwnd") {
    r=this->hsform();
  } else if (p=="id") {
    r=id;
  } else if (p=="lastfocus") {
    r=lastfocus;
  } else if (p=="locale") {
    r=locale;
  } else if (p=="maxwh") {
    QSize size=this->maximumSize();
    r=i2s(size.width())+" "+i2s(size.height());
  } else if (p=="minwh") {
    QSize size=this->minimumSize();
    r=i2s(size.width())+" "+i2s(size.height());
  } else if (p=="sizepolicy") {
    std::string h,vr;
    int hoz=this->sizePolicy().horizontalPolicy();
    if (hoz==QSizePolicy::Fixed)
      h="fixed";
    else if (hoz==QSizePolicy::Minimum)
      h="minimum";
    else if (hoz==QSizePolicy::Maximum)
      h="maximum";
    else if (hoz==QSizePolicy::Preferred)
      h="preferred";
    else if (hoz==QSizePolicy::Expanding)
      h="expanding";
    else if (hoz==QSizePolicy::MinimumExpanding)
      h="minimumexpanding";
    else if (hoz==QSizePolicy::Ignored)
      h="ignored";
    else
      h="unknown";
    int ver=this->sizePolicy().verticalPolicy();
    if (ver==QSizePolicy::Fixed)
      vr="fixed";
    else if (ver==QSizePolicy::Minimum)
      vr="minimum";
    else if (ver==QSizePolicy::Maximum)
      vr="maximum";
    else if (ver==QSizePolicy::Preferred)
      vr="preferred";
    else if (ver==QSizePolicy::Expanding)
      vr="expanding";
    else if (ver==QSizePolicy::MinimumExpanding)
      vr="minimumexpanding";
    else if (ver==QSizePolicy::Ignored)
      vr="ignored";
    else
      vr="unknown";
    r=h + " " + vr;
  } else if (p=="state") {
    r=this->state(0);
  } else if (p=="stylesheet") {
    r=q2s(this->styleSheet());
  } else if (p=="sysdata") {
    r=sysdata;
  } else if (p=="sysmodifiers") {
    r=getsysmodifiers();
  } else if (p=="tooltip") {
    r=q2s(this->toolTip());
  } else if (p=="visible") {
    r=i2s(this->isVisible());
  } else if (p=="wh") {
    QSize size=this->size();
    r=i2s(size.width())+" "+i2s(size.height());
  } else if (p=="xywh") {
    r=i2s(pos().x())+" "+i2s(pos().y())+" "+i2s(size().width())+" "+i2s(size().height());
  } else
    error("get command not recognized: " + p + " " + v);
  return r;
}

// ---------------------------------------------------------------------
std::string Form::getfocus()
{
  QWidget *w=app->focusWidget();
  if (!w || !this->children.size()) return "";
  for (int i=this->children.size()-1; 0<=i; i--) {
    QWidget *c;
    if ((c=this->children.at(i)->widget) && (w==c || c->isAncestorOf(w)))
      return this->children.at(i)->id;
  }
  return "";
}

// ---------------------------------------------------------------------
std::string Form::getsysmodifiers()
{
  return getsysmodifiers(app->keyboardModifiers());
}

// ---------------------------------------------------------------------
std::string Form::getsysmodifiers(Qt::KeyboardModifiers mod)
{
  return i2s((mod.testFlag(Qt::ShiftModifier) ? 1 : 0) +
             (mod.testFlag(Qt::ControlModifier)? 2 : 0) +
             (mod.testFlag(Qt::AltModifier)? 4 : 0));
}

// ---------------------------------------------------------------------
std::string Form::hschild()
{
  return p2s((void *)child);
}

// ---------------------------------------------------------------------
std::string Form::hsform()
{
  return p2s((void *)this);
}

// ---------------------------------------------------------------------
std::string Form::qform()
{
  return i2s(pos().x())+" "+i2s(pos().y())+" "+i2s(size().width())+" "+i2s(size().height());
}

// ---------------------------------------------------------------------
Child *Form::id2child(std::string n)
{
  for (int i=0; i<children.size(); i++)
    if ("menu"!=children.at(i)->type && n==children.at(i)->id)
      return children.at(i);
  return (Child *) 0;
}

// ---------------------------------------------------------------------
bool Form::ischild(Child *n)
{
  return children.contains(n);
}

// ---------------------------------------------------------------------
void Form::keyPressEvent(QKeyEvent *e)
{
  int k=e->key();
  if (ismodifier(k)) return;
  if (k==Qt::Key_Escape) {
    e->ignore();
    if (closed) return;
    if (escclose) {
      if (closeok) {
        closeit();
      } else  {
        event="close";
        fakeid="";
        form=this;
        signalevent(0);
      }
    } else {
      event="cancel";
      fakeid="";
      form=this;
      signalevent(0);
    }
  } else if (k>=Qt::Key_F1 && k<=Qt::Key_F35) {
    event="fkey";
    form=this;
    signalevent(0,e);
  } else if ((e->modifiers() & Qt::ControlModifier) &&
             ((k>=Qt::Key_A && k<=Qt::Key_Z) || (k>=Qt::Key_0 && k<=Qt::Key_9) || (k>=Qt::Key_Home && k<=Qt::Key_PageDown))) {
    event="fkey";
    form=this;
    signalevent(0,e);
  } else
    QWidget::keyPressEvent(e);
}

// ---------------------------------------------------------------------
void Form::resizeEvent(QResizeEvent *e)
{
  Q_UNUSED(e);
  event="resize";
  signalevent(0);
}

// ---------------------------------------------------------------------
void Form::saveformpos()
{
  if (savepos && !closed)
    config.formpos_save((QWidget*)this, s2q(form->id));
}

// ---------------------------------------------------------------------
void Form::set(std::string p,std::string v)
{
  if (p=="enable") {
    setEnabled(remquotes(v)!="0");
  } else if (p=="font") {
    setFont((Font(v)).font);
  } else if (p=="invalid") {
    update();
  } else if (p=="maxvisibleitems") {
    maxvisibleitems=c_strtoi((v));
  } else if (p=="show"||p=="visible") {
    setVisible(remquotes(v)!="0");
  } else if (p=="stylesheet") {
    setStyleSheet(s2q(remquotes(v)));
  } else if (p=="taborder") {
    settaborder(v);
  } else if (p=="theme") {
    settheme(remquotes(v));
  } else if (p=="tooltip") {
    setToolTip(s2q(remquotes(v)));
  } else if (p=="wh") {
    wdsetwh(this,v);
  } else
    error("set command not recognized: " + p + " " + v);
}

// ---------------------------------------------------------------------
Child *Form::setmenuid(std::string id)
{
  if (menubar && menubar->items.contains(s2q(id)))
    return (Child *) menubar;
  return (Child *) 0;
}

// ---------------------------------------------------------------------
void Form::setpadding(int l,int t,int r,int b)
{
  layout->setContentsMargins(l,t,r,b);
}

// ---------------------------------------------------------------------
void Form::setpn(std::string p)
{
  setWindowTitle(s2q(p));
}

// ---------------------------------------------------------------------
void Form::setpicon(std::string p)
{
  int spi;
  if (p.substr(0,8)=="qstyle::" && -1!=(spi=wdstandardicon(p)))
    setWindowIcon(this->style()->standardIcon((QStyle::StandardPixmap)spi));
  else
    setWindowIcon(QIcon(s2q(p)));
}

// ---------------------------------------------------------------------
void Form::settaborder(std::string p)
{
  Child *c0,*c1;
  QStringList cs=qsplit(p);
  if (2>cs.size()) {
    error("taborder requires at least 2 child ids: " + p);
    return;
  }
  for (int i=0; cs.size()-1>i; i++) {
    c0=this->id2child(q2s(cs.at(i)));
    if (!c0 || !c0->widget) {
      error("taborder invalid child id: '" + q2s(cs.at(i)) + "' in " + p);
      return;
    }
    c1=this->id2child(q2s(cs.at(i+1)));
    if (!c1 || !c1->widget) {
      error("taborder invalid child id: '" + q2s(cs.at(i+1)) + "' in " + p);
      return;
    }
    QWidget::setTabOrder(c0->widget,c1->widget);
  }
}

// ---------------------------------------------------------------------
void Form::settheme(std::string theme)
{
  if (theme == "dark") {
    QFile f(":qdarkstyle/dark/style.qss");
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream ts(&f);
    setStyleSheet(ts.readAll());
  } else
    setStyleSheet("");
}

// ---------------------------------------------------------------------
void Form::settimer(std::string p)
{
  int n=c_strtoi(p);
  if (n)
    timer->start(n);
  else
    timer->stop();
}

// ---------------------------------------------------------------------
void Form::showit(std::string p)
{
  if (!shown) {
    for (int i=tabs.size()-1; i>=0; i--)
      tabs.last()->tabend();
    for (int i=panes.size()-1; i>=0; i--)
      panes.last()->fini();
    layout->addWidget(pane);
    setLayout(layout);
    shown = true;
  }
  if (p=="hide")
    hide();
  else if (p=="minimized")
    showMinimized();
  else {
    if (p=="fullscreen")
      showFullScreen();
    else if (p=="maximized")
      showMaximized();
    else if (p=="normal")
      showNormal();
    else {
      if (p!="")
        error("unrecognized style: " + p);
      show();
    }
    activateWindow();
    raise();
  }
}

// ---------------------------------------------------------------------
void Form::signalevent(Child *c, QKeyEvent *e)
{
  if (NoEvents || closed) return;
  std::string ctype="";
  std::string loc = locale;
  evtform=this;
  if (c) {
    ctype=c->type;
    evtchild=c;
    c->setform();
    sysmodifiers=c->sysmodifiers;
    if (sysmodifiers.empty())
      sysmodifiers=getsysmodifiers();
    sysdata=c->sysdata;
    loc = (""!=c->locale)?c->locale:locale;
  } else {
    evtchild=0;
    if (event=="fkey") {
      int k=e->key();
      if ((e->modifiers() & Qt::ControlModifier) && k>=Qt::Key_A && k<=Qt::Key_Z) {
        fakeid=(char)e->key()+32;  // lower case
        fakeid=fakeid + "ctrl" + std::string( (e->modifiers() & Qt::ShiftModifier) ? "shift" : "" );
      } else if ((e->modifiers() & Qt::ControlModifier) && k>=Qt::Key_0 && k<=Qt::Key_9) {
        fakeid=(char)e->key();  // turn to std::string
        fakeid=fakeid + "ctrl" + std::string( (e->modifiers() & Qt::ShiftModifier) ? "shift" : "" );
      } else if ((e->modifiers() & Qt::ControlModifier) && k>=Qt::Key_Home && k<=Qt::Key_PageDown) {
        std::string keynames[] = {"home", "end", "left", "up", "right", "down", "pgup", "pgdn"} ;
        fakeid=keynames[e->key()-Qt::Key_Home];  // select event name
        fakeid=fakeid + "ctrl" + std::string( (e->modifiers() & Qt::ShiftModifier) ? "shift" : "" );
      } else if (k>=Qt::Key_F1 && k<=Qt::Key_F35) {
        std::ostringstream ostr;
        ostr << e->key()+1-Qt::Key_F1;
        fakeid="f"+ ostr.str() + std::string((e->modifiers() & Qt::ControlModifier) ? "ctrl" : "") + std::string((e->modifiers() & Qt::ShiftModifier) ? "shift" : "");
      }
    }
  }
  std::string fc=getfocus();
  if (fc.size()) lastfocus=fc;

  std::string cmd=("wdhandlerx_jqtide_ '" + loc + "'") + '\0' + evtform->state(1);
  // paint events in isigraph/opengl have priority, because all the graphics must
  // be drawn WHILE THE Qt PAINT EVENT IS ACTIVE, and not wait for the callback
  //  queue to be scheduled.
#ifdef DEBUG_JDO
  std::string cmdtmp=cmd;
  std::replace( cmdtmp.begin(), cmdtmp.end(), '\0', '\\');
  qDebug() << "form event "+s2q(cmdtmp);
#endif
  if ((ctype=="isigraph" || ctype=="opengl") && c && c->event=="paint")
    jcon->cmddop(cmd);
  else
    jcon->cmddo(cmd);
}

// ---------------------------------------------------------------------
std::string Form::state(int evt)
{
  std::string c,c1,e,r,s,ec;

  if (evt) {
    if (evtchild) {
      c=evtchild->eid;
      e=evtchild->event;
      ec=evtchild->locale;
    } else {
      c=fakeid;
      e=event;
    }
    c1=(c.empty()) ? std::string("") : (c+"_") ;
    r+=spair("syshandler",id+"_handler");
    r+=spair("sysevent",id+"_"+c1+e);
    r+=spair("sysdefault",id+"_default");
    r+=spair("sysparent",id);
    r+=spair("syschild",c);
    r+=spair("systype",e);
    r+=spair("syslocalec",ec);
  }

  // need only syslocale (not syslocalep, syslocalec)?...  in isigraph
  r+=spair("syslocalep",locale);
  r+=spair("syshwndp",hsform());
  r+=spair("syshwndc",hschild());
  r+=spair("syslastfocus",lastfocus);
  r+=spair("sysfocus",getfocus());
  r+=spair("sysmodifiers",sysmodifiers);
  r+=spair("sysdata",sysdata);

  QWidget *act = activetab();

  if (activeonly && act) {
    QWidget *ctab;
    for (int i=0; i<children.size(); i++) {
      ctab=children.at(i)->ptab;
      if (ctab && act != ctab) continue;
      s+=children.at(i)->state();
    }
  } else
    for (int i=0; i<children.size(); i++)
      s+=children.at(i)->state();

  return r+s;
}

// ---------------------------------------------------------------------
// for debugging
void Form::status(std::string s)
{
  qDebug() << "form status: " << s2q(s);
  qDebug() << "current pane, panes: " << pane << panes;
  qDebug() << "current tab, tabs: " << tab << tabs;
}

// ---------------------------------------------------------------------
void Form::systimer()
{
  event="timer";
  fakeid="";
  signalevent(0);
}
