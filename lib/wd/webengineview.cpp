
#include <QDesktopServices>
#include <QDir>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <locale.h>

#include "wd.h"
#include "webengineview.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
WebEngineView::WebEngineView(std::string n, std::string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="webview";
  curl="";
  Qwebengineview *w=new Qwebengineview(this,p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);

  qcom=new WebEngineViewCom(this);
  QWebChannel *channel = new QWebChannel(this);
  channel->registerObject(QString("qcom"),qcom);
  w->page()->setWebChannel(channel);
  if (s == "transparent")
    w->page()->setBackgroundColor(QColor(Qt::transparent));
  else if (s.length())
    w->page()->setBackgroundColor(QColor(s2q(s)));
  baseUrl = currenturl();
  connect(w,SIGNAL(urlChanged(const QUrl &)), this,SLOT(urlChanged( const QUrl &)));
  connect(w->page(),SIGNAL(loadFinished(bool)),this,SLOT(loadFinished(bool)));
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
void WebEngineView::cmd(std::string p,std::string v)
{
  QStringList s=qsplit(v);
  s.prepend(s2q(p));
  qcom->qwrite(s);
}

// ---------------------------------------------------------------------
void WebEngineView::cmdpost(QStringList s)
{
  QString c=s[0];
  if (c=="post") {
    event=q2s(c);
    cb_name=s[1];
    cb_value=s[2];
    pform->signalevent(this);
  }
}

// ---------------------------------------------------------------------
void WebEngineView::loadFinished(bool ok)
{
  loadok=ok;
  event="load";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
void WebEngineView::set(std::string p,std::string v)
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
  } else if (p=="url") {
    w->load(QUrl(s2q(remquotes(v))));
    w->show();
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
std::string WebEngineView::state()
{
  std::string r;
  if (event=="mmove") return r;
  if (event=="curl")
    r+=spair(id+"_curl",q2s(curl));
  else if (event=="load")
    r+=spair(id+"_load",q2s(loadok ? "1" : "0"));
  else if (event=="post") {
    r+=spair(id+"_name",q2s(cb_name));
    r+=spair(id+"_value",q2s(cb_value));
  }
  return r;
}

// ---------------------------------------------------------------------
void WebEngineView::urlChanged(const QUrl & url)
{
  if (curl==url.toString()) return;
  curl=url.toString();
  event="curl";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
WebEngineViewCom::WebEngineViewCom(WebEngineView *c, QWidget *parent)
{
  Q_UNUSED(parent);
  pchild=c;
}

// ---------------------------------------------------------------------
void WebEngineViewCom::qPost(QStringList s)
{
  pchild->cmdpost(s);
}

// ---------------------------------------------------------------------
void WebEngineViewCom::qwrite(QStringList s)
{
  emit qNotify(s);
}

// ---------------------------------------------------------------------
Qwebenginepage::Qwebenginepage()
{
  ;
}

// ---------------------------------------------------------------------
// urls are external if ending in ?external or &external
bool Qwebenginepage::acceptNavigationRequest(
  const QUrl &url,QWebEnginePage::NavigationType type, bool isMainFrame)
{

  Q_UNUSED(isMainFrame);

  if (type != QWebEnginePage::NavigationTypeLinkClicked)
    return true;

  QString q=url.query();
  if (!(q == "external" || q.endsWith("&external")))
    return true;

  QUrl qurl=QUrl(url);
  if (q == "external")
    qurl.setQuery(QString());
  else {
    q.remove(q.length()-9,9);
    qurl.setQuery(q);
  }
  QDesktopServices::openUrl(qurl);
  return false;
}

// ---------------------------------------------------------------------
Qwebengineview::Qwebengineview(Child *c, QWidget *parent)
{
  Q_UNUSED(parent);
  pchild=c;
  setMouseTracking(true);           // for mmove event
  setFocusPolicy(Qt::StrongFocus);  // for char event
  setPage(new Qwebenginepage());    // open urls

// following avoids rendering problems on Linux Mint 18.3, whereby
// canvas drawings become corrupted when other web controls are accessed.
// probably unnecessary on Win+OSX, but not tested
// requires Qt 5.7
#if defined( __linux__ ) && defined( QT57 )
  settings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, false);
#endif
}

// ---------------------------------------------------------------------
void Qwebengineview::buttonEvent(QEvent::Type type, QMouseEvent *event)
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
void Qwebengineview::wheelEvent(QWheelEvent *event)
{
  char deltasign = ' ';
  int delta = event->angleDelta().y() / 8;  // degree
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
      sprintf(sysdata, "%s", event->text().toUtf8().constData());
    else sprintf(sysdata, "%s", QString(QChar(key1)).toUtf8().constData());

    pchild->event=std::string("char");
    pchild->sysmodifiers=pchild->pform->getsysmodifiers(event->modifiers());
    pchild->sysdata=std::string(sysdata);
    pchild->pform->signalevent(pchild);
    // for ESC key, abort further processing lest we generate a second J event.
    if (key==Qt::Key_Escape) return;
  }
  QWebEngineView::keyPressEvent(event);
}
