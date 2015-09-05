
#include <QApplication>
#include <QBoxLayout>
#include <QEventLoop>
#include <QMenuBar>
#include <QSignalMapper>
#include <QTimer>

#include <sstream>  //include this to use string streams

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
extern QEventLoop *evloop;

// ---------------------------------------------------------------------
Form::Form(string s, string p, string loc, QWidget *parent) : QWidget (parent)
{
  Q_UNUSED(parent);
  id=s;
  child=0;
  evtchild=0;
  locale=loc;
  menubar=0;
  seq=FormSeq++;
  tab=0;
  closed=false;
  shown=false;
  lastfocus="";
#ifdef QT_OS_ANDROID
  backButtonPressed=false;
#endif
  setAttribute(Qt::WA_DeleteOnClose);
  QStringList m=s2q(p).split(' ',QString::SkipEmptyParts);
  if (invalidopt(s,m,"escclose closeok dialog popup minbutton maxbutton closebutton ptop owner nosize")) return;
  escclose=m.contains("escclose");
  closeok=m.contains("closeok");
  setpn(s);

  Qt::WindowFlags flags=0;
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
#ifdef QT_OS_ANDROID
  if (!fontdef) setFont(QApplication::font());
#endif
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
#ifdef QT_OS_ANDROID
  if (!Forms.isEmpty()) {
    form=Forms.last();
    wdactivateform();
  }
#endif
  if (Forms.isEmpty() && (!ShowIde)) {
    if (jdllproc) evloop->exit();
    else {
      var_cmddo("(i.0 0)\"_ (2!:55)0", true);  // force into the engine even in suspension
      state_quit();
      QApplication::quit();
    }
  }
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

#ifdef QT_OS_ANDROID
// ---------------------------------------------------------------------
void Form::backButtonTimer()
{
  backButtonPressed=false;
  if (2>Forms.size()) return;
  Forms.removeOne(this);
  Forms.prepend(this);
  form=Forms.last();
  wdactivateform();
}
#endif

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
// close if not the main pane
void Form::closepane()
{
  if (panes.size()<=1) return;
  panes.removeLast();
  pane=panes.last();
}

// ---------------------------------------------------------------------
string Form::get(string p,string v)
{
  string r="";
  if (v.size() && p!="extent") {
    error("extra parameters: " + p + " " + v);
    return "";
  }
  if (p=="property") {
    r+=string("caption")+"\012"+ "children"+"\012"+ "enable"+"\012"+ "extent"+"\012"+ "focus"+"\012";
    r+=string("focuspolicy")+"\012"+ "font"+"\012"+ "hasfocus"+"\012"+ "hwnd"+"\012";
    r+=string("id")+"\012"+ "lastfocus"+"\012"+ "locale"+"\012";
    r+=string("maxwh")+"\012"+ "minwh"+"\012"+ "property"+"\012"+ "sizepolicy"+"\012"+ "state"+"\012";
    r+=string("stylesheet")+"\012"+ "sysdata"+"\012"+ "sysmodifiers"+"\012";
    r+=string("tooltip")+"\012"+ "visible"+"\012"+ "wh"+"\012"+ "xywh"+"\012";
  } else if (p=="caption") {
    r=q2s(this->windowTitle());
  } else if (p=="children") {
    for (int i=0; children.size()>i; i++)
      r+=children.at(i)->id+"\012";
  } else if (p=="enable") {
    r=i2s(this->isEnabled());
  } else if (p=="extent") {
    QFontMetrics fm = QFontMetrics(this->font());
    r=i2s(fm.width(s2q(v)))+" "+i2s(fm.height());
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
    string h,vr;
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
string Form::getsysmodifiers()
{
  Qt::KeyboardModifiers mod = QApplication::keyboardModifiers();
  return i2s((mod.testFlag(Qt::ShiftModifier) ? 1 : 0) +
             (mod.testFlag(Qt::ControlModifier)? 2 : 0));
}

// ---------------------------------------------------------------------
string Form::getfocus()
{
  QWidget *w=QApplication::focusWidget();
  if (!w || !this->children.size()) return "";
  for (int i=this->children.size()-1; 0<=i; i--) {
    QWidget *c;
    if ((c=this->children.at(i)->widget) && (w==c || c->isAncestorOf(w)))
      return this->children.at(i)->id;
  }
  return "";
}

// ---------------------------------------------------------------------
string Form::hschild()
{
  return p2s((void *)child);
}

// ---------------------------------------------------------------------
string Form::hsform()
{
  return p2s((void *)this);
}

// ---------------------------------------------------------------------
string Form::qform()
{
  return i2s(pos().x())+" "+i2s(pos().y())+" "+i2s(size().width())+" "+i2s(size().height());
}

// ---------------------------------------------------------------------
Child *Form::id2child(string n)
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
#ifdef QT_OS_ANDROID
  if (k==Qt::Key_Back) {
    QWidget::keyPressEvent(e);
    return;
  }
#endif
  if (k==Qt::Key_Escape) {
    e->ignore();
    if (closed) return;
    if (escclose) {
      if (closeok) {
        closed=true;
        close();
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
void Form::keyReleaseEvent(QKeyEvent *e)
{
#ifdef QT_OS_ANDROID
  if (e->key()==Qt::Key_Back) {
    if (!(backButtonPressed||(Qt::NonModal!=windowModality()))) {
      backButtonPressed=true;
      QTimer::singleShot(2000, this, SLOT(backButtonTimer()));
    } else {
      if (closed) return;
      if (closeok) {
        closed=true;
        close();
      } else {
        event="close";
        fakeid="";
        form=this;
        signalevent(0);
      }
    }
  } else QWidget::keyReleaseEvent(e);
#else
  QWidget::keyReleaseEvent(e);
#endif
}

// ---------------------------------------------------------------------
void Form::set(string p,string v)
{
  if (p=="enable") {
    setEnabled(remquotes(v)!="0");
  } else if (p=="font") {
    setFont((Font(v)).font);
  } else if (p=="invalid") {
    update();
  } else if (p=="show"||p=="visible") {
    setVisible(remquotes(v)!="0");
  } else if (p=="stylesheet") {
    setStyleSheet(s2q(remquotes(v)));
  } else if (p=="taborder") {
    settaborder(v);
  } else if (p=="tooltip") {
    setToolTip(s2q(remquotes(v)));
  } else if (p=="wh") {
    wdsetwh(this,v);
  } else
    error("set command not recognized: " + p + " " + v);
}

// ---------------------------------------------------------------------
Child *Form::setmenuid(string id)
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
void Form::setpn(string p)
{
  setWindowTitle(s2q(p));
}

// ---------------------------------------------------------------------
void Form::setpicon(string p)
{
  int spi;
  if (p.substr(0,8)=="qstyle::" && -1!=(spi=wdstandardicon(p)))
    setWindowIcon(this->style()->standardIcon((QStyle::StandardPixmap)spi));
  else
    setWindowIcon(QIcon(s2q(p)));
}

// ---------------------------------------------------------------------
void Form::settaborder(string p)
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
void Form::settimer(string p)
{
  int n=c_strtoi(p);
  if (n)
    timer->start(n);
  else
    timer->stop();
}

// ---------------------------------------------------------------------
void Form::showit(string p)
{
  Q_UNUSED(p);
  if (!shown) {
#ifdef QT_OS_ANDROID
// showide(false);
    if (Forms.size()>1)
      (Forms.at(Forms.size()-2))->setVisible(false);
#endif
    for (int i=tabs.size()-1; i>=0; i--)
      tabs.last()->tabend();
    for (int i=panes.size()-1; i>=0; i--)
      panes.last()->fini();
    layout->addWidget(pane);
    setLayout(layout);
    if (p=="fullscreen")
      showFullScreen();
    else if (p=="maximized")
      showMaximized();
    else if (p=="minimized")
      showMinimized();
    else if (p=="normal")
      showNormal();    // restore from maximized or minimized
    else
      show();          // show or hide
    if (jdllproc && 1==Forms.size()) evloop->exec(QEventLoop::AllEvents);
  }
#ifndef QT_OS_ANDROID
  if (p=="") {
    if (!isVisible()) setVisible(true);
  } else if (p=="hide") {
    if (isVisible()) setVisible(false);
  } else if (p=="fullscreen") {
    if (shown) showFullScreen();
  } else if (p=="maximized") {
    if (shown) showMaximized();
  } else if (p=="minimized") {
    if (shown) showMinimized();
  } else if (p=="normal") {
    if (shown) showNormal();
  } else {
    shown=true;
    error("unrecognized style: " + p);
  }
  shown=true;
#endif
}

// ---------------------------------------------------------------------
void Form::signalevent(Child *c, QKeyEvent *e)
{
  if (NoEvents || closed) return;
  string ctype="";
  string loc = locale;
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
        fakeid=fakeid + "ctrl" + string( (e->modifiers() & Qt::ShiftModifier) ? "shift" : "" );
      } else if ((e->modifiers() & Qt::ControlModifier) && k>=Qt::Key_0 && k<=Qt::Key_9) {
        fakeid=(char)e->key();  // turn to string
        fakeid=fakeid + "ctrl" + string( (e->modifiers() & Qt::ShiftModifier) ? "shift" : "" );
      } else if ((e->modifiers() & Qt::ControlModifier) && k>=Qt::Key_Home && k<=Qt::Key_PageDown) {
        string keynames[] = {"home", "end", "left", "up", "right", "down", "pgup", "pgdn"} ;
        fakeid=keynames[e->key()-Qt::Key_Home];  // select event name
        fakeid=fakeid + "ctrl" + string( (e->modifiers() & Qt::ShiftModifier) ? "shift" : "" );
      } else if (k>=Qt::Key_F1 && k<=Qt::Key_F35) {
        ostringstream ostr;
        ostr << e->key()+1-Qt::Key_F1;
        fakeid="f"+ ostr.str() + string((e->modifiers() & Qt::ControlModifier) ? "ctrl" : "") + string((e->modifiers() & Qt::ShiftModifier) ? "shift" : "");
      }
    }
  }
  string fc=getfocus();
  if (fc.size()) lastfocus=fc;
  if (jecallback) {
    if (ctype=="isigraph")
      var_cmddo("wdhandlerx_jqtide_ '" + s2q(loc) + "'",true);
    else {
      term->removeprompt();
      var_cmddo("wdhandlerx_jqtide_ '" + s2q(loc) + "'");
    }
  } else
    var_cmddo("wdhandler_" + s2q(loc) + "_$0");
}

// ---------------------------------------------------------------------
string Form::state(int evt)
{
  string c,c1,e,r,s,ec;

  if (evt) {
    if (evtchild) {
      c=evtchild->eid;
      e=evtchild->event;
      ec=evtchild->locale;
    } else {
      c=fakeid;
      e=event;
    }
    c1=(c.empty()) ? string("") : (c+"_") ;
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

  for (int i=0; i<children.size(); i++)
    s+=children.at(i)->state();

  return r+s;
}

// ---------------------------------------------------------------------
// for debugging
void Form::status(string s)
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
