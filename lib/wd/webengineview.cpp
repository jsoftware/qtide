
#include <QDir>
#include <QWebEnginePage>
#include <locale.h>

#include "wd.h"
#include "webengineview.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
WebEngineView::WebEngineView(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="webview";
  Qwebengineview *w=new Qwebengineview(this,p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  qcom=new WebEngineViewCom(this);
  baseUrl = currenturl();
  connect(w,SIGNAL(urlChanged(const QUrl &)), this,SLOT(urlChanged( const QUrl &)));
// not yet implemented
//  connect(w->page(),SIGNAL(javaScriptWindowObjectCleared()), this,SLOT(addJavaScriptObject()));
}

// ---------------------------------------------------------------------
WebEngineView::~WebEngineView()
{
  Qwebengineview *w = (Qwebengineview *)widget;
  w->stop();
  w->close();
  w->setParent(0);
}

// ---------------------------------------------------------------------
void WebEngineView::addJavaScriptObject()
{
  Qwebengineview *w = (Qwebengineview *)widget;
  Q_UNUSED(w);
// not yet implemented, should use QWebChannel instead
//  w->page()->addToJavaScriptWindowObject("qcom",qcom);
}

// ---------------------------------------------------------------------
void WebEngineView::cmd1(QList<QVariant> obj)
{
  QString c=obj.at(0).toString();
  if (c=="callback")
    cmd_callback(obj);
}

// ---------------------------------------------------------------------
void WebEngineView::cmd_callback(QList<QVariant> obj)
{
  event="callback";
  cb_name=obj.at(1).toString();
  cb_value=obj.at(2).toString();
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
string WebEngineView::get(string p,string v)
{
  string r;
  if (p=="evaljs") {
    Qwebengineview *w = (Qwebengineview *)widget;
    QVariant t;
// not yet implemented, result returned asynchronously
    w->page()->runJavaScript(s2q(v), [](const QVariant &result) {
      qDebug() << result;
    });
    r=q2s(t.toString());
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void WebEngineView::set(string p,string v)
{
  Qwebengineview *w = (Qwebengineview *)widget;
  if (p=="baseurl") {
    QString t = s2q(remquotes(v));
#if defined(_WIN32) && !defined(QT50)
    baseUrl = QUrl(t);
#else
    if (t.contains("://"))
      baseUrl = QUrl(t);
    else baseUrl = QUrl::fromLocalFile(t);
#endif
  } else if (p=="html") {
    w->setHtml(s2q(remquotes(v)), baseUrl);
    w->show();
// workaround locale switched to system default
    setlocale(LC_NUMERIC,"C");
  } else if (p=="url") {
    w->load(QUrl(s2q(remquotes(v))));
    w->show();
// workaround locale switched to system default
    setlocale(LC_NUMERIC,"C");
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
string WebEngineView::state()
{
  string r;
  if (event=="mmove") return r;
  if (event=="curl")
    r+=spair(id+"_curl",q2s(curl));
  else if (event=="callback") {
    r+=spair(id+"_name",q2s(cb_name));
    r+=spair(id+"_value",q2s(cb_value));
  }
  return r;
}

// ---------------------------------------------------------------------
WebEngineViewCom::WebEngineViewCom(WebEngineView *c, QWidget *parent)
{
  Q_UNUSED(parent);
  pchild=c;
}

// ---------------------------------------------------------------------
void WebEngineViewCom::cmd(QList<QVariant> obj)
{
  pchild->cmd1(obj);
}

// ---------------------------------------------------------------------
void WebEngineView::urlChanged(const QUrl & url)
{
  curl=url.toString();
  event="curl";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
Qwebengineview::Qwebengineview(Child *c, QWidget *parent)
{
  Q_UNUSED(parent);
  pchild=c;
  setMouseTracking(true);           // for mmove event
  setFocusPolicy(Qt::StrongFocus);  // for char event
}

// ---------------------------------------------------------------------
void Qwebengineview::buttonEvent(QEvent::Type type, QMouseEvent *event)
{
  string lmr = "";
  switch (event->button()) {
  case Qt::LeftButton:
    lmr = "l";
    break;
  case Qt::MidButton:
    lmr = "m";
    break;
  case Qt::RightButton:
    lmr = "r";
    break;
  default:
    break;
  }

  string evtname = "mmove";
  switch (type) {
  case QEvent::MouseButtonPress:
    evtname = "mb" + lmr + "down";
    break;
  case QEvent::MouseButtonRelease:
    evtname = "mb" + lmr + "up";
    break;
  case QEvent::MouseButtonDblClick:
    evtname = "mb" + lmr + "dbl";
    break;
  case QEvent::MouseMove:
    evtname = "mmove";
    break;
  default:
    break;
  }

  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysdata[200];
  sprintf(sysdata , "%d %d %d %d %d %d %d %d %d %d %d %d",
          event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)),
          (!!(event->buttons() & Qt::MidButton)), (!!(event->modifiers() & Qt::CTRL)),
          (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, 0);

  pchild->event=evtname;
  pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Qwebengineview::wheelEvent(QWheelEvent *event)
{
  char deltasign = ' ';
  int delta = event->delta() / 8;  // degree
  if (delta<0) {
    delta = -delta;
    deltasign = '_';
  }

  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysdata[200];
  sprintf(sysdata , "%d %d %d %d %d %d %d %d %d %d %d %c%d",
          event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)),
          (!!(event->buttons() & Qt::MidButton)), (!!(event->modifiers() & Qt::CTRL)),
          (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, deltasign, delta);

  pchild->event=string("mwheel");
  pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
  pchild->sysdata=string(sysdata);
  pchild->pform->signalevent(pchild);
  QWebEngineView::wheelEvent(event);
}

// ---------------------------------------------------------------------
void Qwebengineview::mousePressEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonPress, event);
  QWebEngineView::mousePressEvent(event);
}

// ---------------------------------------------------------------------
void Qwebengineview::mouseMoveEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseMove, event);
  QWebEngineView::mouseMoveEvent(event);
}

// ---------------------------------------------------------------------
void Qwebengineview::mouseDoubleClickEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonDblClick, event);
  QWebEngineView::mouseDoubleClickEvent(event);
}

