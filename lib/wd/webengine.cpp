
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QDir>

#include "wd.h"
#include "webengine.h"
#include "form.h"
#include "pane.h"
#include "cmd.h"

// ---------------------------------------------------------------------
Webengine::Webengine(string n, string s, Form *f, Pane *p) : Child(n,s,f,p)
{
  type="webengine";
  QWebEngineView *w=new QWebEngineView;
  widget=(QWidget *) w;
  QString qn=s2q(n);
  w->setObjectName(qn);
  baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("dummy.html"));
  connect(w,SIGNAL(urlChanged( const QUrl & )), this,SLOT(urlChanged( const QUrl & )));
}

// ---------------------------------------------------------------------
void Webengine::set(string p,string v)
{
  QWebEngineView *w = (QWebEngineView *)widget;
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
void Webengine::urlChanged(const QUrl & url)
{
  curl=url.toString();
  event="curl";
  pform->signalevent(this);
}

// ---------------------------------------------------------------------
string Webengine::state()
{
  return spair(id+"_curl",q2s(curl));
}
