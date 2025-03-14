
#include <QDir>
#include <QWebFrame>

#include "wd.h"
#include "webview.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
WebView::WebView(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="webview";
  Qwebview *w=new Qwebview(this,p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  qcom=new WebViewCom(this);
  baseUrl = currenturl();
  connect(w,SIGNAL(urlChanged(const QUrl &)), this,SLOT(urlChanged( const QUrl &)));
  connect(w->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),
          this,SLOT(addJavaScriptObject()));
}

// ---------------------------------------------------------------------
void WebView::addJavaScriptObject()
{
  Qwebview *w = (Qwebview *)widget;
  w->page()->mainFrame()->addToJavaScriptWindowObject("qcom",qcom);
}

// ---------------------------------------------------------------------
void WebView::cmd1(QList<QVariant> obj)
{
  QString c=obj.at(0).toString();
  if (c=="callback")
    cmd_callback(obj);
}

// ---------------------------------------------------------------------
void WebView::cmd_callback(QList<QVariant> obj)
{
  event="callback";
  cb_name=obj.at(1).toString();
  cb_value=obj.at(2).toString();
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
std::string WebView::get(std::string p,std::string v)
{
  std::string r;
  if (p=="evaljs") {
    Qwebview *w = (Qwebview *)widget;
    QVariant t=w->page()->mainFrame()->evaluateJavaScript(s2q(v));
    r=q2s(t.toString());
  } else
    r=Child::get(p,v);
  return r;
}

// ---------------------------------------------------------------------
void WebView::set(std::string p,std::string v)
{
  Qwebview *w = (Qwebview *)widget;
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
  } else if (p=="url") {
    w->load(QUrl(s2q(remquotes(v))));
    w->show();
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string WebView::state()
{
  std::string r;
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
WebViewCom::WebViewCom(WebView *c, QWidget *parent)
{
  Q_UNUSED(parent);
  pchild=c;
}

// ---------------------------------------------------------------------
void WebViewCom::cmd(QList<QVariant> obj)
{
  pchild->cmd1(obj);
}

// ---------------------------------------------------------------------
void WebView::urlChanged(const QUrl & url)
{
  curl=url.toString();
  event="curl";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
Qwebview::Qwebview(Child *c, QWidget *parent)
{
  Q_UNUSED(parent);
  pchild=c;
  setMouseTracking(true);           // for mmove event
  setFocusPolicy(Qt::StrongFocus);  // for char event
}

// ---------------------------------------------------------------------
void Qwebview::buttonEvent(QEvent::Type type, QMouseEvent *event)
{
  std::string lmr = "";
  switch (event->button()) {
  case Qt::LeftButton:
    lmr = "l";
    break;
  case Qt::MiddleButton:
    lmr = "m";
    break;
  case Qt::RightButton:
    lmr = "r";
    break;
  default:
    break;
  }

  std::string evtname = "mmove";
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
  sprintf(sysdata, "%d %d %d %d %d %d %d %d %d %d %d %d",
#if defined(QT60)
          (int)event->position().x(), (int)event->position().y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)),
#else
          event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)),
#endif
          (!!(event->buttons() & Qt::MiddleButton)), (!!(event->modifiers() & Qt::CTRL)),
          (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, 0);

  pchild->event=evtname;
  pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
  pchild->sysdata=std::string(sysdata);
  pchild->pform->signalevent(pchild);
}

// ---------------------------------------------------------------------
void Qwebview::wheelEvent(QWheelEvent *event)
{
  char deltasign = ' ';
#ifdef QT50
  int delta = event->angleDelta().y() / 8;  // degree
#else
  int delta = event->delta() / 8;  // degree
#endif
  if (delta<0) {
    delta = -delta;
    deltasign = '_';
  }

  // sysdata = mousex,mousey,gtkwh,button1,button2,control,shift,button3,0,0,wheel
  char sysdata[200];
  sprintf(sysdata, "%d %d %d %d %d %d %d %d %d %d %d %c%d",
#if defined(QT515)
          (int)event->position().x(), (int)event->position().y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)),
#else
          event->x(), event->y(), this->width(), this->height(), (!!(event->buttons() & Qt::LeftButton)),
#endif
          (!!(event->buttons() & Qt::MiddleButton)), (!!(event->modifiers() & Qt::CTRL)),
          (!!(event->modifiers() & Qt::SHIFT)), (!!(event->buttons() & Qt::RightButton)), 0, 0, deltasign, delta);

  pchild->event=std::string("mwheel");
  pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
  pchild->sysdata=std::string(sysdata);
  pchild->pform->signalevent(pchild);
  QWebView::wheelEvent(event);
}

// ---------------------------------------------------------------------
void Qwebview::mousePressEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonPress, event);
  QWebView::mousePressEvent(event);
}

// ---------------------------------------------------------------------
void Qwebview::mouseMoveEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseMove, event);
  QWebView::mouseMoveEvent(event);
}

// ---------------------------------------------------------------------
void Qwebview::mouseDoubleClickEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonDblClick, event);
  QWebView::mouseDoubleClickEvent(event);
}

// ---------------------------------------------------------------------
void Qwebview::mouseReleaseEvent(QMouseEvent *event)
{
  buttonEvent(QEvent::MouseButtonRelease, event);
  QWebView::mouseReleaseEvent(event);
}

// ---------------------------------------------------------------------
void Qwebview::focusInEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focus";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
  QWebView::focusInEvent(event);
}

// ---------------------------------------------------------------------
void Qwebview::focusOutEvent(QFocusEvent *event)
{
  Q_UNUSED(event);
  pchild->event="focuslost";
  pchild->sysmodifiers="";
  pchild->sysdata="";
  pchild->pform->signalevent(pchild);
  QWebView::focusOutEvent(event);
}

// ---------------------------------------------------------------------
void Qwebview::keyPressEvent(QKeyEvent *event)
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
      sprintf(sysdata, "%s", event->text().toUtf8().constData());
    else sprintf(sysdata, "%s", QString(QChar(key1)).toUtf8().constData());

    pchild->event=std::string("char");
    pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
    pchild->sysdata=std::string(sysdata);
    pchild->pform->signalevent(pchild);
    // for ESC key, abort further processing lest we generate a second J event.
    if (key==Qt::Key_Escape) return;
  }
  QWebView::keyPressEvent(event);
}