// ---------------------------------------------------------------------
void Qwebengineview::mouseReleaseEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonRelease, event);
  QWebEngineView::mouseReleaseEvent(event);
}

// ---------------------------------------------------------------------
void Qwebengineview::focusInEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focus";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
  QWebEngineView::focusInEvent(event);
}

// ---------------------------------------------------------------------
void Qwebengineview::focusOutEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focuslost";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
  QWebEngineView::focusOutEvent(event);
}

// ---------------------------------------------------------------------
void Qwebengineview::keyPressEvent(QKeyEvent *event)
{
  int key1=0;
  int key=event->key();
  if (ismodifier(key)) return;
  if ((key==Qt::Key_unknown)||((key>=Qt::Key_F1)&&(key<=Qt::Key_F35))) {
    QWidget::keyPressEvent(event);
    return;
  } else
    key1=translateqkey(key);
  // Ctrl+anything becomes (possibly) a _fkey event; others become _char
  if (!event->modifiers().testFlag(Qt::ControlModifier)) {
    char sysdata[20];
    if (key==key1)
      sprintf(sysdata , "%s", event->text().toUtf8().constData());
    else sprintf(sysdata , "%s", QString(QChar(key1)).toUtf8().constData());

    pchild->event=string("char");
    pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
    pchild->sysdata=string(sysdata);
    pchild->pform->signalevent(pchild);
    // for ESC key, abort further processing lest we generate a second J event.
    if (key==Qt::Key_Escape) return;
  }
  QWebEngineView::keyPressEvent(event);
}
