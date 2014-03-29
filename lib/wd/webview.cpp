
#ifdef QT50
#include <QtWebKitWidgets>
#else
#include <QtWebKit>
#endif
#include <QDir>

#include "wd.h"
#include "webview.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Webview::Webview(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="webview";
  QWebView *w=new QWebView;
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("dummy.html"));
  connect(w,SIGNAL(urlChanged( const QUrl & )), this,SLOT(urlChanged( const QUrl & )));
}

// ---------------------------------------------------------------------
void Webview::set(string p,string v)
{
  QWebView *w = (QWebView *)widget;
  if (p=="baseurl") {
    QString t = s2q(v);
#if defined(_WIN32) && !defined(QT50)
    baseUrl = QUrl(t);
#else
    if (t.contains("://"))
      baseUrl = QUrl(t);
    else baseUrl = QUrl::fromLocalFile(t);
#endif
  } else if (p=="html") {
    w->setHtml(s2q(v), baseUrl);
    w->show();
  } else if (p=="url") {
    w->load(QUrl(s2q(v)));
    w->show();
  } else Child::set(p,v);
}

// ---------------------------------------------------------------------
void Webview::urlChanged(const QUrl & url)
{
  curl=url.toString();
  event="curl";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
string Webview::state()
{
  return spair(id+"_curl",q2s(curl));
}
