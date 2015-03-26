
#if defined(WEBKITVIEW)

#ifdef QT50
#include <QtWebKitWidgets>
#else
#include <QtWebKit>
#endif
#include "webview.h"
#include "webkitview.h"

#define QWEBVIEW WebKitView
#define WEBVIEW WebView

#elif defined(WEBENGINEVIEW)

#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include "webengine.h"
#include "webengineview.h"

#define QWEBVIEW WebEngineView
#define WEBVIEW WebEngine

#endif

#include <QDir>

#include "wd.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
WEBVIEW::WEBVIEW(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
#if defined(WEBKITVIEW)
  type="webview";
#else
  type="webengine";
#endif
  QWEBVIEW *w=new QWEBVIEW(this, p);
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("dummy.html"));
  connect(w,SIGNAL(urlChanged( const QUrl & )), this,SLOT(urlChanged( const QUrl & )));
}

// ---------------------------------------------------------------------
WEBVIEW::~WEBVIEW()
{
  if (widget) delete (QWEBVIEW *)widget;
  widget=0;
}

// ---------------------------------------------------------------------
string WEBVIEW::get(string p,string v)
{
  return Child::get(p,v);
}

// ---------------------------------------------------------------------
void WEBVIEW::set(string p,string v)
{
  QWEBVIEW *w = (QWEBVIEW *)widget;
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
void WEBVIEW::urlChanged(const QUrl & url)
{
  curl=url.toString();
  event="curl";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
string WEBVIEW::state()
{
  return spair(id+"_curl",q2s(curl));
}
